/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   corsair.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tvillare <tvillare@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/08 18:27:51 by tvillare          #+#    #+#             */
/*   Updated: 2023/05/08 18:27:52 by tvillare         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CORSAIR_H
# define CORSAIR_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>

# include <math.h>
# include <string.h>

# include <openssl/aes.h>
# include <openssl/bio.h>
# include <openssl/bn.h>

# include <openssl/crypto.h>
# include <openssl/err.h>
# include <openssl/evp.h>

# include <openssl/objects.h>
# include <openssl/pem.h>
# include <openssl/pkcs12.h>
# include <openssl/rsa.h>

# include <openssl/ssl.h>
# include <openssl/x509.h>
# include <openssl/x509v3.h>


typedef struct s_oppenssl
{
	char	*texa t;
	int		type;
}	t_openssl;

#endif
