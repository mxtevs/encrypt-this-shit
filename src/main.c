#include <stdio.h>
#include <windows.h>
#include "aes_enc.h"
#include "rc4_enc.h"

// cl.exe /utf-8 /Iinclude /Fo"bin\obj\\" /Fe"bin\encrypt-this-shit.exe" src\*.c
int main(int argc, char *argv[]) {
    art(); 
    
    BYTE ourKey[32];
    NTSTATUS STATUS = NULL;
    char* basePath;
    FILE *file = NULL;
    AES aes_struct = {0};
    LONG payloadSize = NULL;
    PBYTE Data = NULL;
    PBYTE DataCipher = NULL;
    PBYTE KeyCipher = NULL;
    PBYTE ivCipher = NULL;

    if (argc < 3) {
        usage();
        return 1;
    }

    // Função para gerar 32 bytes aleatorios necessários para chave
    fillKey(&ourKey, sizeof(ourKey));
    

    if (strcmp(argv[1], "-e") == 0) {
        fopen_s(&file, argv[2], "rb");

        if (!file) {
            printf("Falha ao abrir arquivo!\n");
            goto cleanup;
        }

        fseek(file, 0, SEEK_END);
        payloadSize = ftell(file);
        fseek(file, 0, SEEK_SET);
        
        if (strcmp(argv[3], "aes") == 0) {

            Data = (PBYTE)HeapAlloc(GetProcessHeap(), 0, payloadSize);
            if (!Data) {
                printf("Falha ao alocar o buffer!\n");
                goto cleanup;
            }

            fread(Data, 1, payloadSize, file);

            aes_struct.pPlainData = Data;
            aes_struct.cbPlainDataSize = payloadSize;
			aes_struct.pKey = ourKey;
			aes_struct.pIV = NULL;
			aes_struct.ivSize = 16;
			aes_struct.pKeySize = 32;

            if (!enc_aes_data(&aes_struct)) {
                printf("[#] Encrypted Failed...\n");
                goto cleanup;
            }

            if(!generate_aes_output(&aes_struct, argv[4])){ 
                printf("[#] Falha ao gerar o seu output!\n");
                goto cleanup;
            }

            printf("\nSize in Bytes of Encrypted Data: %d\n", aes_struct.cbCipherDataSize);

            // Zera a estrutura AES ao final do processo
            memset(&aes_struct, 0, sizeof(aes_struct));

        } else if (strcmp(argv[3], "rc4") == 0) {

            Data = HeapAlloc(GetProcessHeap(), 0, payloadSize);
            if (!Data) {
                printf("Falha ao alocar o buffer!\n");
                goto cleanup;
            }

            fread(Data, 1, payloadSize, file);

            USTRING rc4_Data = {
                .Buffer = Data,
                .Length = payloadSize, 
                .MaximumLength = payloadSize,
            };

            USTRING Key = {
                .Buffer = ourKey,
                .Length = sizeof(ourKey),
                .MaximumLength = sizeof(ourKey)
            };

            funcToSystemFunction032 pSystemFunc032 = (funcToSystemFunction032) GetProcAddress(LoadLibraryA("Advapi32"), "SystemFunction032");
            if(!pSystemFunc032) {
                printf("[#] Falha ao recuperar a função SystemFunction032 e Advapi32.dll\nErro Code: %d\n", GetLastError());
                goto cleanup;
            }

            STATUS = pSystemFunc032(&rc4_Data, &Key);
            if(STATUS != 0x0) {
                printf("[#] Falha ao realizar a criptografia RC4!\n");
                goto cleanup;
            }
            
    
            if(!generate_rc4_output(&Data, &Key, argv[4])) { 
                printf("Falha ao gerar seu RC4 output!\n");
                goto cleanup;
            }
            
        } else {
            usage();
        }



    } else if (strcmp(argv[1], "-d") == 0) {

        if (strcmp(argv[6], "aes") == 0) {

            
            DataCipher = read_bin_file(argv[2]); 
            KeyCipher = read_bin_file(argv[3]);
            ivCipher = read_bin_file(argv[4]);

            aes_struct.pCipherData = DataCipher;
            aes_struct.pKey = KeyCipher;
            aes_struct.pIV = ivCipher;
            aes_struct.ivSize = 16;
			aes_struct.pKeySize = 32;
            aes_struct.cbCipherDataSize = (DWORD)atoi(argv[5]);
            
            if(!dec_aes_data(&aes_struct)) {
                printf("FALHOU PAPAI!\n");
                goto cleanup;
            }

            for(int i = 0; i < 288; i++) {
                printf("0x%0.2X ", ((PBYTE)aes_struct.pPlainData)[i]);
            }

            // Decriptação AES
        } else if (strcmp(argv[3], "rc4") == 0) {
            // Decriptação RC4
        }

    } else {
        usage();
    }

    goto cleanup;

    cleanup:
	if (file) {
		fclose(file);
	}

    if (Data) {
        HeapFree(GetProcessHeap(), 0, Data);
    }

    if (aes_struct.pCipherData) {
        HeapFree(GetProcessHeap(), 0, aes_struct.pCipherData);
    }

    if (DataCipher) {
        HeapFree(GetProcessHeap(), 0, DataCipher);
    }

    if (KeyCipher) {
        HeapFree(GetProcessHeap(), 0, KeyCipher);
    }

    if (ivCipher) {
        HeapFree(GetProcessHeap(), 0, ivCipher);
    }

	HeapFree(GetProcessHeap(), 0, aes_struct.pPlainData);
	HeapFree(GetProcessHeap(), 0, aes_struct.pIV);

    return 0;
}