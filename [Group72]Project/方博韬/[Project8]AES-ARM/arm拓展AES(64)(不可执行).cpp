#include<iostream>
#include <arm_neon.h>
using namespace std;
uint8x16_t vaeseq_u8(uint8x16_t block, uint8x16_t key) {
	/*
	* AESE <block>.16B, <key>.16B
	* 
	* return block;
	*/
}
uint8x16_t vaesmcq_u8(uint8x16_t p) {
	/*
	* uint8x16_t Vd;
	* 
	* AESMC <Vd>.16B, <p>.16B
	* 
	* return Vd;
	*/
}
void aes128(const uint8_t in[16],uint8_t ou[16],const uint32_t rk[44]) {
	uint8x16_t block = vld1q_u8(in);//∂¡»°
	uint8_t* p8 = (uint8_t*)rk;
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 0)));//mcq «MixColumn,eq «ShiftRow+SubBytes
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 1)));
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 2)));
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 3)));
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 4)));
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 5)));
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 6)));
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 7)));
	block = vaesmcq_u8(vaeseq_u8(block, vld1q_u8(p8 + 16 * 8)));
	block = vaeseq_u8(block, vld1q_u8(p8 + 16 * 9));
	block = veorq_u8(block, vld1q_u8(p8 + 16 * 10));
	vst1q_u8(ou, block);//¥Ê¥¢
}