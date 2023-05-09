
#include "corsair.h"


void free_openssl(char *n_hex, char *e_hex, BIO *bio, RSA *rsa_key)
{
	//OPENSSL_free(n_hex); // Liberar la memoria asignada a la cadena n_hex
	//OPENSSL_free(e_hex); // Liberar la memoria asignada a la cadena e_hex
	RSA_free(rsa_key); // Liberar la estructura RSA
	BIO_free_all(bio); // Liberar la estructura BIO
	//EVP_cleanup(); // Limpiar los módulos de OpenSSL
	//ERR_free_strings(); // Liberar las cadenas de error de OpenSSL
}

t_openssl *read_public_key(const char *file)
{
	//const BIGNUM *n, *e; // Punteros a los componentes n y e de la clave RSA
	t_openssl	*stats;
	BIO *bio = NULL; // Estructura BIO para leer el archivo
	RSA *rsa_key = NULL; // Clave RSA

	printf("Clave publica %s\n", file);
	stats = malloc(1  * sizeof(t_openssl));
	stats->file = file;
	//OpenSSL_add_all_algorithms(); // Inicializar los algoritmos de OpenSSL
	//ERR_load_crypto_strings(); // Cargar los mensajes de error de OpenSSL
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

	RSA_get0_key(rsa_key, &stats->n, &stats->e, NULL); // Obtener los componentes n y e de la clave RSA

	stats->n_hex = BN_bn2hex(stats->n); // Convertir el componente n a formato hexadecimal
	stats->e_hex = BN_bn2hex(stats->e); // Convertir el componente e a formato hexadecimal
	printf("n: %s\n", stats->n_hex); // Imprimir el componente n en hexadecimal
	printf("e: %s\n", stats->e_hex); // Imprimir el componente e en hexadecimal

	free_openssl(stats->n_hex, stats->e_hex, bio, rsa_key);
	return (stats);
}

void common_divisor(t_openssl **list)
{
	int	i;
	BIGNUM		*result;
	char 		*result_str;
	char		*n_hex, *e_hex;


	i = -1;
	while (list[++i] != NULL)
	{
		//n_hex = BN_bn2hex(list[i]->n); // Convertir el componente n a formato hexadecimal
		//e_hex = BN_bn2hex(list[i]->e); // Convertir el componente e a formato hexadecimal
		printf("n: %s\n", list[i]->n_hex); // Imprimir el componente n en hexadecimal
		printf("e: %s\n", list[i]->e_hex); // Imprimir el componente e en hexadecimal
		OPENSSL_free(list[i]->n_hex); // Liberar la memoria asignada a la cadena n_hex
		OPENSSL_free(list[i]->e_hex); // Liberar la memoria asignada a la cadena e_hex
		BN_gcd(result, list[i]->n, list[i + 1]->n, BN_CTX_new());
		result_str = BN_bn2dec(list[i]->n);
	    printf("El máximo común divisor es: %s\n", result_str);
	}
}

int main(int argc, char **argv)
{
	t_openssl	**list;
	int			i, j;
	char		*n_hex, *e_hex;
	BIGNUM		*result;
	char 		*result_str;

	OpenSSL_add_all_algorithms(); // Inicializar los algoritmos de OpenSSL
	ERR_load_crypto_strings(); // Cargar los mensajes de error de OpenSSL
	i = 0;
	j = 0;
	list = malloc((argc + 1) * sizeof(t_openssl *));
	while (argv[++j] != NULL)
		list[i++] = read_public_key(argv[j]);
	list[i] = NULL;
	i = -1;
	common_divisor(list);
	/*while (list[++i] != NULL)
	{
		//n_hex = BN_bn2hex(list[i]->n); // Convertir el componente n a formato hexadecimal
		//e_hex = BN_bn2hex(list[i]->e); // Convertir el componente e a formato hexadecimal
		printf("n: %s\n", list[i]->n_hex); // Imprimir el componente n en hexadecimal
		printf("e: %s\n", list[i]->e_hex); // Imprimir el componente e en hexadecimal
		OPENSSL_free(n_hex); // Liberar la memoria asignada a la cadena n_hex
		OPENSSL_free(e_hex); // Liberar la memoria asignada a la cadena e_hex
		BN_gcd(result, list[i]->n, list[i + 1]->n, BN_CTX_new());
		
		result_str = BN_bn2dec(list[i]->n);
	    printf("El máximo común divisor es: %s\n", result_str);
	}*/
	return (0);
}