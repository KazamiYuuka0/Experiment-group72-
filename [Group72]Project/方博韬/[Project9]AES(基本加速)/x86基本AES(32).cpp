#include <iostream>
#include <ctime>
using namespace std;
/*
常数部件
*/
const uint8_t S[256]={
};
const uint8_t RC[11] = {0x00, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1B, 0x36};
/*
基本部件
*/
uint32_t rotl(uint32_t a,uint32_t n) {
	return (a << n) | (a >> (32 - n));
}
uint32_t T(uint32_t W,uint32_t j) {
	uint32_t temp = rotl(W, 1);
	temp = (S[temp >> 24] << 24) |//最高位
		(S[(uint8_t)(temp >> 16)] << 16) |//次高位
		(S[(uint8_t)(temp >> 8)] << 8) |//第三位
		(S[(uint8_t)temp]);//末位
	temp = temp ^ (uint32_t)RC[j >> 2] << 24;
	return temp;
}
uint32_t mul02(uint8_t b) {
	if (b >> 7 == 0x0) {
		return b << 1;
	}
	else {
		return (b << 1) ^ 0x1b;
	}
}
uint32_t mulGF28(uint32_t column) {
	uint8_t b0, b1, b2, b3;
	uint8_t s0, s1, s2, s3;
	b0 = column >> 24;
	b1 = (uint8_t)(column >> 16);
	b2 = (uint8_t)(column >> 8);
	b3 = (uint8_t)column;
	s0 = mul02(b0) ^ (mul02(b1) ^ b1) ^ b2 ^ b3;//02 03 01 01
	s1 = b0 ^ mul02(b1) ^ (mul02(b2) ^ b2) ^ b3;//01 02 03 01
	s2 = b0 ^ b1 ^ mul02(b2) ^ (mul02(b3) ^ b3);//01 01 02 03
	s3 = (mul02(b0) ^ b0) ^ b1 ^ b2 ^ mul02(b3);//03 01 01 02
	return s0 << 24 | s1 << 16 | s2 << 8 | s3;
}
/*
密码组件
*/
void Keyexpend(uint32_t key[4], uint32_t W[44]) {//输入key:16B==16x8bit=4x32bit
	W[0] = key[0];
	W[1] = key[1];
	W[2] = key[2];
	W[3] = key[3];
	for (int i = 4;i < 44;i++) {
		if (i % 4 == 0) {
			W[i] = W[i - 4] ^ T(W[i - 1], i);
		}
		else {
			W[i] = W[i - 4] ^ W[i - 1];
		}
	}
}
//
void SubBytes(uint32_t state[4]) {
	uint32_t temp0,temp1,temp2,temp3;
	temp0 = state[0];
	temp1 = state[1];
	temp2 = state[2];
	temp3 = state[3];
	state[0] = (S[temp0 >> 24] << 24) |
		(S[(uint8_t)(temp0 >> 16)] << 16) |
		(S[(uint8_t)(temp0 >> 8)] << 8) |
		(S[(uint8_t)temp0]);
	state[1] = (S[temp1 >> 24] << 24) |
		(S[(uint8_t)(temp1 >> 16)] << 16) |
		(S[(uint8_t)(temp1 >> 8)] << 8) |
		(S[(uint8_t)temp1]);
	state[2] = (S[temp2 >> 24] << 24) |
		(S[(uint8_t)(temp2 >> 16)] << 16) |
		(S[(uint8_t)(temp2 >> 8)] << 8) |
		(S[(uint8_t)temp2]);
	state[3] = (S[temp3 >> 24] << 24) |
		(S[(uint8_t)(temp3 >> 16)] << 16) |
		(S[(uint8_t)(temp3 >> 8)] << 8) |
		(S[(uint8_t)temp3]);
}
void ShiftRows(uint32_t state[4]) {
	uint32_t temp[4];
	temp[0] = state[0];
	temp[1] = state[1];
	temp[2] = state[2];
	temp[3] = state[3];
	state[0] = (temp[0] & 0xff000000) | (temp[1] & 0x00ff0000) | (temp[2] & 0x0000ff00) | (uint8_t)temp[3];
	state[1] = (uint8_t)temp[0] | (temp[1] & 0xff000000) | (temp[2] & 0x00ff0000) | (temp[3] & 0x0000ff00);
	state[2] = (temp[0] & 0x0000ff00) | (uint8_t)temp[1] | (temp[2] & 0xff000000) | (temp[3] & 0x00ff0000);
	state[3] = (temp[0] & 0x00ff0000) | (temp[1] & 0x0000ff00) | (uint8_t)temp[2] | (temp[3] & 0xff000000);
}
void MixColumns(uint32_t state[4]) {
	state[0] = mulGF28(state[0]);
	state[1] = mulGF28(state[1]);
	state[2] = mulGF28(state[2]);
	state[3] = mulGF28(state[3]);
}
void AddRoundKey(uint32_t state[4],uint32_t key[4]) {
	state[0] = state[0] ^ key[0];
	state[1] = state[1] ^ key[1];
	state[2] = state[2] ^ key[2];
	state[3] = state[3] ^ key[3];
}
/*
* 组合
*/
void AES128_encrypt(uint32_t state[4],uint32_t key[4]) {
	uint32_t W[44] = {};

	Keyexpend(key, W);
	
	AddRoundKey(state, W);
	for (int i = 0;i < 9;i++) {
		SubBytes(state);
		ShiftRows(state);
		MixColumns(state);
		AddRoundKey(state, W + 4);
	}
	SubBytes(state);
	ShiftRows(state);
	AddRoundKey(state, W + 40);

}
/*
* 测试空间
* 结论1：移位效率比数组高大约89%
* 结论2：强制转换比左右移位快大约6%
* 结论3：两者和与操作比快一些，但是难以统计。
*/
int main() {
	uint32_t a[4] = { 0x00,0x01,0x02,0x03 };
	uint32_t k[4] = { 0x00,0x01,0x02,0x03 };
	clock_t start, end;
	start = clock();
	for (int i = 0;i < 10000;i++) {
		AES128_encrypt(a, k);
	}
	end = clock();
	cout <<"time:" << end - start << "ms";
}

