#include <stdio.h>
#include <windows.h>
#include "aes_enc.h"

// cl.exe /Iinclude /Fo"bin\obj\\" /Fe"bin\encrypt-this-shit.exe" src\*.c
int main(int argc, char *argv[]) {
    
    BYTE ourKey[32];
    char keyPath[MAX_PATH]; 
    char ivPath[MAX_PATH];
    char payloadPath[MAX_PATH];
    char* basePath;
    
    FILE *file = NULL;
    
    AES aes_struct = {0};
    LONG payloadSize = NULL;

    PBYTE Data = NULL;
    
    errno_t err = NULL;

    if (argc < 3) {
        usage();
        return 1;
    }

    // Função para gerar 32 bytes aleatorios necessários para chave
    fillKey(&ourKey, sizeof(ourKey));
    

    if (strcmp(argv[1], "-e") == 0) {
        
        if (strcmp(argv[3], "aes") == 0) {
            fopen_s(&file, argv[2], "rb");

            if (!file) {
                printf("Falha ao abrir arquivo!\n");
                goto cleanup;
            }

            fseek(file, 0, SEEK_END);
            payloadSize = ftell(file);
            fseek(file, 0, SEEK_SET);

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
                printf("DEU ERRO!!!!!");
            }


        } else if (strcmp(argv[3], "rc4") == 0) {
            // Implementa criptografia RC4
            printf("rc4");
        } else {
            usage();
        }



    } else if (strcmp(argv[1], "-d") == 0) {
        // Decriptação



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

	HeapFree(GetProcessHeap(), 0, aes_struct.pPlainData);
	HeapFree(GetProcessHeap(), 0, aes_struct.pIV);

    return 0;
}