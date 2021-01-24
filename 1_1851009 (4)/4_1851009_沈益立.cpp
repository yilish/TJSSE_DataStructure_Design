#include <iostream>
#include <math.h>
using namespace std;

template<class T>
class Stack {
public:
	Stack();
	~Stack();
	void push(const T& val);
	void showElements();
	bool isEmpty();
	void makeEmpty();
	T pop();
	T top();
	int getSize();
	T* getFirstAddress();
private:
	int m_top;	//表示栈顶
	int m_maxSize;
	T* m_elements;
};

template<class T>
Stack<T>::Stack() {
	m_maxSize = 2;
	m_elements = new T[m_maxSize];
	m_top = -1;
}

template<class T>
Stack<T>::~Stack() {
	delete[] m_elements;
}

template<class T>
void Stack<T>::push(const T& val) {
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
void Stack<T>::showElements() {
	for (int i = 0; i < m_top; i++) {
		cout << m_elements[i] << "->";
	}
	cout << m_elements[m_top] << endl;
}
template<class T>
bool Stack<T>::isEmpty() {
	return m_top == -1;
}
template<class T>
void Stack<T>::makeEmpty() {
	m_top = -1;
}
template<class T>
T Stack<T>::pop() {
	if (isEmpty()) {
		cerr << "Empty stack, nothing to pop." << endl;
		return T();
	}
	T retVal = m_elements[m_top--];
	return retVal;
}
template<class T>
T Stack<T>::top() {
	if (isEmpty()) {
		cerr << "Empty stack, nothing on the top." << endl;
		return T();
	}
	return m_elements[m_top];
}
template<class T>
int Stack<T>::getSize() {
	return m_top + 1;
}

template<class T>
T* Stack<T>::getFirstAddress() {
	return m_elements;
}



class PolandConvert {
public:

	void IOLoop();
	bool convertExpression();
	bool proceedPost();
	bool isOpChr(string s);
	int getPriorVal(char chr);
	int getValue();
private:
	int m_val = 0;
	Stack<int> valStack;
	Stack<char> charStack;
	string midExpression;
	Stack<string> postExpression;
};

void PolandConvert::IOLoop() {
	char fContinue = 'y';
	//获取中缀表达式
	while (fContinue == 'y') {
		//init
		valStack.makeEmpty();
		charStack.makeEmpty();
		postExpression.makeEmpty();
		cout << "输入表达式(一定要以'='结尾):" << endl;
		cin >> midExpression;
		//最简单的检测
		int flag = 0;
		for (auto letter : midExpression) {

			if (getPriorVal(letter) == -1) {
				if (!(letter >= '0' && letter <= '9') && letter != '.') {
					cerr << "您输入的表达式有非法字字符" << endl;
					flag = 1;
					break;
				}
			}
		}
		if (flag) {
			continue;
		}
		if (midExpression.back() != '=') {
			cerr << "您输入的表达式格式有误" << endl;

			continue;
		}
		convertExpression();
		if (proceedPost())
			cout << getValue() << endl;
		else {
			cerr << "表达式是否有非法值或者缺少必要元素？" << endl;
		}
		cout << "是否继续(y/n)?";
		cin >> fContinue;
		while (fContinue != 'y' && fContinue != 'n') {
			cout << "请输入正确的判断符(y/n):";
			cin >> fContinue;
		}
	}
}

bool PolandConvert::convertExpression() {//中缀表达式用栈处理，之后更新val值
	string tmp;
	for (int i = 0; i < midExpression.size() - 1; i++) {
		//操作数处理
		if ((midExpression[i] == '-' || (midExpression[i] == '+') && (i == 0
			|| string("+-*/(^%").find(midExpression[i - 1]) != string::npos))//正负号处理
			|| getPriorVal(midExpression[i]) == -1) {
			tmp = midExpression[i];
			while (i + 1 < midExpression.size()
				&& getPriorVal(midExpression[i + 1]) == -1) {
				tmp += midExpression[++i];
			}

			if (tmp == "+" || tmp == "-") {
				while (charStack.getSize() && (getPriorVal(tmp[0]) <= getPriorVal(charStack.top()))) {
					string t = "";
					t += charStack.pop();
					postExpression.push(t);
				}
				charStack.push(midExpression[i]);
			}
			else {
				if (tmp[0] == '+' || tmp[0] == '-') {
					if (!postExpression.isEmpty() && !isOpChr(postExpression.top())) {
						string a = "";
						a += tmp[0];
						string b = "";
						for (int i = 1; i < tmp.size(); i++) {
							b += tmp[i];
						}
						postExpression.push(b);
						postExpression.push(a);
						continue;
					}
				}

				postExpression.push(tmp);

			}
		}
		else {//出现操作符
			if (midExpression[i] == '(') {
				charStack.push(midExpression[i]);
			}
			else if (midExpression[i] == ')') {
				while (charStack.top() != '(') {
					string t = "";
					t += charStack.pop();
					postExpression.push(t);
				}
				charStack.pop();
			}
			else {
				while (charStack.getSize() && (getPriorVal(midExpression[i]) <= getPriorVal(charStack.top()))) {
					string t = "";
					t += charStack.pop();
					postExpression.push(t);
				}
				charStack.push(midExpression[i]);
			}
		}
	}
	//最终处理
	while (charStack.getSize()) {
		string t = "";
		t += charStack.pop();
		postExpression.push(t);
	}
	return true;
}

bool PolandConvert::proceedPost() {
	postExpression.showElements();
	string* vec = postExpression.getFirstAddress();
	int vLen = postExpression.getSize();
	for (int i = 0; i < vLen; i++) {
		if (isOpChr(vec[i])) {
			int b = valStack.pop();
			if (!valStack.getSize()) {
				valStack.push(b);
				break;
			}
			int a = valStack.pop();
			int res;
			if (vec[i] == "+") {
				res = a + b;
			}
			else if (vec[i] == "-") {
				res = a - b;
			}
			else if (vec[i] == "*") {
				res = a * b;
			}
			else if (vec[i] == "/") {
				if (b == 0) {
					cerr << "不可以除以0" << endl;
					return false;
				}
				res = a / b;
			}

			else if (vec[i] == "%") {
				if (b == 0) {
					cerr << "不可以模0" << endl;
					return false;
				}
				res = a % b;

			}
			else if (vec[i] == "^") {
				res = pow(a, b);
			}
			valStack.push(res);
		}
		else {
			valStack.push(atoi(vec[i].c_str()));
		}
	}
	if (valStack.getSize() != 1) {
		return false;
	}
	m_val = valStack.top();

	return true;
}

bool PolandConvert::isOpChr(string s) {
	string opChr[7] = {
		"=", "+", "-", "*", "/", "^", "%"
	};
	for (int i = 0; i < 7; i++) {
		if (s == opChr[i]) {
			return 1;
		}
	}
	return 0;
}

int PolandConvert::getPriorVal(char chr) {//获取运算的优先级
	if (chr == '=') {
		return 0;
	}
	else if (chr == '(' || chr == ')') {
		return 1;
	}
	else if (chr == '+' || chr == '-') {
		return 2;
	}
	else if (chr == '*' || chr == '/') {
		return 3;
	}
	else if (chr == '%') {
		return 4;
	}
	else if (chr == '^') {
		return 5;
	}
	else {
		return -1;//other characters
	}
	return -1;
}

int PolandConvert::getValue() {
	return m_val;
}

int main() {
	PolandConvert sol;
	sol.IOLoop();
	return 0;
}
