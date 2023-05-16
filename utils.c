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

void printOpenSSLError()
{
	unsigned long errCode;
	const char *errDesc;

	errCode = ERR_get_error();
	errDesc = ERR_error_string(errCode, NULL);
	printf("Error en OpenSSL: %s\n", errDesc);
}

int is_pem(char *cadena)
{
	char *punto;

	punto = strrchr(cadena, '.'); // Encuentra la última aparición del punto en la cadena
	if (punto != NULL)
		if (strncmp(punto + 1, "pem", 4) == 0)
			return (1);
	return (0);
}