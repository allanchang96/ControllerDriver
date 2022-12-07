# ControllerDriver

This project maps the input data from the Switch Pro Controller to a virtual
Xbox 360 controller.

## SwitchDriver

Overrides the default Windows Driver when the Switch Pro Controller is
connected to Windows 10 by USB. Initializes and parses controller input data
to conform to Xbox 360 packet data. Sends data to VirtualXbox driver with
WDFREQUEST objects.

## VirtualXbox

Uses UDE (USB Device Emulation) to create a USB device that reports itself
as an Xbox 360 Controller. Reads WDFREQUEST objects from SwitchDriver and
passes the data to the virtual USB device.