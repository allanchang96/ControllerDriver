EXTERN_C_START

NTSTATUS
Misc_WdfDeviceAllocateUsbContext(
	_In_ WDFDEVICE Device
);

NTSTATUS
Misc_WdfDeviceAllocateQueueContext(
	_In_ WDFDEVICE Device
);

EXTERN_C_END