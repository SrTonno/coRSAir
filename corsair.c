/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corsair.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvillare <tvillare@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 17:16:47 by tvillare          #+#    #+#             */
/*   Updated: 2023/05/27 16:06:11 by tvillare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corsair.h"

t_openssl *read_public_key(const char *file)
{
	BIGNUM 		*n			=	BN_new();
	BIGNUM		*e			=	BN_new();
	BIO			*bio		=	NULL; // Estructura BIO para leer el archivo
	RSA			*rsa_key	=	NULL; // Clave RSA
	t_openssl	*stats;

	stats = malloc(1  * sizeof(t_openssl));
	if (stats == NULL)
		return (NULL);
	bio = BIO_new_file(file, "r"); // Crear una estructura BIO para leer el archivo
	if (bio == NULL)
	{
		ERR_print_errors_fp(stderr); // Imprimir errores si la apertura del archivo falla
		free(stats);
		return (NULL);
	}
	rsa_key = PEM_read_bio_RSA_PUBKEY(bio, &rsa_key, NULL, NULL); // Leer la clave pública RSA desde el archivo
	if (rsa_key == NULL)
	{
		ERR_print_errors_fp(stderr); // Imprimir errores si la lectura de la clave falla
		BIO_free_all(bio); // Liberar la estructura BIO
		free(stats);
		return (NULL);
	}
	save_info(n, e, rsa_key, stats);
	stats->file = file;
	RSA_free(rsa_key); // Liberar la estructura RSA
	BIO_free_all(bio); // Liberar la estructura BIO
	BN_free(n);
	BN_free(e);
	return (stats);
}

static void common_divisor(t_openssl **list)
{
	int		i;
	int		j;
	char 	*result_str;
	BIGNUM	*result	=	BN_new();
	BN_CTX	*ctx	=	BN_CTX_new();

	i = -1;
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

t_openssl	**create_list(int argc,char **argv)
{
	t_openssl	**list;
	int			i;
	int			j;

	i = 0;
	j = 0;
	list = malloc((argc + 1) * sizeof(t_openssl *));
	if (list == NULL)
		return (NULL);
	while (argv[++j] != NULL)
	{
		if (is_pem(argv[j]) == 1)
			list[i++] = read_public_key(argv[j]);
		if (list[i - 1] == NULL)
		{
			free_all(list);
			return(NULL);
		}
	}
	list[i] = NULL;
	return (list);
}

int main(int argc, char **argv)
{
	t_openssl	**list;

	if (argc < 3)
	{
		printf("Minimo 2 ficheros .pem\n");
		return (0);
	}
	OpenSSL_add_all_algorithms(); // Inicializar los algoritmos de OpenSSL
	ERR_load_crypto_strings(); // Cargar los mensajes de error de OpenSSL
	list = create_list(argc, argv);
	if (list == NULL)
		return (1);
	common_divisor(list);
	free_all(list);
	return (0);
}
