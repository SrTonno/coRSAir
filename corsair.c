
#include "corsair.h"

void save_info(const BIGNUM *n, const BIGNUM *e, RSA *rsa_key, t_openssl *stats)
{
	RSA_get0_key(rsa_key, &n, &e, NULL); // Obtener los componentes n y e de la clave RSA

	stats->n = BN_dup(n);
	stats->e = BN_dup(e);
}
t_openssl *read_public_key(const char *file)
{
	BIGNUM 			*n			=	BN_new();
	BIGNUM			*e			=	BN_new(); // Punteros a los componentes n y e de la clave RSA
	BIO				*bio		=	NULL; // Estructura BIO para leer el archivo
	RSA				*rsa_key	=	NULL; // Clave RSA
	t_openssl		*stats;

	stats = malloc(1  * sizeof(t_openssl));
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
	save_info(n, e, rsa_key, stats);
	stats->file = file;
	RSA_free(rsa_key); // Liberar la estructura RSA
	BIO_free_all(bio); // Liberar la estructura BIO
	BN_free(n);
	BN_free(e);
	
	return (stats);
}

void common_divisor(t_openssl **list)
{
	int			i, j;
	BIGNUM		*result;
	char 		*result_str;
	BN_CTX		*ctx	=	BN_CTX_new();

	i = -1; 
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
				printf("file->%s.\n",list[i]->file);
				calcular_clave_privada(result, list[i]->n, list[i]->e, list[i]->file);
				printf("file->%s.\n",list[j]->file);
				calcular_clave_privada(result, list[j]->n, list[j]->e, list[j]->file);
			}
			OPENSSL_free(result_str);
		}
	}
	BN_free(result);
	BN_CTX_free(ctx);
}
void free_all(t_openssl	**list)
{
	int	i;
	
	i = -1;
	while (list[++i] != NULL)
	{
		BN_free(list[i]->e);
		BN_free(list[i]->n);
		free(list[i]);
	}
	free(list);
	EVP_cleanup(); // Limpiar los módulos de OpenSSL
	ERR_free_strings(); // Liberar las cadenas de error de OpenSSL
}


int main(int argc, char **argv)
{
	t_openssl	**list;
	int			i, j;

	if (argc < 3)
	{
		printf("Minimo 2 ficheros .pem\n");
		return (0);
	}
	OpenSSL_add_all_algorithms(); // Inicializar los algoritmos de OpenSSL
	ERR_load_crypto_strings(); // Cargar los mensajes de error de OpenSSL
	i = 0;
	j = 0;
	list = malloc((argc + 1) * sizeof(t_openssl *));
	while (argv[++j] != NULL)
		if (is_pem(argv[j]) == 1)
			list[i++] = read_public_key(argv[j]);
	list[i] = NULL;
	common_divisor(list);
	free_all(list);
	system("leaks -q corsair");
	return (0);
}