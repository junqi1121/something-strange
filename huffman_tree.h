#ifndef __HUFFMAN_TREE_H__
#define __HUFFMAN_TREE_H__

#include <iostream>											// 编译预处理命令
#include <cstdlib>											// 含C函数exit()的声明（stdlib.h与cstdlib是C的头文件）
#include "char_string.h"									// 串类
#include "huffman_tree_node.h"								// 哈夫曼树结点类模板

// 哈夫曼树类模板
template <class CharType, class WeightType>
class HuffmanTree
{
protected:
// 数据成员:
	HuffmanTreeNode<WeightType> *nodes;						// 存储结点信息，nodes[0]未用
	CharType *leafChars;									// 叶结点字符信息，leafChars[0]未用
	CharString *leafCharCodes;								// 叶结点字符编码信息，leafCharCodes[0]未用
	int curPosition;										// 译码时从根结点到叶结点路径的当前结点
	int num;												// 叶结点个数

// 辅助函数模板:
	void Select(int cur, int &r1, int &r2);					// nodes[1 ~ cur]中选择双亲为0，权值最小的两个结点r1，r2

public:
// 哈夫曼树方法声明及重载编译系统默认方法声明:
	HuffmanTree(CharType ch[], WeightType w[], int n);		// 由字符，权值和字符个数构造哈夫曼树
	virtual ~HuffmanTree();									// 析构函数模板
	CharString Encode(CharType ch);							// 编码
	LinkList<CharType> Decode(CharString codeStr);			// 译码
	HuffmanTree(const HuffmanTree<CharType, WeightType> &source);	// 复制构造函数模板
	HuffmanTree<CharType, WeightType> &operator=(const HuffmanTree<CharType, 
		WeightType> &source);								// 重载赋值运算符
};

// 孩子兄弟表示哈夫曼树类模板的实现部分
template <class CharType, class WeightType>
void HuffmanTree<CharType, WeightType>::Select(int cur, int &r1, int &r2)
// 操作结果：nodes[1 ~ cur]中选择双亲为0，权值最小的两个结点r1，r2
{
	r1 = r2 = 0;											// 0表示空结点
	for (int temPos = 1; temPos <= cur; temPos++)
	{	// 查找树值最小的两个结点
		if (nodes[temPos].parent != 0) continue;			// 只处理双亲不为0的结点
		if (r1 == 0)
		{	// r1为空,将temPos赋值给r1
			r1 = temPos;
		}
		else if (r2 == 0)
		{	// r2为空,将temPos赋值给r2
			r2 = temPos;
		}
		else if	(nodes[temPos].weight < nodes[r1].weight)
		{	// nodes[temPos]权值比nodes[r1]更小，将temPos赋为r1
			r1 = temPos;
		}
		else if (nodes[temPos].weight < nodes[r2].weight)
		{	// nodes[temPos]权值比nodes[r2]更小，将temPos赋为r2
			r2 = temPos;
		}
	}
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType>::HuffmanTree(CharType ch[], WeightType w[], int n)
// 操作结果：由字符，权值和字符个数构造哈夫曼树
{
	num = n;												// 叶结点个数
	int m = 2 * n - 1;										// 结点个数

	nodes = new HuffmanTreeNode<WeightType>[m + 1];			// nodes[0]未用
	leafChars = new CharType[n + 1];						// leafChars[0]未用
	leafCharCodes = new CharString[n + 1];					// leafCharCodes[0]未用

	int temPos;												// 临时变量 
	for (temPos = 1; temPos <= n; temPos++)
	{	// 存储叶结点信息
		nodes[temPos].weight = w[temPos - 1];				// 权值
		leafChars[temPos] = ch[temPos - 1];					// 字符
	}

	for (temPos = n + 1; temPos <= m; temPos++)
	{	// 建立哈夫曼树
		int r1, r2;
		Select(temPos - 1, r1, r2);	// nodes[1 ~ temPos - 1]中选择双亲为0，权值最小的两个结点r1，r2

		// 合并以r1,r2为根的树
		nodes[r1].parent = nodes[r2].parent = temPos;		// r1，r2双亲为temPos
		nodes[temPos].leftChild = r1;						// r1为temPos的左孩子
		nodes[temPos].rightChild = r2;						// r2为temPos的右孩子
		nodes[temPos].weight = nodes[r1].weight + nodes[r2].weight;//temPos的权为r1，r2的权值之和
	}

	for (temPos = 1; temPos <= n; temPos++)
	{	// 求n个叶结点字符的编码
		LinkList<char> charCode;							// 暂存叶结点字符编码信息
		for (unsigned int child = temPos, parent = nodes[child].parent; parent != 0; 
			child = parent, parent = nodes[child].parent)
		{	// 从叶结点到根结点逆向求编码
			if (nodes[parent].leftChild == child) charCode.Insert(1, '0');// 左分支编码为'0'
			else charCode.Insert(1, '1');					// 右分支编码为'1'
		}
		leafCharCodes[temPos] = charCode;					// charCode中存储字符编码
	}
	
	curPosition = m;										// 译码时从根结点开始，m为根
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType>::~HuffmanTree()
// 操作结果：销毁哈夫曼树
{	
	if (nodes != NULL) delete []nodes;						// 释放结点信息
	if (leafChars != NULL) delete []leafChars;				// 释放叶结点字符信息
	if (leafCharCodes != NULL) delete []leafCharCodes;		// 释放叶结点字符编码信息
}

template <class CharType, class WeightType>
CharString HuffmanTree<CharType, WeightType>::Encode(CharType ch)
// 操作结果：返回字符编码
{
	int temPos;												// 临时变量 
	for (temPos = 1; temPos <= num; temPos++)
	{	// 查找字符的位置
		if (leafChars[temPos] == ch) break;					// 找到字符，退出循环
	}
	if (temPos <= num)
	{	// 表示找到字符
		return leafCharCodes[temPos];						// 找到字符，得到编码
	}
	else
	{	// 表示未找到字符，出现异常
		cout << "非法字符，无法编码!" << endl;				// 提示信息
		exit(1);											// 退出程序
	}
}

template <class CharType, class WeightType>
LinkList<CharType> HuffmanTree<CharType, WeightType>::Decode(CharString codeStr)
// 操作结果：对编码串codeStr进行译码,返回编码前的字符序列
{
	LinkList<CharType> charList;							// 编码前的字符序列
	
	for (int temPos = 0; temPos < codeStr.Length(); temPos++)
	{	// 处理每位编码
		if (codeStr[temPos] == '0') curPosition = nodes[curPosition].leftChild;	// '0'表示左分支	
		else curPosition = nodes[curPosition].rightChild;			// '1'表示左分支	

		if (nodes[curPosition].leftChild == 0 && nodes[curPosition].rightChild == 0)
		{	// 译码时从根结点到叶结点路径的当前结点为叶结点
			charList.Insert(charList.Length() + 1, leafChars[curPosition]);
			curPosition = 2 * num - 1;						// curPosition回归根结点
		}
	}
	return charList;										// 返回编码前的字符序列
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType>::HuffmanTree(const HuffmanTree<CharType, WeightType> &source)
// 操作结果：由哈夫曼树source构造新哈夫曼树——复制构造函数模板
{
	num = source.num;										// 叶结点个数
	curPosition = source.curPosition;						// 译码时从根结点到叶结点路径的当前结点
	int m = 2 * num - 1;									// 结点总数 
	nodes = new HuffmanTreeNode<WeightType>[m + 1];			// nodes[0]未用
	leafChars = new CharType[num + 1];						// leafChars[0]未用
	leafCharCodes = new CharString[num + 1];				// leafCharCodes[0]未用

	int temPos;												// 临时变量 
	for (temPos = 1; temPos <= m; temPos++)
	{	// 复制结点信息
		nodes[temPos] = source.nodes[temPos];				// 结点信息
	}
	
	for (temPos = 1; temPos <= num; temPos++)
	{	// 复制叶结点字符信息与叶结点字符编码信息
		leafChars[temPos] = source.leafChars[temPos];		// 叶结点字符信息
		leafCharCodes[temPos] = source.leafCharCodes[temPos];	// 叶结点字符编码信息
	}
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType> &HuffmanTree<CharType, WeightType>::operator=(const HuffmanTree<CharType, WeightType>& source)	
// 操作结果：将哈夫曼树source赋值给当前哈夫曼树——重载赋值运算符
{
	if (&source != this)
	{
		if (nodes != NULL) delete []nodes;					// 释放结点信息
		if (leafChars != NULL) delete []leafChars;			// 释放叶结点字符信息
		if (leafCharCodes != NULL) delete []leafCharCodes;	// 释放叶结点字符编码信息

		num = source.num;									// 叶结点个数
		curPosition = source.curPosition;					// 译码时从根结点到叶结点路径的当前结点
		int m = 2 * num - 1;								// 结点总数 
		nodes = new HuffmanTreeNode<WeightType>[m + 1];		// nodes[0]未用
		leafChars = new CharType[num + 1];					// leafChars[0]未用
		leafCharCodes = new CharString[num + 1];			// leafCharCodes[0]未用

		int temPos;											// 临时变量 
		for (temPos = 1; temPos <= m; temPos++)
		{	// 复制结点信息
			nodes[temPos] = source.nodes[temPos];			// 结点信息
		}
		
		for (temPos = 1; temPos <= num; temPos++)
		{	// 复制叶结点字符信息与叶结点字符编码信息
			leafChars[temPos] = source.leafChars[temPos];	// 叶结点字符信息
			leafCharCodes[temPos] = source.leafCharCodes[temPos];	// 叶结点字符编码信息
		}	
	}
	return *this;
}

#endif
