// XBOX 360 Controller Descriptor
// Reference: https://www.partsnotincluded.com/understanding-the-xbox-360-wired-controllers-usb-data/

#include "driver.h"
#include "Descriptor.tmh"

const UCHAR Descriptor_Device[18] = {
    // Device Descriptor
    0x12,	//bLength
    0x01,	//bDescriptorType
    0x00, 0x02,	//bcdUSB
    0xFF,	//bDeviceClass(Vendor specific)
    0xFF,	//bDeviceSubClass
    0xFF,	//bDeviceProtocol
    0x08,	//bMaxPacketSize0(8 bytes)
    0x5E, 0x04,	//idVendor
    0x8E, 0x02,	//idProduct
    0x14, 0x01,	//bcdDevice
    0x01,	//iManufacturer
    0x02,	//iProduct
    0x03,	//iSerialNumber
    0x01	//bNumConfigurations
};

const UCHAR Descriptor_Configuration[] = {
    0x09,
    0x02,
    0x99, 0x00,
    0x04,
    0x01,
    0x00,
    0xA0,
    0xFA,

    0x09,
    0x04,
    0x00,
    0x00,
    0x02,
    0xff,
    0x5d,
    0x01,
    0x00,

    0x11,
    0x21,
    0x00, 0x01, 0x01, 0x25, 0x81, 0x14, 0x00, 0x00,
    0x00, 0x00, 0x13, 0x01, 0x08, 0x00, 0x00,

    0x07,
    0x05,
    0x81,
    0x03,
    0x20, 0x00,
    0x04,

    0x07,
    0x05,
    0x01,
    0x03,
    0x20, 0x00,
    0x08,

    0x09,
    0x04,
    0x01,
    0x00,
    0x04,
    0xff,
    0x5d,
    0x03,
    0x00,

    0x1b,
    0x21,
    0x00, 0x01, 0x01, 0x01, 0x82, 0x40, 0x01, 0x02,
    0x20, 0x16, 0x83, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x16,
    0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,

    0x07,
    0x05,
    0x82,
    0x03,
    0x20, 0x00,
    0x02,

    0x07,
    0x05,
    0x02,
    0x03,
    0x20, 0x00,
    0x04,

    0x07,
    0x05,
    0x83,
    0x03,
    0x20, 0x00,
    0x40,

    0x07,
    0x05,
    0x03,
    0x03,
    0x20, 0x00,
    0x10,

    0x09,
    0x04,
    0x02,
    0x00,
    0x01,
    0xff,
    0x5d,
    0x02,
    0x00,

    0x09,
    0x21,
    0x00, 0x01, 0x01, 0x22, 0x84, 0x07, 0x00,

    0x07,
    0x05,
    0x84,
    0x03,
    0x20, 0x00,
    0x10,

    0x09,
    0x04,
    0x03,
    0x00,
    0x00,
    0xff,
    0xfd,
    0x13,
    0x04,

    0x06,
    0x41,
    0x00, 0x01, 0x01, 0x03
};
