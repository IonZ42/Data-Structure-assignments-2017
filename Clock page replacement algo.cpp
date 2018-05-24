#include<iostream>
#include<cmath>
#include<ctime>

using namespace std;

class Page {
private:
	bool R;
	int pageName;
public:
	Page() { R = false; pageName = -1; }
	//Page(int pagename) { R = false; pageName = pagename; }
	~Page() {}
	void setR(bool r) { R = r; }
	bool getR() { return R; }
	void setPageName(int pagename) { pageName = pagename; }
	int getPageName() { return pageName; }
	void hit() { R = true; cout << "HIT!pagename:" << pageName << endl; }
	void replace(int pagename) { R = false; int temp = pageName; pageName = pagename; cout << "page" << temp << " is replaced by page" << pageName << endl; }
};

class Buffer {
private:
	int length;
	Page* pages;
	Page* P;
	int p;
public:
	Buffer() {
		length = 1;
		pages = new Page[length];
		P = &pages[0];
		p = 0;
	}
	Buffer(int buf_len) {
		length = buf_len;
		pages = new Page[length];
		P = &pages[0];
		p = 0;
	}
	~Buffer() { delete[] pages; pages = NULL; P = NULL; }
	void visitPage(int pagename) {
		for (int i = 0; i < length; i++) {
			if (pagename == pages[i].getPageName()) { pages[i].hit(); break; }
			else if (i == length - 1) { allocPage(pagename); }
		}
		for (int i = 0; i < length; i++) {
			cout << pages[i].getPageName() << " ";
			if (i == length - 1) { cout << endl; }
		}
	}
	void allocPage(int pagename) {
		if (pages[p].getR()) {
			cout << "page" << pages[p].getPageName() << " is replaceable now" << endl;
			pages[p].setR(false); p = (p + 1) % length;
			allocPage(pagename);
		}
		else { pages[p].replace(pagename); p = (p + 1) % length; }
	}
};

void genSeq(int seq_len, int * visitSeq) {
	srand(time(0));
	cout << "random visit sequence:";
	for (int *temp = visitSeq; temp - visitSeq < seq_len; temp++) {
		*temp = (int)(rand() % 20);
		cout << *temp << " ";
	}
	cout << endl;
}

void ClockPR(int buf_len, int seq_len) {
	cout << "Running CLOCK algorithm" << endl;
	Buffer buffer = Buffer(buf_len);
	int* visitSeq = new int[seq_len];
	genSeq(seq_len, visitSeq);
	for (int i = 0; i < seq_len; i++) { buffer.visitPage(visitSeq[i]); }
	cout << "algorithm end" << endl;
	buffer.~Buffer();
	delete[] visitSeq;
	visitSeq = NULL;
}

int main() {
	int buf_len; int seq_len;
	cout << "please input buffer length (5-10):" << endl;
	cin >> buf_len;
	if (buf_len < 5 || buf_len>10) { cout << "wrong input!" << endl; abort(); }
	cout << "please input random pages number (>0):" << endl;
	cin >> seq_len;
	if (seq_len < 0) { cout << "wrong input!" << endl; abort(); }
	ClockPR(buf_len, seq_len);
	//system("pause");
	return 0;
}