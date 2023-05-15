#include "corsair.h"

void convertirExtension(char *cadena) {
    char *punto = strrchr(cadena, '.'); // Encuentra la última aparición del punto en la cadena

    if (punto != NULL)
        strcpy(punto + 1, "bin"); // Reemplaza la extensión por "bin"
}

void printOpenSSLError()
{
    unsigned long errCode;
    const char *errDesc;

    errCode = ERR_get_error();
    errDesc = ERR_error_string(errCode, NULL);
    printf("Error en OpenSSL: %s\n", errDesc);
}

void	leerfichero(char *file, RSA *key_rsa)
{
	int				fd;
	unsigned char	buffer[100];
	unsigned char	fileDescifrado[100];
	int				longitudDescifrado;
	size_t			bytes_leidos;

	fd = open(file, O_RDONLY);
	if (fd == -1)
		return ;
	bytes_leidos = 1;
	while (bytes_leidos > 0) 
	{
		bytes_leidos = read(fd, buffer, 100);
		//printf("%s\n", buffer);
		longitudDescifrado = RSA_private_decrypt(bytes_leidos, buffer, fileDescifrado, key_rsa, RSA_PKCS1_PADDING);
        if (longitudDescifrado < 0) {
			printf("Error al descifrar el archivo.\n");
            printOpenSSLError();
			close(fd);
			return;
		}
		printf("(%d)-%s",longitudDescifrado, fileDescifrado);
	}
	close(fd);
}

int desencrytar_fichero(RSA *key_rsa, const char *file)
{

	char	*bin;
	char	*input;

	bin = strdup(file);
	convertirExtension(bin);
	printf("\n%s\n", bin);
	leerfichero(bin, key_rsa);
	free(bin);
	return (1);
	
}