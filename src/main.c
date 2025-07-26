#include <stdio.h>
#include <windows.h>
#include "aes_enc.h"

int main(int argc, char *argv[]) {
    AES myAes = {0};
    LONG payloadSize = NULL;
    FILE *file = NULL;
    FILE *fileOutput = NULL;
    BYTE ourKey[32];
    PBYTE Data = NULL;

    // for(int i = 0; i < argc; i++) {
    //     printf("argv[%d] = %s\n", i, argv[i]);
    // }


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

            myAes.pPlainData = Data;
            myAes.cbPlainDataSize = payloadSize;
			myAes.pKey = ourKey;
			myAes.pIV = NULL;
			myAes.ivSize = 16;
			myAes.pKeySize = 32;

            if (!enc_aes_data(&myAes)) {
                printf("[#] Encrypted Failed...\n");
                goto cleanup;
            }

            errno_t err = fopen_s(&fileOutput, argv[4], "wb");
            if (!fileOutput) {
                printf("[#] Failed to Generate Output\n[!] Erro fopen_s: %d\n", err);
                perror("fopen_s");
                goto cleanup;
            }

            fwrite(myAes.pCipherData, sizeof(BYTE), myAes.cbCipherDataSize, fileOutput);


			PrintHex(L"Key: ", myAes.pKey, myAes.pKeySize);
			printf("\n");
			PrintHex(L"IV: ", myAes.pIV, myAes.ivSize);

            printf("aes");
        } else if (strcmp(argv[3], "rc4") == 0) {
            // Implementa criptografia RC4
            printf("rc4");
        } else {
            usage();
        }



    } else if (strcmp(argv[1], "-d") == 0) {
        // Decriptação




    }

    cleanup:
	if (file) {
		fclose(file);
	}

    if (Data) {
        HeapFree(GetProcessHeap(), 0, Data);
    }

	if (fileOutput) {
		fclose(fileOutput);
	}

	// HeapFree(GetProcessHeap(), 0, myAes.pCipherData);
	// HeapFree(GetProcessHeap(), 0, myAes.pPlainData);
	// HeapFree(GetProcessHeap(), 0, myAes.pIV);

    return 0;
}