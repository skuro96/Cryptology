// 高速指数演算法を用いたDiffie_Hellman鍵配送方式及び、ElGamal暗号のプログラム

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

long exp_operation(long g, long exp, long p);
long Diffie_Hellman(long a, long b, long g, long p);
long ElGamal_enc(long p, long K, long M);
long ElGamal_dec(long p, long K, long C);
long Euclid(long a, long n);

int main(void)
{
	long a;			// Aliceの秘密鍵 0<a<p
	long b;			// Bobの秘密鍵 0<b<p
	long p;			// p<32768の大きな素数
	long g;			// (Z/pZ)の生成元
	long C[256];
	long K;			// 共通鍵

	a = 7;						
	b = 11;						
	p = 167;					
	char *M = "Hello World!";	// 平文(メッセージ)

	int len = strlen(M);
	char *C1 = malloc(sizeof(char) * (len + 1));

	g = 2;
	while (exp_operation(g, (p - 1) / 2, p) == 1)
		g++;

	// Diffie_Hellman鍵配送方式を用いて共通鍵(K)を生成
	K = Diffie_Hellman(a, b, g, p);

	// ElGamal暗号と鍵Kを使って、平文Mを暗号文Cに暗号化する
	for (int i = 0; i < len; i++)
		C[i] = ElGamal_enc(p, K, M[i]);

	// 暗号文Cと鍵Kを用いて、復号を行う
	for (int i = 0; i < len; i++)
		C1[i] = ElGamal_dec(p, K, C[i]);
	C1[len] = '\0';

	// 結果の表示
	printf("p = %ld, g = %ld\n", p, g);
	printf("平文		:%s\n", M);
	printf("平文(ascii)	:");
	for (int i = 0; i < len; i++)
		printf("%d ", (int)M[i]);
	printf("\n");
	printf("共通鍵		:%ld\n", K);
	printf("暗号文		:");
	for (int i = 0; i < len; i++)
		printf("%ld ", C[i]);
	printf("\n");
	printf("復号(ascii)	:");
	for (int i = 0; i < len; i++)
		printf("%d ", (int)C1[i]);
	printf("\n");
	printf("復号		:%s\n", C1);
}

// Diffie_Hellman鍵配送方式を用いて、共通鍵を生成
long Diffie_Hellman(long a ,long b, long g, long p)
{
	long A;
	long key;

	// Alice側 A=g^a mod p の計算
	A = exp_operation(g, a, p);

	// Bob側 A^b=g^ab mod p の計算
	key = exp_operation(A, b, p);
	return (key);
}

// 高速指数演算法  g^exp mod pを高速に計算する関数
long exp_operation(long g, long exp, long p)
{
	if (exp == 0)
		return (1);
	else if (exp % 2 == 0)
		return (exp_operation(g * g, exp / 2, p) % p);
	else
		return (g * exp_operation(g, exp - 1, p) % p);
}

// ElGamal暗号の暗号化
long ElGamal_enc(long p, long K, long M)
{
	return (K * M % p);
}

// ElGamal暗号の復号化
long ElGamal_dec(long p, long K, long C)
{
	long K1;

	// K*K^(-1) mod p=1 となる K^(-1)を求める
	K1 = Euclid(K, p);
	return ((C * K1) % p);	
}

// 拡張Euclid互除法
long Euclid(long a, long n)
{
	long u1, u2;
	long r1, r2;
	long q, w;

	r1 = n; 
	r2 = a;
	u1 = 0; 
	u2 = 1;
	while(r1 > 0)
	{
		q = r2 / r1;
		w = r2 - q * r1;
		r2 = r1;
		r1 = w;
		w = u2 - q * u1;
		u2 = u1;
		u1 = w;
	}
	return ((u2 + n) % n);
}

/*
p = 167, g = 2
平文            :Hello World!
平文(ascii)     :72 101 108 108 111 32 87 111 114 108 100 33
共通鍵          :107
暗号文          :22 119 33 33 20 84 124 20 7 33 12 24
復号(ascii)     :72 101 108 108 111 32 87 111 114 108 100 33
復号            :Hello World!
*/
