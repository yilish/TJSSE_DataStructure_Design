#include <iostream>

using namespace std;

//无需求结构，只需求结果，即用小根堆

template <class T>
class MinHeap {
public:
	MinHeap();				//无参构造函数
	MinHeap(int size)		//含大小的构造函数
		:m_maxSize(size) {
		m_heap = new T[size];
		auto p = m_heap;
		for (int i = 0; i < size; i++) {
			*p = 0;
			p++;
		}
	};
	void insert(T val);		//向小根堆插入新元素
	T pop();				//小根堆弹出元素
	T getTop();				//获取堆顶值
	bool isEmpty();			//判断堆空
	bool isFull();			//判断堆满
	
	~MinHeap() {};			//默认析构函数
private:
	void filterUp(int start);//自底向上调整堆
	void filterDown(int start, int end);//自顶向下调整堆
	T* m_heap;				//用数组模拟堆
	int m_maxSize;			//堆的最大大小
	int m_defaultSize = 10;	//默认大小
	int m_curSize = 0;		//当前堆的大小
};

template<class T>
MinHeap<T>::MinHeap() {
	m_maxSize = m_defaultSize;
	m_heap = new T[m_maxSize];
	auto p = m_heap;
	for (int i = 0; i < m_maxSize; i++) {
		*p = 0;
		p++;
	}
}

template<class T>
void MinHeap<T>::insert(T val) {
	if (isFull()) {
		cerr << "堆满, 请建更大的堆" << endl;
		return;
	}
	//插入逻辑，尾插后上浮调整
	m_heap[m_curSize] = val;
	filterUp(m_curSize++);
}

template<class T>
T MinHeap<T>::pop() {
	if (isEmpty()) {
		cerr << "空集，无法弹出堆顶元素" << endl;
		return T();
	}
	T retVal = m_heap[0];
	m_heap[0] = m_heap[m_curSize - 1];	//取堆尾元素
	m_curSize--;
	filterDown(0, m_curSize - 1);	//自上而下调整堆
	return retVal;
}

template<class T>
T MinHeap<T>::getTop() {
	return m_heap[0];
}

template<class T>
bool MinHeap<T>::isEmpty() {
	return m_curSize == 0;
}

template<class T>
bool MinHeap<T>::isFull() {
	return m_curSize == m_maxSize;
}

template<class T>
void MinHeap<T>::filterUp(int start) {
	int j = start, i = (j - 1) / 2;	//i表示j的父亲节点
	T tmp = m_heap[start];			//暂存，从下向上调整，取要调整的节点
	while (j) {
		if (m_heap[i] <= tmp) {		//父节点较小的情况
			break;
		}
		else {						//父节点较大，与父节点互换
			m_heap[j] = m_heap[i];
			j = i;
			i = (i - 1) / 2;
		}
	}
	m_heap[j] = tmp;					//回送
}

template<class T>
void MinHeap<T>::filterDown(int start, int end) {
	int i = start, j = 2 * i + 1;	//i表示父节点，j表示左子树
	T tmp = m_heap[start];			//需要下浮，因此暂存起始节点
	while (j <= end) {				//j到达终点前
		if (j < end && m_heap[j] > m_heap[j + 1]) {
			j++;					//取两子女中小者
		}
		if (tmp <= m_heap[j]) {		//如果小，就调整结束
			break;
		}
		else {						//否则继续调整
			m_heap[i] = m_heap[j];
			i = j;
			j = 2 * j + 1;
		}
	}
	m_heap[i] = tmp;					//回放tmp中的暂存元素
}

class RepairFarm {
public:
	RepairFarm() {};				//默认构造函数
	~RepairFarm() {};				//默认析构函数
	void initIO();					//初始化IO
	void processHeap();				//处理堆
	void outputRes();				//输出结论
private:	
	int m_totalNum = 0;				//木头总数
	MinHeap<int> m_minHeap;			//小根堆成员
	int m_result = 0;				//输出结论
	bool m_fLegalInput = 1;			//输入合法标记
};

void RepairFarm::initIO() {
	cout << "请输入要锯的木头数:";
	cin >> m_totalNum;
	m_minHeap = MinHeap<int>(m_totalNum + 1);
	while (m_totalNum <= 0) {
		cout << "木头总数错误, 请重输入:";
		cin >> m_totalNum;
	}
	cout << "请分别输入木头长度:";
	for (int i = 0; i < m_totalNum; i++) {
		int tmp;
		cin >> tmp;
		if (tmp < 0) {
			cerr << "请不要输入负数" << endl;
			m_fLegalInput = 0;
			return;
		}
		m_minHeap.insert(tmp);
	}
}

void RepairFarm::processHeap() {
	if (m_fLegalInput == 0) {
		return;
	}
	while (1) {
		int min1 = m_minHeap.pop();
		m_result += min1;
		if (m_minHeap.isEmpty()) {			
			break;
		}

		int min2 = m_minHeap.pop();
		m_result += min2;
		if (m_minHeap.isEmpty()) {
			break;
		}
		m_minHeap.insert(min1 + min2);
	}
}

void RepairFarm::outputRes() {
	if (m_fLegalInput == 0) {
		return;
	}
	cout << "总花费为: " <<  m_result << endl;
}

int main() {

	RepairFarm sol;
	sol.initIO();
	sol.processHeap();
	sol.outputRes();
}

