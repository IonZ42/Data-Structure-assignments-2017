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

//��ȡʮλ��λ���֣���k<10�򷵻ظ�λ�����򷵻ظ�λ*10+ʮλ
int rev(int k) {
	int a = k % 10, b = (k - a) / 10;
	if (b == 0)return a;
	else return a * 10 + b;
}

void genSseq(int seq_len, int * visitSeq) {
	srand(time(0));cout << "random search sequence: ";
	for (int *temp = visitSeq; temp - visitSeq < seq_len; temp++) {
		*temp = rev((int)(rand() % 100));//������ͬ��seedʱ�����̣�revһ������FAIL�ʣ���һ�ַ�����ֱ��rand����seed
		cout << *temp << " ";
	}
	cout << endl;
}

void genSeq(int seq_len, int * visitSeq) {
	srand(time(0));
	cout << "random key sequence: ";
	for (int *temp = visitSeq; temp - visitSeq < seq_len;) {//��������Ŀ�����
		int ran = (int)(rand() % 100); bool isOK = true;//ÿ��keyֵ�ظ���ָ�벻�������ظ���ָ��ǰ��1λ����Ҫ����rand��һ��keyֵ
		for (int *check = temp; check - visitSeq >= 0; check-- ) {
			if (ran == *check) { isOK = false; break; }//���ظ����ӵ�ǰλ�õ��ص�[0]Ԫ�أ��ظ���break
		}
		if (isOK) {
			*temp = ran; temp++;//���ظ����������ָ��ǰ��һλ�������
		}
	}
	for (int *temp = visitSeq; temp - visitSeq < seq_len; temp++) {
		cout << *temp << " ";
		if ((temp - visitSeq) % 10 == 9) { cout << endl; }
	}
	cout << endl;
}

int H1(int k) { return k % 59; }

int H2(int k) { return rev(k + 1) % 53 == 0 ? 1: rev(k + 1) % 53; }//����>=0

int main() {
	int list_len = 50, search_len = 10;
	int *listseq = new int[list_len], *searchseq = new int[search_len], *searchresult = new int[search_len];
	genSeq(list_len, listseq); genSseq(search_len, searchseq);
	HashSlot *hashtable = new HashSlot[59];
	for (int i = 0; i < list_len; i++) {
		int k = listseq[i];//keyֵ������ֻ����д�ģ�kΪ��ǰ��������keyֵ
		int home = H1(k);//��ȡһ��H1һ�ι�ϣ�Ľ��
		if (!hashtable[home].isFull)hashtable[home].add(k);//home���ǿյľ���ӣ������
		else {
			int next = home;
			bool isOK = false;
			while (!isOK) {
				next = (next + H2(k)) % 59;//ÿ��̽��Ĳ�index���Ӳ�����H2����ֵ��
				if (!hashtable[next].isFull) { hashtable[next].add(k); isOK = true; }//̽��Ĳ�Ϊ�վͼӷ������ѭ��
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
		while (isOK) {//��������insert�Ĺ��̣�̽��Ĳ۵Ľ�����Ͼͼ�¼��index��������̽��ֱ���ߵ��ղۡ�������keyֵ���ڴ˱���
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