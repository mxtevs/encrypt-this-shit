#include "utilities.h"
#pragma once
#ifndef AES_ENC_H
#define AES_ENC_H

// Definição da Estrutura AES
typedef struct _AES {
	PVOID pPlainData;
	DWORD cbPlainDataSize;

	PVOID pCipherData;
	DWORD cbCipherDataSize;

	PVOID pKey;
	PVOID pIV;

	DWORD pKeySize;
	DWORD ivSize;
} AES, * pAES;

BOOL enc_aes_data(pAES pAes);

#endif