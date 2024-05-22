#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/evp.h>

int main (int argc, char *argv[]) {
  // Clave AES 256
  unsigned char key[] = {
    0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80,
    0x11, 0x21, 0x31, 0x41, 0x51, 0x61, 0x71, 0x81,
    0x12, 0x22, 0x32, 0x42, 0x52, 0x62, 0x72, 0x82,
    0x13, 0x23, 0x33, 0x43, 0x53, 0x63, 0x73, 0x83
  };
  // Vector de Inicialización
  unsigned char iv[] = {
    0x15, 0x25, 0x35, 0x45, 0x55, 0x65, 0x75, 0x85,
    0x16, 0x26, 0x36, 0x46, 0x56, 0x66, 0x76, 0x86
  };

  char               txt[1024];
  unsigned char      txt_enc[1024];
  long unsigned long txt_len = 0, txt_enc_len = 0;
  int                len, r;

  const EVP_CIPHER *c     = EVP_get_cipherbyname (argv[1]);
  size_t            bsize = EVP_CIPHER_get_block_size(c);

  // Inicializamos buffers
  memset (txt,     0, 1024); 
  memset (txt_enc, 0, 1024);
  
  printf ("%s", "Mensaje ? ");
  fgets (txt, 1024, stdin);
  
  txt_len = strlen(txt);
  txt_enc_len = 0;
  
  EVP_CIPHER_CTX *ctx;
  ctx = EVP_CIPHER_CTX_new();

  EVP_EncryptInit (ctx, c, key, iv);
  // Mientras tengamos bloques que cifrar.... Ciframos
  while (txt_len > bsize) {
    r = EVP_EncryptUpdate (ctx, txt_enc + txt_enc_len, &len, 
			        txt     + txt_enc_len, txt_len);
    txt_enc_len += len;
    txt_len -= len;
  }
  len = 0;
  
  // Procesa cualquier dato que quede pendiente
  EVP_EncryptFinal (ctx, txt_enc + txt_enc_len, &len);
  txt_enc_len += len;
  
  // Liberamos el contexto
  EVP_CIPHER_CTX_free (ctx);
  
  printf ("Total mensaje cifrado : %d\n", txt_enc_len);
  BIO_dump_fp (stdout, txt_enc, txt_enc_len);

  
  // Descifrado
  // Destruimos cadena original
  memset (txt, 0, 1024); 
  txt_len = 0;

  // Creammos e inicializamos un nuevo contexto de cifrado
  ctx = EVP_CIPHER_CTX_new();
  EVP_DecryptInit (ctx, c, key, iv);

  // Mientras tengamos datos pendientes, desencriptamos
  while (txt_enc_len > bsize) {
    EVP_DecryptUpdate (ctx, txt+txt_len, &len, txt_enc+txt_len, txt_enc_len);
    txt_len += len;
    txt_enc_len -= len;
  }
  len = 0;
  BIO_dump_fp (stdout, txt, txt_len);
  
  if ((r = EVP_DecryptFinal (ctx, txt + txt_len, &len)) == 0) 
    fprintf (stderr, "Datos de entrada corruptos\n");
  
  txt_len += len;

  // Mostramos resltado
  BIO_dump_fp (stdout, txt, txt_len);

  // Liberamos recursos
  EVP_CIPHER_CTX_free (ctx);
}

