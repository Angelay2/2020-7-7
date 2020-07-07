/*
�²���Ľ���ڸ��������,ƽ������-1, ���ұ�+1,

��ߵ���߸�: ����
�ұߵ��ұ߸�: ����
��ߵ��ұ߸�: �Ƚ�������������, �ٽ������������
�ұߵ���߸�: �Ƚ�������������, �ٽ������������

AVL��ת:
	1. ����:
		a. ����: parent->bf == 2 && cur->bf == 1   (parent,subRL,subR)
			subR->left = parent;
			parent->right = subRL;
				subR->bf = parent->bf = 0;
		b. ����: parent->bf == -2 &&cur->bf == -1   (parent, subL, subLR)
			subL->right = parent;
			parent->left = subLR;
				subR->bf = parent->bf = 0;
	2. ˫��
		a. ����˫��: parent->bf == -2 && cur->bf == 1   (subL,subLR,parent)
			subLR->left = subL;
			subLR->right = parent;
			subL->right = subLRԭ�е�������
			parent->left = subLRԭ�е�������
				sub,parent:
				subLRԭʼƽ������Ϊ-1ʱ, sub->bf = 0; parent->bf = 1;
				subLRԭʼƽ������Ϊ1ʱ, parent->bf = 0; sub->bf = -1;
		b. ����˫��: parent->bf ==2 && cur->bf == -1;   (parent,subRL,subR)
			subRL->left = parent;
			subRL->right = subR;
			subR->left = subRԭ�е�������;
			parent->right = subRԭ�е�������;
				sub,parent:
				subRLԭʼƽ������Ϊ-1ʱ, parent->bf = 0; sub->bf = 1;
				subRLԭʼƽ������Ϊ1ʱ, sub->bf = 0; parent->bf = -1;

			
*/
#include <iostream>
using namespace std;
template <class T>
struct AVLNode
{
	T _value;
	int _bf;
	AVLNode<T>* _parent;
	AVLNode<T>* _left;
	AVLNode<T>* _right;

	AVLNode(const T& val = T())
		:_value(val)
		, _bf(0)
		, _parent(nullptr)
		, _left(nullptr)
		, _right(nullptr)
	{}
};

template <class T>
class AVLTree
{
public:
	typedef AVLNode<T> Node;
	AVLTree(Node* root = nullptr)
		:_root(root)
	{}

	bool insert(const T& val)
	{
		if (_root == nullptr)
		{
			_root = new Node(val);
			return true;
		}

		Node* cur = _root;
		Node* parent = nullptr;
		while (cur)
		{
			parent = cur;
			if (cur->_value == val)
				return false;
			else if (cur->_value < val)
				cur = cur->_right;
			else
				cur = cur->_left;
		}
		cur = new Node(val);
		if (parent->_value < val)
			parent->_right = cur;
		else
			parent->_left = cur;

		cur->_parent = parent;

		// 1. ����
		while (parent)
		{
			//ƽ�����Ӹ���
			if (parent->_left == cur)
				--parent->_bf;
			else
				++parent->_bf;
			//�ж��Ƿ���Ҫ�������º͵���
			if (parent->_bf == 0)
				break;
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				//�������ϸ���
				cur = parent;
				parent = parent->_parent;
			}
			else if (abs(parent->_bf) == 2)
			{
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					//��ߵ���߸ߣ���������
					RotateR(parent);
					cout << "�ڵ㣺" << parent->_value << " ����" << endl;
				}
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					//�ұߵ��ұ߸ߣ� ��������
					RotateL(parent);
					cout << "�ڵ㣺" << parent->_value << " ����" << endl;
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					//��ߵ��ұ߸ߣ� ����˫��
					cout << "�ڵ㣺" << cur->_value << " " << parent->_value << " ����˫��" << endl;
					RotateL(cur);
					RotateR(parent);

				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					Node* subR = parent->_right;
					Node* subRL = subR->_left;
					int bf = subRL->_bf;

					//�ұߵ���߸ߣ�����˫��
					cout << "�ڵ㣺" << cur->_value << " " << parent->_value << "����˫��" << endl;
					RotateR(cur);
					RotateL(parent);

					//ƽ�����Ӹ���
					if (bf == 1)
					{
						subR->_bf = 0;
						parent->_bf = -1;
					}
					else
					{
						subR->_bf = 1;
						parent->_bf = 0;
					}
				}
				break;
			}
		}
		return true;
	}

	void RotateR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;

		subL->_right = parent;
		parent->_left = subLR;

		if (subLR)
			subLR->_parent = parent;

		if (parent == _root)
		{
			_root = subL;
			subL->_parent = nullptr;
		}
		else
		{
			Node* pp = parent->_parent;
			subL->_parent = pp;
			if (pp->_left == parent)
				pp->_left = subL;
			else
				pp->_right = subL;
		}

		parent->_parent = subL;

		//����ƽ������
		subL->_bf = parent->_bf = 0;
	}

	void RotateL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;

		subR->_left = parent;
		parent->_right = subRL;

		if (subRL)
			subRL->_parent = parent;

		if (parent == _root)
		{
			subR->_parent = nullptr;
			_root = subR;
		}
		else
		{
			Node* pp = parent->_parent;
			subR->_parent = pp;
			if (pp->_left == parent)
				pp->_left = subR;
			else
				pp->_right = subR;
		}
		parent->_parent = subR;

		subR->_bf = parent->_bf = 0;
	}

	void inorder()
	{
		_inorder(_root);
		cout << endl;
	}

	void _inorder(Node* root)
	{
		if (root)
		{
			_inorder(root->_left);
			cout << root->_value << " ";
			_inorder(root->_right);
		}
	}

	bool isBalance()
	{
		return _isBalance(_root);
	}

	bool _isBalance(Node* root)
	{
		if (root == nullptr)
			return true;
		//ÿ���ڵ����������ĸ߶Ȳ����ֵ������1
		int left = Height(root->_left);
		int right = Height(root->_right);
		if (right - left != root->_bf)
		{
			cout << "�ڵ㣺 " << root->_value << "�쳣: �߶Ȳ� " << right - left << " != bf " << root->_bf << endl;
			return false;
		}

		return abs(root->_bf) < 2
			&& _isBalance(root->_left)
			&& _isBalance(root->_right);
	}

	int Height(Node* root)
	{
		if (root == nullptr)
			return 0;
		int left = Height(root->_left);
		int right = Height(root->_right);
		return left > right ? left + 1 : right + 1;
	}

private:
	Node* _root;
};

void testAVL()
{
	AVLTree<int>  avl;
	avl.insert(1);
	avl.insert(2);
	avl.insert(3);
	avl.insert(4);
	avl.insert(5);
	avl.insert(6);
	avl.insert(7);
	avl.insert(8);
	avl.insert(9);

	avl.insert(20);
	avl.insert(19);
	avl.insert(18);
	avl.insert(17);
	avl.insert(16);
	avl.insert(15);
	avl.insert(14);

	avl.inorder();

	cout << "isBalance: " << avl.isBalance() << endl;
}

void testAVL2()
{
	//int array[] = { 16, 3, 7, 11, 9, 26, 18, 14, 15 };
	int array[] = { 4, 2, 6, 1, 3, 5, 15, 7, 16, 14 };
	AVLTree<int> avl;
	for (const auto& e : array)
	{
		avl.insert(e);
	}
	avl.inorder();
	cout << endl;
	cout << "isBalance: " << avl.isBalance() << endl;
}

int main()
{
	//testAVL();
	testAVL2();
	return 0;
}