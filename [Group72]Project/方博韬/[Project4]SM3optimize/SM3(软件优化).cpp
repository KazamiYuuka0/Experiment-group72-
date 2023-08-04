#include <iostream>
#include <cstring>
#include <ctime>
using namespace std;
/*
基本运算部件(这些部件已经不太可能被优化)
*/
uint32_t IV[256] = { 0x7380166f ,0x4914b2b9 ,0x172442d7 ,0xda8a0600 ,0xa96f30bc ,0x163138aa ,0xe38dee4d ,0xb0fb0e4e };
uint32_t rotl(uint32_t a, uint8_t n) {//32位循环左移
	return (a << n) | (a >> (32 - n));
}
uint32_t T(int j) {
	if (j < 16) {
		return 0x79cc4519;
	}
	else {
		return 0x7a879d8a;
	}
}
uint32_t FF(int j, uint32_t X, uint32_t Y, uint32_t Z) {
	if (j < 16) {
		return X ^ Y ^ Z;
	}
	else {
		return (X & Y) | (X & Z) | (Y & Z);
	}
}
uint32_t GG(int j, uint32_t X, uint32_t Y, uint32_t Z) {
	if (j < 16) {
		return X ^ Y ^ Z;
	}
	else {
		return (X & Y) | ((~X) & Z);
	}
}
uint32_t P0(uint32_t X) {
	return X ^ rotl(X, 9) ^ rotl(X, 17);
}
uint32_t P1(uint32_t X) {
	return X ^ rotl(X, 15) ^ rotl(X, 23);
}
/*
格式部件
*/
void str_to_hex(const char* m, uint32_t* Bi, uint64_t mlen) {//分字，还不分组//mlen是比特数//blen应是未补全长度
	uint64_t blen = mlen >> 5;//mlen改用比特数//注意blen==实际字数-1，仅当mlen%32==0时为真实字数
	int ii = blen * 4;
	if (blen == 0) {
		if ((mlen & 31) == 24) {
			Bi[blen] = (m[ii] << 24) | (m[ii + 1] << 16) | (m[ii + 2] << 8);
		}
		else if ((mlen & 31) == 16) {
			Bi[blen] = (m[ii] << 24) | (m[ii + 1] << 16);
		}
		else if ((mlen & 31) == 8) {
			Bi[blen] = (m[ii] << 24);
		}
		else {//mlen%4==0
		}
		return;//出现不足一组的情况就必须处理完直接返回。
	}
	//blen>0
	for (int i = 0;i < blen;i++) {//
		ii = 4 * i;
		Bi[i] = (m[ii] << 24) | (m[ii + 1] << 16) | (m[ii + 2] << 8) | (m[ii + 3]);
	}
	ii = (blen) * 4;
	if ((mlen & 31) == 24) {
		Bi[blen] = (m[ii] << 24) | (m[ii + 1] << 16) | (m[ii + 2] << 8);
	}
	else if ((mlen & 31) == 16) {
		Bi[blen] = (m[ii] << 24) | (m[ii + 1] << 16);
	}
	else if ((mlen & 31) == 8) {
		Bi[blen] = (m[ii] << 24);
	}
	else {//mlen%32==0
		//注意blen==实际字数-1，仅当mlen%32==0时为真实字数//若为真实数字则不需要此步骤。
	}
}
void padding(uint32_t* Bi, uint64_t bsize, uint64_t mlen) {//只需要在末尾补足1个1，尾部补充64位(l)即可。
	//显然m最小单位是Byte，也就是说，我需要找到最后一个字节所在的位置，把后一个字节替换成1000_0000.
	int a = mlen >> 5;//在第几个uint32_t//mlen是比特数
	int b = mlen % 32;//检查溢出几个比特
	if (b == 0) {
		Bi[a] = Bi[a] | 0x80000000;
	}
	else if (b == 8) {
		Bi[a] = Bi[a] | 0x00800000;
	}
	else if (b == 16) {
		Bi[a] = Bi[a] | 0x00008000;
	}
	else {//b=24
		Bi[a] = Bi[a] | 0x00000080;
	}
	Bi[bsize - 2] = (uint32_t)(mlen >> 32); //高位
	Bi[bsize - 1] = (uint32_t)mlen; //低位
}
/*
密码部件
*/
void extend(uint32_t* Bi, uint32_t W[132]) {//每16位拓展成132位。B是需要按16分组的
	//uint32_t W[132];
	W[0] = Bi[0];
	W[1] = Bi[1];
	W[2] = Bi[2];
	W[3] = Bi[3];
	W[4] = Bi[4];
	W[5] = Bi[5];
	W[6] = Bi[6];
	W[7] = Bi[7];
	W[8] = Bi[8];
	W[9] = Bi[9];
	W[10] = Bi[10];
	W[11] = Bi[11];
	W[12] = Bi[12];
	W[13] = Bi[13];
	W[14] = Bi[14];
	W[15] = Bi[15];
	for (int j = 16;j < 68;j++) {
		W[j] = P1(W[j - 16] ^ W[j - 9] ^ rotl(W[j - 3], 15)) ^ rotl(W[j - 13], 7) ^ W[j - 6];
	}
	for (int j = 0;j < 64;j++) {
		W[j + 68] = W[j] ^ W[j + 4];
	}
}
void CF(uint32_t V[8], uint32_t W[132]) {//4x32
	uint32_t A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2;
	A = V[0];
	B = V[1];
	C = V[2];
	D = V[3];
	E = V[4];
	F = V[5];
	G = V[6];
	H = V[7];
	for (int j = 0;j < 64;j++) {
		SS1 = rotl((rotl(A, 12) + E + rotl(T(j), j)), 7);
		SS2 = SS1 ^ rotl(A, 12);
		TT1 = FF(j, A, B, C) + D + SS2 + W[j + 68];//
		TT2 = GG(j, E, F, G) + H + SS1 + W[j];//
		D = C;
		C = rotl(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = rotl(F, 19);
		F = E;
		E = P0(TT2);
	}
	V[0] = V[0] ^ A;
	V[1] = V[1] ^ B;
	V[2] = V[2] ^ C;
	V[3] = V[3] ^ D;
	V[4] = V[4] ^ E;
	V[5] = V[5] ^ F;
	V[6] = V[6] ^ G;
	V[7] = V[7] ^ H;
}
void iter(uint32_t* Bi, uint64_t blen, uint32_t V[8]) {//512=16X32//blen=Bi数组大小(字数)
	uint32_t W[132];
	for (int i = 0;i < blen;i += 16) {//16
		extend(Bi + i, W);
		CF(V, W);
	}
	//return V;
}
/*
组合算法
*/
void SM3(const char* m) {
	uint64_t l = strlen(m) << 3;//比特数(Bx8)
	int k = (447 - (l % 512)) % 512;
	if (k < 0) {
		k += 512;
	}
	uint64_t length = l + 1 + k + 64;//真实比特数//此数值必然是512的倍数
	uint64_t size = length >> 5;//字数//补全后的大小，包含尾部空间。//由于length的特性，此值必然是16的倍数
	uint32_t* Bi = new uint32_t[size];//补全后的大小，包含尾部空间。
	for (int i = 0;i < size;i++) {
		Bi[i] = 0;
	}
	str_to_hex(m, Bi, l);
	padding(Bi, size, l);

	uint32_t V[8];
	for (int i = 0;i < 8;i++) {
		V[i] = IV[i];
	}
	//cout << "Hash:" << endl;
	//clock_t start, end;
	//start = clock();
	//for (int i = 0;i < 9999;i++) {
	//	iter(Bi, size, V);
	//}
	iter(Bi, size, V);
	//end = clock();
	//for (int i = 0;i < 8;i++) {
	//	cout << hex << V[i] << " ";
	//}
	//cout << endl << endl;
	//cout << "Time use:" << endl << end - start << "ms" << endl;
}
int main() {
	SM3("abc");
	clock_t start, end;
	start = clock();
	for (int i = 0;i < 10000;i++) {
		SM3("abc");
	}
	end = clock();
	cout << "Time use:" << endl << end - start << "ms" << endl;
	//66c7f0f4 62eeedd9 d1f2d46b dc10e4e2 4167c487 5cf2f7a2 297da02b 8f4ba8e0
	//(不含padding)运行一万次大约需要25ms
	//(含padding)运行一万次大约需要35ms
	//这么一看还挺快的嘛
}