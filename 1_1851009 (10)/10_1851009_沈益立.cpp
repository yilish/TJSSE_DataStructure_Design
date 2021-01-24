#include <iostream>
#include <ctime>

using namespace std;

class Sort {
public:
	void initIO();				//初始化IO

	void bubbleSort();			//冒泡排序
	void selectSort();			//选择排序
	void insertSort();			//插入排序
	void shellSort();			//希尔排序
	void quickSort();			//快速排序
	void heapSort();			//堆排序
	void mergeSort();			//归并排序
	void radixSort();			//基数排序

private:
	/*辅助函数*/
	void randomizeArr();		//每次操作前对数组随机化
	void swap(int& a, int& b);	//基于引用的交换函数

	void recursQSort(int left, int right, long long& count);
	//递归地求解快速排序的辅助函数
	void recursMergeSort(int left, int right, long long& cnt);
	//递归地求解归并排序的辅助函数
	void merge(int left, int mid, int right, long long& cnt);
	//合并两个数组的辅助函数
	int partition(int low, int high, long long& count);
	//快排中分治的辅助函数
	void siftDown(int start, int end, long long& cnt);
	//堆排序中从上向下调整的辅助函数
	int m_sortMethod;			//选择排序方式码
	long long int m_sortNum;	//待排序数组的长度
	int* m_array;				//存放待排序数的数组
	int* m_supArr;				//归并排序中的辅助数组
};

void Sort::initIO() {
	cout << "**         排序算法比较         **" << endl;
	cout << "---------------------------------" << endl;
	cout << "**        1 --- 冒泡排序        **" << endl;
	cout << "**        2 --- 选择排序        **" << endl;
	cout << "**       3 --- 直接插入排序     **" << endl;
	cout << "**        4 --- 希尔排序        **" << endl;
	cout << "**        5 --- 快速排序        **" << endl;
	cout << "**        6 ---  堆排序         **" << endl;
	cout << "**       7 ---  归并排序        **" << endl;
	cout << "**       8 ---  基数排序        **" << endl;
	cout << "**        9 --- 退出程序        **" << endl;
	cout << "---------------------------------" << endl;



	if (m_sortMethod == 9) {
		return;
	}
	cout << "请输入要产生随机数的个数：";
	cin >> m_sortNum;
	while (m_sortNum <= 0) {
		cerr << "随机数个数有误" << endl;
		cin >> m_sortNum;
	}
	m_array = new int[m_sortNum + 1];
	while (1) {
		randomizeArr();
		cout << "请选择排序方法：";
		cin >> m_sortMethod;
		while (m_sortMethod >= 10 || m_sortMethod <= 0) {
			cerr << "选择错误，请重新输入：" << endl;
			cin >> m_sortMethod;
		}
		if (m_sortMethod == 1) {
			bubbleSort();
		}
		else if (m_sortMethod == 2) {
			selectSort();
		}
		else if (m_sortMethod == 3) {
			insertSort();
		}
		else if (m_sortMethod == 4) {
			shellSort();
		}
		else if (m_sortMethod == 5) {
			quickSort();
		}
		else if (m_sortMethod == 6) {
			heapSort();
		}
		else if (m_sortMethod == 7) {
			mergeSort();
		}
		else if (m_sortMethod == 8) {
			radixSort();
		}
		else if (m_sortMethod == 9) {
			return;
		}
		cout << endl;
	}

}

void Sort::randomizeArr() {
	for (auto i = 0; i < m_sortNum; i++) {
		m_array[i] = rand();
	}
}

void Sort::bubbleSort() {
	long long cnt = 0;
	clock_t tStart = clock();
	for (auto i = 0; i < m_sortNum - 1; i++) {
		for (auto j = 0; j < m_sortNum - 1 - i; j++) {
			if (m_array[j] > m_array[j + 1]) {
				swap(m_array[j], m_array[j + 1]);
				cnt += 3;
			}
		}
	}
	cout << "冒泡排序所用时间" << clock() - tStart << endl;
	cout << "冒泡排序交换次数" << cnt << endl;
}

void Sort::selectSort() {
	long long cnt = 0;
	clock_t tStart = clock();
	int minIndex, tmp;
	for (auto i = 0; i < m_sortNum; i++) {
		minIndex = i;
		for (auto j = i; j < m_sortNum; j++) {
			if (m_array[j] < m_array[minIndex]) {
				minIndex = j;
			}
		}
		
		if (i != minIndex) {
			swap(m_array[i], m_array[minIndex]);
			cnt += 3;
		}
	}
	cout << "选择排序所用时间" << clock() - tStart << endl;
	cout << "选择排序交换次数" << cnt << endl;
}

void Sort::insertSort() {
	long long cnt = 0;
	clock_t tStart = clock();
	int tmp;
	for (int i = 1; i < m_sortNum; i++) {
		if (m_array[i] < m_array[i - 1]) {
			tmp = m_array[i];
			int j = 1;
			while (tmp < m_array[i - j]) {
				m_array[i - j + 1] = m_array[i - j];
				cnt++;
				j++;
			}
			m_array[i - j + 1] = tmp;
			cnt++;
		}
	}
	cout << "直接插入排序所用时间" << clock() - tStart << endl;
	cout << "直接插入排序交换次数" << cnt << endl;

}

void Sort::shellSort() {
	long long cnt = 0;
	clock_t tStart = clock();

	//cout << endl;
	for (int gap = m_sortNum / 2; gap > 0; gap /= 2) {
		for (int i = gap; i < m_sortNum; i++) {
			int j = i;
			while (j - gap >= 0 && m_array[j] < m_array[j - gap]) {
				swap(m_array[j], m_array[j - gap]);
				j -= gap;
				cnt += 3;
			}
		}
	}

	cout << "希尔排序所用时间" << clock() - tStart << endl;
	cout << "希尔排序交换次数" << cnt << endl;
}

void Sort::quickSort() {
	long long cnt = 0;
	clock_t tStart = clock();
	recursQSort(0, m_sortNum - 1, cnt);
	cout << "快速排序所用时间" << clock() - tStart << endl;
	cout << "快速排序交换次数" << cnt << endl;
}

void Sort::heapSort() {
	long long cnt = 0;
	clock_t tStart = clock();
	for (int i = (m_sortNum - 2) / 2; i >= 0; i--) {
		siftDown(i, m_sortNum - 1, cnt);
	}
	for (int i = m_sortNum - 1; i >= 0; i--) {
		swap(m_array[0], m_array[i]);
		cnt += 3;
		siftDown(0, i - 1, cnt);
	}
	cout << "堆排序所用时间" << clock() - tStart << endl;
	cout << "堆排序交换次数" << cnt << endl;
}
/*归并排序主框架*/
void Sort::mergeSort() {
	m_supArr = new int[m_sortNum];
	long long cnt = 0;
	clock_t tStart = clock();
	recursMergeSort(0, m_sortNum - 1, cnt);
	cout << "归并排序所用时间" << clock() - tStart << endl;
	cout << "归并排序交换次数" << cnt << endl;
	delete[] m_supArr;
}

void Sort::radixSort() {
	long long cnt = 0;
	clock_t tStart = clock();
	m_supArr = new int[m_sortNum];
	int maxNum = -1;
	for (int i = 0; i < m_sortNum; i++) {
		maxNum = max(maxNum, m_array[i]);
	}//find maximum number.
	int d = 0;
	while (maxNum) {
		d++;
		maxNum /= 10;
	}
	//find maximum bits.
	int radix = 1;
	int bucket[10] = { 0 };
	int* tmp = new int[m_sortNum];
	for (int i = 1; i <= d; i++) {
		for (int j = 0; j < 10; j++) {
			bucket[j] = 0;
		}

		for (int j = 0; j < m_sortNum; j++) {
			int k = (m_array[j] / radix) % 10;
			bucket[k]++;
		}
		for (int j = 1; j < 10; j++) {
			bucket[j] += bucket[j - 1];
		}

		for (int j = m_sortNum - 1; j >= m_sortNum; j--) {
			int k = (m_array[j] / radix) % 10;
			tmp[bucket[k] - 1] = m_array[j];
			cnt++;
			bucket[k]--;
		}

		for (int j = 0; j < m_sortNum; j++) {
			m_array[j] = tmp[j];
			cnt++;
		}
		radix *= 10;
	}

	//radSort(0, m_sortNum - 1, i, cnt);
	cout << "基数排序所用时间" << clock() - tStart << endl;
	cout << "基数排序值传递次数" << cnt << endl;
}






void Sort::swap(int& a, int& b) {
	auto tmp = a;
	a = b;
	b = tmp;
}

/*快排的框架函数*/
void Sort::recursQSort(int left, int right, long long& cnt) {
	if (left < right) {
		int pivotPos = partition(left, right, cnt);
		recursQSort(left, pivotPos - 1, cnt);
		recursQSort(pivotPos + 1, right, cnt);
	}
}
/*递归地处理归并排序*/
void Sort::recursMergeSort(int left, int right, long long& cnt) {
	if (left >= right) {
		return;
	}
	int mid = (left + right) / 2;
	recursMergeSort(left, mid, cnt);
	recursMergeSort(mid + 1, right, cnt);
	merge(left, mid, right, cnt);
}

/*两数列的合并*/
void Sort::merge(int left, int mid, int right, long long& cnt) {
	for (int i = left; i <= right; i++) {
		m_supArr[i] = m_array[i];
		cnt++;
	}
	int s1 = left, s2 = mid + 1, tmpPos = left;

	while (s1 <= mid && s2 <= right) {
		if (m_supArr[s1] <= m_supArr[s2]) {
			m_array[tmpPos++] = m_supArr[s1++];
		}
		else {
			m_array[tmpPos++] = m_supArr[s2++];
			cnt++;

		}
	}

	while (s1 <= mid) {
		m_array[tmpPos++] = m_supArr[s1++];
		cnt++;
	}

	while (s2 <= right) {
		m_array[tmpPos++] = m_supArr[s2++];
		cnt++;
	}
}

/*快排的分治函数*/
int Sort::partition(int low, int high, long long& cnt) {
	int pivotPos = low;
	int pivot = m_array[low];
	for (int i = low + 1; i <= high; i++) {
		if (m_array[i] < pivot) {
			pivotPos++;
			if (pivotPos != i) {
				swap(m_array[pivotPos], m_array[i]);
				cnt++;
			}
		}
	}
	m_array[low] = m_array[pivotPos];
	m_array[pivotPos] = pivot;
	cnt += 2;
	return pivotPos;
}

/*堆排序向下调整函数*/
void Sort::siftDown(int start, int end, long long& cnt) {
	int i = start, j = 2 * i + 1; // j是i的左子女
	int tmp = m_array[i];
	cnt++;
	while (j <= end) {
		if (j < end && m_array[j] < m_array[j + 1]) {
			j++;
		}
		if (tmp >= m_array[j]) {
			break;
		}
		else {
			m_array[i] = m_array[j];
			cnt++;
			i = j;
			j = 2 * j + 1;
		}
	}
	m_array[i] = tmp;
	cnt++;
}


int main() {
	Sort sol;
	sol.initIO();
}
