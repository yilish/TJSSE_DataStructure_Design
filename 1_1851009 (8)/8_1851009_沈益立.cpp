#include <iostream>

using namespace std;
const int MAXVALUE = 2e5;
const int VERTEXMAX = 100;

struct EdgeNode {
    char head;									//头结点名
    char tail;									//尾结点名
    int cost;									//权值
}; 


class Graph {   //无向图
public:
    Graph();
    void insertVertex(char vertexName);			//插入一个新的顶点，并且给它映射出一个唯一的编号
    bool insertEdge(EdgeNode& e);				//插入新边，存入邻接矩阵中
    int getVertexNum() { return m_vertexNum; }	//返回顶点总量
    int getEdgeNum() { return m_edgeNum; }		//返回无向边的总量
    int getEdgeWeight(char u, char v);			//返回u, v节点之间的权值
    char getVertexByIndex(int index);			//通过顶点的编号获得顶点的名称
    int getVertexByName(char name);				//通过顶点的名称获得顶点的编号
    void showMatrix();							//展示邻接矩阵
private:
    int m_edgeNum = 0;							//存储总边数
    int m_vertexNum = 0;						//存储总节点数
    
    char m_charHash[101];						//将顶点编号映射为字符
    int m_hash[256];							//将字符名称映射成顶点编号
    int m_graphMatrix[101][101];				//邻接矩阵容器
    
};

Graph::Graph() {
    
    for (int i = 0; i < 256; i++) {
        m_hash[i] = -1;
    }
    for (int i = 0; i < 101; i++) {
        m_charHash[i] = 0;
    }
    for (int i = 0; i < 101; i++) {
        for (int j = 0; j < 101; j++) {
            m_graphMatrix[i][j] = MAXVALUE;  //初始化路径
        }
        m_graphMatrix[i][i] = 0;        //自身距离为0
    }
    m_edgeNum = 0;
}

void Graph::insertVertex(char vertexName) {
    if (m_hash[vertexName] != -1) {
        cerr << "this vertex is existed, insertion failed." << endl;
        return;
    }

    if (m_vertexNum >= VERTEXMAX) {
        cerr << "too much vertexes, insertion failed." << endl;
        return;
    }
    m_hash[vertexName] = ++m_vertexNum;
    m_charHash[m_hash[vertexName]] = vertexName;
}

bool Graph::insertEdge(EdgeNode& e) {
    if (m_hash[e.head] == -1 || m_hash[e.tail] == -1) {
        cerr << "Insert edge failed. The vertexes of this edge does not exist, please check." << endl;
        return false;
    }

    if (e.head == e.tail) {
        cerr << "Insert edge failed. The cost to vertex itself should be 0. (There should be no ring in the graph.)" << endl;
        return false;
    }
    if (m_graphMatrix[e.head][e.tail] == MAXVALUE) {
        m_edgeNum++;
    }
    int indexU = m_hash[e.head] , indexV = m_hash[e.tail] ;
    m_graphMatrix[indexU][indexV] = e.cost;
    m_graphMatrix[indexV][indexU] = e.cost;//无向图，对称

    return true;
}

int Graph::getEdgeWeight(char u, char v) {
    if (m_hash[u] == -1 || m_hash[v] == -1) {
        cerr << "no such vertex." << endl;
        return 0;
    }

    int indexU = m_hash[u], indexV = m_hash[v];

    return m_graphMatrix[indexU][indexV];
}

char Graph::getVertexByIndex(int index) {
    if (index > m_vertexNum) {
        cerr << "No such index." << endl;
        return 0;
    }
    return m_charHash[index + 1];
}

int Graph::getVertexByName(char name) {
    if (m_hash[name] == -1) {
        cerr << "No such vertex." << endl;
        return -1;
    }
    return m_hash[name];
}

void Graph::showMatrix() {
    cout << '\t';
    for (int i = 1; i <= m_vertexNum; i++) {
        cout << m_charHash[i] << "\t";
    }
    cout << endl;
    for (int i = 1; i <= m_vertexNum; i++) {
        cout << m_charHash[i] << "\t";
        for (int j = 1; j <= m_vertexNum; j++) {
            cout << m_graphMatrix[i][j] << "\t";
        }
        cout << endl;
    }
}

class MinHeap {
public:
    MinHeap();				//无参构造函数
    MinHeap(int size)		//含大小的构造函数
        :m_maxSize(size) {
        m_heap = new EdgeNode[size];
        auto p = m_heap;
        for (int i = 0; i < m_maxSize; i++) {
            p = new EdgeNode();
            p++;
        }

    };
    void insert(EdgeNode val);		//向小根堆插入新元素
    EdgeNode pop();				//小根堆弹出元素
    EdgeNode getTop();				//获取堆顶值
    bool isEmpty();			//判断堆空
    bool isFull();			//判断堆满

    ~MinHeap() {};			//默认析构函数
private:
    void filterUp(int start);//自底向上调整堆
    void filterDown(int start, int end);//自顶向下调整堆
    EdgeNode* m_heap;				//用数组模拟堆
    int m_maxSize;			//堆的最大大小
    int m_defaultSize = 10;	//默认大小
    int m_curSize = 0;		//当前堆的大小
};

class UFS {//并查集
public:
    UFS(int size);							//含初始大小的构造函数
    int find(int index);					//寻找某节点的并查集的根
    void unite(int i, int j);				//将i, j节点合并至同一集合中
private:
    int m_size;								//并查集的大小
    int* m_parent;							//存储并查集的数组
};


class MST {
public:
    MST() { m_edgeArr = new EdgeNode[VERTEXMAX]; }	//无参构造函数
    MST(int maxSize);								//含参构造函数
    void insert(EdgeNode item);						//插入新的最小生成树的边

    void showMST();									//展示MST的结论
private:

    EdgeNode* m_edgeArr;							//存放最小生成树边的数组
    int m_num = 0;									//最小生成树总数
    int m_maxSize = VERTEXMAX;						//数组的最大值
};

class Solution {
public:
    Solution() { m_graph = new Graph(); m_mst = new MST(); m_minPQ = new MinHeap(); }
	void initIO();
    void kruskal();
    void reset();
    MST* m_mst ;
    Graph* m_graph;
    MinHeap* m_minPQ;
};


void Solution::initIO() {
    cout << "**        电网造价模拟系统      **" << endl;
    cout << "==================================" << endl;
    cout << "**       A --- 创建电网顶点　　 **" << endl;
    cout << "**       B --- 添加电网的边     **" << endl;
    cout << "**       C --- 构造最小生成树   **" << endl;
    cout << "**       D --- 显示最小生成树   **" << endl;
    cout << "**       E --- 退出程序         **" << endl;
    cout << "**       F --- 显示邻接矩阵     **" << endl;
    cout << "**       G --- 重新初始化       **" << endl;
   
    cout << "==================================" << endl;
    cout << "注意：执行完一轮之后输入新图需初始化" << endl;
    cout << endl;
    char optCode;
    int flagA = 0;
    //cin >> optCode;
    while (1) {
        cout << "请选择操作:" << endl;
        cin >> optCode;
        if (optCode == 'A') {
            cout << "请输入顶点个数:";
            int vertexNum;
            cin >> vertexNum;
            if (vertexNum >= VERTEXMAX || vertexNum <= 0) {
                cerr << "顶点太多或非正";
                return;
            }
            cout << "请依次输入顶点的名称（仅支持一个char）:" << endl;
            while (vertexNum--) {
                char name;
                cin >> name;
                m_graph->insertVertex(name);
            }
            flagA = 1;
        }

        else if (optCode == 'B') {
            if (!flagA) {
                cout << "请先建立顶点！" << endl;
                continue;
            }
            cout << "请输入边的个数:（请注意，重复输入的边会覆盖此前输入的边）";
            int edgeNum;
            cin >> edgeNum;
            if (edgeNum < 0) {
                cerr << "边数非法";
                break;
            }
            if (edgeNum == 0) {
                continue;
            }
            cout << "请依次输入两个顶点和边花费:" << endl;
            while (edgeNum--) {
                char h, t;
                int c;
                EdgeNode e;
                cin >> h >> t >> c;
                e.head = h, e.tail = t, e.cost = c;
                if (!m_graph->insertEdge(e)) {
                    cout << "插入失败, 请检查是否存在这些顶点" << endl;
                    continue;
                }
            }
        }

        else if (optCode == 'C') {
            kruskal();
            cout << "成功生成MST(使用kruskal算法)" << endl;
        }

        else if (optCode == 'D') {
            m_mst->showMST();
        }

        else if (optCode == 'F') {
            m_graph->showMatrix();
        }

        else if (optCode == 'E') {
            return;
        }
        else if (optCode == 'G') {
            reset();
        }
        else {
            cout << "请输入正确的选项" << endl;
        }
    }
}

void Solution::kruskal() {
    auto n = m_graph->getVertexNum();
    auto m = m_graph->getEdgeNum();
    m_mst = new MST(n - 1);
    m_minPQ = new MinHeap(m);
    UFS F(n);
    for (auto u = 0; u < n; u++) {
        for (auto v = u + 1; v < n; v++) {
            auto charU = m_graph->getVertexByIndex(u),
                 charV = m_graph->getVertexByIndex(v);

            auto uvCost = m_graph->getEdgeWeight(charU, charV);
            
            
            if (uvCost != MAXVALUE) {//uv间有边
                EdgeNode tmpEdge({ charU, charV, uvCost });
                m_minPQ->insert(tmpEdge);
            }
        }
    }

    auto cnt = 1;
    while (cnt < n) {
        auto edge = m_minPQ->pop();

        auto indexU = m_graph->getVertexByName(edge.head)
           , indexV = m_graph->getVertexByName(edge.tail);
        auto u = F.find(indexU), v = F.find(indexV);

        if (u != v) {
            F.unite(u, v);
            m_mst->insert(edge);
            cnt++;
        }

    }
}

void Solution::reset() {
    delete m_graph;
    delete m_mst;
    delete m_minPQ;
    m_graph = new Graph();
    m_mst = new MST(); m_minPQ = new MinHeap();
}

int main() {
    Solution sol;
    sol.initIO();
}

UFS::UFS(int size) {
    m_size = size;
    m_parent = new int[size];
    for (int i = 0; i < size; i++) {
        m_parent[i] = -1;
    }
}

int UFS::find(int index) {
    while (m_parent[index] >= 0) {
        index = m_parent[index];
    }

    return index;
}

void UFS::unite(int i, int j) {
    auto iSet = find(i), jSet = find(j);
    if (iSet == jSet) {
        cerr << "They belong to the same set." << endl;
        return;
    }

    m_parent[iSet]--;
    m_parent[jSet] = iSet;
}




MinHeap::MinHeap() {
    m_maxSize = m_defaultSize;
    m_heap = new EdgeNode[m_maxSize];
    auto p = m_heap;
    for (int i = 0; i < m_maxSize; i++) {
        p = new EdgeNode();
        p++;
    }
}


void MinHeap::insert(EdgeNode val) {
    if (isFull()) {
        cerr << "堆满, 请建更大的堆" << endl;
        return;
    }
    //插入逻辑，尾插后上浮调整
    m_heap[m_curSize] = val;
    filterUp(m_curSize++);
}


EdgeNode MinHeap::pop() {
    if (isEmpty()) {
        cerr << "空集，无法弹出堆顶元素" << endl;
        return EdgeNode();
    }
    EdgeNode retVal = m_heap[0];
    m_heap[0] = m_heap[m_curSize - 1];	//取堆尾元素
    m_curSize--;
    filterDown(0, m_curSize - 1);	//自上而下调整堆
    return retVal;
}


EdgeNode MinHeap::getTop() {
    return m_heap[0];
}


bool MinHeap::isEmpty() {
    return m_curSize == 0;
}


bool MinHeap::isFull() {
    return m_curSize == m_maxSize;
}


void MinHeap::filterUp(int start) {
    int j = start, i = (j - 1) / 2;	//i表示j的父亲节点
    EdgeNode tmp = m_heap[start];			//暂存，从下向上调整，取要调整的节点
    while (j) {
        if (m_heap[i].cost <= tmp.cost) {		//父节点较小的情况
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


void MinHeap::filterDown(int start, int end) {
    int i = start, j = 2 * i + 1;	//i表示父节点，j表示左子树
    EdgeNode tmp = m_heap[start];			//需要下浮，因此暂存起始节点
    while (j <= end) {				//j到达终点前
        if (j < end && m_heap[j].cost > m_heap[j + 1].cost) {
            j++;					//取两子女中小者
        }
        if (tmp.cost <= m_heap[j].cost) {		//如果小，就调整结束
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

MST::MST(int maxSize) {
    m_maxSize = maxSize;
    m_edgeArr = new EdgeNode[m_maxSize];
    m_num = 0;
}

void MST::insert(EdgeNode item) {
    if (m_num >= m_maxSize) {
        cerr << "too much items." << endl;
        return;
    }

    else {
        m_edgeArr[m_num++] = item;
    }
}

void MST::showMST() {
    for (int i = 0; i < m_num; i++) {
        cout << m_edgeArr[i].head << "<-"
             << m_edgeArr[i].cost << "->"
             << m_edgeArr[i].tail << '\t';
    }

    cout << endl << endl;
}
