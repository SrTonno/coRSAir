
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

	//printf("Clave publica %s\n", file);
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

void reemplazar_subcadena(char *cadena, const char *str_buscar, const char *str_reemplazar)
{
	char *posicion = strstr(cadena, str_buscar);
	if (posicion != NULL)
	{
		strcpy(posicion, str_reemplazar);
	}
}

char *leerfichero(char *file)
{
	int	fd;
	
	fd = open(file, O_RDONLY);
	if (fd == -1) {
        perror("Error al abrir el archivo");
        return 1;
    }
}
int desencrytar_fichero(RSA *key_rsa, char *file)
{
	reemplazar_subcadena(file, ".pem", ".bin")
	
}

void imprimir_clave_rsa(RSA *clave_rsa)
{
	BIO *bio_salida = BIO_new_fp(stdout, BIO_NOCLOSE);
	PEM_write_bio_RSAPrivateKey(bio_salida, clave_rsa, NULL, NULL, 0, NULL, NULL);
	BIO_free(bio_salida);
}

void calcular_clave_privada(BIGNUM *p ,BIGNUM *n, BIGNUM *k, char *filname)
{
	BN_CTX	*ctx		=	BN_CTX_new();
	BIGNUM	*q			=	BN_new();
	BIGNUM	*z			=	BN_new();
	BIGNUM	*tq			=	BN_new();
	BIGNUM	*tp			=	BN_new();
	BIGNUM	*dp			=	BN_new();
	BIGNUM	*dq			=	BN_new();
	BIGNUM	*rem		=	BN_new();
	BIGNUM	*rem2 		=	BN_new();
	//BIGNUM	*tmp1		=	BN_new();
	//BIGNUM	*tmp2 		=	BN_new();
	RSA		*privkey	=	RSA_new();
	
	// CALCULATE THE OTHER PRIME NUMBER, AND OTHER VALUES NEEDED
	BN_div(q, NULL, n, p, ctx);
	BN_sub(tq, q, BN_value_one());
	BN_sub(tp, p, BN_value_one());
	BN_mul(z, tp, tq, ctx);
	BN_mod_inverse(rem, k, z, ctx);
	BN_mod(dp, rem, tp, ctx);
	BN_mod(dq, rem, tq, ctx);
	BN_mod_inverse(rem2, q, p, ctx);
	
	
	// GENERATE THE NEW PRIVATE KEY WITH THE VALUES
	//tmp1 = BN_dup(n);
	//tmp2 = BN_dup(k);
	RSA_set0_key(privkey, n, k, rem);
	RSA_set0_factors(privkey, p, q);
	RSA_set0_crt_params(privkey, dp, dq, rem2);
	imprimir_clave_rsa(privkey);
	
	BN_free(z);
	BN_free(q);
	BN_free(tq);
	BN_free(tp);
	BN_free(dq);
	BN_free(dp);
	BN_free(rem);
	BN_free(rem2);
	//BN_free(tmp1);
	//BN_free(tmp2);
	BN_CTX_free(ctx);
	//RSA_free(privkey); //leaks por no poder liberar rsa
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
				calcular_clave_privada(result, list[j]->n, list[j]->e, list[i]->file);
			}
			OPENSSL_free(result_str);
		}
	}
	BN_free(result);
	BN_CTX_free(ctx);
}

int main(int argc, char **argv)
{
	t_openssl	**list;
	int			i, j;

	OpenSSL_add_all_algorithms(); // Inicializar los algoritmos de OpenSSL
	ERR_load_crypto_strings(); // Cargar los mensajes de error de OpenSSL
	i = 0;
	j = 0;
	list = malloc((argc + 1) * sizeof(t_openssl *));
	while (argv[++j] != NULL)
		list[i++] = read_public_key(argv[j]);
	list[i] = NULL;
	printf("HOLA\n");
	common_divisor(list);
   
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
	//system("leaks -q corsair");
	

	return (0);
}