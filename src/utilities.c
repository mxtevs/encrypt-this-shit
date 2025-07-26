#include "../include/utilities.h"

VOID usage() {
	printf("Operation types: -e to encrypt or -d to decrypt\n");
	printf("Usage: <encryption-that-shit.exe> <op_type> <payload.bin> <enc_type> <output_directory>\n\n");
	printf("Example: encrypt_that_shit.exe payload.bin aes /home/user/documents/output.txt\n");
}

VOID fillKey(BYTE* arr, size_t arr_size) {
	srand((unsigned int)time(NULL));
	for(int i = 0; i < arr_size; i++) {
		if ((rand() % 0xFF) != 0) {
			arr[i] = rand() % 0xFF;
		}
	}
}

void PrintHex(LPCWSTR Tittle, PBYTE Data, DWORD DataLen) {
	wprintf(L"%s", Tittle);
	for (DWORD i = 0; i < DataLen; i++) {
		wprintf(L"0x%02x ", Data[i]);
	}
	wprintf(L"\n");
}