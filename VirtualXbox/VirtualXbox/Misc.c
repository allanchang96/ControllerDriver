#include "driver.h"
#include "Misc.tmh"

NTSTATUS
Misc_WdfDeviceAllocateUsbContext(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, USB_CONTEXT);

    status = WdfObjectAllocateContext(Device, &attributes, NULL);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfObjectAllocateContext failed %!STATUS!", status);
        return status;
    }

    return status;
}

NTSTATUS
Misc_WdfDeviceAllocateQueueContext(
    _In_ WDFDEVICE Device
)
{
    NTSTATUS status;
    WDF_OBJECT_ATTRIBUTES attributes;

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, QUEUE_CONTEXT);

    status = WdfObjectAllocateContext(Device, &attributes, NULL);

    if (!NT_SUCCESS(status)) {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfObjectAllocateContext failed %!STATUS!", status);
        return status;
    }

    return status;
}