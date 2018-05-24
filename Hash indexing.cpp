#include<iostream>
#include<cmath>
#include<ctime>
#include<string>
#include<fstream>
using namespace std;

class HashSlot {
public:
	int data = -1;
	bool isFull = false;
	HashSlot() {}
	~HashSlot() {}
	HashSlot(int d) { data = d; isFull = true; }
	void add(int d) { data = d; isFull = true; }
};

//提取十位个位数字，若k<10则返回个位，否则返回个位*10+十位
int rev(int k) {
	int a = k % 10, b = (k - a) / 10;
	if (b == 0)return a;
	else return a * 10 + b;
}

void genSseq(int seq_len, int * visitSeq) {
	srand(time(0));cout << "random search sequence: ";
	for (int *temp = visitSeq; temp - visitSeq < seq_len; temp++) {
		*temp = rev((int)(rand() % 100));//基本相同，seed时间间隔短，rev一下增加FAIL率；另一种方法是直接rand不改seed
		cout << *temp << " ";
	}
	cout << endl;
}

void genSeq(int seq_len, int * visitSeq) {
	srand(time(0));
	cout << "random key sequence: ";
	for (int *temp = visitSeq; temp - visitSeq < seq_len;) {//遍历传入的空数组
		int ran = (int)(rand() % 100); bool isOK = true;//每次key值重复（指针不动）或不重复（指针前进1位）都要重新rand出一个key值
		for (int *check = temp; check - visitSeq >= 0; check-- ) {
			if (ran == *check) { isOK = false; break; }//查重复，从当前位置倒回到[0]元素，重复就break
		}
		if (isOK) {
			*temp = ran; temp++;//不重复才允许过，指针前进一位继续填充
		}
	}
	for (int *temp = visitSeq; temp - visitSeq < seq_len; temp++) {
		cout << *temp << " ";
		if ((temp - visitSeq) % 10 == 9) { cout << endl; }
	}
	cout << endl;
}

int H1(int k) { return k % 59; }

int H2(int k) { return rev(k + 1) % 53 == 0 ? 1: rev(k + 1) % 53; }//步长>=0

int main() {
	int list_len = 50, search_len = 10;
	int *listseq = new int[list_len], *searchseq = new int[search_len], *searchresult = new int[search_len];
	genSeq(list_len, listseq); genSseq(search_len, searchseq);
	HashSlot *hashtable = new HashSlot[59];
	for (int i = 0; i < list_len; i++) {
		int k = listseq[i];//key值序列是只读不写的，k为当前遍历到的key值
		int home = H1(k);//先取一下H1一次哈希的结果
		if (!hashtable[home].isFull)hashtable[home].add(k);//home槽是空的就添加，否则↓
		else {
			int next = home;
			bool isOK = false;
			while (!isOK) {
				next = (next + H2(k)) % 59;//每次探测的槽index增加步长（H2函数值）
				if (!hashtable[next].isFull) { hashtable[next].add(k); isOK = true; }//探测的槽为空就加否则继续循环
			}
		}
	}
	cout << endl << "Slot    " << "Number    "<<endl;
	for (int i = 0; i < 59; i++) {
		printf("%-8d", i);
		printf("%-8d", hashtable[i].data);
		cout << endl;
	}
	for (int i = 0; i < search_len; i++) {
		searchresult[i] = -1;
		int k = searchseq[i];
		int next = H1(k);
		bool isOK = true;
		while (isOK) {//基本类似insert的过程，探测的槽的结果符合就记录槽index，否则逐步探测直到走到空槽——检查的key值不在此表中
			if (hashtable[next].data == k) { searchresult[i] = next; isOK = false; break; }
			if (!hashtable[next].isFull) { isOK = false; }
			else { next = (next + H2(k)) % 59; }
		}
	}
	cout << endl << "SearchData  " << "Result    " << endl;
	for (int i = 0; i < search_len; i++) {
		printf("%-12d", searchseq[i]);
		if (searchresult[i]!=-1) { printf("%-12d", searchresult[i]); }
		else { printf("%-12s", "Fail"); }
		cout << endl;
	}
	system("pause");
	return 0;
}