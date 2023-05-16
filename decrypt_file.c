#include "corsair.h"


/*unsigned char *strjoin(unsigned char *str1, unsigned char *str2) {
    size_t len1 = strlen((char *)str1);
    size_t len2 = strlen((char *)str2);
	unsigned char *result = malloc((len1 + len2 + 1) * sizeof(char));

    if (result == NULL) {
        printf("Error de memoria al concatenar las cadenas.\n");
        return NULL;
    }

    strcpy((char *) result, (char *)str1);
    strcat((char *) result, (char *)str2);
	free(str1);
    return result;
}*/

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

		/*longitudDescifrado = RSA_private_decrypt(bytes_leidos, buffer, fileDescifrado, key_rsa, RSA_PKCS1_PADDING);
        if (longitudDescifrado < 0) {
			printf("Error al descifrar el archivo.\n");
            printOpenSSLError();
			close(fd);
			return;
		}
		printf("(%d)-%s",longitudDescifrado, fileDescifrado);*/

void	leerfichero(char *file, RSA *key_rsa)
{
	int				fd;
	int				tmp;
	unsigned char	buffer[100];
	unsigned char	*input;
	unsigned char	*fileDescifrado;
	int				longitudDescifrado;
	size_t			bytes_leidos;
	RSA				*tmp_rsa = RSA_new();

	
	
	if (fd == -1)
		return ;
	
	bytes_leidos = 1;
	tmp = 1;
	fd = open(file, O_RDONLY);
	while (tmp > 0)
	{
		tmp = read(fd, buffer, 100);
		bytes_leidos += tmp;
	}
	printf("Hola\n");
	close(fd);
	fd = open(file, O_RDONLY);
	input = malloc((bytes_leidos + 1) * sizeof(unsigned char));
	fileDescifrado = malloc((bytes_leidos + 1)* sizeof(unsigned char));
	read(fd, input, bytes_leidos);
	input[bytes_leidos + 1] = '\0';
	//RSA_copy(tmp_rsa, key_rsa);
	tmp_rsa = RSA_dup(key_rsa);
	RSA_private_decrypt(bytes_leidos, input, fileDescifrado, tmp_rsa, RSA_PKCS1_PADDING);
	printOpenSSLError();
	printf("%s\n", fileDescifrado);
	free(input);
	close(fd);
}

int desencrytar_fichero(RSA *key_rsa, const char *file)
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