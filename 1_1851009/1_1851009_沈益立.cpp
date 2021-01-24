#include <iostream>
using namespace std;
class Node {
public:
	Node(string stuNum, string name, string sex,	//含参构造函数
		int age, string type, Node* next) :
		m_stuNum(stuNum), m_name(name), m_sex(sex),
		m_age(age), m_type(type), m_next(next) {};
	Node() {};										//不含参的默认构造函数
	void show();									//格式化地输出该节点的考生信息

	string m_stuNum = "";							//考号
	string m_name = "";								//姓名
	string m_sex = "";								//性别
	int m_age = 0;									//年龄
	string m_type = "";								//类别
	Node* m_next = NULL;							//后继节点地址
};

class LinkedList {
public:
	LinkedList() {};								//构造函数
	~LinkedList();									//析构函数，会释放所有节点的内存，以防内存泄漏
	Node* search(const string stuNum) const;		//查找函数，以考号为键值，从链表的头结点向后依次查找该考生，返回其地址
	bool insert(int index, const string& stuNum,	//插入函数，传入插入的目标位置和考生的参数，将新考生插入到链表指定位置
		const string& name, const string& sex,
		int& age, const string& type);
	Node* erase(string stuNum);						//删除函数，以考号为键值，从链表的头结点向后
													//依次查找该考生，若找到则将其从链表中移除，并返回该节点地址。
	bool showCurrentList() const;					//格式化地输出当前表单情况
	int getLength() const { return m_length; }		//返回链表的长度，该接口被主函数调用


private:
	int m_length = 0;								//链表长度
	Node* m_head = nullptr;							//头结点，默认为空指针
};

//implement of member functions

void Node::show()
{
	Node* pCur = this;
	cout << pCur->m_stuNum << "\t"
		<< pCur->m_name << "\t"
		<< pCur->m_sex << "\t"
		<< pCur->m_age << "\t"
		<< pCur->m_type << endl;
	return;
}

LinkedList::~LinkedList()
{
	//析构函数，释放每个节点的内存
	Node* pCur = m_head;
	while (pCur)
	{
		Node* tmp = pCur->m_next;
		delete pCur;
		pCur = tmp;
	}
}

Node* LinkedList::search(const string stuNum) const
{
	//按学号检索学生，若找到则返回该节点的地址，否则返回一个空指针
	Node* pCur = m_head;
	while (pCur)
	{
		if (pCur->m_stuNum == stuNum)
		{
			return pCur;
		}
		pCur = pCur->m_next;
	}
	//若已查到该学生，则之前的while循环中将返回。
	//未查到该学生，返回空指针
	return nullptr;
}

bool LinkedList::insert(int index, const string& stuNum,
	const string& name, const string& sex,
	int& age, const string& type)
{
	//双指针法插入链表，若插入失败，会返回false
	if (index > m_length + 1 || index <= 0)
	{
		return false;
	}
	//new Node object
	Node* stuToInsert = new Node(stuNum, name, sex, age, type, nullptr);
	m_length++;
	if (!m_head)
	{
		m_head = stuToInsert;
		return true;
	}
	else if (index == 1)
	{
		stuToInsert->m_next = m_head;
		m_head = stuToInsert;
		return true;
	}
	//insertion
	Node* fakeHead = new Node;
	fakeHead->m_next = m_head;
	//double pointer
	Node* prev = fakeHead;
	Node* cur = m_head;
	for (int i = 0; i < index - 1; i++)
	{
		prev = cur;
		cur = cur->m_next;
	}
	prev->m_next = stuToInsert;
	stuToInsert->m_next = cur;
	return true;
}

Node* LinkedList::erase(string stuNum)
{
	Node* fakeHead = new Node();
	fakeHead->m_next = m_head;
	Node* pCur = m_head, * prev = fakeHead;
	while (pCur)
	{
		if (pCur->m_stuNum == stuNum)
		{
			prev->m_next = pCur->m_next;
			if (m_head == pCur)
			{
				m_head = pCur->m_next;
			}
			return pCur;
		}
		prev = pCur;
		pCur = pCur->m_next;
	}
	return nullptr;
}

bool LinkedList::showCurrentList() const
{
	printf("考号\t姓名\t性别\t年龄\t报考类别\n");
	Node* pCur = m_head;
	while (pCur)
	{
		cout << pCur->m_stuNum << "\t"
			<< pCur->m_name << "\t"
			<< pCur->m_sex << "\t"
			<< pCur->m_age << "\t"
			<< pCur->m_type << endl;
		pCur = pCur->m_next;
	}
	return true;
}

int main()
{
	//initIO, establishing
	cout << "请先建立考生信息系统！" << endl;
	LinkedList* stuSys = new LinkedList;
	int totalNum;
	cout << "请输入考生人数:";
	cin >> totalNum;
	if (totalNum <= 0)
	{
		cout << "请输入正确的考生人数。" << endl;
		return 0;
	}
	cout << "请依次输入考生的考号、姓名、性别、年龄和报考类别" << endl;
	for (int i = 1; i <= totalNum; i++)
	{
		string stuNum, name, sex, type;
		int age;
		cin >> stuNum >> name >> sex >> age >> type;
		stuSys->insert(i, stuNum, name, sex, age, type);
	}
	stuSys->showCurrentList();//print the result of establishment


	//operation loop
	cout << "选择您要进行的操作（1为插入，2为删除，3为查找，"
		<< "4为修改，5为展示表格，0为取消操作）" << endl;
	int optCode;
	while (1)
	{
		cout << "请选择要进行的操作：";
		cin >> optCode;
		if (optCode == 1)
		{
			//插入考生
			cout << "请输入要插入考生的位置：";
			int posToInsert;
			cin >> posToInsert;
			if (posToInsert > stuSys->getLength() + 1 || posToInsert <= 0)
			{
				cout << "考生位置不合法。" << endl;
				continue;
			}
			cout << "请依次输入要插入的考生的考号、"
				<< "姓名、性别、年龄和报考类别" << endl;
			string stuNum, name, sex, type;
			int age;
			cin >> stuNum >> name >> sex >> age >> type;
			stuSys->insert(posToInsert, stuNum, name, sex, age, type);
			stuSys->showCurrentList();
		}

		else if (optCode == 2)
		{
			//删除考生
			cout << "请输入要删除考生的考号：";
			string stuNum;
			cin >> stuNum;
			Node* stuToDel = stuSys->erase(stuNum);
			//若未找到
			if (stuToDel == nullptr)
			{
				cout << "不存在该考生！" << endl;
			}
			else
			{
				cout << "你删除的考生信息是：";
				stuToDel->show();
				cout << endl;
				delete stuToDel;
				stuSys->showCurrentList();
			}
		}

		else if (optCode == 3)
		{
			//查找考生
			cout << "请输入要查找的考生的考号：";
			string stuNumToSearch;
			cin >> stuNumToSearch;
			Node* stuSearched = stuSys->search(stuNumToSearch);
			//若未找到
			if (!stuSearched)
			{
				cout << "未找到该学生信息。" << endl;
			}
			else
			{
				printf("考号\t姓名\t性别\t年龄\t报考类别\n");
				stuSearched->show();
			}
		}

		else if (optCode == 4)
		{
			//修改考生信息
			cout << "请输入要修改的考生的考号：";
			string stuNum;
			cin >> stuNum;
			Node* stuToChange = stuSys->search(stuNum);
			if (!stuToChange)
			{
				cout << "未找到该学生信息。" << endl;
			}
			else
			{
				//修改
				cout << "请依次输入该考生修改后的的考号、"
					<< "姓名、性别、年龄和报考类别" << endl;
				string stuNum, name, sex, type;
				int age;
				cin >> stuNum >> name >>
					sex >> age >> type;
				stuToChange->m_stuNum = stuNum;
				stuToChange->m_name = name;
				stuToChange->m_sex = sex;
				stuToChange->m_age = age;
				stuToChange->m_type = type;
				stuSys->showCurrentList();
			}
		}

		else if (optCode == 5)
		{
			//统计表格
			cout << "目前的表格情况：" << endl;
			stuSys->showCurrentList();
		}

		else if (optCode == 0)
		{
			cout << "取消操作" << endl;
			break;
		}
		else
		{
			//default
			cout << "请输入正确的操作号。" << endl;
		}
		cout << endl;
	}
}
