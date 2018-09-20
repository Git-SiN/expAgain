
#include "ntddk.h"
#include "..\Headers\expAgain.h";


WCHAR nameBuffer[] = L"\\Device\\expAgain";
WCHAR linkBuffer[] = L"\\DosDevices\\expAgain";
PDEVICE_OBJECT pMyDevice = NULL;

VOID OnUnload(PDRIVER_OBJECT pDriverObject) {
	UNICODE_STRING linkName;
	
	if (pMyDevice)
	{
		RtlInitUnicodeString(&linkName, linkBuffer);
		IoDeleteSymbolicLink(&linkName);
		IoDeleteDevice(pMyDevice);
		DbgPrintEx(101, 0, "Delete my Device...\n");
	}

	DbgPrintEx(101, 0, "Driver Unloaded...\n");
	return;
}

NTSTATUS DispatchRoutine(PDEVICE_OBJECT pDeviceObject, PIRP pIrp) {
	NTSTATUS ntStatus = STATUS_SUCCESS;
	PIO_STACK_LOCATION pStack = NULL;
	
	pStack = IoGetCurrentIrpStackLocation(pIrp);
	pIrp->IoStatus.Status = ntStatus;
	DbgPrintEx(101, 0, "IN %dth IRP...", (ULONG)(pStack->MajorFunction));
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return ntStatus;
}

NTSTATUS DispatchCreate(PDEVICE_OBJECT pDeviceObject, PIRP pIrp) {
	NTSTATUS ntStatus = STATUS_SUCCESS;
	
	DbgPrintEx(101, 0, "HI, my Console...\n");
	pIrp->IoStatus.Status = ntStatus;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return ntStatus;
}

NTSTATUS DispatchControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp) {
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	PIO_STACK_LOCATION pStack = NULL;
	ULONG ioctl = 0;

	pStack = IoGetCurrentIrpStackLocation(pIrp);

///////////    
	ioctl = pStack->Control;
	switch (ioctl) {
	case AGAIN_IOCTL_READ:
	case AGAIN_IOCTL_WRITE:
		DbgPrintEx(101, 0, "    IOCTL : %u", ioctl);
		ntStatus = STATUS_SUCCESS;
		break;

	default:
		ntStatus = STATUS_SUCCESS;
	}
	
	pIrp->IoStatus.Status = ntStatus;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	return ntStatus;

}

NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObject, PUNICODE_STRING regPath) {
	NTSTATUS ntStatus = STATUS_UNSUCCESSFUL;
	ULONG i = 0;
	UNICODE_STRING deviceName;
	UNICODE_STRING linkName;

	for (i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++)
		pDriverObject->MajorFunction[i] = DispatchRoutine;

	pDriverObject->MajorFunction[IRP_MJ_CREATE] = DispatchCreate;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DispatchControl;
 	pDriverObject->DriverUnload = OnUnload;

	RtlInitUnicodeString(&deviceName, nameBuffer);
	ntStatus = IoCreateDevice(pDriverObject, 0, &deviceName, (ULONG)DEVICE_AGAIN, 0, TRUE, &pMyDevice);
	if (NT_SUCCESS(ntStatus)) {
		DbgPrintEx(101, 0, "Created my Device...\n");

		RtlInitUnicodeString(&linkName, linkBuffer);
		ntStatus = IoCreateSymbolicLink(&linkName, &deviceName);
		if (NT_SUCCESS(ntStatus)) {
			DbgPrintEx(101, 0, "Created the Symbolic Link...\n");


			DbgPrintEx(101, 0, "Driver Loaded...\n");
		}
		else {
			DbgPrintEx(101, 0, "Failed to Create the Symbolic Link...\n");
			IoDeleteDevice(pMyDevice);
		}

	}
	else{		
		DbgPrintEx(101, 0, "Failed to Create Device...\n");
	}

	return ntStatus;
}


