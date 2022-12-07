EXTERN_C_START

typedef struct _USB_CONTEXT {

	PUDECXUSBDEVICE_INIT	UdecxUsbDeviceInit;
	UDECXUSBDEVICE			VirtualUsbDevice;
	UDECXUSBENDPOINT        VirtualUsbDeviceControlEndpoint;
	UDECXUSBENDPOINT        VirtualUsbDeviceEndpoint1In;
	UDECXUSBENDPOINT        VirtualUsbDeviceEndpoint1Out;
	UDECXUSBENDPOINT        VirtualUsbDeviceEndpoint2In;
	UDECXUSBENDPOINT        VirtualUsbDeviceEndpoint2Out;
	UDECXUSBENDPOINT        VirtualUsbDeviceEndpoint3In;
	UDECXUSBENDPOINT        VirtualUsbDeviceEndpoint3Out;
	UDECXUSBENDPOINT        VirtualUsbDeviceEndpoint4In;

} USB_CONTEXT, * PUSB_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(USB_CONTEXT, WdfDeviceGetUsbContext);

NTSTATUS
Usb_Initialize(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_ReadDescriptorsAndPlugIn(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_Disconnect(
	_In_ WDFDEVICE Device
);

VOID
Usb_Destroy(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateControlEndpoint(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateEndpoint1In(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateEndpoint1Out(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateEndpoint2In(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateEndpoint2Out(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateEndpoint3In(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateEndpoint3Out(
	_In_ WDFDEVICE Device
);

NTSTATUS
Usb_CreateEndpoint4In(
	_In_ WDFDEVICE Device
);

VOID
Usb_EndpointReset(
	_In_ UDECXUSBENDPOINT   UdecxUsbEndpoint,
	_In_ WDFREQUEST         Request
);

EXTERN_C_END