#include<iostream>
#include<cmath>
#include<ctime>
#include<string>
#include<fstream>
using namespace std;

const time_t startTime = time(0);

void genSeq(int length) {
	srand(time(0));
	string path = to_string(length)+".dat";
	ofstream out(path);
	if (out.good()) {
		for (int i = 0; i < length; i++) {
			int t = rand();
			out << (int)((t % 2 == 1) ? (-(t % 1001)) : (+(t % 1001)));
			out << " ";
		}
	}
	out.close();
}

int* readSeq(int length,int* seq) {
	string path = to_string(length) + ".dat";
	ifstream ifs(path);
	if (ifs.good())
	{
		for (int i = 0; i < length; i++)
			ifs >> seq[i];
	}
	ifs.close();
	return seq;
}

int dataCounter = 0;
void saveQ(int length,int* seq) {
	const time_t endTime = time(0);
	string path;
	switch (length) {
	case 100:path = "q1.dat"; break;
	case 1000:path = "q2.dat"; break;
	case 10000:path = "q3.dat"; break;
	case 100000:path = "q4.dat"; break;
	default:path = "q0.dat";
	}
	ofstream out(path);
	//dataCounter = 0;
	if (out.good()) {
		out << endTime - startTime << "ms" << endl;
		//cout <<endl<< endTime - startTime << "ms" << endl;
		for (int i = 0; i < length; i++) {
			out << seq[i] << " "; if (++dataCounter % 80 == 0) { cout << endl; }//bat用txt打开自动换行
			//cout << seq[i] << " "; if (++dataCounter % 80 == 0) { cout << endl; }
		}
	}
	out.close();
}

void saveM(int length,int *seq) {
	const time_t endTime = time(0);
	string path;
	switch (length) {
	case 100:path = "m1.dat"; break;
	case 1000:path = "m2.dat"; break;
	case 10000:path = "m3.dat"; break;
	case 100000:path = "m4.dat"; break;
	default:path = "m0.dat";
	}
	ofstream out(path);
	//dataCounter = 0;
	if (out.good()) {
		out << endTime - startTime << "ms" << endl;
		//cout <<endl<< endTime - startTime << "ms" << endl;
		for (int i = 0; i < length; i++) {
			out << seq[i] << " "; if (++dataCounter % 80 == 0) { cout << endl; }
			//cout << seq[i] << " "; if (++dataCounter % 80 == 0) { cout << endl; }
		}
	}
	out.close();
}

inline int partition(int* seq,int left,int right,int& pivot) {
	do {//目标：pivot左都<它，右都>它
		while (seq[++left] > pivot);
		while (left < right&&seq[--right] < pivot);
		int t = seq[right]; seq[right] = seq[left]; seq[left] = t;//过程：从左向右读到<pivot元素后从右向左读到>pivot元素，二者换位后均达成要求，重复直到right<=left
	} while (left < right);
	return left;
}

int counter = 0;
void quickSort(int length, int* seq,int left,int right) {//cour内容拖慢速度，手动注释掉
	if (right <= left)return;//单元素时返回
	int pivotIndex = (left+ right)/2;
	int temp = seq[right];seq[right] = seq[pivotIndex]; seq[pivotIndex] = temp;//中间元素视为锚点，将最右元素与中间元素换位
	int k = partition(seq, left - 1, right, seq[right]);
	temp = seq[right];seq[right] = seq[k]; seq[k] = temp;//将排序后序列分为pivot左(<pivot)右(>pivot)两个序列
	//cout << "sorting left sublist from index " << left << " to " << k - 1 << "          "; if (++counter % 4 == 0) { cout << endl; }
	quickSort(length, seq, left, k - 1);
	//cout << "sorting right sublist from index " << k + 1 << " to " << right << "          "; if (++counter % 4 == 0) { cout << endl; }
	quickSort(length, seq, k + 1, right);
	if (left + right == length - 1)saveQ(length, seq);
}

int counter2 = 0;
void mergeSort(int length, int* seq,int left,int right,int* temp) {//cour内容拖慢速度，手动注释掉
	if (left == right) {return;}//sublist为单元素时返回
	int mid = (left + right) / 2;//每次取二分之一，sublist数*2
	//cout << "sorting sublist from index " << left << " to " << mid << "          "; if (++counter2 % 4 == 0) { cout << endl; }
	mergeSort(length, seq, left, mid, temp);
	//cout << "sorting sublist from index " << mid + 1 << " to " << right << "          "; if (++counter2 % 4 == 0) { cout << endl; }
	mergeSort(length, seq, mid+1,right, temp);
	for (int i = left; i <= right; i++) {
		temp[i] = seq[i];
	}
	int i1 = left, i2 = mid + 1;
	for (int curr = left; curr <= right; curr++) {
		if (i1 == mid + 1)seq[curr] = temp[i2++];
		else if (i2 > right)seq[curr] = temp[i1++];
		else if (temp[i1] > temp[i2])seq[curr] = temp[i1++];
		else seq[curr] = seq[i2++];
	}
	if(left+right==length-1)saveM(length, seq);
}

int main() {
	int length;
	cout << "input data array length:" << endl;
	cin >> length;
	cout << "choose the sorting way (input 'M' as merge sort, 'Q' as quick sort):" << endl;
	char type; cin >> type;
	int* seq = new int[length];
	//genSeq(length);//填充随机数据到dat，已完成
	int * a = readSeq(length,seq);
	int * b = new int[length];
	//for (int i = 0; i < length; i++) { cout << a[i] <<" "; }//显示填充数据，测试用
	if(type=='M')mergeSort(length, a, 0, length - 1, b);
	else if(type=='Q')quickSort(length, a, 0, length - 1);
	else cout << "wrong type" << endl;
	cout << "sort up!" << endl;
	//delete[]a, b;delete[] seq; seq = NULL; a = NULL; b = NULL;
	return 0;
}