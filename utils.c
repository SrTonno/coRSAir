/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvillare <tvillare@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 17:17:16 by tvillare          #+#    #+#             */
/*   Updated: 2023/05/27 16:02:34 by tvillare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corsair.h"

void	ft_bzero(void *s, size_t n)
{
	size_t	i;

	i = 0;
	while (i < n)
	{
		((char *)s)[i] = '\0';
		i++;
	}
}
/*
void printOpenSSLError()
{
	unsigned long	errCode;
	const char		*errDesc;

	errCode = ERR_get_error();
	errDesc = ERR_error_string(errCode, NULL);
	printf("Error en OpenSSL: %s\n", errDesc);
}*/

int	is_pem(char *cadena)
{
	char	*punto;

	punto = strrchr(cadena, '.'); // Encuentra la última aparición del punto en la cadena
	if (punto != NULL)
		if (strncmp(punto, ".pem", 5) == 0)
			return (1);
	return (0);
}

void	save_info(const BIGNUM *n, const BIGNUM *e, RSA *rsa_key, t_openssl *stats)
{
	RSA_get0_key(rsa_key, &n, &e, NULL); // Obtener los componentes n y e de la clave RSA

	stats->n = BN_dup(n);
	stats->e = BN_dup(e);
}

void	free_all(t_openssl	**list)
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
