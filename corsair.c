/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corsair.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvillare <tvillare@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/02 11:51:49 by tvillare          #+#    #+#             */
/*   Updated: 2023/05/02 12:51:30 by tvillare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <math.h>
#include <string.h>
#include <openssl/opensslv.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/rsa.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>



int main()
{
	EVP_PKEY_CTX *ctx;
	EVP_PKEY *pkey;
	int rc;

	// Crear un nuevo contexto de clave
	ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_RSA, NULL);

	// Establecer la longitud de la clave en 64 bits
	rc = EVP_PKEY_CTX_set_rsa_keygen_bits(ctx, 2048);
	if (rc <= 0) {
		printf("Error al establecer la longitud de la clave RSA\n");
		return 1;
	}

	// Crear una nueva clave RSA
	pkey = EVP_PKEY_new();
	rc = EVP_PKEY_keygen_init(ctx);
	if (rc <= 0) {
		printf("Error al inicializar la generación de claves RSA\n");
		return 1;
	}
	rc = EVP_PKEY_keygen(ctx, &pkey);
	if (rc <= 0) {
		printf("Error al generar la clave RSA\n");
		return 1;
	}

	// Hacer algo con la clave...

	// Liberar recursos
	EVP_PKEY_CTX_free(ctx);
	EVP_PKEY_free(pkey);

	return 0;

}
