#include "Public.h"

EXTERN_C_START

NTSTATUS
Controller_CreateDevice(
    _Inout_ PWDFDEVICE_INIT DeviceInit
);

EVT_WDF_DEVICE_D0_ENTRY             Controller_EvtWdfDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT              Controller_EvtWdfDeviceD0Exit;

EVT_WDF_OBJECT_CONTEXT_CLEANUP      Controller_EvtWdfCleanupCallback;

EVT_UDECX_WDF_DEVICE_QUERY_USB_CAPABILITY Controller_EvtUdecxWdfDeviceQueryUsbCapability;

EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL  Controller_EvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_WRITE           Controller_EvtIoWrite;

EXTERN_C_END