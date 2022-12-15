#ifndef __HUFFMAN_TREE_H__
#define __HUFFMAN_TREE_H__

#include <iostream>											// ����Ԥ��������
#include <cstdlib>											// ��C����exit()��������stdlib.h��cstdlib��C��ͷ�ļ���
#include "char_string.h"									// ����
#include "huffman_tree_node.h"								// �������������ģ��

// ����������ģ��
template <class CharType, class WeightType>
class HuffmanTree
{
protected:
// ���ݳ�Ա:
	HuffmanTreeNode<WeightType> *nodes;						// �洢�����Ϣ��nodes[0]δ��
	CharType *leafChars;									// Ҷ����ַ���Ϣ��leafChars[0]δ��
	CharString *leafCharCodes;								// Ҷ����ַ�������Ϣ��leafCharCodes[0]δ��
	int curPosition;										// ����ʱ�Ӹ���㵽Ҷ���·���ĵ�ǰ���
	int num;												// Ҷ������

// ��������ģ��:
	void Select(int cur, int &r1, int &r2);					// nodes[1 ~ cur]��ѡ��˫��Ϊ0��Ȩֵ��С���������r1��r2

public:
// ���������������������ر���ϵͳĬ�Ϸ�������:
	HuffmanTree(CharType ch[], WeightType w[], int n);		// ���ַ���Ȩֵ���ַ����������������
	virtual ~HuffmanTree();									// ��������ģ��
	CharString Encode(CharType ch);							// ����
	LinkList<CharType> Decode(CharString codeStr);			// ����
	HuffmanTree(const HuffmanTree<CharType, WeightType> &source);	// ���ƹ��캯��ģ��
	HuffmanTree<CharType, WeightType> &operator=(const HuffmanTree<CharType, 
		WeightType> &source);								// ���ظ�ֵ�����
};

// �����ֵܱ�ʾ����������ģ���ʵ�ֲ���
template <class CharType, class WeightType>
void HuffmanTree<CharType, WeightType>::Select(int cur, int &r1, int &r2)
// ���������nodes[1 ~ cur]��ѡ��˫��Ϊ0��Ȩֵ��С���������r1��r2
{
	r1 = r2 = 0;											// 0��ʾ�ս��
	for (int temPos = 1; temPos <= cur; temPos++)
	{	// ������ֵ��С���������
		if (nodes[temPos].parent != 0) continue;			// ֻ����˫�ײ�Ϊ0�Ľ��
		if (r1 == 0)
		{	// r1Ϊ��,��temPos��ֵ��r1
			r1 = temPos;
		}
		else if (r2 == 0)
		{	// r2Ϊ��,��temPos��ֵ��r2
			r2 = temPos;
		}
		else if	(nodes[temPos].weight < nodes[r1].weight)
		{	// nodes[temPos]Ȩֵ��nodes[r1]��С����temPos��Ϊr1
			r1 = temPos;
		}
		else if (nodes[temPos].weight < nodes[r2].weight)
		{	// nodes[temPos]Ȩֵ��nodes[r2]��С����temPos��Ϊr2
			r2 = temPos;
		}
	}
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType>::HuffmanTree(CharType ch[], WeightType w[], int n)
// ������������ַ���Ȩֵ���ַ����������������
{
	num = n;												// Ҷ������
	int m = 2 * n - 1;										// ������

	nodes = new HuffmanTreeNode<WeightType>[m + 1];			// nodes[0]δ��
	leafChars = new CharType[n + 1];						// leafChars[0]δ��
	leafCharCodes = new CharString[n + 1];					// leafCharCodes[0]δ��

	int temPos;												// ��ʱ���� 
	for (temPos = 1; temPos <= n; temPos++)
	{	// �洢Ҷ�����Ϣ
		nodes[temPos].weight = w[temPos - 1];				// Ȩֵ
		leafChars[temPos] = ch[temPos - 1];					// �ַ�
	}

	for (temPos = n + 1; temPos <= m; temPos++)
	{	// ������������
		int r1, r2;
		Select(temPos - 1, r1, r2);	// nodes[1 ~ temPos - 1]��ѡ��˫��Ϊ0��Ȩֵ��С���������r1��r2

		// �ϲ���r1,r2Ϊ������
		nodes[r1].parent = nodes[r2].parent = temPos;		// r1��r2˫��ΪtemPos
		nodes[temPos].leftChild = r1;						// r1ΪtemPos������
		nodes[temPos].rightChild = r2;						// r2ΪtemPos���Һ���
		nodes[temPos].weight = nodes[r1].weight + nodes[r2].weight;//temPos��ȨΪr1��r2��Ȩֵ֮��
	}

	for (temPos = 1; temPos <= n; temPos++)
	{	// ��n��Ҷ����ַ��ı���
		LinkList<char> charCode;							// �ݴ�Ҷ����ַ�������Ϣ
		for (unsigned int child = temPos, parent = nodes[child].parent; parent != 0; 
			child = parent, parent = nodes[child].parent)
		{	// ��Ҷ��㵽��������������
			if (nodes[parent].leftChild == child) charCode.Insert(1, '0');// ���֧����Ϊ'0'
			else charCode.Insert(1, '1');					// �ҷ�֧����Ϊ'1'
		}
		leafCharCodes[temPos] = charCode;					// charCode�д洢�ַ�����
	}
	
	curPosition = m;										// ����ʱ�Ӹ���㿪ʼ��mΪ��
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType>::~HuffmanTree()
// ������������ٹ�������
{	
	if (nodes != NULL) delete []nodes;						// �ͷŽ����Ϣ
	if (leafChars != NULL) delete []leafChars;				// �ͷ�Ҷ����ַ���Ϣ
	if (leafCharCodes != NULL) delete []leafCharCodes;		// �ͷ�Ҷ����ַ�������Ϣ
}

template <class CharType, class WeightType>
CharString HuffmanTree<CharType, WeightType>::Encode(CharType ch)
// ��������������ַ�����
{
	int temPos;												// ��ʱ���� 
	for (temPos = 1; temPos <= num; temPos++)
	{	// �����ַ���λ��
		if (leafChars[temPos] == ch) break;					// �ҵ��ַ����˳�ѭ��
	}
	if (temPos <= num)
	{	// ��ʾ�ҵ��ַ�
		return leafCharCodes[temPos];						// �ҵ��ַ����õ�����
	}
	else
	{	// ��ʾδ�ҵ��ַ��������쳣
		cout << "�Ƿ��ַ����޷�����!" << endl;				// ��ʾ��Ϣ
		exit(1);											// �˳�����
	}
}

template <class CharType, class WeightType>
LinkList<CharType> HuffmanTree<CharType, WeightType>::Decode(CharString codeStr)
// ����������Ա��봮codeStr��������,���ر���ǰ���ַ�����
{
	LinkList<CharType> charList;							// ����ǰ���ַ�����
	
	for (int temPos = 0; temPos < codeStr.Length(); temPos++)
	{	// ����ÿλ����
		if (codeStr[temPos] == '0') curPosition = nodes[curPosition].leftChild;	// '0'��ʾ���֧	
		else curPosition = nodes[curPosition].rightChild;			// '1'��ʾ���֧	

		if (nodes[curPosition].leftChild == 0 && nodes[curPosition].rightChild == 0)
		{	// ����ʱ�Ӹ���㵽Ҷ���·���ĵ�ǰ���ΪҶ���
			charList.Insert(charList.Length() + 1, leafChars[curPosition]);
			curPosition = 2 * num - 1;						// curPosition�ع�����
		}
	}
	return charList;										// ���ر���ǰ���ַ�����
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType>::HuffmanTree(const HuffmanTree<CharType, WeightType> &source)
// ����������ɹ�������source�����¹��������������ƹ��캯��ģ��
{
	num = source.num;										// Ҷ������
	curPosition = source.curPosition;						// ����ʱ�Ӹ���㵽Ҷ���·���ĵ�ǰ���
	int m = 2 * num - 1;									// ������� 
	nodes = new HuffmanTreeNode<WeightType>[m + 1];			// nodes[0]δ��
	leafChars = new CharType[num + 1];						// leafChars[0]δ��
	leafCharCodes = new CharString[num + 1];				// leafCharCodes[0]δ��

	int temPos;												// ��ʱ���� 
	for (temPos = 1; temPos <= m; temPos++)
	{	// ���ƽ����Ϣ
		nodes[temPos] = source.nodes[temPos];				// �����Ϣ
	}
	
	for (temPos = 1; temPos <= num; temPos++)
	{	// ����Ҷ����ַ���Ϣ��Ҷ����ַ�������Ϣ
		leafChars[temPos] = source.leafChars[temPos];		// Ҷ����ַ���Ϣ
		leafCharCodes[temPos] = source.leafCharCodes[temPos];	// Ҷ����ַ�������Ϣ
	}
}

template <class CharType, class WeightType>
HuffmanTree<CharType, WeightType> &HuffmanTree<CharType, WeightType>::operator=(const HuffmanTree<CharType, WeightType>& source)	
// �������������������source��ֵ����ǰ���������������ظ�ֵ�����
{
	if (&source != this)
	{
		if (nodes != NULL) delete []nodes;					// �ͷŽ����Ϣ
		if (leafChars != NULL) delete []leafChars;			// �ͷ�Ҷ����ַ���Ϣ
		if (leafCharCodes != NULL) delete []leafCharCodes;	// �ͷ�Ҷ����ַ�������Ϣ

		num = source.num;									// Ҷ������
		curPosition = source.curPosition;					// ����ʱ�Ӹ���㵽Ҷ���·���ĵ�ǰ���
		int m = 2 * num - 1;								// ������� 
		nodes = new HuffmanTreeNode<WeightType>[m + 1];		// nodes[0]δ��
		leafChars = new CharType[num + 1];					// leafChars[0]δ��
		leafCharCodes = new CharString[num + 1];			// leafCharCodes[0]δ��

		int temPos;											// ��ʱ���� 
		for (temPos = 1; temPos <= m; temPos++)
		{	// ���ƽ����Ϣ
			nodes[temPos] = source.nodes[temPos];			// �����Ϣ
		}
		
		for (temPos = 1; temPos <= num; temPos++)
		{	// ����Ҷ����ַ���Ϣ��Ҷ����ַ�������Ϣ
			leafChars[temPos] = source.leafChars[temPos];	// Ҷ����ַ���Ϣ
			leafCharCodes[temPos] = source.leafCharCodes[temPos];	// Ҷ����ַ�������Ϣ
		}	
	}
	return *this;
}

#endif
