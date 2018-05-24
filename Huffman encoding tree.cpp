#include <iostream>
#include <string>
using namespace std;


class HuffNode
{
public:
	virtual ~HuffNode() {}
	virtual int weight() = 0;
	virtual bool isLeaf() = 0;
};

class LeafNode :public HuffNode
{
private:
	char value;
	int wgt;
	string code;
public:
	LeafNode(const char& val, int fre)
	{
		value = val;
		wgt = fre;
	}
	~LeafNode() {}
	int weight()
	{
		return wgt;
	}
	bool isLeaf()
	{
		return true;
	}
	char val()
	{
		return value;
	}
	void setCode(string cod) { code = cod; }
	string const getCode() { return code; }
};


class InternalNode :public HuffNode
{
private:
	HuffNode* lc;
	HuffNode* rc;
	int wgt;
public:
	InternalNode(HuffNode* l, HuffNode* r)
	{
		lc = l;
		rc = r;
		wgt = l->weight() + r->weight();
	}
	~InternalNode() {}
	int weight()
	{
		return wgt;
	}
	bool isLeaf()
	{
		return false;
	}
	HuffNode* left()
	{
		return lc;
	}
	HuffNode* right() 
	{
		return rc;
	}
};

class HuffTree
{
private:
	HuffNode* Root;
	int cost = 0;
public:
	HuffTree(char&val, int fre)
	{
		Root = new LeafNode(val, fre);
	}

	HuffTree(HuffTree* l, HuffTree* r)
	{
		Root = new InternalNode(l->root(), r->root());
	}
	HuffTree()
	{
		Root = new LeafNode(' ',0);
	}
	~HuffTree() {}
	HuffNode* root()
	{
		return Root;
	}
	int weight()
	{
		return Root->weight();
	}
	void coding(HuffNode* now,string code)
	{
		if (now->isLeaf()) { 
			((LeafNode*)now)->setCode(code); 
			cout << ((LeafNode*)now)->val() << ": " << ((LeafNode*)now)->getCode() << endl; 
			cost += ((LeafNode*)now)->weight()*(((LeafNode*)now)->getCode()).length();
			return; }
		else {
			coding(((InternalNode*)now)->left(), code+'0');
			coding(((InternalNode*)now)->right(), code+'1');
		}
	}
	int getCost() { return cost; }
};

class Heap
{

private:
	HuffTree** heap;
	int maxsize;
	int size;

	void shiftDown(int pos)
	{
		while (!isLeaf(pos))
		{
			int l = leftChild(pos);
			int r = rightChild(pos);
			if ((r<size) && heap[r]->root()->weight()<heap[l]->root()->weight())
			{
				l = r;
			}
			if (heap[pos]->root()->weight()< heap[l]->root()->weight())
			{
				return;
			}
			swap(heap, pos, l);
			pos = l;
		}
	}
public:
	Heap(HuffTree** h, int number, int max)
	{
		heap = h;
		size = number;
		maxsize = max;
		bulidHeap();
	}
	void swap(HuffTree** h, int p, int l) { HuffTree* temp = h[p]; h[p] = h[l]; h[l] = temp; }
	int getmaxsize()
	{
		return maxsize;
	}
	int getsize() const
	{
		return size;
	}
	bool isLeaf(int pos) const
	{
		return ((pos<size) && (pos >= size / 2));
	}
	int leftChild(int pos)const
	{
		return 2 * pos + 1;
	}

	int rightChild(int pos)const
	{
		return 2 * pos + 2;
	}
	int parent(int pos)const
	{
		return (pos - 1) / 2;
	}
	void bulidHeap()
	{
		for (int i = size / 2 - 1; i >= 0; i--) {
			shiftDown(i);
		}
	}

	void insert(HuffTree* & it)
	{
		if (size >= maxsize) { cout << "Heap is full!"; }
		int curr = size++;
		heap[curr] = it;
		while ((curr != 0) && (heap[curr]->root()->weight()<heap[parent(curr)]->root()->weight()))
		{
			swap(heap, curr, parent(curr));
			curr = parent(curr);
		}

	}

	HuffTree* removeFirst()
	{
		if (size <= 0) {
			cout << "Heap is empty";
		}
		swap(heap, 0, --size);
		if (size != 0)
		{
			shiftDown(0);
		}
		return heap[size];
	}

	char Remove(int pos) {  }
};


HuffTree* buildHuff(HuffTree** TreeArray, int count)
{
	cout <<endl<< "start HuffTree building" << endl;
	Heap* forest = new Heap(TreeArray, count, count);
	HuffTree *temp1, *temp2, *temp3 = NULL;
	while (forest->getsize()>1)
	{
		temp1 = forest->removeFirst();
		cout << "take the left smallest weight tree, root weight: "<<temp1->root()->weight()<<" ";
		temp2 = forest->removeFirst();
		cout << "take the right smallest weight tree, root weight: " << temp2->root()->weight()<<endl;
		temp3 = new HuffTree(temp1, temp2);
		cout << "merge the two trees into a new tree, root weight: " << temp3->root()->weight()<<endl;
		forest->insert(temp3);
		delete temp1;
		delete temp2;
	}
	cout << "HuffTree build up! final root weight: " << temp3->root()->weight() << endl << endl;
	return temp3;
}

void showCode(HuffTree* ans)
{
	cout << "the Huffman codes are:" << endl;
	ans->coding(ans->root(), "");
	cout << "the average digits of code is: " << (double)ans->getCost()/(double)ans->weight() << endl;
}

int main()
{
	cout << "Please input leeters and frequencies in this format:A 31 B 22 ¡­ £¿" << endl << "ÊäÈë£¿ÒÔ½áÊø" << endl;
	int count = 0;
	char temp = 0;int tempf = 0;
	char letters[50];int fres[50];
	while (temp != '?') { cin >> temp;if (temp == '?') { break; }cin >> tempf; letters[count] = temp;fres[count] = tempf; count++; }
	HuffTree** treeArray = new HuffTree* [count];
	int sumfre = 0;
	for (int i = 0; i < count; i++) { treeArray[i] = new HuffTree(letters[i], fres[i]); sumfre += fres[i]; }
	cout << "sum frequency is " << sumfre << endl;
	HuffTree* ans = buildHuff(treeArray, count);
	showCode(ans);
}