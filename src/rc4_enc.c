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
