#include <iostream>

using namespace std;

template<class T>
class LNode {
public:
	LNode() {};
	LNode(int val, LNode<T>* next)
		:m_val(val), m_next(next) {};
	T m_val;
	LNode<T>* m_next;
};
template<class T>
class LinkedQueue {
public:
	LinkedQueue();			//默认的构造函数
	~LinkedQueue();			//析构函数，执行所有链表节点内存的释放
	void pushBack(T& val);	//推入元素
	T popFront();			//弹出元素
	T getFront();			//得到队首元素的值
	bool isEmpty();			//返回该队列是否为空的布尔值
	void makeEmpty();		//清空队列，释放内存
	void printQueue();		//将队列的值输出到屏幕上

private:
	LNode<T>* m_rear = nullptr;	//队尾指针，默认为空
	LNode<T>* m_front = nullptr;//队头指针，默认为空
};


template<class T>
LinkedQueue<T>::LinkedQueue() {
}

template<class T>
LinkedQueue<T>::~LinkedQueue() {
	makeEmpty();
}

template<class T>
void LinkedQueue<T>::pushBack(T& val) {
	LNode<T>* nToPush = new LNode<T>(val, nullptr);
	if (!m_rear && !m_front) {//空集判定，若空则将头尾指针都设为其值
		m_rear = nToPush;
		m_front = nToPush;
		return;
	}
	if (!m_rear || !m_front) {
		cerr << "指针错误" << endl;
		return;
	}
	m_rear->m_next = nToPush;
	m_rear = nToPush;
	return;
}

template<class T>
T LinkedQueue<T>::popFront(){
	if (isEmpty()) {
		cerr << "空队列，不可弹出" << endl;
		return T();
	}
	LNode<T>* tmp = m_front;
	T retVal = tmp->m_val;
	m_front = m_front->m_next;
	if (m_front == nullptr) {//空集情况判定
		m_rear = nullptr;
	}
	delete tmp;//Tips: 内存泄漏
	return retVal;
}

template<class T>
T LinkedQueue<T>::getFront() {
	T retVal = m_front->m_val;
	return retVal;
}

template<class T>
bool LinkedQueue<T>::isEmpty() {
	return !(m_rear || m_front);
}

template<class T>
void LinkedQueue<T>::makeEmpty() {
	LNode<T>* pCur = m_front;
	while (pCur) {
		LNode<T>* tmp = pCur;
		pCur = pCur->m_next;
		delete tmp;
	}
	m_front = nullptr;
	m_rear = nullptr;
	return;
}

template<class T>
void LinkedQueue<T>::printQueue() {
	LNode<T>* pCur = m_front;
	int flag = 0;
	while (pCur) {
		if (flag++) {
			cout << " ";
		}
		cout << pCur->m_val;
		pCur = pCur->m_next;
	}
	cout << endl;
	return;
}

class BankQueue {
public:
	void getQueueData();
	void solveByPrior();
private:
	int m_totalNum;
	int m_failToSolve = 0;
	LinkedQueue<int> m_queueA;
	LinkedQueue<int> m_queueB;
	LinkedQueue<int> m_resQueue;
};

void BankQueue::getQueueData() {
	cout << "请输入一个[1, 1000]内的总人数N并且输入其顺序：" << endl;
	cin >> m_totalNum;
	if (m_totalNum <= 0 || m_totalNum > 1000) {
		cerr << "总人数有问题" << endl;
		m_failToSolve = 1;
		return;
	}
	for (int i = 0; i < m_totalNum; i++) {
		int tmp;
		cin >> tmp;
		if (tmp % 2) {
			m_queueA.pushBack(tmp);
		}
		else {
			m_queueB.pushBack(tmp);
		}
	}
}
void BankQueue::solveByPrior() {
	if (m_failToSolve) {
		return;
	}
	while (!m_queueA.isEmpty() && !m_queueB.isEmpty()) {
		int tmp = m_queueA.popFront();
		m_resQueue.pushBack(tmp);
		if (m_queueA.isEmpty()) {
			break;
		}
		tmp = m_queueA.popFront();
		m_resQueue.pushBack(tmp);
		tmp = m_queueB.popFront();
		m_resQueue.pushBack(tmp);
	}
	while (!m_queueA.isEmpty()) {
		int tmp = m_queueA.popFront();
		m_resQueue.pushBack(tmp);
	}
	while (!m_queueB.isEmpty()) {
		int tmp = m_queueB.popFront();
		m_resQueue.pushBack(tmp);
	}
	cout << "顾客办理业务最终的顺序为：" << endl;
	m_resQueue.printQueue();
}

int main() {
	BankQueue probQ;
	probQ.getQueueData();
	probQ.solveByPrior();
}


