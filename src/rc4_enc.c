#include "..\include\rc4_enc.h"

BOOL generate_rc4_output(PUSTRING data, PUSTRING key, char* basePath) {
    FILE *file = NULL;
    BOOL ret = TRUE;
    errno_t err = NULL;

    char keyPath[MAX_PATH];
    char bufferPath[MAX_PATH];

    sprintf_s(keyPath, MAX_PATH, "%s\\%s" ,basePath, "key.bin");
    sprintf_s(bufferPath, MAX_PATH, "%s\\%s", basePath, "payload.bin");

    err = fopen_s(&file, bufferPath, "wb");
    if (!file) {
        printf("[#] Falha ao gerar a saída do seu payload!\nErro Code: %d\n", err);
        ret = FALSE;
    }


    fwrite(data->Buffer, sizeof(BYTE), data->Length, file);
    fclose(file);

    err = fopen_s(&file, keyPath, "wb");
    if (!file) {
        printf("[#] Falha ao gerar a saída da sua chave RC4!\nErro Code: %d\n", err);
        ret = FALSE;
    }

    fwrite(key->Buffer, sizeof(BYTE), key->Length, file);
    fclose(file);

    return ret;
}

PBYTE read_bin_rc4_file(char* path) {
    FILE *file = NULL;
    DWORD payloadSize = NULL;

    fopen_s(&file, path, "rb");
    if(!file) {
        printf("[#] Não foi possível ler o arquivo!\n");
		exit(1);
    }

    fseek(file, 0, SEEK_END);
    payloadSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    PBYTE temp = HeapAlloc(GetProcessHeap(), 0, payloadSize);
	if (!temp) {
		printf("[#] Falha ao alocar recurso!\n");
		exit(1);
	}

    fread(temp, 1, payloadSize, file);
    fclose(file);

    return temp;
}
