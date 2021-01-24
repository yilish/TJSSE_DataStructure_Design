#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>

using namespace std;


struct Course {
	string id = "";
	string name = "";
	int period = 0;
	int term = 0;
	string prevCourses[10] = {};
	int prevNum = 0;
	bool termLocked = 0;
};
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
T LinkedQueue<T>::popFront() {
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
class Solution {
public:
	Solution();
	Course* m_courseArr = nullptr;
	void loadFile();
	void topoSort();

	void saveFile(ostream& fs);
	void arrangeCourse(int x);
	int put3(int curTerm, int& curDay, int i);
	int put2(int curTerm, int& curDay, int i);
	int put1(int curTerm, int& curDay, int i);
	int m_courseTable[8][5][10];	//container of result
	void outputCourse();
	int m_courseNum;
};

Solution::Solution() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 5; j++) {
			for (int k = 0; k < 10; k++) {
				m_courseTable[i][j][k] = -1;
			}
		}
	}
	m_courseNum = 0;
}

void Solution::loadFile() {
	string fileName = "testpoint";

	ifstream ifs;
	ifs.open(fileName);
	ifs >> m_courseNum;
	string tmp;
	getline(ifs, tmp);
	m_courseArr = new Course[m_courseNum + 1];
	int i = 0;
	for (i = 0; !ifs.eof(); i++) {
		string strLine;
		getline(ifs, strLine);
		stringstream ss(strLine);

		ss >> m_courseArr[i].id >>
			m_courseArr[i].name >>
			m_courseArr[i].period >>
			m_courseArr[i].term;
		if (m_courseArr[i].term > 0) {
			m_courseArr[i].termLocked = 1;
			m_courseArr[i].term--;
		}
		else {
			m_courseArr[i].termLocked = 0;
		}
		int k = 0;
		while (!ss.eof()) {
			string prev;
			ss >> prev;
			m_courseArr[i].prevCourses[k++] = prev;
		}
		m_courseArr[i].prevNum = k;
	}
	ifs.close();

	if (i != m_courseNum) {
		cerr << "Input data doesn't match the total number of courses.";
		m_courseNum = i;
	}
	//outputCourse();
}

void Solution::topoSort() {
	LinkedQueue<int> q;

	int* preCount = new int[m_courseNum];
	//优先安排锁定学期的课程

	for (int i = 0; i < m_courseNum; i++) {
		if (m_courseArr[i].termLocked && m_courseArr[i].prevNum == 0) {
			q.pushBack(i);
		}
	}
	//先入队没有任何前驱的元素
	for (int i = 0; i < m_courseNum; i++) {
		preCount[i] = m_courseArr[i].prevNum;
		if (m_courseArr[i].termLocked && m_courseArr[i].prevNum == 0) {
			continue;
		}
		if (!preCount[i]) {
			q.pushBack(i);
		}
	}

	while (!q.isEmpty()) {
		auto x = q.popFront();
		arrangeCourse(x);// sure to succeed
		for (int i = 0; i < m_courseNum; i++) {
			for (int j = 0; j < m_courseArr[i].prevNum; j++) {
				if (m_courseArr[i].prevCourses[j] == m_courseArr[x].id) {
					preCount[i]--;
					if (preCount[i] == 0) {


						if (m_courseArr[i].term < m_courseArr[x].term + 1) {
							if (!m_courseArr[i].termLocked) {
								m_courseArr[i].term = m_courseArr[x].term + 1; //置后一学期
							}
						}
						q.pushBack(i);
					}
				}
			}
		}

	}

}

void Solution::saveFile(ostream& fs) {

	for (int curTerm = 0; curTerm < 8; curTerm++) {
		fs << "Term" << curTerm + 1 << ":" << endl;
		if (curTerm == 7) {
			int a = 1;
		}
		for (int i = 0; i < 10; i++) {
			for (int j = 0; j < 5; j++) {
				int x = m_courseTable[curTerm][j][i];

				if (x == -1) {
					fs << left << setw(20) << "-------free------" << '\t' << '\t';
				}
				else {
					fs << left << setw(20) << m_courseArr[x].name
						+ " " + m_courseArr[x].id << '\t' << '\t';
				}

			}
			fs << endl;
		}
		fs << endl;
	}
}

void Solution::arrangeCourse(int x) {
	if (x == 37) {
		int a = 1;
	}
	int curDay = 0;
	int alreadyPut = 0;
	auto remainToPut = m_courseArr[x].period - alreadyPut;
	int loopTime = 0;
	if (m_courseArr[x].termLocked) {
		while (alreadyPut < m_courseArr[x].period && m_courseArr[x].term < 8) {
			remainToPut = m_courseArr[x].period - alreadyPut;
			int flag = 0;
			if (remainToPut >= 3) {
				flag = put3(m_courseArr[x].term, curDay, x);

			}
			else if (remainToPut == 2) {
				flag = put2(m_courseArr[x].term, curDay, x);
			}
			else {
				flag = put1(m_courseArr[x].term, curDay, x);
			}

			alreadyPut += flag;
			loopTime++;
			if (loopTime > 15) {
				if (remainToPut) {
					cerr << "第" << x + 1 << "门课不可排进课表中。" << endl;
				}
			}
		}
	}
	else {
		while (alreadyPut < m_courseArr[x].period && m_courseArr[x].term < 8) {
			remainToPut = m_courseArr[x].period - alreadyPut;
			int flag = 0;
			if (remainToPut >= 3) {
				flag = put3(m_courseArr[x].term, curDay, x);

			}
			else if (remainToPut == 2) {
				flag = put2(m_courseArr[x].term, curDay, x);
			}
			else {
				flag = put1(m_courseArr[x].term, curDay, x);
			}

			alreadyPut += flag;

			if (!flag) {//寻找下个学期的机会
				if (!m_courseArr[x].termLocked) {
					m_courseArr[x].term++;
					curDay = 0;
				}
			}
		}
	}
	remainToPut = m_courseArr[x].period - alreadyPut;

	if (remainToPut) {
		cerr << "第" << x + 1 << "门课不可排进课表中。" << endl;
	}
}

int Solution::put3(int curTerm, int& curDay, int i) {
	for (auto t = 0; t < 5; t++) {

		//234连排
		if (m_courseTable[curTerm][curDay][2] == -1
			&& m_courseTable[curTerm][curDay][3] == -1
			&& m_courseTable[curTerm][curDay][4] == -1) {

			m_courseTable[curTerm][curDay][2] = i;
			m_courseTable[curTerm][curDay][3] = i;
			m_courseTable[curTerm][curDay][4] = i;

			curDay += 2;
			curDay %= 5;
			return 3;
		}
		//789连排
		if (m_courseTable[curTerm][curDay][7] == -1
			&& m_courseTable[curTerm][curDay][8] == -1
			&& m_courseTable[curTerm][curDay][9] == -1) {
			m_courseTable[curTerm][curDay][7] = i;
			m_courseTable[curTerm][curDay][8] = i;
			m_courseTable[curTerm][curDay][9] = i;
			curDay += 2;
			curDay %= 5;
			return 3;
		}

		curDay += 2;
		curDay %= 5;
	}
	return 0;
}

int Solution::put2(int curTerm, int& curDay, int i) {
	for (auto t = 0; t < 5; t++) {

		//01连排
		if (m_courseTable[curTerm][curDay][0] == -1
			&& m_courseTable[curTerm][curDay][1] == -1) {
			m_courseTable[curTerm][curDay][0] = i;
			m_courseTable[curTerm][curDay][1] = i;


			curDay += 2;
			curDay %= 5;
			return 2;
		}
		//56连排
		if (m_courseTable[curTerm][curDay][5] == -1
			&& m_courseTable[curTerm][curDay][6] == -1) {
			m_courseTable[curTerm][curDay][5] = i;
			m_courseTable[curTerm][curDay][6] = i;

			curDay += 2;
			curDay %= 5;
			return 2;
		}
		curDay += 2;
		curDay %= 5;

	}
	return 0;
}

int Solution::put1(int curTerm, int& curDay, int i) {
	for (auto t = 0; t < 5; t++) {

		for (int j = 0; j < 5; j++) {
			if (m_courseTable[curTerm][curDay][j] == -1) {
				m_courseTable[curTerm][curDay][j] = i;
				curDay += 2;
				curDay %= 5;
				return 1;
			}
		}
		curDay += 2;
		curDay %= 5;
	}
	return 0;
}


void Solution::outputCourse() {
	for (int i = 0; i < m_courseNum; i++) {
		cout << m_courseArr[i].id << " "
			<< m_courseArr[i].name << " "
			<< m_courseArr[i].period << " "
			<< m_courseArr[i].term << " ";
		cout << m_courseArr[i].prevNum << "courses:";
		for (int j = 0; j < m_courseArr[i].prevNum; j++) {
			cout << m_courseArr[i].prevCourses[j] << " ";
		}
		cout << endl;
	}
}

int main() {
	Solution sol;
	sol.loadFile();
	sol.topoSort();
	ofstream ofs("res");

	sol.saveFile(ofs);
}
