#include "../include/utilities.h"

VOID usage() {
	printf("Operation types: -e to encrypt or -d to decrypt\n\n");

    printf("To encrypt: \n");
    printf("Usage: <encryption-this-shit.exe> <op_type> <payload.bin> <enc_type> <output_directory>\n");
	printf("\tEncryption Example: encrypt-this-shit.exe -e payload.bin aes /home/user/documents/\n\n");

    printf("\nTo Decrypt AES: \n");
    printf("Usage: <encrypt-this-shit.exe> <op_type> <payload.bin> <key.bin> <iv.bin> <size_bytes_cipher_data> <denc_type>\n");
    printf("\tDecryption Example:encrypt-this-shit.exe -d payload.bin key.bin iv.bin <128> aes\n\n");

    printf("To Decrypt RC4: \n");
    printf("encryption-this-shit.exe -d payload.bin key.bin rc4 <size_bytes_buffer>\n");

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

void art() {
	    printf(
        "         .m.                                   ,_                               \n"
        "         ' ;M;                                ,;m `                              \n"
        "           ;M;.           ,      ,           ;SMM;                              \n"
        "          ;;Mm;         ,;  ____  ;,         ;SMM;                              \n"
        "         ;;;MM;        ; (.MMMMMM.) ;       ,SSMM;;                             \n"
        "       ,;;;mMp'        l  ';mmmm;/  j       SSSMM;;                             \n"
        "     .;;;;;MM;         .\\,.mmSSSm,,/,      ,SSSMM;;;                            \n"
        "    ;;;;;;mMM;        .;MMmSSSSSSSmMm;     ;MSSMM;;;;                           \n"
        "   ;;;;;;mMSM;     ,_ ;MMmS;;;;;;mmmM;  -,;MMMMMMm;;;;                          \n"
        "  ;;;;;;;MMSMM;     \\\"*;M;( ( '') );m;*\"/ ;MMMMMM;;;;;,                         \n"
        " .;;;;;;mMMSMM;      \\(@;! _     _ !;@)/ ;MMMMMMMM;;;;;,                        \n"
        " ;;;;;;;MMSSSM;       ;,;.*o*> <*o*.;m; ;MMMMMMMMM;;;;;;,                       \n"
        ".;;;;;;;MMSSSMM;     ;Mm;           ;M;,MMMMMMMMMMm;;;;;;.                      \n"
        ";;;;;;;mmMSSSMMMM,   ;Mm;,   '-    ,;M;MMMMMMMSMMMMm;;;;;;;                     \n"
        ";;;;;;;MMMSSSMMMMMMMm;Mm;;,  ___  ,;SmM;MMMMMMSSMMMM;;;;;;;;                    \n"
        ";;'\\\";;;MMMSSSSMMMMMM;MMmS;;,  \\\"  ,;SmMM;MMMMMMSSMMMM;;;;;;;;.                   \n"
        "!   ;;;MMMSSSSSMMMMM;MMMmSS;;._.;;SSmMM;MMMMMMSSMMMM;;;;;;;;;                  \n"
        "    ;;;;*MSSSSSSMMMP;Mm*\\\"'q;'   `;p*\\\"*M;MMMMMSSSSMMM;;;;;;;;;                 \n"
        "    ';;;  ;SS*SSM*M;M;'     `-.        ;;MMMMSSSSSMM;;;;;;;;;,                \n"
        "     ;;;. ;P  `q; qMM.                 ';MMMMSSSSSMp' ';;;;;;;               \n"
        "     ;;;; ',    ; .mm!     \\.   `.   /  ;MMM' `qSS'    ';;;;;;               \n"
        "     ';;;       ' mmS';     ;     ,  `. ;'M'   `S       ';;;;;               \n"
        "      `;;.        mS;;`;    ;     ;    ;M,!     '        ';;;;               \n"
        "       ';;       .mS;;, ;   '. o  ;   oMM;                ;;;;               \n"
        "        ';;      MMmS;; `,   ;._.' -_.'MM;                 ;;;                \n"
        "         `;;     MMmS;;; ;   ;      ;  MM;                 ;;;                \n"
        "           `'.   'MMmS;; `;) ',    .' ,M;'                 ;;;                \n"
        "              \\    '' ''; ;   ;    ;  ;'                   ;;                 \n"
        "               ;        ; `,  ;    ;  ;                   ;;                 \n"
        "                        |. ;  ; (. ;  ;      _.-.         ;;                 \n"
        "           .-----..__  /   ;  ;   ;' ;\\  _.-' .- `.      ;;                \n"
        "         ;' ___      `*;   `; ';  ;  ; ;'  .-'    :      ;                  \n"
        "         ;     \\\"\\\"\\\"*-.   `.  ;  ;  ;  ; ' ,'      /       |                  \n"
        "         ',          `-_    (.--',`--'..'      .'        ',                 \n"
        "           `-_          `*-._'.\\\\\\\\\\;||\\\\\\\\)     ,'                           \n"
        "              `\\\"*-._        \\\"*`-ll_ll'l    ,'                                \n"
        "                 ,==;*-._           \\\"-.  .'                                   \n"
        "              _-'    \\\"*-=`*;-._        ;'                                     \n"
        "            .\\\"            ;'  ;\\\"*-.    `                                      \n"
        "            ;   ____      ;//'     \\\"-   `,                                   \n"
        "            `+   .-/                 \\\".\\\\;                                    \n"
        "              `*\\\" /                    \\\"'                                     \n"
    );
	printf("\n\t\t - [ ENCRYPT THIS SHIT ] - \n\n");
}