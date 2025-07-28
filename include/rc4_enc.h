#include <utilities.h>
#pragma once
#ifndef RC4_ENC_H
#define RC4_ENC_H

// Estrutura necessária para utilizar a SystemFunction032 
typedef struct {
    DWORD Length;
    DWORD MaximumLength;
    PVOID Buffer;
} USTRING, *PUSTRING;

// Definindo um ponteiro para a função SystemFunction032 
typedef NTSTATUS (NTAPI* funcToSystemFunction032) (struct USTRING* Data, struct USTRING* Key);

BOOL generate_rc4_output(struct USTRING *data, struct USTRING *key, char* basePath);
PVOID read_bin_rc4_file(char* path);
#endif