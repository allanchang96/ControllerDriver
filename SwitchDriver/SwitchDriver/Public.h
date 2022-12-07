/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    user and kernel

--*/

//
// Define an Interface Guid so that app can find the device and talk to it.
//

DEFINE_GUID (GUID_DEVINTERFACE_SwitchDriver,
    0xc6074c03,0x0b07,0x479e,0x9b,0xe6,0x3d,0x22,0x7f,0xc8,0xcb,0x26);
// {c6074c03-0b07-479e-9be6-3d227fc8cb26}
