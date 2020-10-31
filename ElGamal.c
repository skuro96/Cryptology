// 高速指数演算法を用いたDiffie_Hellman鍵配送方式及び、ElGamal暗号のプログラム

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

long exp_operation(long g, long exp, long p);
long Diffie_Hellman(long a, long b, long g, long p);
long ElGamal_enc(long p, long K, long M);
long ElGamal_dec(long g, long p, long K, long C);
long Euclid(long a, long n);
int my_strlen(char *str);

int main(void)
{
	long a;
	long b;
	long p, g;
	long K;     				// 共通鍵

	a = 7;						// Aliceの秘密鍵 0<a<p
	b = 11;						// Bobの秘密鍵 0<b<p
	p = 167;					// p<32768の大きな素数
	char *M = "Hello World!";	// 平文(メッセージ)

	int len = my_strlen(M);
	long *C = malloc(sizeof(long) * len);
	char *C1 = malloc(sizeof(char) * (len + 1));

	// (Z/pZ)の生成元gを見つける
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
		C1[i] = ElGamal_dec(g, p, K, C[i]);
	C1[len] = '\0';

	// 結果の表示
	printf("p = %ld, g = %ld\n", p, g);
	printf("平文		:%s\n", M);
	printf("平文(ascii)	:");
	for (int i = 0; i < len; i++)
		printf("%3d ", (int)M[i]);
	printf("\n");
	printf("共通鍵		:%ld\n", K);
	printf("暗号文		:");
	for (int i = 0; i < len; i++)
		printf("%3ld ", C[i]);
	printf("\n");
	printf("復号(ascii)	:");
	for (int i = 0; i < len; i++)
		printf("%3d ", (int)C1[i]);
	printf("\n");
	printf("復号		:%s\n", C1);

	free(C);
	free(C1);
	return (0);
}

// Diffie_Hellman鍵配送方式を用いて、共通鍵を生成
long Diffie_Hellman(long a ,long b, long g, long p)
{
	long A;
	long B;
	long key;

	// 穴埋め箇所
	// 4ヶ所、高速指数演算法の関数を使って計算してください
	// また、Bob側の鍵とAlice側の鍵が同じであることを確認してください

	// Bob側 B=g^b mod p の計算
	B = exp_operation(g, b, p);

	// Alice側 A=g^a mod p の計算
	A = exp_operation(g, a, p);

	// Bob側 A^b=g^ab mod p の計算
	key = exp_operation(A, b, p);
	// printf("%ld\n", key);

	// Alice側 B^a=g^ab mod p の計算
	// printf("%ld\n", exp_operation(B, a, p));

	return (key);
}

// 高速指数演算法  g^exp mod pを高速に計算する関数
long exp_operation(long g, long exp, long p)
{
	// 穴埋め箇所
	// 高速指数演算法のプログラムを書いてください
	if (exp == 0)
		return (1);
	else if (exp % 2 == 0)
		return (exp_operation(g * g % p, exp / 2, p) % p);
	else
		return (g * exp_operation(g, exp - 1, p) % p);
}

// ElGamal暗号の暗号化
long ElGamal_enc(long p, long K, long M)
{
	return (K * M % p);
}

// ElGamal暗号の復号化
long ElGamal_dec(long g, long p, long K, long C)
{
	long K1;

	// Kの逆元K^(-1)を求める 授業プリントでは K=α^b となっている
	// K*K^(-1) mod p=1 となる K^(-1)を求める
	K1 = Euclid(K, p);
	return ((C * K1) % p);	
}

// 拡張Euclid互除法
long Euclid(long a, long n)
{
	long u1, u2;		// 拡張部分
	long r1, r2;		// Euclid 互除法
	long q, w;			// q は商（r2/r1の床関数）の保持。w は r_k, u_k の計算用一時変数

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
	return ((u2 + n) % n);	// u2 が負数のこともあるので n を足して mod n しておく
}

int my_strlen(char *str)
{
	int i;

	i = 0;
	while (str[i] != '\0')
		i++;
	return (i);
}

/*
p = 167, g = 5
平文		:Hello World!
平文(ascii)	: 72 101 108 108 111  32  87 111 114 108 100  33 
共通鍵		:151
暗号文		: 17  54 109 109  61 156 111  61  13 109  70 140 
復号(ascii)	: 72 101 108 108 111  32  87 111 114 108 100  33 
復号		:Hello World!
*/
