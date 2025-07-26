#pragma once 

#ifndef UTILITIES_H
#define UTILITIES_H

#include <windows.h>
#include <stdio.h>
#include <time.h>

#pragma comment(lib, "bcrypt.lib")

#define NT_SUCCESS(STATUS) ((NTSTATUS)(Status) >= 0)

VOID usage();
VOID fillKey(BYTE* arr[], size_t arr_size);
VOID PrintHex(LPCWSTR Tittle, PBYTE Data, DWORD DataLen);

#endif