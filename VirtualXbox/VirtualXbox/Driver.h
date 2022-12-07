/*++

Module Name:

    driver.h

Abstract:

    This file contains the driver definitions.

Environment:

    Kernel-mode Driver Framework

--*/

#include <ntddk.h>
#include <wdf.h>
#include <usb.h>
#include <usbdlib.h>
#include <wdfusb.h>
#include <initguid.h>

#include <udecx.h>

#include "Controller.h"
#include "Descriptor.h"
#include "Misc.h"
#include "Queue.h"
#include "Trace.h"
#include "Usb.h"

EXTERN_C_START

//
// WDFDRIVER Events
//

DRIVER_INITIALIZE DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD VirtualXboxEvtDeviceAdd;
EVT_WDF_OBJECT_CONTEXT_CLEANUP VirtualXboxEvtDriverContextCleanup;

EXTERN_C_END
