#include"transform.h"
#include"sm3.h"
using namespace std;

//尝试数
#define max_num 65536//2^16
//查表
string inlist[max_num];
string outlist[max_num];

void birthday_attack() {
	string str;
	string result;
	string paddingValue;
	for (int i = 0; i < max_num; outlist[i++] = result) {
		cout << "目前进度：" << i << endl;
		str = to_string(i);
		inlist[i] = str;
		paddingValue = padding(str);
		result = iteration(paddingValue);
		//查表寻找弱碰撞
		for (int j = 0; j < i; j++) {
			if (outlist[j].substr(0, 4) == result.substr(0, 4)) {
				cout << endl;
				cout << "collision string input 1 :" + str << endl << endl;
				cout << "collision hash value 1:" << endl;
				cout << result.substr(0, 8) << "  ";
				cout << result.substr(8, 8) << "  ";
				cout << result.substr(16, 8) << "  ";
				cout << result.substr(24, 8) << "  ";
				cout << result.substr(32, 8) << "  ";
				cout << result.substr(40, 8) << "  ";
				cout << result.substr(48, 8) << "  ";
				cout << result.substr(56, 8) << "  ";
				cout << endl;
				cout << "collision string input 2 :" + inlist[j] << endl << endl;
				cout << "collision hash value 2:" << endl;
				cout << outlist[j].substr(0, 8) << "  ";
				cout << outlist[j].substr(8, 8) << "  ";
				cout << outlist[j].substr(16, 8) << "  ";
				cout << outlist[j].substr(24, 8) << "  ";
				cout << outlist[j].substr(32, 8) << "  ";
				cout << outlist[j].substr(40, 8) << "  ";
				cout << outlist[j].substr(48, 8) << "  ";
				cout << outlist[j].substr(56, 8) << "  ";
				cout << endl << "finding num in all:  " << i;
				return;
			}
		}
	}
	cout << "birthday attack failed!";
}

int main() {
	birthday_attack();
	return 0;
}