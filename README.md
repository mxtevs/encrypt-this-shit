A simple command line tool to assist with encrypting/obfuscating your payload. 
(Softaware não pronto para uso! Me ajude)

<br>

How to use:

<code><encryption-this-shit.exe> <op_type> <payload.bin> <enc_type> <output_directory></code>
<code>encription-this-shit.exe -d payload.bin aes C:\Users\Documents\teste</code>
<code>encription-this-shit.exe -d payload.bin rc4 C:\Users\Documents\teste</code>

O payload.bin deve ser um arquivo contendo os bytes brutos (raw bytes) do seu payload (Normalmente criado usando o HxD). Para garantir uma melhor segurança na criptografia, tanto a chave de 32 bytes quanto o IV (Vetor de Inicialização) de 16 bytes são gerados aleatoriamente.

<hr>

#### Funcionalidades atuais:

- Criptografia de Payload utilizando AES-256 com a 'Chave' e o 'Vetor de Inicialização' gerados randomicamente por execução
- Salva a chave, o IV e a key em arquivos no diretório indicado como quarto argumento da linha de comando (argv[4])

<hr>

#### Implementações Futuras:

- Criptografia RC4
- Ofuscação de Payload utilizando formatação IPV4/IPV6
- Fornecer funções de descriptografia para implementações em outras aplicações C