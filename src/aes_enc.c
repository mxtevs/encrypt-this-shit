#include "../include/aes_enc.h"

BOOL enc_aes_data(pAES pAes) {

	BOOL bRetorno = 1;
	NTSTATUS Status = NULL;

	BCRYPT_ALG_HANDLE hAes = NULL;
	BCRYPT_KEY_HANDLE hKey = NULL;

	DWORD dwBlockSize = NULL;
	DWORD dwKeyObjectSize = NULL;

	PVOID pbKeyObject = NULL;
	
	DWORD cbOutput = NULL;

	DWORD cbCipherText = NULL;
	PVOID pbCipherText = NULL;

	// Abre o provedor de criptografia do Windows e retorna um handle que será salvo em hAes
	Status = BCryptOpenAlgorithmProvider(&hAes, BCRYPT_AES_ALGORITHM, NULL, 0);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Failed to open AES provider! Error code: 0x%0.8X\n", Status);
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Busca o valor da propriedade 'BCRYPT_BLOCK_LENGTH' e salva em dwBlockSize
	Status = BCryptGetProperty(hAes, BCRYPT_BLOCK_LENGTH, (PVOID)&dwBlockSize, sizeof(DWORD), &cbOutput, 0);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Failed to query property 'BCRYPT_BLOCK_LENGTH'! Error code: 0x%0.8X\n", Status);
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Verifica o valor de BCRYPT_BLOCK_LENGTH 
	// Deve ser 16 bytes independente do tamanho da chave
	if (dwBlockSize != 16) {
		printf("[#] Invalid Block Size!\n"); 
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Recupera o tamanho necessário para BCRYPT_OBJECT_LENGTH (Key-Object)
	Status = BCryptGetProperty(hAes, BCRYPT_OBJECT_LENGTH, (PVOID)&dwKeyObjectSize, sizeof(DWORD), &cbOutput, 0);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Failed to query property 'BCRYPT_OBJECT_LENGTH'! Error code: 0x%0.8X\n", Status);
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Define o modo de operação para AES-CBC
	// Define que os blocos de dados serão criptografados sequencialmente
	Status = BCryptSetProperty(hAes, BCRYPT_CHAINING_MODE, (PBYTE)BCRYPT_CHAIN_MODE_CBC, sizeof(BCRYPT_CHAIN_MODE_CBC), 0);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Failed to set BCRYPT_CHAINING_MODE! Error code: 0x%0.8X\n", Status);
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Aloca um espaço na memória para nosso Key Object
	// O tamanho do espaço de memória necessário para alocar o Key-Object foi adquirido anteriormente utilizando BCryptGetProperty
	pbKeyObject = HeapAlloc(GetProcessHeap(), 0, dwKeyObjectSize);
	if (!pbKeyObject) {
		printf("[#] Falha ao alocar o espaço para sua Key-Object!\n");
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Aloca um espaço na memória para armazenar nosso IV
	// O endereço base alocado é salvo na nossa estrutura pAes no campo pIV que foi recebida no parametro da função
	pAes->pIV = HeapAlloc(GetProcessHeap(), 0, dwBlockSize);
	if (!pAes->pIV) {
		printf("[#] Falha ao alocar o espaço para seu IV!\n");
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Gera o "Key-Object" a partir de uma chave fornecida através da nossa estrutura AES
	// um handle para o key-object é salvo em hKey
	Status = BCryptGenerateSymmetricKey(hAes, &hKey, (PVOID)pbKeyObject, dwKeyObjectSize, pAes->pKey, pAes->pKeySize, 0);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Falha ao Gerar o seu Key-Object! Erro code: 0x%0.8X\n", Status);
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Gera um IV aleatoriamente e salva em pAes-pIV
	Status = BCryptGenRandom(NULL, pAes->pIV, dwBlockSize, BCRYPT_USE_SYSTEM_PREFERRED_RNG);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Falha ao Gerar o seu IV!\n");
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Primeira chamada para BCryptEncrypt, nessa primeira chamada o parametro pbOutput é NULL. Isso é feito afim de obter o tamanho necessário para o texto cifrado
	Status = BCryptEncrypt(hKey, (PUCHAR)pAes->pPlainData, pAes->cbPlainDataSize, NULL, pAes->pIV, pAes->ivSize, NULL, 0, &cbCipherText, BCRYPT_BLOCK_PADDING);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Falha ao realizar BCryptEncrypt[1]! Erro code: 0x%0.8X\n", Status);
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Aloca memória para receber o texto criptografado
	pbCipherText = (PVOID)HeapAlloc(GetProcessHeap(), 0, cbCipherText);
	if (!pbCipherText) {
		bRetorno = 0;
		printf("[#] Falha ao Alocar espaço para o texto cifrado!\n");
		goto Clear_Aes_Structure;
	}

	// Segunda chamada para BCryptEncrypt que realizará a criptografia 
	Status = BCryptEncrypt(hKey, (PUCHAR)pAes->pPlainData, pAes->cbPlainDataSize, NULL, pAes->pIV, pAes->ivSize, pbCipherText, cbCipherText, &cbOutput, BCRYPT_BLOCK_PADDING);
	if (!NT_SUCCESS(Status)) {
		printf("[#] Falha ao realizar a criptogafia dos dados! Erro Code: 0x%0.8X\n", Status);
		bRetorno = 0;
		goto Clear_Aes_Structure;
	}

	// Salva o texto criptografado na nossa estrutura AES
	pAes->pCipherData = pbCipherText;
	pAes->cbCipherDataSize = cbCipherText;
	
	goto Clear_Aes_Structure;

Clear_Aes_Structure:

	if (hKey) {
		BCryptDestroyKey(hKey);
	}
	if (pbKeyObject) {
		HeapFree(GetProcessHeap(), 0, pbKeyObject);
	}
	if (hAes) {
		BCryptCloseAlgorithmProvider(hAes, 0);
	}

	return bRetorno;
}
BOOL generate_aes_output(pAES pAes, char* basePath) {
	FILE* file = NULL;
	BOOL ret = TRUE;
	errno_t err = NULL;

	char keyPath[MAX_PATH]; 
    char ivPath[MAX_PATH];
    char payloadPath[MAX_PATH];

	sprintf_s(keyPath, MAX_PATH, "%s\\%s", basePath, "key.bin");
    sprintf_s(ivPath, MAX_PATH, "%s\\%s", basePath, "iv.bin");
	sprintf_s(payloadPath, MAX_PATH, "%s\\%s", basePath, "payload.bin");

	err = fopen_s(&file, payloadPath, "wb");
	if(!file) {
		printf("[#] Falha ao gerar a saída do seu payload!\nErro Code: %d", err);
		ret = FALSE;
	}

	fwrite(pAes->pCipherData, sizeof(BYTE), pAes->cbCipherDataSize, file);
	fclose(file);

	err = fopen_s(&file, ivPath, "wb");
	if(!file) {
		printf("[#] Falha ao gerar a saída do seu IV!\nErro Code: %d", err);
		ret = FALSE;
	}

	fwrite(pAes->pIV, sizeof(BYTE), pAes->ivSize, file);
	fclose(file);

	err = fopen_s(&file, keyPath, "wb");
	if (!file) {
		printf("[#] Falha ao gerar a saída da sua Key!\nErro Code: %d", err);
		ret = FALSE;
	}

	fwrite(pAes->pKey, sizeof(BYTE), pAes->pKeySize, file);
	fclose(file);

	return ret;

}