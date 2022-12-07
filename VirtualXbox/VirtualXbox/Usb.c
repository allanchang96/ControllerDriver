#include "driver.h"
#include "Usb.tmh"

NTSTATUS
Usb_Initialize(
    _In_ WDFDEVICE Device
)
{

    NTSTATUS                                status;
    PUSB_CONTEXT                            pUsbContext;
    UDECX_USB_DEVICE_STATE_CHANGE_CALLBACKS callbacks;

    pUsbContext = WdfDeviceGetUsbContext(Device);

    pUsbContext->UdecxUsbDeviceInit = UdecxUsbDeviceInitAllocate(Device);

    if (pUsbContext->UdecxUsbDeviceInit == NULL) {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbDeviceInitAllocate failed %!STATUS!", status);
        return status;
    }

    UDECX_USB_DEVICE_CALLBACKS_INIT(&callbacks);

    UdecxUsbDeviceInitSetSpeed(pUsbContext->UdecxUsbDeviceInit, UdecxUsbHighSpeed);

    UdecxUsbDeviceInitSetEndpointsType(pUsbContext->UdecxUsbDeviceInit, UdecxEndpointTypeSimple);

    status = UdecxUsbDeviceInitAddDescriptor(pUsbContext->UdecxUsbDeviceInit,
        (PUCHAR)Descriptor_Device,
        sizeof(Descriptor_Device));

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbDeviceInitAddDescriptor Device failed %!STATUS!", status);
        return status;
    }

    return status;
}

NTSTATUS
Usb_ReadDescriptorsAndPlugIn(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                            status;
    PUSB_CONTEXT                        pUsbContext;
    UDECX_USB_DEVICE_PLUG_IN_OPTIONS    pluginOptions;

    pUsbContext = WdfDeviceGetUsbContext(Device);

    status = UdecxUsbDeviceInitAddDescriptor(pUsbContext->UdecxUsbDeviceInit,
        (PUCHAR)Descriptor_Configuration,
        sizeof(Descriptor_Configuration));

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbDeviceInitAddDescriptor Config failed %!STATUS!", status);
        return status;
    }

    status = UdecxUsbDeviceCreate(&pUsbContext->UdecxUsbDeviceInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDevice);

    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbDeviceCreate failed %!STATUS!", status);
        return status;
    }

    status = Usb_CreateControlEndpoint(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_CreateEndpoint1In(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_CreateEndpoint1Out(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_CreateEndpoint2In(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_CreateEndpoint2Out(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_CreateEndpoint3In(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_CreateEndpoint3Out(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    status = Usb_CreateEndpoint4In(Device);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    UDECX_USB_DEVICE_PLUG_IN_OPTIONS_INIT(&pluginOptions);

    status = UdecxUsbDevicePlugIn(pUsbContext->VirtualUsbDevice, &pluginOptions);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbDevicePlugIn failed %!STATUS!", status);
        return status;
    }

    return status;
}

NTSTATUS
Usb_Disconnect(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS            status;
    PUSB_CONTEXT        pUsbContext;

    pUsbContext = WdfDeviceGetUsbContext(Device);

    status = UdecxUsbDevicePlugOutAndDelete(pUsbContext->VirtualUsbDevice);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbDevicePlugOutAndDelete failed %!STATUS!", status);
        return status;
    }

    return status;

}

VOID
Usb_Destroy(
    _In_ WDFDEVICE Device
)
{
    PUSB_CONTEXT    pUsbContext;

    pUsbContext = WdfDeviceGetUsbContext(Device);

    if (pUsbContext != NULL && pUsbContext->UdecxUsbDeviceInit != NULL) {
        UdecxUsbDeviceInitFree(pUsbContext->UdecxUsbDeviceInit);
        pUsbContext->UdecxUsbDeviceInit = NULL;
    }

    return;

}

NTSTATUS
Usb_CreateControlEndpoint(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        controlQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveControlQueue(Device, &controlQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UDEQueue_RetrieveControlQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, USB_DEFAULT_ENDPOINT_ADDRESS);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceControlEndpoint);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceControlEndpoint,
        controlQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

NTSTATUS
Usb_CreateEndpoint1In(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        endpoint1InQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveEndpoint1InQueue(Device, &endpoint1InQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Queue_RetrieveEndpoint1InQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, 0x81);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceEndpoint1In);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceEndpoint1In,
        endpoint1InQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

NTSTATUS
Usb_CreateEndpoint1Out(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        endpoint1OutQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveEndpoint1InQueue(Device, &endpoint1OutQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Queue_RetrieveEndpoint1OutQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, 0x01);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceEndpoint1Out);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceEndpoint1Out,
        endpoint1OutQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

NTSTATUS
Usb_CreateEndpoint2In(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        endpoint2InQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveEndpoint2InQueue(Device, &endpoint2InQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Queue_RetrieveEndpoint2InQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, 0x82);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceEndpoint2In);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceEndpoint2In,
        endpoint2InQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

NTSTATUS
Usb_CreateEndpoint2Out(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        endpoint2OutQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveEndpoint2InQueue(Device, &endpoint2OutQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Queue_RetrieveEndpoint2OutQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, 0x02);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceEndpoint2Out);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceEndpoint2Out,
        endpoint2OutQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

NTSTATUS
Usb_CreateEndpoint3In(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        endpoint3InQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveEndpoint3InQueue(Device, &endpoint3InQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Queue_RetrieveEndpoint3InQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, 0x83);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceEndpoint3In);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceEndpoint3In,
        endpoint3InQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

NTSTATUS
Usb_CreateEndpoint3Out(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        endpoint3OutQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveEndpoint3InQueue(Device, &endpoint3OutQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Queue_RetrieveEndpoint3OutQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, 0x03);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceEndpoint3Out);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceEndpoint3Out,
        endpoint3OutQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

NTSTATUS
Usb_CreateEndpoint4In(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS                        status;
    PUSB_CONTEXT                    pUsbContext;
    WDFQUEUE                        endpoint4InQueue;
    UDECX_USB_ENDPOINT_CALLBACKS    callbacks;
    PUDECXUSBENDPOINT_INIT          endpointInit;

    pUsbContext = WdfDeviceGetUsbContext(Device);
    endpointInit = NULL;

    endpointInit = UdecxUsbSimpleEndpointInitAllocate(pUsbContext->VirtualUsbDevice);

    if (endpointInit == NULL)
    {
        status = STATUS_INSUFFICIENT_RESOURCES;
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbSimpleEndpointInitAllocate failed %!STATUS!", status);
        return status;
    }

    status = Queue_RetrieveEndpoint4InQueue(Device, &endpoint4InQueue);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "Queue_RetrieveEndpoint4InQueue failed %!STATUS!", status);
        return status;
    }

    UdecxUsbEndpointInitSetEndpointAddress(endpointInit, 0x84);

    UDECX_USB_ENDPOINT_CALLBACKS_INIT(&callbacks, Usb_EndpointReset);
    UdecxUsbEndpointInitSetCallbacks(endpointInit, &callbacks);

    status = UdecxUsbEndpointCreate(&endpointInit,
        WDF_NO_OBJECT_ATTRIBUTES,
        &pUsbContext->VirtualUsbDeviceEndpoint4In);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUsbEndpointCreate failed %!STATUS!", status);
        if (endpointInit != NULL) {
            UdecxUsbEndpointInitFree(endpointInit);
            endpointInit = NULL;
        }
        return status;
    }

    UdecxUsbEndpointSetWdfIoQueue(pUsbContext->VirtualUsbDeviceEndpoint4In,
        endpoint4InQueue);

    if (endpointInit != NULL) {
        UdecxUsbEndpointInitFree(endpointInit);
        endpointInit = NULL;
    }

    return status;
}

VOID
Usb_EndpointReset(
    _In_ UDECXUSBENDPOINT   UdecxUsbEndpoint,
    _In_ WDFREQUEST         Request
)
{
    UNREFERENCED_PARAMETER(UdecxUsbEndpoint);
    UNREFERENCED_PARAMETER(Request);
}