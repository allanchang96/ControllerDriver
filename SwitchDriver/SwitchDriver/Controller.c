#include "driver.h"
#include "controller.tmh"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (PAGE, Controller_Init)
#pragma alloc_text (PAGE, Controller_Parse)
#endif

NTSTATUS Controller_Init(
	_In_ WDFDEVICE Device
)
{
	NTSTATUS				status;
	WDF_OBJECT_ATTRIBUTES   controllerAttributes;

	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&controllerAttributes, CONTROLLER_CONTEXT);
	status = WdfObjectAllocateContext(Device, &controllerAttributes, NULL);

	if (!NT_SUCCESS(status)) {
		TraceEvents(TRACE_LEVEL_ERROR, TRACE_DEVICE,
			"WdfObjectAllocateContext failed 0x%x", status);
		return status;
	}
	return status;
}

VOID Controller_Parse(
	_In_ WDFDEVICE Device,
	_In_ PUCHAR Input
)
{
	PCONTROLLER_CONTEXT pControllerContext = ControllerGetContext(Device);

	pControllerContext->bA = (*(Input + 3) & (1 << 3)) != 0;
	pControllerContext->bB = (*(Input + 3) & (1 << 2)) != 0;
	pControllerContext->bX = (*(Input + 3) & (1 << 1)) != 0;
	pControllerContext->bY = (*(Input + 3) & 1) != 0;
	pControllerContext->bDpadUp = (*(Input + 5) & (1 << 1)) != 0;
	pControllerContext->bDpadDown = (*(Input + 5) & 1);
	pControllerContext->bDpadLeft = (*(Input + 5) & (1 << 3)) != 0;
	pControllerContext->bDpadRight = (*(Input + 5) & (1 << 2)) != 0;
	pControllerContext->bL = (*(Input + 5) & (1 << 6)) != 0;
	pControllerContext->bR = (*(Input + 3) & (1 << 6)) != 0;
	pControllerContext->bZL = (*(Input + 5) & (1 << 7)) != 0;
	pControllerContext->bZR = (*(Input + 3) & (1 << 7)) != 0;
	pControllerContext->bLeftThumb = (*(Input + 4) & (1 << 3)) != 0;
	pControllerContext->bRightThumb = (*(Input + 4) & (1 << 2)) != 0;
	pControllerContext->bMinus = (*(Input + 4) & 1) != 0;
	pControllerContext->bPlus = (*(Input + 4) & (1 << 1)) != 0;
	pControllerContext->bCapture = (*(Input + 4) & (1 << 5)) != 0;
	pControllerContext->bHome = (*(Input + 4) & (1 << 4)) != 0;
	pControllerContext->sThumbLX = ((*(Input + 6) | (*(Input + 7) & 0xf) << 8) - (1 << 11)) * (1 << 4);
	pControllerContext->sThumbLY = (((*(Input + 7) & 0xf0) >> 4 | *(Input + 8) << 4) - (1 << 11)) * (1 << 4);
	pControllerContext->sThumbRX = ((*(Input + 9) | (*(Input + 10) & 0xf) << 8) - (1 << 11)) * (1 << 4);
	pControllerContext->sThumbRY = (((*(Input + 10) & 0xf0) >> 4 | *(Input + 11) << 4) - (1 << 11)) * (1 << 4);
	/*TraceEvents(TRACE_LEVEL_INFORMATION, TRACE_DEVICE,
		"A:%d B:%d X:%d Y:%d Up:%d Down:%d Left:%d Right:%d L:%d R:%d ZL:%d ZR:%d L3:%d R3:%d -:%d +:%d Capture:%d Home:%d LX:%d LY:%d RX:%d RY:%d",
		pControllerContext->bA,
		pControllerContext->bB,
		pControllerContext->bX,
		pControllerContext->bY,
		pControllerContext->bDpadUp,
		pControllerContext->bDpadDown,
		pControllerContext->bDpadLeft,
		pControllerContext->bDpadRight,
		pControllerContext->bL,
		pControllerContext->bR,
		pControllerContext->bZL,
		pControllerContext->bZR,
		pControllerContext->bLeftThumb,
		pControllerContext->bRightThumb,
		pControllerContext->bMinus,
		pControllerContext->bPlus,
		pControllerContext->bCapture,
		pControllerContext->bHome,
		pControllerContext->sThumbLX,
		pControllerContext->sThumbLY,
		pControllerContext->sThumbRX,
		pControllerContext->sThumbRY);*/
}