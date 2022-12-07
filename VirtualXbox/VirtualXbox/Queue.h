EXTERN_C_START

typedef struct _QUEUE_CONTEXT {

	WDFQUEUE ControlQueue;
	WDFQUEUE Endpoint1InQueue;
	WDFQUEUE Endpoint1OutQueue;
	WDFQUEUE Endpoint2InQueue;
	WDFQUEUE Endpoint2OutQueue;
	WDFQUEUE Endpoint3InQueue;
	WDFQUEUE Endpoint3OutQueue;
	WDFQUEUE Endpoint4InQueue;

} QUEUE_CONTEXT, * PQUEUE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(QUEUE_CONTEXT, WdfDeviceGetQueueContext);

NTSTATUS
Queue_RetrieveControlQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

NTSTATUS
Queue_RetrieveEndpoint1InQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

NTSTATUS
Queue_RetrieveEndpoint1OutQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

NTSTATUS
Queue_RetrieveEndpoint2InQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

NTSTATUS
Queue_RetrieveEndpoint2OutQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

NTSTATUS
Queue_RetrieveEndpoint3InQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

NTSTATUS
Queue_RetrieveEndpoint3OutQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

NTSTATUS
Queue_RetrieveEndpoint4InQueue(
	_In_ WDFDEVICE Device,
	_Out_ WDFQUEUE* Queue
);

VOID
Queue_EvtControlUrb(
	_In_ WDFQUEUE   Queue,
	_In_ WDFREQUEST Request,
	_In_ size_t     OutputBufferLength,
	_In_ size_t     InputBufferLength,
	_In_ ULONG      IoControlCode
);

EXTERN_C_END