#include "driver.h"
#include "Queue.tmh"

NTSTATUS
Queue_RetrieveControlQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT     pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->ControlQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchSequential);

        queueConfig.EvtIoInternalDeviceControl = Queue_EvtControlUrb;

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->ControlQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->ControlQueue;

    return status;
}

NTSTATUS
Queue_RetrieveEndpoint1InQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT      pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->Endpoint1InQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->Endpoint1InQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->Endpoint1InQueue;

    return status;
}

NTSTATUS
Queue_RetrieveEndpoint1OutQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT      pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->Endpoint1OutQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->Endpoint1OutQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->Endpoint1OutQueue;

    return status;
}

NTSTATUS
Queue_RetrieveEndpoint2InQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT      pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->Endpoint2InQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->Endpoint2InQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->Endpoint2InQueue;

    return status;
}

NTSTATUS
Queue_RetrieveEndpoint2OutQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT      pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->Endpoint2OutQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->Endpoint2OutQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->Endpoint2OutQueue;

    return status;
}

NTSTATUS
Queue_RetrieveEndpoint3InQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT      pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->Endpoint3InQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->Endpoint3InQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->Endpoint3InQueue;

    return status;
}

NTSTATUS
Queue_RetrieveEndpoint3OutQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT      pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->Endpoint3OutQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->Endpoint3OutQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->Endpoint3OutQueue;

    return status;
}

NTSTATUS
Queue_RetrieveEndpoint4InQueue(
    _In_ WDFDEVICE Device,
    _Out_ WDFQUEUE* Queue
)
{
    NTSTATUS            status;
    PQUEUE_CONTEXT      pQueueContext;
    WDF_IO_QUEUE_CONFIG queueConfig;

    pQueueContext = WdfDeviceGetQueueContext(Device);

    status = STATUS_SUCCESS;
    *Queue = NULL;

    if (pQueueContext->Endpoint4InQueue == NULL)
    {
        WDF_IO_QUEUE_CONFIG_INIT(&queueConfig, WdfIoQueueDispatchManual);

        status = WdfIoQueueCreate(Device,
            &queueConfig,
            WDF_NO_OBJECT_ATTRIBUTES,
            &pQueueContext->Endpoint4InQueue);

        if (!NT_SUCCESS(status)) {

            TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "WdfIoQueueCreate failed %!STATUS!", status);
            return status;
        }
    }

    *Queue = pQueueContext->Endpoint4InQueue;

    return status;
}

VOID
Queue_EvtControlUrb(
    _In_ WDFQUEUE   Queue,
    _In_ WDFREQUEST Request,
    _In_ size_t     OutputBufferLength,
    _In_ size_t     InputBufferLength,
    _In_ ULONG      IoControlCode
)
{
    NTSTATUS status;
    PUCHAR transferBuffer;
    ULONG transferBufferLength;

    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);
    UNREFERENCED_PARAMETER(OutputBufferLength);
    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(IoControlCode);

    status = UdecxUrbRetrieveBuffer(Request, &transferBuffer, &transferBufferLength);
    if (!NT_SUCCESS(status))
    {
        TraceEvents(TRACE_LEVEL_ERROR, TRACE_QUEUE, "UdecxUrbRetrieveBuffer failed %!STATUS!", status);
        return;
    }
    UdecxUrbSetBytesCompleted(Request, transferBufferLength);
    UdecxUrbCompleteWithNtStatus(Request, STATUS_SUCCESS);
}