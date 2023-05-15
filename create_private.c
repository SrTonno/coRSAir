#include "corsair.h"

void imprimir_clave_rsa(RSA *clave_rsa)
{
	BIO *bio_salida = BIO_new_fp(stdout, BIO_NOCLOSE);
	PEM_write_bio_RSAPrivateKey(bio_salida, clave_rsa, NULL, NULL, 0, NULL, NULL);
	BIO_free(bio_salida);
}

void calcular_clave_privada(BIGNUM *p ,BIGNUM *n, BIGNUM *k, const char *file)
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
	
	
	// GENERATE THE NEW PRIVATE KEY WITH THE VALUE
	RSA_set0_key(privkey, BN_dup(n), BN_dup(k), BN_dup(rem));
	RSA_set0_factors(privkey, BN_dup(p), BN_dup(q));
	RSA_set0_crt_params(privkey, BN_dup(dp), BN_dup(dq), BN_dup(rem2));
    desencrytar_fichero(privkey, file);
	imprimir_clave_rsa(privkey);
	
	
	BN_free(z);
	BN_free(q);
	BN_free(tq);
	BN_free(tp);
	BN_free(dq);
	BN_free(dp);
	BN_free(rem);
	BN_free(rem2);
	BN_CTX_free(ctx);
	RSA_free(privkey); //leaks por no poder liberar rsa
}