#include <stdio.h>
#include <windows.h>

#define BUFFER_SIZE		1024
WCHAR deviceName[] = L"\\\\.\\\\expAgain";
HANDLE hDevice = NULL;



ULONG InputMessage(PWCHAR *pInput) {
	WCHAR buffer[BUFFER_SIZE] = { 0, };
	ULONG messageLength = 0;


	

	return messageLength;

}

BOOLEAN CommandReadFile() {
	BOOLEAN result = FALSE;
	printf("Command : READ\n");

	return result;
}

BOOLEAN CommandWriteFile() {
	BOOLEAN result = FALSE;
	PWCHAR pBuffer = NULL;
	ULONG length = 0;

	length = InputMessage(&pBuffer);

	printf("Command : WRITE\n");

	return result;
}

BOOLEAN DisplayMenu() {
	ULONG command = 0;
	BOOLEAN result = FALSE;

	printf("---------------  SHOW MENU  ---------------\n");
	printf("\t\t1. READ\n");
	printf("\t\t2. WRITE\n");
	printf("-------------------------------------------\n");

	scanf_s("%d", &command);

	
	switch (command) {
	case 1:
		result = CommandReadFile();
		break;
	case 2:
		result = CommandWriteFile();
		break;
	default:
		return result;
	}



	return result;
}


void main()
{
	hDevice = CreateFile(deviceName, GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, (HANDLE)NULL);
	if (hDevice == INVALID_HANDLE_VALUE) {
		printf("Failed to open the handle...\n");
		getchar();
	}
	else {
		printf("Succeded to open the handle...\n");
		while (DisplayMenu()) {
			printf("\n");
		}

		CloseHandle(hDevice);
		printf("Closing....\n");
		getchar();
	}

}
