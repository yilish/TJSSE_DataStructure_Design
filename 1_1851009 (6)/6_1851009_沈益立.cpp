#include <iostream>
#include <assert.h>
using namespace std;


class FamilyMember {
public:
	FamilyMember* m_nextSibling = nullptr;	//存储下一个兄弟
	FamilyMember* m_firstChild = nullptr;	//存储长子
	string m_name;							//存储姓名
	FamilyMember() {};						//无参构造函数
	FamilyMember(FamilyMember* nextSibling, FamilyMember* firstChild,
		string name) :						//三个参数的构造函数
		m_nextSibling(nextSibling), m_firstChild(firstChild), m_name(name) {};
	FamilyMember(string name) :				//仅包含名字的构造函数
		m_name(name) {};
};

class FamilyTree {
public:
	FamilyMember* m_root;		//存储家族树的根节点
	FamilyTree() :				//无参构造函数
		m_root(nullptr) {};
	void initTree();			//树的初始化IO，输入根节点的值
	void promoteTree();			//完善树的某节点
	FamilyMember* getMemberAdd(FamilyMember* p, string name);
								//根据姓名递归地得到某子成员地址
	FamilyMember* getPreAdd(FamilyMember* p, string name);
								//根据姓名递归地得到某子成员的前继地址
	void addMember();			//为某双亲增加孩子
	void dissolve();			//解散局部家庭
	void recursiveDissolve(FamilyMember* root);
								//递归地删除某个树及其子树
	void changeName();			//为某家庭成员改名

};

void FamilyTree::initTree() {
	string rootName;
	cout << "首先建立一个家谱!" << endl;
	cout << "请输入祖先的姓名:";
	cin >> rootName;
	cout << "此家谱的祖先是:";
	cout << rootName << endl << endl;
	
	m_root = new FamilyMember(rootName);
}


void FamilyTree::promoteTree() {
	string memberName;
	int siblingNum;
	cout << "请输入要建立家庭的人的姓名:";
	cin >> memberName;
	auto targetMember = getMemberAdd(m_root, memberName);
	
	if (! targetMember) {
		cerr << "家谱中找不到该成员。" << endl;
		return;
	}
	if (targetMember->m_firstChild) {
		cerr << "该成员已经有子女了" << endl;
		return;
	}
	cout << "请输入" << memberName << "的儿女人数:";
	cin >> siblingNum;
	if (siblingNum < 1) {
		cout << "子孙节点数太少，无需添加" << endl;
		return;
	}

	cout << "请依次输入" << memberName << "的儿女姓名:";
	cin >> memberName;
	auto firstChild = new FamilyMember(memberName);
	targetMember->m_firstChild = firstChild;
	auto pCur = firstChild;
	for (int i = 0; i < siblingNum - 1; i++) {
		cin >> memberName;
		auto newMember = new FamilyMember(memberName);
		pCur->m_nextSibling = newMember;
		pCur = newMember;
	}
	pCur = firstChild;
	cout << targetMember->m_name << "的第一代子孙是:";
	while (pCur) {
		cout << pCur->m_name << '\t';
		pCur = pCur->m_nextSibling;
	}
	cout << endl;
}

FamilyMember* FamilyTree::getMemberAdd(FamilyMember* p, string name) {
	if (!p) {
		return nullptr;
	}
	if (p->m_name == name) {
		return p;
	}
	FamilyMember* sibAdd = getMemberAdd(p->m_nextSibling, name);
	if (sibAdd) {
		return sibAdd;
	}
	FamilyMember* childAdd = getMemberAdd(p->m_firstChild, name);
	if (childAdd) {
		return childAdd;
	}
	return nullptr;
}

FamilyMember* FamilyTree::getPreAdd(FamilyMember* p, string name)
{
	if (!p) {
		return nullptr;
	}
	FamilyMember* sib = nullptr;
	FamilyMember* child = nullptr;
	if (p->m_nextSibling) {
		if (p->m_nextSibling->m_name == name) {
			return p;
		}
		sib = getPreAdd(p->m_nextSibling, name);
	}
	if (p->m_firstChild) {
		if (p->m_firstChild->m_name == name) {
			return p;
		}
		child = getPreAdd(p->m_firstChild, name);
	}
	if (sib) {
		return sib;
	}
	if (child) {
		return child;
	}
	return nullptr;
}




void FamilyTree::addMember() {
	cout << "请输入要添加子女的人的姓名:";
	string memberName;
	cin >> memberName;
	auto targetMember = getMemberAdd(m_root, memberName);
	if (!targetMember) {
		cerr << "家庭中没有该成员" << endl;
		return;
	}
	auto pCur = targetMember->m_firstChild;
	if (!pCur) {//没有子女
		cout << "请输入" << memberName << "新添加的子女的姓名:";
		cin >> memberName;
		targetMember->m_firstChild = new FamilyMember(memberName);
		return;
	}
	while (pCur->m_nextSibling) {
		pCur = pCur->m_nextSibling;
	}

	cout << "请输入" << memberName << "新添加的子女的姓名:";
	cin >> memberName;
	pCur->m_nextSibling = new FamilyMember(memberName);
}

void FamilyTree::dissolve() {
	cout << "请输入要解散家庭的人的姓名:";
	string memberName;
	cin >> memberName;
	auto preAdd = getPreAdd(m_root, memberName);
	if (!preAdd) {
		cout << "不存在这样的家庭" << endl;
		return;
	}
	FamilyMember* targetTree = nullptr;
	if (preAdd->m_firstChild->m_name == memberName) {
		targetTree = preAdd->m_firstChild;
		preAdd->m_firstChild = targetTree->m_nextSibling;
	}
	else {
		targetTree = preAdd->m_nextSibling;
		preAdd->m_nextSibling = targetTree->m_nextSibling;
	}
	auto pCur = targetTree->m_firstChild;
	if (!pCur) {
		cout << memberName << "没有子孙" << endl;
		recursiveDissolve(targetTree);
		return;
	}

	cout << memberName << "的第一代子孙是:";
	while (pCur) {
		cout << pCur->m_name << '\t';
		pCur = pCur->m_nextSibling;
	}
	cout << endl;
	recursiveDissolve(targetTree);
}

void FamilyTree::recursiveDissolve(FamilyMember* root) {
	if (!root) {
		return;
	}
	auto sib = root->m_nextSibling;
	auto child = root->m_firstChild;
	
	recursiveDissolve(sib);
	recursiveDissolve(child);

	delete root;
}

void FamilyTree::changeName() {
	cout << "请输入要修改的成员目前的姓名:";
	string memberName;
	cin >> memberName;
	auto targetMember = getMemberAdd(m_root, memberName);
	if (!targetMember) {
		cout << "此人不存在" << endl;
		return;
	}
	cout << "请输入更改后的姓名:" ;
	string targetName;
	cin >> targetName;
	targetMember->m_name = targetName;
	cout << memberName << "已更名为" << targetName << endl;
}

class Solution {
public:
	void init();
	void IOLoop();
	FamilyTree m_tree;
};


void Solution::init() {
	cout << "**             家谱管理系统            **" << endl;
	cout << "---------------------------------------" << endl;
	cout << "**           请选择要执行的操作          **" << endl;
	cout << "**            A --- 完善家谱           **" << endl;
	cout << "**          B --- 添加家庭成员          **" << endl;
	cout << "**          C --- 解散局部家庭          **" << endl;
	cout << "**        D --- 更改家庭成员姓名         **" << endl;
	cout << "**           E --- 查找家庭成员         **" << endl;

	cout << "**            F --- 退出程序            **" << endl;
	cout << "----------------------------------------" << endl;
	//cout << "首先建立一个家谱！" << endl;
}


void Solution::IOLoop() {
	m_tree = FamilyTree();
	m_tree.initTree();
	char optCode;
	while (1) {
		cout << "请输入要执行的操作:";
		cin >> optCode;
		if (optCode == 'F') {
			break;
		}
		else if (optCode == 'A') {
			m_tree.promoteTree();
		}
		else if (optCode == 'B') {
			m_tree.addMember();
		}
		else if (optCode == 'C') {
			m_tree.dissolve();
		}
		else if (optCode == 'D') {
			m_tree.changeName();
		}
		
		else if (optCode == 'E') {
			string memberName;
			cout << "请输入要找的家庭成员姓名:";
			cin >> memberName;

			auto address = m_tree.getMemberAdd(m_tree.m_root, memberName);
			if (!address) {
				cout << "未找到该成员" << endl;
				return;
			}
			
			auto pCur = address->m_firstChild;
			if (!pCur) {
				cout << memberName << "没有子孙" << endl;
				return;
			}
			cout << memberName << "的第一代子孙是:";
			while (pCur) {
				cout << pCur->m_name << '\t';
				pCur = pCur->m_nextSibling;
			}
		}
		else {
			cerr << "请输入正确的操作码" << endl;
		}
		cout << endl;
	}
}

int main() {
	Solution sol;
	sol.init();
	sol.IOLoop();
	
}

