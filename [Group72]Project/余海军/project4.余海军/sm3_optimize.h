#pragma once
#include <iostream>
#include <string>
#include <cmath>

string padding(string str);//对数据进行填充的函数
string LeftShift(string str, int len);//循环左移len位功能
string XOR(string str1, string str2);//异或操作
string AND(string str1, string str2);//与操作
string OR(string str1, string str2);//或操作
string NOT(string str);//非操作
char binXor(char str1, char str2);//单比特的异或操作
char binAnd(char str1, char str2);//单比特的与操作
string ModAdd(string str1, string str2);//mod 2^32运算的函数
string P1(string str);//置换功能P1（X）
string P0(string str);//置换功能P0（X）
string T(int j);//Tj常量值的函数实现
string FF(string str1, string str2, string str3, int j);//布尔函数FF功能
string GG(string str1, string str2, string str3, int j);//布尔函数GG
string extension(string str);//消息扩展函数
string compress(string str1, string str2);//消息压缩函数
string iteration(string str);//迭代压缩函数

string padding(string str) {
	string res = "";
	for (int i = 0; i < str.size(); i++) {
		res += DecToHex((int)str[i]);
	}
	cout << "string ASCII is:" << endl;
	for (int i = 0; i < res.size(); i++) {
		cout << res[i];
		if ((i + 1) % 8 == 0) {
			cout << "  ";
		}
		if ((i + 1) % 64 == 0 || (i + 1) == res.size()) {
			cout << endl;
		}
	}
	cout << endl;
	int res_length = res.size() * 4;
	//根据sm3填充规则,在获得的数据后面添1，在16进制下相当于是添加8
	res += "8";
	while (res.size() % 128 != 112) {
		res += "0";//“0”数据填充
	}
	string res_len = DecToHex(res_length);//记录数据长度的字符串
	while (res_len.size() != 16) {
		res_len = "0" + res_len;
	}
	res += res_len;
	return res;
}

string LeftShift(string str, int len) {
	string res = HexToBin(str);
	res = res.substr(len) + res.substr(0, len);
	return BinToHex(res);
}

string XOR(string str1, string str2) {
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == res2[i]) {
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return BinToHex(res);
}

string AND(string str1, string str2) {
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '1' && res2[i] == '1') {
			res += "1";
		}
		else {
			res += "0";
		}
	}
	return BinToHex(res);
}

string OR(string str1, string str2) {
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '0' && res2[i] == '0') {
			res += "0";
		}
		else {
			res += "1";
		}
	}
	return BinToHex(res);
}

string NOT(string str) {
	string res1 = HexToBin(str);
	string res = "";
	for (int i = 0; i < res1.size(); i++) {
		if (res1[i] == '0') {
			res += "1";
		}
		else {
			res += "0";
		}
	}
	return BinToHex(res);
}

char binXor(char str1, char str2) {
	return str1 == str2 ? '0' : '1';
}

char binAnd(char str1, char str2) {
	return (str1 == '1' && str2 == '1') ? '1' : '0';
}

string ModAdd(string str1, string str2) {
	string res1 = HexToBin(str1);
	string res2 = HexToBin(str2);
	char temp = '0';
	string res = "";
	for (int i = res1.size() - 1; i >= 0; i--) {
		res = binXor(binXor(res1[i], res2[i]), temp) + res;
		if (binAnd(res1[i], res2[i]) == '1') {
			temp = '1';
		}
		else {
			if (binXor(res1[i], res2[i]) == '1') {
				temp = binAnd('1', temp);
			}
			else {
				temp = '0';
			}
		}
	}
	return BinToHex(res);
}

string P1(string str) {
	return XOR(XOR(str, LeftShift(str, 15)), LeftShift(str, 23));
}

string P0(string str) {
	return XOR(XOR(str, LeftShift(str, 9)), LeftShift(str, 17));
}

string T(int j) {
	//常量
	if (0 <= j && j <= 15) {
		return "79CC4519";
	}
	else {
		return "7A879D8A";
	}
}

string FF(string str1, string str2, string str3, int j) {
	if (0 <= j && j <= 15) {
		return XOR(XOR(str1, str2), str3);
	}
	else {
		return OR(OR(AND(str1, str2), AND(str1, str3)), AND(str2, str3));
	}
}

string GG(string str1, string str2, string str3, int j) {
	if (0 <= j && j <= 15) {
		return XOR(XOR(str1, str2), str3);
	}
	else {
		return OR(AND(str1, str2), AND(NOT(str1), str3));
	}
}
string extension(string str) {
	string res = str;//字符串类型存储前68位存储扩展字W值
	for (int i = 16; i < 68; i++) {//根据公式生成第17位到第68位的W值
		res += XOR(XOR(P1(XOR(XOR(res.substr((i - 16) * 8, 8), res.substr((i - 9) * 8, 8)), LeftShift(res.substr((i - 3) * 8, 8), 15))), LeftShift(res.substr((i - 13) * 8, 8), 7)), res.substr((i - 6) * 8, 8));
	}
	cout << "after extension:" << endl;
	cout << "W0,W1,……,W67：" << endl;
	//循环展开
	cout << res.substr(0 + 0, 8) << "  ";
	cout << res.substr(0 + 8, 8) << "  ";
	cout << res.substr(0 + 16, 8) << "  ";
	cout << res.substr(0 + 24, 8) << "  ";
	cout << res.substr(0 + 32, 8) << "  ";
	cout << res.substr(0 + 40, 8) << "  ";
	cout << res.substr(0 + 48, 8) << "  ";
	cout << res.substr(0 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(64 + 0, 8) << "  ";
	cout << res.substr(64 + 8, 8) << "  ";
	cout << res.substr(64 + 16, 8) << "  ";
	cout << res.substr(64 + 24, 8) << "  ";
	cout << res.substr(64 + 32, 8) << "  ";
	cout << res.substr(64 + 40, 8) << "  ";
	cout << res.substr(64 + 48, 8) << "  ";
	cout << res.substr(64 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(128 + 0, 8) << "  ";
	cout << res.substr(128 + 8, 8) << "  ";
	cout << res.substr(128 + 16, 8) << "  ";
	cout << res.substr(128 + 24, 8) << "  ";
	cout << res.substr(128 + 32, 8) << "  ";
	cout << res.substr(128 + 40, 8) << "  ";
	cout << res.substr(128 + 48, 8) << "  ";
	cout << res.substr(128 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(192 + 0, 8) << "  ";
	cout << res.substr(192 + 8, 8) << "  ";
	cout << res.substr(192 + 16, 8) << "  ";
	cout << res.substr(192 + 24, 8) << "  ";
	cout << res.substr(192 + 32, 8) << "  ";
	cout << res.substr(192 + 40, 8) << "  ";
	cout << res.substr(192 + 48, 8) << "  ";
	cout << res.substr(192 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(256 + 0, 8) << "  ";
	cout << res.substr(256 + 8, 8) << "  ";
	cout << res.substr(256 + 16, 8) << "  ";
	cout << res.substr(256 + 24, 8) << "  ";
	cout << res.substr(256 + 32, 8) << "  ";
	cout << res.substr(256 + 40, 8) << "  ";
	cout << res.substr(256 + 48, 8) << "  ";
	cout << res.substr(256 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(320 + 0, 8) << "  ";
	cout << res.substr(320 + 8, 8) << "  ";
	cout << res.substr(320 + 16, 8) << "  ";
	cout << res.substr(320 + 24, 8) << "  ";
	cout << res.substr(320 + 32, 8) << "  ";
	cout << res.substr(320 + 40, 8) << "  ";
	cout << res.substr(320 + 48, 8) << "  ";
	cout << res.substr(320 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(384 + 0, 8) << "  ";
	cout << res.substr(384 + 8, 8) << "  ";
	cout << res.substr(384 + 16, 8) << "  ";
	cout << res.substr(384 + 24, 8) << "  ";
	cout << res.substr(384 + 32, 8) << "  ";
	cout << res.substr(384 + 40, 8) << "  ";
	cout << res.substr(384 + 48, 8) << "  ";
	cout << res.substr(384 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(448 + 0, 8) << "  ";
	cout << res.substr(448 + 8, 8) << "  ";
	cout << res.substr(448 + 16, 8) << "  ";
	cout << res.substr(448 + 24, 8) << "  ";
	cout << res.substr(448 + 32, 8) << "  ";
	cout << res.substr(448 + 40, 8) << "  ";
	cout << res.substr(448 + 48, 8) << "  ";
	cout << res.substr(448 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(512, 8) << "  " << res.substr(520, 8) << "  " << res.substr(528, 8) << "  " << res.substr(536, 8) << endl;
	cout << endl;
	//根据公式生成64位W'
	res += XOR(res.substr(0 * 8, 8), res.substr((0 + 4) * 8, 8));
	res += XOR(res.substr(1 * 8, 8), res.substr((1 + 4) * 8, 8));
	res += XOR(res.substr(2 * 8, 8), res.substr((2 + 4) * 8, 8));
	res += XOR(res.substr(3 * 8, 8), res.substr((3 + 4) * 8, 8));
	res += XOR(res.substr(4 * 8, 8), res.substr((4 + 4) * 8, 8));
	res += XOR(res.substr(5 * 8, 8), res.substr((5 + 4) * 8, 8));
	res += XOR(res.substr(6 * 8, 8), res.substr((6 + 4) * 8, 8));
	res += XOR(res.substr(7 * 8, 8), res.substr((7 + 4) * 8, 8));
	res += XOR(res.substr(8 * 8, 8), res.substr((8 + 4) * 8, 8));
	res += XOR(res.substr(9 * 8, 8), res.substr((9 + 4) * 8, 8));
	res += XOR(res.substr(10 * 8, 8), res.substr((10 + 4) * 8, 8));
	res += XOR(res.substr(11 * 8, 8), res.substr((11 + 4) * 8, 8));
	res += XOR(res.substr(12 * 8, 8), res.substr((12 + 4) * 8, 8));
	res += XOR(res.substr(13 * 8, 8), res.substr((13 + 4) * 8, 8));
	res += XOR(res.substr(14 * 8, 8), res.substr((14 + 4) * 8, 8));
	res += XOR(res.substr(15 * 8, 8), res.substr((15 + 4) * 8, 8));
	res += XOR(res.substr(16 * 8, 8), res.substr((16 + 4) * 8, 8));
	res += XOR(res.substr(17 * 8, 8), res.substr((17 + 4) * 8, 8));
	res += XOR(res.substr(18 * 8, 8), res.substr((18 + 4) * 8, 8));
	res += XOR(res.substr(19 * 8, 8), res.substr((19 + 4) * 8, 8));
	res += XOR(res.substr(20 * 8, 8), res.substr((20 + 4) * 8, 8));
	res += XOR(res.substr(21 * 8, 8), res.substr((21 + 4) * 8, 8));
	res += XOR(res.substr(22 * 8, 8), res.substr((22 + 4) * 8, 8));
	res += XOR(res.substr(23 * 8, 8), res.substr((23 + 4) * 8, 8));
	res += XOR(res.substr(24 * 8, 8), res.substr((24 + 4) * 8, 8));
	res += XOR(res.substr(25 * 8, 8), res.substr((25 + 4) * 8, 8));
	res += XOR(res.substr(26 * 8, 8), res.substr((26 + 4) * 8, 8));
	res += XOR(res.substr(27 * 8, 8), res.substr((27 + 4) * 8, 8));
	res += XOR(res.substr(28 * 8, 8), res.substr((28 + 4) * 8, 8));
	res += XOR(res.substr(29 * 8, 8), res.substr((29 + 4) * 8, 8));
	res += XOR(res.substr(30 * 8, 8), res.substr((30 + 4) * 8, 8));
	res += XOR(res.substr(31 * 8, 8), res.substr((31 + 4) * 8, 8));
	res += XOR(res.substr(32 * 8, 8), res.substr((32 + 4) * 8, 8));
	res += XOR(res.substr(33 * 8, 8), res.substr((33 + 4) * 8, 8));
	res += XOR(res.substr(34 * 8, 8), res.substr((34 + 4) * 8, 8));
	res += XOR(res.substr(35 * 8, 8), res.substr((35 + 4) * 8, 8));
	res += XOR(res.substr(36 * 8, 8), res.substr((36 + 4) * 8, 8));
	res += XOR(res.substr(37 * 8, 8), res.substr((37 + 4) * 8, 8));
	res += XOR(res.substr(38 * 8, 8), res.substr((38 + 4) * 8, 8));
	res += XOR(res.substr(39 * 8, 8), res.substr((39 + 4) * 8, 8));
	res += XOR(res.substr(40 * 8, 8), res.substr((40 + 4) * 8, 8));
	res += XOR(res.substr(41 * 8, 8), res.substr((41 + 4) * 8, 8));
	res += XOR(res.substr(42 * 8, 8), res.substr((42 + 4) * 8, 8));
	res += XOR(res.substr(43 * 8, 8), res.substr((43 + 4) * 8, 8));
	res += XOR(res.substr(44 * 8, 8), res.substr((44 + 4) * 8, 8));
	res += XOR(res.substr(45 * 8, 8), res.substr((45 + 4) * 8, 8));
	res += XOR(res.substr(46 * 8, 8), res.substr((46 + 4) * 8, 8));
	res += XOR(res.substr(47 * 8, 8), res.substr((47 + 4) * 8, 8));
	res += XOR(res.substr(48 * 8, 8), res.substr((48 + 4) * 8, 8));
	res += XOR(res.substr(49 * 8, 8), res.substr((49 + 4) * 8, 8));
	res += XOR(res.substr(50 * 8, 8), res.substr((50 + 4) * 8, 8));
	res += XOR(res.substr(51 * 8, 8), res.substr((51 + 4) * 8, 8));
	res += XOR(res.substr(52 * 8, 8), res.substr((52 + 4) * 8, 8));
	res += XOR(res.substr(53 * 8, 8), res.substr((53 + 4) * 8, 8));
	res += XOR(res.substr(54 * 8, 8), res.substr((54 + 4) * 8, 8));
	res += XOR(res.substr(55 * 8, 8), res.substr((55 + 4) * 8, 8));
	res += XOR(res.substr(56 * 8, 8), res.substr((56 + 4) * 8, 8));
	res += XOR(res.substr(57 * 8, 8), res.substr((57 + 4) * 8, 8));
	res += XOR(res.substr(58 * 8, 8), res.substr((58 + 4) * 8, 8));
	res += XOR(res.substr(59 * 8, 8), res.substr((59 + 4) * 8, 8));
	res += XOR(res.substr(60 * 8, 8), res.substr((60 + 4) * 8, 8));
	res += XOR(res.substr(61 * 8, 8), res.substr((61 + 4) * 8, 8));
	res += XOR(res.substr(62 * 8, 8), res.substr((62 + 4) * 8, 8));
	res += XOR(res.substr(63 * 8, 8), res.substr((63 + 4) * 8, 8));
	cout << "W0',W1',……,W63'：" << endl;
	cout << res.substr(544 + 0 + 0, 8) << "  ";
	cout << res.substr(544 + 0 + 8, 8) << "  ";
	cout << res.substr(544 + 0 + 16, 8) << "  ";
	cout << res.substr(544 + 0 + 24, 8) << "  ";
	cout << res.substr(544 + 0 + 32, 8) << "  ";
	cout << res.substr(544 + 0 + 40, 8) << "  ";
	cout << res.substr(544 + 0 + 48, 8) << "  ";
	cout << res.substr(544 + 0 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(544 + 64 + 0, 8) << "  ";
	cout << res.substr(544 + 64 + 8, 8) << "  ";
	cout << res.substr(544 + 64 + 16, 8) << "  ";
	cout << res.substr(544 + 64 + 24, 8) << "  ";
	cout << res.substr(544 + 64 + 32, 8) << "  ";
	cout << res.substr(544 + 64 + 40, 8) << "  ";
	cout << res.substr(544 + 64 + 48, 8) << "  ";
	cout << res.substr(544 + 64 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(544 + 128 + 0, 8) << "  ";
	cout << res.substr(544 + 128 + 8, 8) << "  ";
	cout << res.substr(544 + 128 + 16, 8) << "  ";
	cout << res.substr(544 + 128 + 24, 8) << "  ";
	cout << res.substr(544 + 128 + 32, 8) << "  ";
	cout << res.substr(544 + 128 + 40, 8) << "  ";
	cout << res.substr(544 + 128 + 48, 8) << "  ";
	cout << res.substr(544 + 128 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(544 + +0, 8) << "  ";
	cout << res.substr(544 + 192 + 8, 8) << "  ";
	cout << res.substr(544 + 192 + 16, 8) << "  ";
	cout << res.substr(544 + 192 + 24, 8) << "  ";
	cout << res.substr(544 + 192 + 32, 8) << "  ";
	cout << res.substr(544 + 192 + 40, 8) << "  ";
	cout << res.substr(544 + 192 + 48, 8) << "  ";
	cout << res.substr(544 + 192 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(544 + 256 + 0, 8) << "  ";
	cout << res.substr(544 + 256 + 8, 8) << "  ";
	cout << res.substr(544 + 256 + 16, 8) << "  ";
	cout << res.substr(544 + 256 + 24, 8) << "  ";
	cout << res.substr(544 + 256 + 32, 8) << "  ";
	cout << res.substr(544 + 256 + 40, 8) << "  ";
	cout << res.substr(544 + 256 + 48, 8) << "  ";
	cout << res.substr(544 + 256 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(544 + 320 + 0, 8) << "  ";
	cout << res.substr(544 + 320 + 8, 8) << "  ";
	cout << res.substr(544 + 320 + 16, 8) << "  ";
	cout << res.substr(544 + 320 + 24, 8) << "  ";
	cout << res.substr(544 + 320 + 32, 8) << "  ";
	cout << res.substr(544 + 320 + 40, 8) << "  ";
	cout << res.substr(544 + 320 + 48, 8) << "  ";
	cout << res.substr(544 + 320 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(544 + 384 + 0, 8) << "  ";
	cout << res.substr(544 + 384 + 8, 8) << "  ";
	cout << res.substr(544 + 384 + 16, 8) << "  ";
	cout << res.substr(544 + 384 + 24, 8) << "  ";
	cout << res.substr(544 + 384 + 32, 8) << "  ";
	cout << res.substr(544 + 384 + 40, 8) << "  ";
	cout << res.substr(544 + 384 + 48, 8) << "  ";
	cout << res.substr(544 + 384 + 56, 8) << "  ";
	cout << endl;
	cout << res.substr(544 + 448 + 0, 8) << "  ";
	cout << res.substr(544 + 448 + 8, 8) << "  ";
	cout << res.substr(544 + 448 + 16, 8) << "  ";
	cout << res.substr(544 + 448 + 24, 8) << "  ";
	cout << res.substr(544 + 448 + 32, 8) << "  ";
	cout << res.substr(544 + 448 + 40, 8) << "  ";
	cout << res.substr(544 + 448 + 48, 8) << "  ";
	cout << res.substr(544 + 448 + 56, 8) << "  ";
	cout << endl;
	/*
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			cout << res.substr(544 + i * 64 + j * 8, 8) << "  ";
		}
		cout << endl;
	}
	cout << endl;*/
	return res;
}

string compress(string str1, string str2) {
	string IV = str2;
	string A = IV.substr(0, 8), B = IV.substr(8, 8), C = IV.substr(16, 8), D = IV.substr(24, 8), E = IV.substr(32, 8), F = IV.substr(40, 8), G = IV.substr(48, 8), H = IV.substr(56, 8);
	string SS1 = "", SS2 = "", TT1 = "", TT2 = "";
	cout << "Iteratively compress intermediate values: " << endl;
	cout << "    A         B         C         D         E         F        G         H " << endl;
	cout << A << "  " << B << "  " << C << "  " << D << "  " << E << "  " << F << "  " << G << "  " << H << endl;
	for (int j = 0; j < 64; j++) {
		SS1 = LeftShift(ModAdd(ModAdd(LeftShift(A, 12), E), LeftShift(T(j), (j % 32))), 7);
		SS2 = XOR(SS1, LeftShift(A, 12));
		TT1 = ModAdd(ModAdd(ModAdd(FF(A, B, C, j), D), SS2), str1.substr((j + 68) * 8, 8));
		TT2 = ModAdd(ModAdd(ModAdd(GG(E, F, G, j), H), SS1), str1.substr(j * 8, 8));
		D = C;
		C = LeftShift(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = LeftShift(F, 19);
		F = E;
		E = P0(TT2);
		cout << A << "  " << B << "  " << C << "  " << D << "  " << E << "  " << F << "  " << G << "  " << H << endl;
	}
	string res = (A + B + C + D + E + F + G + H);
	cout << endl;
	return res;
}

string iteration(string str) {
	int num = str.size() / 128;
	cout << "group num:" + to_string(num) << endl;
	cout << endl;
	string V = "7380166F4914B2B9172442D7DA8A0600A96F30BC163138AAE38DEE4DB0FB0E4E";
	string B = "", extensionB = "", compressB = "";
	for (int i = 0; i < num; i++) {
		cout << "the " << to_string(i + 1) << " group:" << endl;
		cout << endl;
		B = str.substr(i * 128, 128);
		extensionB = extension(B);
		compressB = compress(extensionB, V);
		V = XOR(V, compressB);
	}
	return V;
}
