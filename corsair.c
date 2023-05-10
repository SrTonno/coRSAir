
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
	const BIGNUM *n, *e; // Punteros a los componentes n y e de la clave RSA
	t_openssl	*stats;
	BIO *bio = NULL; // Estructura BIO para leer el archivo
	RSA *rsa_key = NULL; // Clave RSA

	//printf("Clave publica %s\n", file);
	stats = malloc(1  * sizeof(t_openssl));
	stats->file = file;
	n = BN_new();
	e = BN_new();

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

	RSA_get0_key(rsa_key, &n, &e, NULL); // Obtener los componentes n y e de la clave RSA

	stats->n_hex = BN_bn2hex(n); // Convertir el componente n a formato hexadecimal
	stats->e_hex = BN_bn2hex(e); // Convertir el componente e a formato hexadecimal
	stats->n = BN_dup(n);
	stats->e = BN_dup(e);
	//printf("n: %s\n", stats->n_hex); // Imprimir el componente n en hexadecimal
	//printf("e: %s\n", stats->e_hex); // Imprimir el componente e en hexadecimal*/

	free_openssl(stats->n_hex, stats->e_hex, bio, rsa_key);
	//BN_free(n);
	//BN_free(e);
	return (stats);
}
void print_info(t_openssl *ssl)
{
	char		*n_hex, *e_hex;
	n_hex = BN_bn2hex(ssl->n); // Convertir el componente n a formato hexadecimal
	e_hex = BN_bn2hex(ssl->e); // Convertir el componente e a formato hexadecimal
	printf("n: %s\n", n_hex); // Imprimir el componente n en hexadecimal
	printf("e: %s\n", e_hex); // Imprimir el componente e en hexadecimal
	OPENSSL_free(ssl->n_hex); // Liberar la memoria asignada a la cadena n_hex
	OPENSSL_free(ssl->e_hex); // Liberar la memoria asignada a la cadena e_hex
} 

void calcular_clave_privada(BIGNUM *p ,BIGNUM *q)
{
	BIGNUM	*n = NB_new();
	BIGNUM	*z = NB_new();
	BN_CTX	*ctx = BN_CTX_new();
	BIGNUM	*temp1 = BN_new();
	BIGNUM	*temp2 = BN_new();

	//Calc n
	BN_mul(n, p, q, ctx);

	//calc z
	BN_sub(temp1, q, BN_value_one());
	BN_sub(temp2, p, BN_value_one());
	BN_mul(z, temp1, temp2, ctx);



	BN_free(z);
	BN_free(temp1);
	BN_free(temp2);
}
void common_divisor(t_openssl **list)
{
	int			i, j;
	BIGNUM		*result;
	char 		*result_str;
	char		*n_hex, *e_hex; 
	int 		tot;
	BN_CTX		*ctx = BN_CTX_new();

	i = -1; 
	tot = 0;
	result = BN_new();
	while (list[++i] != NULL)
	{
		j = i;
		while (list[++j] != NULL)
		{
			BN_gcd(result, list[i]->n, list[j]->n, ctx);
			result_str = BN_bn2dec(result);
			if (strncmp(result_str, "1", 1) == 1 || strlen(result_str) != 1)
			{
				//print_info(list[i]);
				printf("%s y %s -> El máximo común divisor es: %s\n",list[i]->file, list[j]->file, result_str);
				tot++;
			}
		}
	}
	printf("%d\n", tot);
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
	return (0);
}