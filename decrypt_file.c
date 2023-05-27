/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   decrypt_file.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvillare <tvillare@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/16 17:16:35 by tvillare          #+#    #+#             */
/*   Updated: 2023/05/27 15:27:16 by tvillare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "corsair.h"

static void	convertirExtension(char *cadena) {
	char	*punto;

	punto= strrchr(cadena, '.'); // Encuentra la última aparición del punto en la cadena
	if (punto != NULL)
		strcpy(punto + 1, "bin"); // Reemplaza la extensión por "bin"
}

void	leerfichero(char *file, RSA *key_rsa)
{
	int				fd;
	unsigned char	buffer[129];
	unsigned char	*input;
	unsigned char	*fileDescifrado;
	int				longitudDescifrado;
	size_t			bytes_leidos;
	size_t			tmp;

	bytes_leidos = 0;
	tmp = 1;
	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		printf("Error: File not exist\n");
		return ;
	}
	while (tmp > 0)
	{
		tmp = read(fd, buffer, 1);
		bytes_leidos += tmp;
	}
	close(fd);
	fd = open(file, O_RDONLY);
	input = malloc((bytes_leidos + 1) * sizeof(unsigned char));
	fileDescifrado = malloc((bytes_leidos + 1)* sizeof(unsigned char));
	ft_bzero(fileDescifrado, bytes_leidos + 1);
	read(fd, input, bytes_leidos);
	input[bytes_leidos + 1] = '\0';
	RSA_private_decrypt(bytes_leidos, input, fileDescifrado, key_rsa, RSA_PKCS1_PADDING);
	//printOpenSSLError();
	printf("%s\n", fileDescifrado);
	free (fileDescifrado);
	free(input);
	close(fd);
}

int	desencrytar_fichero(RSA *key_rsa, const char *file)
{
	char	*bin;
	char	*input;

	bin = strdup(file);
	convertirExtension(bin);
	printf("\n'%s'\n", bin);
	leerfichero(bin, key_rsa);
	free(bin);
	return (1);
}
