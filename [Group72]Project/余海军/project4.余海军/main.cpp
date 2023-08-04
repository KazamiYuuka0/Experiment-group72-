#include"transform.h"
#include"sm3_optimize.h"


int main() {
	//测试数据
	string str;
	str = "202100460129余海军";
	cout << "string input :" + str << endl << endl;
	string paddingValue = padding(str);
	cout << "after padding:" << endl;
	for (int i = 0; i < paddingValue.size() / 64; i++) {
		for (int j = 0; j < 8; j++) {
			cout << paddingValue.substr(i * 64 + j * 8, 8) << "  ";
		}
		cout << endl;
	}
	cout << endl;
	string result = iteration(paddingValue);
	cout << "hash value:" << endl;
	cout << result.substr(0, 8) << "  ";
	cout << result.substr(8, 8) << "  ";
	cout << result.substr(16, 8) << "  ";
	cout << result.substr(24, 8) << "  ";
	cout << result.substr(32, 8) << "  ";
	cout << result.substr(40, 8) << "  ";
	cout << result.substr(48, 8) << "  ";
	cout << result.substr(56, 8) << "  ";
	/*for (int i = 0; i < 8; i++) {
		cout << result.substr(i * 8, 8) << "  ";
	}*/
	cout << endl << endl;
}
