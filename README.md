A simple command line tool to assist with encrypting/obfuscating your payload. 

![succubus_png](img/suc_gif.gif)

How to use:

Para criptografar:

AES:
<code>encription-this-shit.exe -e payload.bin aes C:\Users\Documents\pasta_teste</code><br>

RC4:
<code>encription-this-shit.exe -e payload.bin rc4 C:\Users\Documents\pasta_teste</code><br>

Para descriptografar:

AES:<br>
<code>encription-this-shit.exe <op_type> <payload.bin> <key.bin> <iv.bin> <payload_size> <enc_type><br></code>
<code>encription-this-shit.exe -d payloadbin.bin key.bin iv.bin 288 aes</code>

RC4:<br>
<code>encription-this-shit.exe <op_type> <payload.bin> <enc_type_> <payload_size><br></code>
<code>encription-this-shit.exe -d payload.bin key.bin 270 rc4</code>

<br>

O payload.bin deve ser um arquivo contendo os bytes brutos (raw bytes) do seu payload (Normalmente criado usando o HxD). Para garantir uma melhor segurança na criptografia, tanto a chave de 32 bytes quanto o IV (Vetor de Inicialização) de 16 bytes são gerados aleatoriamente.

<hr>

#### Funcionalidades atuais:

- Criptografia de Payload com AES-256 utilizando a Bcrypt da WinAPI
- Criptografia de Payload com RC4 através da SystemFunction032 da Advapi
- Salva todas as informações em uma pasta determinada por argv[4] (Key, IV e Payload para AES) e (Key e Payload se for RC4)
- Permite visualizar o Output descriptografado para fins de debug

<hr>

#### Implementações Futuras:

- Ofuscação de Payload utilizando formatação IPV4/IPV6
- Fornecer funções de descriptografia para implementações em outras aplicações C

<hr>

Para compilar utilize exclusivamente o MSVC<br>
<code>cl.exe /utf-8 /Iinclude /Fo"bin\obj\\" /Fe"bin\encrypt-this-shit.exe" src\*.c</code>

Se possível, essa deve ser a estrutura do projeto na hora da compilação:<br>
```
├── bin/
    └── obj/
├── img/
└── include/
└── src/
```

O .exe será salvo dentro de bin/

<hr>

![usage](img/usage_screenshot.png)