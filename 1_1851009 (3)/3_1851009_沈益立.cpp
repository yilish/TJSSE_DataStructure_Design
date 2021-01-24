#include <iostream>

using namespace std;

template<class T>
class Stack {
public:
	//Stack<T>();
	Stack();					//构造函数
	~Stack();					//析构函数
	void push(const T &val);	//推入新元素到栈顶
	void showElements();		//在屏幕上按次序输出栈内元素
	bool isEmpty();				//获取是否栈空
	void makeEmpty();			//将栈复位
	T pop();					//弹出栈顶元素并返回其值
	T top();					//返回栈顶元素
	int getSize();				//返回栈大小
private:
	int m_top;					//表示栈顶
	int m_maxSize;				//栈目前的最大大小
	T* m_elements;				//数组的首元地址
};

template<class T>
Stack<T>::Stack(){
	m_maxSize = 2;
	m_elements = new T[m_maxSize];
	m_top = -1;
}

template<class T>
Stack<T>::~Stack(){
	delete[] m_elements;
}

template<class T>
void Stack<T>::push(const T& val){
	m_top++;
	if (m_top >= m_maxSize) {//栈溢出
		T* tmp = m_elements;
		m_maxSize *= 2;
		m_elements = new T[m_maxSize];
		for (int i = 0; i < m_maxSize / 2; i++) {
			m_elements[i] = tmp[i];
		}//copy
		delete[] tmp;
	}
	m_elements[m_top] = val;
}
template<class T>
void Stack<T>::showElements(){
	for (int i = 0; i < m_top; i++) {
		cout << m_elements[i] << "->";
	}
	cout << m_elements[m_top] << endl;
}
template<class T>
bool Stack<T>::isEmpty(){
	return m_top == -1;
}
template<class T>
void Stack<T>::makeEmpty(){
	m_top = -1;
}
template<class T>
T Stack<T>::pop(){
	if (isEmpty()) {
		cerr << "Empty stack, nothing to pop." << endl;
		return T();
	}
	T retVal = m_elements[m_top--];
	return retVal;
}
template<class T>
T Stack<T>::top(){
	if (isEmpty()) {
		cerr << "Empty stack, nothing on the top." << endl;
		return T();
	}
	return m_elements[m_top];
}
template<class T>
int Stack<T>::getSize(){
	return m_top + 1;
}

template<class T1, class T2>
class MyPair {
public:
	T1 first;	//第一个元素
	T2 second;	//第二个元素
	MyPair() :	//默认构造函数
		first(T1()), second(T2()) {};
	MyPair(const T1& a, const T2& b) :
	//含两个参数的构造函数，直接赋值
		first(a), second(b) {};
	bool operator ==(const MyPair<T1, T2>& b) {
		//==运算符重载，判定两个元素是否完全相等
		return this->first == b.first && this->second == b.second;
	}

	MyPair operator +(const MyPair<T1, T2>& b) {
		//+运算符重载，返回<x1+x2, y1+y2>
		MyPair<T1, T2> res;
		res.first = this->first + b.first;
		res.second = this->second + b.second;
		return res;
	}

	friend ostream& operator<<(ostream& output,
		const MyPair<T1, T2>& p){
		//输出流重载，输出为(x, y)形式
		output << "(" << p.first << "," << p.second << ")";
		return output;
	}
};

class Maze {
public:
	Maze();					//构造函数
	void getMazeData();		//通过IO实现迷宫信息的初始化
	bool isInRange(MyPair<int, int> p);//判断某个点是否在迷宫内
	void dfsTrace();		//通过回溯找到可行的路径
	void showTrace();		//将找到的路径展示在屏幕上
private:
	int m_rows;				//行数
	int m_cols;				//列数
	MyPair<int, int> m_start;//入口点位
	MyPair<int, int> m_end;	//出口点位
	int m_map[11][11];		//数组模拟迷宫
	int m_visited[11][11];	//数组模拟访问过的点
	MyPair<int, int> dirs[4] = {//搜索的四个方向
		{1, 0}, {0, 1}, {-1, 0}, {0, -1}
	};
	Stack<MyPair<int, int>> m_traceStack;//路径栈
	int m_fFindWay;			//是否找到路径的flag
};

Maze::Maze(){
	for (int i = 0; i < 11; i++) {
		for (int j = 0; j < 11; j++) {
			m_map[i][j] = 0;
			m_visited[i][j] = 0;
		}
	}
	m_fFindWay = 0;
}

void Maze::getMazeData(){
	cout << "输入迷宫的行数，最大为10：";
	cin >> m_rows;
	while (m_rows >= 11 || m_rows <= 0) {
		cerr << "行数非法，请重新输入合法的行数";
		cin >> m_rows;
	}
	cout << "输入迷宫的列数，最大为10：";
	cin >> m_cols;
	while (m_cols >= 11 || m_cols <= 0) {
		cerr << "列数非法，请重新输入合法的列数:" ;
		cin >> m_cols;
	}
	cout << "请输入迷宫的入口坐标(x, y均从0开始)：";
	cin >> m_start.first >> m_start.second;
	while (! isInRange(m_start)) {
		cerr << "坐标非法，请重新输入合法的坐标:" ;
		cin >> m_start.first >> m_start.second;
	}
	cout << "请输入迷宫的出口坐标:";
	cin >> m_end.first >> m_end.second;
	while (!isInRange(m_end)) {
		cerr << "坐标非法，请重新输入合法的坐标:" ;
		cin >> m_end.first >> m_end.second;
	}
	cout << "请输入迷宫，1表示墙，0表示路：" << endl;
	int errFlag = 0;
	for (int i = 0; i < m_rows; i++) {
		for (int j = 0; j < m_cols; j++) {
			cin >> m_map[i][j];
			if (m_map[i][j] != 0 && m_map[i][j] != 1) {
				errFlag = 1;
			}
		}
	}
	while (errFlag) {
		cerr << "迷宫有非法数据，请重新输入。" << endl;
		errFlag = 0;
		for (int i = 0; i < m_rows; i++) {
			for (int j = 0; j < m_cols; j++) {
				cin >> m_map[i][j];
				if (m_map[i][j] != 0 && m_map[i][j] != 1) {
					errFlag = 1;
				}
			}
		}
	}
	m_traceStack.push(m_start);
}

bool Maze::isInRange(MyPair<int, int> p){
	return p.first >= 0 && p.first < m_rows
		&& p.second >= 0 && p.second < m_cols;
}

void Maze::dfsTrace(){
	if (!m_traceStack.getSize()) {
		return;
	}
	if (! isInRange(m_traceStack.top())) {//出界
		return;
	}
	if (m_map[m_traceStack.top().first][m_traceStack.top().second]
	 || m_visited[m_traceStack.top().first][m_traceStack.top().second]) {//踩墙或者已经访问过
		return;
	}
	if (m_fFindWay) {
		return;
	}
	if (m_traceStack.top() == m_end) {
		m_fFindWay = 1;
		return;
	}
	m_visited[m_traceStack.top().first][m_traceStack.top().second] = 1;
	for (int i = 0; i < 4; i++) {
		m_traceStack.push(m_traceStack.top() + dirs[i]);
		dfsTrace();
		if (m_fFindWay) {
			return;
		}
		m_traceStack.pop();
	}
}

void Maze::showTrace(){
	if (! m_fFindWay) {
		cout << "对于给定的入口、出口、迷宫，没有合理解" << endl;
		return;
	}
	cout << "一条可行路径为：";
	m_traceStack.showElements();
	while (m_traceStack.getSize()) {
		auto t = m_traceStack.pop();
		m_map[t.first][t.second] = 2;
	}
	cout << "可视化路径：";
	for (int i = 0; i < m_rows; i++) {
		cout << endl;
		for (int j = 0; j < m_cols; j++) {
			if (m_map[i][j] == 2) {
				cout << "* ";
			}
			else{
				cout << m_map[i][j] << " ";
			}
		}
		
	}
	cout << endl;
}

int main() {
	Maze probMaze;
	probMaze.getMazeData();
	probMaze.dfsTrace();
	probMaze.showTrace();
	return 0;
}

