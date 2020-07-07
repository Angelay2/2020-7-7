/*
新插入的结点在父结点的左边,平衡因子-1, 在右边+1,

左边的左边高: 右旋
右边的右边高: 左旋
左边的右边高: 先进行子树的左旋, 再进行整体的右旋
右边的左边高: 先进行子树的右旋, 再进行整体的左旋

AVL旋转:
	1. 单旋:
		a. 左旋: parent->bf == 2 && cur->bf == 1   (parent,subRL,subR)
			subR->left = parent;
			parent->right = subRL;
				subR->bf = parent->bf = 0;
		b. 右旋: parent->bf == -2 &&cur->bf == -1   (parent, subL, subLR)
			subL->right = parent;
			parent->left = subLR;
				subR->bf = parent->bf = 0;
	2. 双旋
		a. 左右双旋: parent->bf == -2 && cur->bf == 1   (subL,subLR,parent)
			subLR->left = subL;
			subLR->right = parent;
			subL->right = subLR原有的左子树
			parent->left = subLR原有的右子树
				sub,parent:
				subLR原始平衡因子为-1时, sub->bf = 0; parent->bf = 1;
				subLR原始平衡因子为1时, parent->bf = 0; sub->bf = -1;
		b. 右左双旋: parent->bf ==2 && cur->bf == -1;   (parent,subRL,subR)
			subRL->left = parent;
			subRL->right = subR;
			subR->left = subR原有的右子树;
			parent->right = subR原有的左子树;
				sub,parent:
				subRL原始平衡因子为-1时, parent->bf = 0; sub->bf = 1;
				subRL原始平衡因子为1时, sub->bf = 0; parent->bf = -1;

			
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

		// 1. 调整
		while (parent)
		{
			//平衡因子更新
			if (parent->_left == cur)
				--parent->_bf;
			else
				++parent->_bf;
			//判断是否需要继续更新和调整
			if (parent->_bf == 0)
				break;
			else if (parent->_bf == -1 || parent->_bf == 1)
			{
				//继续向上更新
				cur = parent;
				parent = parent->_parent;
			}
			else if (abs(parent->_bf) == 2)
			{
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					//左边的左边高，进行右旋
					RotateR(parent);
					cout << "节点：" << parent->_value << " 右旋" << endl;
				}
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					//右边的右边高， 进行左旋
					RotateL(parent);
					cout << "节点：" << parent->_value << " 左旋" << endl;
				}
				else if (parent->_bf == -2 && cur->_bf == 1)
				{
					//左边的右边高： 左右双旋
					cout << "节点：" << cur->_value << " " << parent->_value << " 左右双旋" << endl;
					RotateL(cur);
					RotateR(parent);

				}
				else if (parent->_bf == 2 && cur->_bf == -1)
				{
					Node* subR = parent->_right;
					Node* subRL = subR->_left;
					int bf = subRL->_bf;

					//右边的左边高：右左双旋
					cout << "节点：" << cur->_value << " " << parent->_value << "右左双旋" << endl;
					RotateR(cur);
					RotateL(parent);

					//平衡因子更新
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

		//更新平衡因子
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
		//每个节点左右子树的高度差绝对值不超过1
		int left = Height(root->_left);
		int right = Height(root->_right);
		if (right - left != root->_bf)
		{
			cout << "节点： " << root->_value << "异常: 高度差 " << right - left << " != bf " << root->_bf << endl;
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