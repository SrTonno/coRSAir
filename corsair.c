
#include "corsair.h"


void free_openssl(char *n_hex, char *e_hex, BIO *bio, RSA *rsa_key)
{
	OPENSSL_free(n_hex); // Liberar la memoria asignada a la cadena n_hex
	OPENSSL_free(e_hex); // Liberar la memoria asignada a la cadena e_hex
	RSA_free(rsa_key); // Liberar la estructura RSA
	BIO_free_all(bio); // Liberar la estructura BIO
	EVP_cleanup(); // Limpiar los módulos de OpenSSL
	ERR_free_strings(); // Liberar las cadenas de error de OpenSSL
}

void read_public_key(const char *file)
{
	const BIGNUM *n, *e; // Punteros a los componentes n y e de la clave RSA
	char *n_hex, *e_hex; // Cadenas para almacenar los componentes n y e en formato hexadecimal
	BIO *bio = NULL; // Estructura BIO para leer el archivo
	RSA *rsa_key = NULL; // Clave RSA

	printf("Clave publica\n");
	OpenSSL_add_all_algorithms(); // Inicializar los algoritmos de OpenSSL
	ERR_load_crypto_strings(); // Cargar los mensajes de error de OpenSSL
	bio = BIO_new_file(file, "r"); // Crear una estructura BIO para leer el archivo
	if (bio == NULL)
	{
		ERR_print_errors_fp(stderr); // Imprimir errores si la apertura del archivo falla
		exit(EXIT_FAILURE);
	}

	rsa_key = PEM_read_bio_RSA_PUBKEY(bio, &rsa_key, NULL, NULL); // Leer la clave pública RSA desde el archivo
	if (rsa_key == NULL) {
		ERR_print_errors_fp(stderr); // Imprimir errores si la lectura de la clave falla
		BIO_free_all(bio); // Liberar la estructura BIO
		exit(EXIT_FAILURE);
	}

	RSA_get0_key(rsa_key, &n, &e, NULL); // Obtener los componentes n y e de la clave RSA
	n_hex = BN_bn2hex(n); // Convertir el componente n a formato hexadecimal
	e_hex = BN_bn2hex(e); // Convertir el componente e a formato hexadecimal

	printf("n: %s\n", n_hex); // Imprimir el componente n en hexadecimal
	printf("e: %s\n", e_hex); // Imprimir el componente e en hexadecimal

	free_openssl(n_hex, e_hex, bio, rsa_key);
}


int main(void)
{
	read_public_key("public.pem");
	return (0);
}