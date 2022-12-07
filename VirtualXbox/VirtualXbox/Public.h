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

DEFINE_GUID(GUID_DEVINTERFACE_VirtualXbox,
    0xe7e561a0, 0xa40e, 0x4c53, 0xaa, 0x8e, 0x6a, 0x2d, 0x21, 0x14, 0xf9, 0xd0);
// {e7e561a0-a40e-4c53-aa8e-6a2d2114f9d0}
