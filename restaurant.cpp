#include "main.h"
int MAXSIZE;
struct Node {
	char ch;
	int freq;
	Node* left, * right;
	Node(char ch, int freq, Node* left, Node* right) {
		this->ch = ch;
		this->freq = freq;
		this->left = left;
		this->right = right;
	}
    Node(int freq) {
        this->ch = '\0';
        this->freq = freq;
        this->left = nullptr;
        this->right = nullptr;
    }
	~Node() {
		delete left;
		delete right;
	}
};

struct NodeQueue {
	int val;
	NodeQueue* next;
	NodeQueue(int val) {
		this->val=val;
		this->next=nullptr;
	}
	~NodeQueue() {
		delete next;
	}
};
class Queue{
private:
	int count;
	NodeQueue* front;
	NodeQueue* rear;
public:
	Queue() {
		this->front=nullptr;
		this->rear=nullptr; 
		this->count=0;
			
	}
	~Queue() {
		delete front;
	}
	void EnQueue(int &val) {
				NodeQueue* newNode= new NodeQueue(val);
				if(this->count==0) {
					this->front=newNode;
				}
				else {
					this->rear->next=newNode;
				}
				this->rear=newNode;
				this->count++;
			}
	void DeQueue() {
		if(this->count==0) return;
		if(this->count==1) {	
			delete this->front;
			this->front=nullptr;
			this->rear=nullptr;
		}
		else {
			NodeQueue* temp = this->front;
			this->front = this->front->next;
			free(temp);
		}
		this->count--;
	}
	int TopQueue() {
		if(this->count==0) return -1;
		return this->front->val;
	}
	int getsize() {
		return this->count;
	}
	void printLIFO(NodeQueue* &start,int &lable,int &num) {
		if(start!=nullptr) {
			printLIFO(start->next,lable,num);
			if(num==0) return;
			num--;
			cout<<lable<<"-"<<start->val<<endl;
		}
	}
	void PrintLIFO(int &lable,int num) {
		printLIFO(this->front,lable,num);
	}
};

class Gojo {
private:
	vector<Node*> Hash_Table;
	vector<Queue*> q;
public:
	Gojo() {
		for(int i=0;i<=MAXSIZE; i++) {
			Hash_Table.push_back(nullptr);
			q.push_back(new Queue());		
		}
	}
	~Gojo() {
		for(int i=0; i<=MAXSIZE; i++) {
			delete Hash_Table[i];
			delete q[i];
		}
		q.clear();
		Hash_Table.clear();
	}
	int Hash_Funct(int result) {
		return result%MAXSIZE+1;
	}
	Queue* getQueue(int &val) {
		int index = this->Hash_Funct(val);
		return q[index];
	}
	Node* insertHASH(Node* &root, int &value) {
		if (root == nullptr) {
			root = new Node(value);
			return root;
		}
		else if (value < root->freq) root->left = insertHASH(root->left, value);
		else root->right = insertHASH(root->right, value);
		return root;
	}
	void Inorder(Node* &root) {
		if(root!=nullptr) {
		    Inorder(root->left);
		    cout<<root->freq<<"\n";
		    Inorder(root->right);
		}
	}
	void PrintInOrder(int val) {
		Inorder(this->Hash_Table[val]);
	}
	Node* minValueNode(Node* &node)
	{
		Node* current = node;
		while (current->left != nullptr) current = current->left;
		return current;
	}
	Node* removeHASH(Node* &root, int value) {
		if (root == nullptr) return root;
		if (value < root->freq) root->left = removeHASH(root->left, value);
		else if (value > root->freq) root->right = removeHASH(root->right, value);
		else {
			if (root->left == nullptr || root->right == nullptr)
			{
				Node* temp = root->left ? root->left : root->right;

				if (temp == nullptr)
				{
					temp = root;
					root = nullptr;
				}
				else
					*root = *temp;
				free(temp);
			}
			else
			{
				Node* temp = minValueNode(root->right);
				root->freq = temp->freq;
				root->right = removeHASH(root->right, temp->freq);
			}
		}
		return root;
	}
	void addHash(int &val) {
		int index = this->Hash_Funct(val);
		q[index]->EnQueue(val);
		Hash_Table[index] = insertHASH(Hash_Table[index], val);
	}
	void removeHash(int &val) {
		int index = this->Hash_Funct(val);
		Hash_Table[index] = removeHASH(Hash_Table[index], val);
	}
 	void printPre(Node* &root, vector<int> &a) {
		if(root==nullptr) return;
		if (root != nullptr) {
			a.push_back(root->freq);
			printPre(root->left,a);
			printPre(root->right,a);
		}
	}
	void printPrefix(int &val,vector<int> &a) {
		int index = Hash_Funct(val);
		printPre(Hash_Table[index],a);
	}
};

class Heap {
private:
	vector<int> min_heap;
	vector<Queue*> qofNode;
	vector<int> impact_heap;
public:
	Heap() {
		for(int i=0; i<=MAXSIZE; i++) {
			qofNode.push_back(new Queue());
		}
	}
	~Heap() {
		min_heap.clear();
		for(int i=0; i<=MAXSIZE; i++) {
			delete qofNode[i];
		}
		qofNode.clear();
		impact_heap.clear();
	}
	int getLeftChild(int &parent) {
		return 2 * parent + 1;
	}
	int getRightChild(int &parent) {
		return 2 * parent + 2;
	}
	int getParent(int &child) {
		return (child - 1) / 2;
	}
	void impact(int &lable) {
		bool flag = 0;
		int index = find(impact_heap.begin(),impact_heap.end(), lable)-impact_heap.begin();
		if(qofNode[lable]->getsize()!=0) {
			impact_heap.push_back(lable);
			flag=1;
		}
		if(index!=int(impact_heap.size())-1&&flag==1) impact_heap.erase(impact_heap.begin()+index);	
		else if (index!=int(impact_heap.size())&&flag==0) impact_heap.erase(impact_heap.begin()+index);
	}
	bool comparelable(int &lable1, int &lable2) {
		int index1 = find(impact_heap.begin(), impact_heap.end(), lable1) - impact_heap.begin();
		int index2 = find(impact_heap.begin(), impact_heap.end(), lable2) - impact_heap.begin();
		if (index1 <= index2) return 1;
		return 0;
	}
	void reHeapUp(int child) {
		if(int(min_heap.size())<=1) return;
		if(child ==0) return;
		if (child != 0)
		{
			int curr_parent = getParent(child);
			if ((int(qofNode[min_heap[child]]->getsize()) < int(qofNode[min_heap[curr_parent]]->getsize()))||((int(qofNode[min_heap[child]]->getsize()) == int(qofNode[min_heap[curr_parent]]->getsize()))&&comparelable(min_heap[child],min_heap[curr_parent])))
			{
				swap(min_heap[child], min_heap[curr_parent]);
				reHeapUp(curr_parent);
			}
		}
	}
	void reHeapDown(int parent) {
		if(int(min_heap.size())<=1) return;
		int left = getLeftChild(parent);
		if (left < int(min_heap.size()))
		{
			int right = getRightChild(parent);

			if (right < int(min_heap.size()))
			{
				if ((qofNode[min_heap[left]]->getsize() < qofNode[min_heap[right]]->getsize())||((qofNode[min_heap[left]]->getsize() == qofNode[min_heap[right]]->getsize())&&comparelable(min_heap[left],min_heap[right])))
				{
					if ((qofNode[min_heap[parent]]->getsize() > qofNode[min_heap[left]]->getsize())||((qofNode[min_heap[parent]]->getsize() == qofNode[min_heap[left]]->getsize())&&comparelable(min_heap[left],min_heap[parent])))
					{
						swap(min_heap[parent], min_heap[left]);
						parent = left;
						reHeapDown(parent);
					}
				}
				else if ((qofNode[min_heap[left]]->getsize() > qofNode[min_heap[right]]->getsize())||((qofNode[min_heap[left]]->getsize() == qofNode[min_heap[right]]->getsize())&&comparelable(min_heap[right],min_heap[left])))
				{
					if ((qofNode[min_heap[parent]]->getsize() > qofNode[min_heap[right]]->getsize())||((qofNode[min_heap[parent]]->getsize() == qofNode[min_heap[right]]->getsize())&&comparelable(min_heap[right],min_heap[parent])))
					{
						swap(min_heap[parent], min_heap[right]);
						parent = right;
						reHeapDown(parent);
					}
				}	
			
			}
			// if no right subtree
			else
			{
				if (qofNode[min_heap[parent]]->getsize() > qofNode[min_heap[left]]->getsize()||(qofNode[min_heap[parent]]->getsize() == qofNode[min_heap[left]]->getsize()&&comparelable(min_heap[left],min_heap[parent])))
				{
					swap(min_heap[parent], min_heap[left]);
					parent = left;
					reHeapDown(parent);
				}
			}

		}
	}
	void insertKey(int &data) {
		int index = data % MAXSIZE +1;
		qofNode[index]->EnQueue(data);
		impact(index);
		if(qofNode[index]->getsize()==1) {
			min_heap.push_back(index);
		    int child = int(min_heap.size()) - 1;
		    reHeapUp(child);
		}
		if(qofNode[index]->getsize()>1) {
		//	for(int i=0; i<min_heap.size(); i++) reHeapDown(i);
			reHeapDown(INDEXinHEAP(index));
		}
	}
	void deleteKey() {
		if (int(min_heap.size()) == 0)
		{
			return;
		}
		int min = qofNode[min_heap[0]]->getsize();
		if (min != 0) return;
		int child = int(min_heap.size()) - 1;
		// swap root value with last element
		swap(min_heap[0], min_heap[child]);
		min_heap.pop_back();
		reHeapDown(0);
		deleteKey();
	}
	void printLIFO(int &lable,int num) {
		qofNode[lable]->PrintLIFO(lable,num);
	}
	void PrintPreandLIFO(int start,int num) {
		if(start<int(min_heap.size())) {
			printLIFO(min_heap[start],num);
			PrintPreandLIFO(2*start+1,num);
			PrintPreandLIFO(2*start+2,num);
		}
	}
	vector<int> lableMIN(int &num) {
		vector<int> a = impact_heap;
		for(int i=0; i<int(a.size()); i++) {
			for(int j=0; j<int(a.size()-1); j++) {
				if(qofNode[a[j]]->getsize()>qofNode[a[j+1]]->getsize()) swap(a[j],a[j+1]);
			}
		}
		vector<int> b;
		for(int i=0; (i<int(a.size()))&&(i<num); i++) {
			b.push_back(a[i]);
		}
		return b;
	}
	int INDEXinHEAP(int &n) {
		for(int i=0 ;i<int(min_heap.size()); i++) {
			if(min_heap[i]==n) return i;
		}
		return -1;
	}
	void FindAndDequeue(int &num) {
		vector<int> a = lableMIN(num);
		if(int(a.size())==0) return;
			for(int i=0; i<int(a.size()); i++) {
				int count =num;
			    while(count&&qofNode[a[i]]->getsize()) {
					cout<<qofNode[a[i]]->TopQueue()<<"-"<<a[i]<<endl;
				    qofNode[a[i]]->DeQueue();
				    count = count-1;
			    }
			    reHeapUp(INDEXinHEAP(a[i]));
			    if (qofNode[a[i]]->getsize()==0) {
				    deleteKey();
			    }
			    impact(a[i]);
			}
	} 
};

class Solution {
public:
    int Mod=MAXSIZE;
    //Calcute C_{n} choose k by Pascal's equation
    
    int compute_C_N_choose_K(int N, int K){
        if (K>N/2) K=N-K;//C_N choose K =C_N choose N-K
        vector<int> C_N(K+1, 0), prevC(K+1, 0);
        for(int i=0; i<=N; i++){
            C_N[0]=1;
            for(int j=1; j<=min(i, K);j++){
                C_N[j]=((long)prevC[j-1]+prevC[j])%Mod;  
            }
            prevC=C_N;  
        }
        return C_N[K];
    }

    long Subproblem(vector<int>& nums){
        int n=nums.size();
        if (n<=2) return 1;
        vector<int> left, right;
        int root=nums[0];
        for (int i=1; i<n; i++){
            if (root<=nums[i]) right.push_back(nums[i]);
            else left.push_back(nums[i]);
        }
        long r=Subproblem(right), l=Subproblem(left);
        return compute_C_N_choose_K(n-1, left.size())*r%Mod*l%Mod;

    }
    int numOfWays(vector<int>& nums) {
        return (Subproblem(nums))%Mod;
    }
};



class HuffmanTree {
private:
	string str;
	Node* root;
	int count;
	struct comp {
        bool operator()(pair<Node*, int> &A, pair<Node*, int> &B) {
            return (A.first->freq > B.first->freq) || (A.first->freq == B.first->freq && A.second > B.second);
        }
    };
	priority_queue<pair<Node*,int>, vector<pair<Node*,int>>,comp> pq;
	map<char, string> huffmanCode;
public:
	HuffmanTree() {
		this->str = "";
		this->root = nullptr;
		this->count = 0;
	}
	void inorder(Node*&root,vector<Node*>&a){
		if(root!=nullptr) {
		    inorder(root->left,a);
		    a.push_back(new Node(root->ch,root->freq,nullptr,nullptr));
		    inorder(root->right,a);
		}
	}
	void PrintInorder(vector<Node*>&a) {
		inorder(this->root,a);
	}
	~HuffmanTree() {
		while(!pq.empty()) {
			pq.pop();
		}
		delete root;
	}
	Node* rotateRight(Node* root,bool &flag) {
		Node* x = root->left;
		root->left = x->right;
		//root->freq = root->right->freq+root->left->freq;
		x->right = root;
		//x->freq=x->left->freq+x->right->freq;
		if(x->ch!='\0') flag=1;
		return x;
	}
	Node* rotateLeft(Node* root, bool &flag) {
		Node* x = root->right;
		root->right = x->left;
		//root->freq = root->left->freq+ root->right->freq;
		x->left = root;
		//x->freq = x->left->freq + x->right->freq;
		if(x->ch!='\0') flag=1;
		return x;
	}
	Node* BalanceRoot(Node* root, int &count, bool &flag) {
		int n = getBalance(root);
		if (n > 1) {
			if (getBalance(root->left) >= 0){
				count--;
				return rotateRight(root,flag);
			}
		}
		if (n > 1) {
			if (getBalance(root->left) < 0) {
				count--;
				root->left = rotateLeft(root->left,flag);
				return rotateRight(root,flag);
			}
		}
		if (n < -1) {
			if (getBalance(root->right) <= 0) {
				count--;
				return rotateLeft(root,flag);
			}
		}
		if (n < -1) {
			if (getBalance(root->right) > 0) {
				count--;
				root->right = rotateRight(root->right,flag);
				return rotateLeft(root,flag);
			}
		}
		return root;
	}
	void Balance(Node* &root, int &count, bool &flag) {
		if(count==0||root==nullptr) return;
		//cout<<count<<endl;
		root = BalanceRoot(root,count,flag);
		Balance(root->left,count,flag);
		Balance(root->right,count,flag);
		// if(count>0) {
		 root = BalanceRoot(root,count,flag);
		// }
		//root = BalanceRoot(root,count,flag);
		//Balance(root,count,flag);
		// if(temp!= root) {
		// 	root = temp;
		// 	Balance(root->left,count);
		// 	Balance(root->right,count);
		// }
		// else if(temp==root) {
		// 	Balance(root->left,count);
		// 	Balance(root->right,count);
		// }

	}
	void encode(Node* root, string str, map<char, string>& huffmanCode)
	{
		if (root == nullptr) return;
		if (!root->left && !root->right) {
			huffmanCode[root->ch] = str;
		}
		encode(root->left, str + "0", huffmanCode);
		encode(root->right, str + "1", huffmanCode);
	}
	void PushPrioQueue(char &name, int &count,int &index) {
		this->pq.push({new Node(name, count, nullptr, nullptr),index});
		this->count++;
	}
	int getHeightRec(Node* node)
	{
		if (node == nullptr) return 0;
		int lh = this->getHeightRec(node->left);
		int rh = this->getHeightRec(node->right);
		return (lh > rh ? lh : rh) + 1;
	}
	int getBalance(Node* root) {
		return getHeightRec(root->left) - getHeightRec(root->right);
	}
	bool AddHuffmanTree()
	{
		while (int(this->pq.size()) != 1)
		{
			Node* left =this->pq.top().first;
			this->pq.pop();
			Node* right = this->pq.top().first;	
			this->pq.pop();
			int sum = left->freq + right->freq;
			Node*temp = new Node('\0', sum, left, right);
			bool check = 0;
			int countRotate = 3;
			Balance(temp,countRotate,check);
		//	cout<<endl<<countRotate<<endl;
			if (check==1) return 0;
			//if(check==0) return;
			//temp = BalanceRoot(temp);
			pq.push({temp,++this->count});

		}
		this->root = pq.top().first;
		return 1;
	}
	void PrintEnCode(string &text) {
		encode(root, "", huffmanCode);
		for (char ch : text) {
			this->str += huffmanCode[ch];
		}
	}
	string getEncode() {
		return this->str;
	}
};

void LAPSE(HuffmanTree* &Huff,Gojo* &GojoRes, Heap* &Sanaku,string &name) {
    
	map<char,int> freq;
	string checkname = name;
	int checksize = name.length();
	for(int i=0; i<checksize; i++) {
		if(int(checkname.find(checkname[i]))<i) {
			checkname.erase(checkname.begin()+i);
			i--;
			checksize--;
		}
	}
	if(checkname.length()<3) return;
	delete Huff; // Xoa cay Huff o lenh LAPSE truoc
    Huff = new HuffmanTree();
	int namesize = name.length();
	for(char ch : name) freq[ch]++;
	for(int i=0; i<namesize; i++) {
		int d = freq[name[i]]%26;
		if(name[i]>='a'&&name[i]<='z'&&'z'-name[i]<d) name[i]+=int('a'+d-1-'z');
		else if(name[i]>='A'&&name[i]<='Z'&&'Z'-name[i]<d) name[i]+=int('A'+d-1-'Z');
		else name[i]+=int(d);
	}
	
	freq.clear();
	for(char ch : name) freq[ch]++;
	string nameCeasar = name;
	for(int i=0; i<namesize; i++) {
		if(int(name.find(name[i]))<i) {
			name.erase(name.begin()+i);
			i--;
			namesize--;
		}
	}
	for (int i = 0; i < namesize; i++) {
		for (int j = 0; j < namesize - 1; j++) {
			if (freq[name[j]] > freq[name[j+1]] ||
				((freq[name[j]] == freq[name[j+1]])&&(name[j]>='a')&&(name[j]<='z') && (name[j+1] >= 'a' )&& (name[j+1] <= 'z')&&(name[j]>name[j+1])) ||
				((freq[name[j]] == freq[name[j+1]]) && (name[j] >= 'A') && (name[j] <= 'Z') && (name[j+1] >= 'A') && (name[j+1] <= 'Z') && (name[j] > name[j+1])) ||
				((freq[name[j]] == freq[name[j+1]]) && (name[j] >= 'A') && (name[j] <= 'Z') && (name[j+1] >= 'a') && (name[j+1] <= 'z')))
				swap(name[j], name[j+1]);
		}
	} // sort ten theo BUBBLE sort
	//   for(char ch: name) cout<<ch<<" ";
	//   cout<<endl;
	//   for(char ch: name) cout<<freq[ch]<<" ";
	//   cout<<endl;
	for(int i=0; i<namesize; i++) {
		Huff->PushPrioQueue(name[i],freq[name[i]],i);
	}
	if(Huff->AddHuffmanTree()==0) return;
	//Huff->PrintHuffmanCode();
	Huff->PrintEnCode(nameCeasar);
	string str = Huff->getEncode();
	int sizestr = str.length();
	string strresult = "";
	for( int i=sizestr-1; i>=sizestr-10&&i>=0; i--) strresult+=str[i];
	int Result =0;
	for (unsigned int i = 0; i < strresult.length(); i++) {
		Result = Result * 2 + (strresult[i] - 48);
	}
	cout<<Result<<endl;
	if(Result%2==1) GojoRes->addHash(Result);
	else Sanaku->insertKey(Result);
	freq.clear();
	
}

void KOKUSEN(Gojo* &GojoRes){
	Solution* kokusen = new Solution();
	for(int i=0; i<MAXSIZE; i++) {
		vector<int> a;
		GojoRes->printPrefix(i,a);
		int count = kokusen->numOfWays(a);
	    count = count%MAXSIZE;
		while(GojoRes->getQueue(i)->getsize()!=0&&count) {
		 	int top = GojoRes->getQueue(i)->TopQueue();
		 	GojoRes->getQueue(i)->DeQueue();
			GojoRes->removeHash(top);
			count--;
		}
        if(i==MAXSIZE-1) a.clear();	
	}
	delete kokusen;
}
void KEITEIKEN(Heap*&Sanaku,int num){
	Sanaku->FindAndDequeue(num);
}
void HAND(HuffmanTree* &Huff){
	vector<Node*> a;
	Huff->PrintInorder(a);
	for(int i=0; i<int(a.size()); i++) {
		if(a[i]->ch=='\0') cout<<a[i]->freq<<"\n";
		else cout<<a[i]->ch<<"\n";
	} 
	for(int i=0; i<int(a.size()); i++) {
		delete a[i];
	}
	a.clear();
}
void LIMITLESS(Gojo* &GojoRes,int num) {
	if(num>MAXSIZE) return;
	//GojoRes->PrintBST();
	GojoRes->PrintInOrder(num);
}
void CLEAVE(Heap* &Sanaku,int num) {
	Sanaku->PrintPreandLIFO(0,num);
}
void simulate(string filename)
{
	ifstream ss(filename);
	string str, maxsize,name,num;
	ss>>str;
	ss>>maxsize;
	MAXSIZE = stoi(maxsize);
	Gojo* GojoRes = new Gojo();
	Heap* Sanaku = new Heap();
    HuffmanTree* Huff = new HuffmanTree();
	while(ss>>str) {
		 if(str=="LAPSE") {
			ss>>name;
			//cout<<name<<endl;
			LAPSE(Huff, GojoRes, Sanaku, name);
		}
		else if(str=="KOKUSEN") {
			KOKUSEN(GojoRes);
		}
		else if(str=="KEITEIKEN") {
			ss>>num;
			KEITEIKEN(Sanaku, stoi(num));
		}
		else if(str=="HAND") {
			HAND(Huff);
		}
		else if(str=="LIMITLESS") {
			ss>>num;
			LIMITLESS(GojoRes, stoi(num));
		}
		else if(str=="CLEAVE") {
			ss>>num;
			CLEAVE(Sanaku, stoi(num));
		}
	}
	delete GojoRes;
	delete Sanaku;
	delete Huff;
	return;
}