#pragma once
#include "sm3.h"
#include<iostream>
using namespace std;

//默克尔树结点类
template<class T>
class MerkleTreeNode {
	//友元类声明
	template<class T>
	friend class MerkleTree;
private:
	T data;
	MerkleTreeNode<T>* leftchild;
	MerkleTreeNode<T>* rightchild;
public:
	//构造函数
	MerkleTreeNode() {
		leftchild = NULL;
		rightchild = NULL;
	}
	MerkleTreeNode(const T& dataValue) {
		data = iteration(padding(dataValue));
		leftchild = NULL;
		rightchild = NULL;
	}
	MerkleTreeNode(const T& dataValue, MerkleTreeNode<T>* leftchildValue, MerkleTreeNode<T>* rightchildValue) {
		data = dataValue;
		leftchild = leftchildValue;
		rightchild = rightchildValue;
	}
	//析构函数
	~MerkleTreeNode() {
		leftchild = NULL;
		rightchild = NULL;
	}
	//更新节点值
	void UpdateNode(const T& dataValue) {
		data = dataValue;
	}
	//访问结点
	void VisitNode() {
		cout << data << endl;
	}
	//取左结点
	MerkleTreeNode<T>* getLeft() {
		return leftchild;
	}
	//取右结点
	MerkleTreeNode<T>* getRight() {
		return rightchild;
	}
};

//默克尔树类
template<class T>
class MerkleTree {
private:
	MerkleTreeNode<T>* root;
public:
	//构造函数
	MerkleTree() {
		root = NULL;
		cout << "Now start to construct the MerkleTree" << endl;
		CreateNode(root);
	}
	//析构函数
	~MerkleTree() {
		root = NULL;
	}
	//前序构建默克尔树
	void CreateNode(MerkleTreeNode<T>*& treeNode) {
		cout << "Please enter value or '#' to stop:";
		T value;
		cin >> value;
		treeNode = new MerkleTreeNode<T>(value);
		if (value == "#") {
			delete treeNode;
			treeNode = NULL;
		}
		else {
			CreateNode(treeNode->leftchild);
			CreateNode(treeNode->rightchild);
			//如果不是叶子结点，则对左右两个字结点做联合哈希
			if (treeNode->leftchild != NULL && treeNode->rightchild != NULL) {
				treeNode->data = iteration(padding(treeNode->leftchild->data + treeNode->rightchild->data));
			}
		}
	}
	//取根节点
	MerkleTreeNode<T>* getRoot() {
		return root;
	}
	//递归前序遍历
	void PreOrder(MerkleTreeNode<T>* treeNode) {
		if (treeNode != NULL) {
			treeNode->VisitNode();
			PreOrder(treeNode->getLeft());
			PreOrder(treeNode->getRight());
		}
	}
	//递归中序遍历
	void InOrder(MerkleTreeNode<T>* treeNode) {
		if (treeNode != NULL) {
			InOrder(treeNode->getLeft());
			treeNode->VisitNode();
			InOrder(treeNode->getRight());
		}
	}
	//递归后序遍历
	void PostOrder(MerkleTreeNode<T>* treeNode) {
		if (treeNode != NULL) {
			PostOrder(treeNode->getLeft());
			PostOrder(treeNode->getRight());
			treeNode->VisitNode();
		}
	}
};

