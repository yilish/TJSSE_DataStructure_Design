#include <iostream>

using namespace std;

class LNode {
public:
	LNode() {};
	LNode(int val) :
		m_val(val) {};
	int m_val;
	LNode* m_next = nullptr;
};



class LinkedList {
public:
	
	LinkedList();
	void pushBackNode(int val);
	bool createList();
	void findIntersectionWith(LinkedList B);
private:
	LNode* m_head;
	LNode* m_tail;
};


LinkedList::LinkedList()
{
	LNode* head = new LNode;
	m_head = head;
	m_tail = head;
}


void LinkedList::pushBackNode(int val)
{
	LNode* newNode = new LNode(val);
	m_tail->m_next = newNode;
	m_tail = newNode;
}


bool LinkedList::createList()
{
	int tmp = -1;
	int pre = tmp;
	while (1)
	{
		pre = tmp;
		cin >> tmp;
		
		if (tmp == -1)
			return true;
		if (pre > tmp)
		{
			return false;
		}
		if (tmp < 0)
		{
			return false;
		}
		pushBackNode(tmp);
	}
}


void LinkedList::findIntersectionWith(LinkedList B)
{
	LNode* pA = this->m_head->m_next, *pB = B.m_head->m_next;
	bool flag = 0;
	while (pA && pB)
	{
		if (pA->m_val == pB->m_val)
		{
			if (flag == 1) {
				cout << ' ';
			}
			cout << pA->m_val;
			flag = 1;
			pA = pA->m_next, pB = pB->m_next;
		}
		else if (pA->m_val < pB->m_val)
		{
			pA = pA->m_next;
		}
		else
		{
			pB = pB->m_next;
		}
	}
	if (!flag)	//空集检验
	{
		cout << "NULL" ;
	}
	cout << endl;
}

int main() 
{
	cout << "请输入非降序排列的两组数，以-1为各数列的结尾。" << endl;
	LinkedList A, B;
	if (!A.createList() )
	{
		cout << "输入的数组有误，退出该程序" << endl;
		return 0;
	}
	if (!B.createList())
	{
		cout << "输入的数组有误，退出该程序" << endl;
		return 0;
	}
	A.findIntersectionWith(B);
	return 0;
}



