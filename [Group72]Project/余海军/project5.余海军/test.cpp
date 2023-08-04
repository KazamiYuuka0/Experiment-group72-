#include "sm3.h"
#include "MerkleTree.h"
#include <iostream>
#include "transform.h"
using namespace std;

int main() {
	//前序构建默克尔树
	MerkleTree<string> test;
	//遍历默克尔树
	cout << endl << "前序遍历结果：" << endl;
	test.PreOrder(test.getRoot());
	cout << endl << "中序遍历结果：" << endl;
	test.InOrder(test.getRoot());
	cout << endl << "后序遍历结果：" << endl;
	test.PostOrder(test.getRoot());
}
