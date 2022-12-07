#include "driver.h"
#include "Controller.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, Controller_CreateDevice)
#endif

#define DOS_DEVICE_NAME L"\\DosDevices\\VirtualXbox"

NTSTATUS
Controller_CreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
)
{
    NTSTATUS                        status;
    WDF_PNPPOWER_EVENT_CALLBACKS    pnpPowerCallbacks;
    WDF_OBJECT_ATTRIBUTES           deviceAttributes;
    WDFDEVICE                       device;
    UDECX_WDF_DEVICE_CONFIG         controllerConfig;
    WDF_IO_QUEUE_CONFIG             defaultQueueConfig;
    WDFQUEUE                        hQueue;


    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
    pnpPowerCallbacks.EvtDeviceD0Entry = Controller_EvtWdfDeviceD0Entry;
    pnpPowerCallbacks.EvtDeviceD0Exit = Controller_EvtWdfDeviceD0Exit;

    WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

    status = UdecxInitializeWdfDeviceInit(DeviceInit);

    if (!NT_SUCCESS(status) && status != STATUS_OBJECT_NAME_COLLISION) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxInitializeWdfDeviceInit failed %!STATUS!", status);
        return status;
    }

    WDF_OBJECT_ATTRIBUTES_INIT(&deviceAttributes);

    status = WdfDeviceCreate(&DeviceInit, &deviceAttributes, &device);
    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfDeviceCreate failed %!STATUS!", status);
        return status;
    }

    DECLARE_CONST_UNICODE_STRING(dosDeviceName, DOS_DEVICE_NAME);

    status = WdfDeviceCreateSymbolicLink(device,
        &dosDeviceName);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfDeviceCreateSymbolicLink failed %!STATUS!", status);
        return status;
    }

    UDECX_WDF_DEVICE_CONFIG_INIT(&controllerConfig, Controller_EvtUdecxWdfDeviceQueryUsbCapability);

    status = UdecxWdfDeviceAddUsbDeviceEmulation(device, &controllerConfig);

    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&defaultQueueConfig, WdfIoQueueDispatchSequential);
    defaultQueueConfig.EvtIoDeviceControl = Controller_EvtIoDeviceControl;
    defaultQueueConfig.EvtIoWrite = Controller_EvtIoWrite;
    defaultQueueConfig.PowerManaged = WdfFalse;

    status = WdfIoQueueCreate(device,
        &defaultQueueConfig,
        WDF_NO_OBJECT_ATTRIBUTES,
        &hQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
        return status;
    }

    status = Misc_WdfDeviceAllocateUsbContext(device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Misc_WdfDeviceAllocateQueueContext(device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_Initialize(device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    return status;
}

NTSTATUS
Controller_EvtWdfDeviceD0Entry(
    _In_ WDFDEVICE Device,
    _In_ WDF_POWER_DEVICE_STATE PreviousState
)
{
    UNREFERENCED_PARAMETER(Device);

    NTSTATUS status = STATUS_SUCCESS;

    if (PreviousState == WdfPowerDeviceD3Final) {
        status = Usb_ReadDescriptorsAndPlugIn(Device);

        if (!NT_SUCCESS(status)) {
            return status;
        }
    }
    return status;
}

NTSTATUS
Controller_EvtWdfDeviceD0Exit(
    _In_ WDFDEVICE Device,
    _In_ WDF_POWER_DEVICE_STATE TargetState
)
{
    UNREFERENCED_PARAMETER(Device);

    NTSTATUS status = STATUS_SUCCESS;

    if (TargetState == WdfPowerDeviceD3Final) {
        status = Usb_Disconnect(Device);

        if (!NT_SUCCESS(status)) {
            return status;
        }
    }
    return status;
}

VOID
Controller_WdfEvtCleanupCallback(
    _In_ WDFOBJECT Device
)
{
    Usb_Destroy((WDFDEVICE)Device);
}

NTSTATUS
Controller_EvtUdecxWdfDeviceQueryUsbCapability(
    _In_                WDFDEVICE UdecxWdfDevice,
    _In_                PGUID CapabilityType,
    _In_                ULONG OutputBufferLength,
    _Out_writes_to_opt_(OutputBufferLength, *ResultLength) PVOID OutputBuffer,
    _Out_               PULONG ResultLength
)
{
    UNREFERENCED_PARAMETER(UdecxWdfDevice);
    UNREFERENCED_PARAMETER(CapabilityType);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(OutputBuffer);
    UNREFERENCED_PARAMETER(ResultLength);
    return STATUS_SUCCESS;
}

VOID
Controller_EvtIoDeviceControl(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t OutputBufferLength,
    _In_ size_t InputBufferLength,
    _In_ ULONG IoControlCode
)
{
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(IoControlCode);

    BOOLEAN handled;
    NTSTATUS status;

    handled = UdecxWdfDeviceTryHandleUserIoctl(WdfIoQueueGetDevice(Queue), Request);

    if (handled)
    {
        return;
    }

    status = STATUS_INVALID_DEVICE_REQUEST;
    TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxWdfDeviceTryHandleUserIoctl not handled %!STATUS!", status);
    WdfRequestComplete(Request, status);
    return;
}

VOID
Controller_EvtIoWrite(
    _In_ WDFQUEUE Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t Length
)
{
    UNREFERENCED_PARAMETER(Length);

    NTSTATUS status;
    WDFDEVICE device;
    PQUEUE_CONTEXT pQueueContext;
    WDFREQUEST controllerRequest;
    PUCHAR transferBuffer;
    ULONG transferBufferLength;

    WDFMEMORY reqMemory;
    PUCHAR reqBuffer;

    device = WdfIoQueueGetDevice(Queue);
    pQueueContext = WdfDeviceGetQueueContext(device);

    status = WdfRequestRetrieveInputMemory(Request,
        &reqMemory);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfRequestRetrieveInputMemory failed %!STATUS!", status);
        return;
    }

    WdfIoQueueRetrieveNextRequest(pQueueContext->Endpoint1InQueue, &controllerRequest);

    if (controllerRequest == NULL)
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "No requests available");
        WdfRequestComplete(Request, STATUS_SUCCESS);
        return;
    }

    status = UdecxUrbRetrieveBuffer(controllerRequest, &transferBuffer, &transferBufferLength);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUrbRetrieveBuffer failed %!STATUS!", status);
        return;
    }

    RtlZeroMemory(transferBuffer, transferBufferLength);

    reqBuffer = (PUCHAR)WdfMemoryGetBuffer(reqMemory, NULL);

    memcpy(transferBuffer, reqBuffer, 20);

    UdecxUrbSetBytesCompleted(controllerRequest, 20);
    UdecxUrbCompleteWithNtStatus(controllerRequest, STATUS_SUCCESS);

    WdfRequestComplete(Request, STATUS_SUCCESS);
}