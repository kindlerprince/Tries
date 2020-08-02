#include<iostream>
using namespace std;

#define N 26 

class Tries{
	public:
	char data;
	bool isTerminating;
	Tries **children;
	public:
	Tries(){
		children = new Tries*[N];
		for(int i = 0; i < N; i++){
			children[i] = NULL;
		}
	}
	~Tries(){}
};

void printTries(Tries *r, string str){
	if(r == NULL){
		return;
	}
	str.push_back(r->data);
	if(r->isTerminating == true){
		cout<<str<<endl;
	}
	for(int i = 0; i < N; i++){
		printTries(r->children[i],str);
	}
	str.clear();
}

void insert(Tries **r, string word, int index){
	Tries *root = *r;
	if(index == word.length() - 1){
		if(root == NULL){
			root = new Tries();
			root->data = word.at(index);
		}
		root->isTerminating = true;
		*r = root;
		return;
	}

	if(root == NULL){
		root = new Tries();
		root->data = word.at(index);
		root->isTerminating = false;
		*r = root;
	}
	insert(&root->children[word.at(index + 1) - 'a'], word, index + 1);
}

Tries *searchExact(Tries *root, string word, int index){
	if(root == NULL){
		return NULL;
	}
	if(index == word.length() - 1){
		if(root->isTerminating && root->data == word.at(index)){
			return root;
		}else{
			return NULL;
		}
	}
	return searchExact(root->children[word.at(index + 1) - 'a'], word, index + 1);
}

void suggestion(Tries *root, string word, string str, int index){
	if(root == NULL){
		return;
	}
	str.push_back(root->data);
	if(root->isTerminating == true){
		cout<<str<<endl;
	}
	if(index < word.length() - 1){
		suggestion(root->children[word.at(index + 1) - 'a'], word, str, index + 1);
	}
	else{
		for(int i = 0; i < N; i++){
			suggestion(root->children[i],word,str, index + 1);
		}
	}
	str.clear();
}

bool deleteWord(Tries *root, string word, int index){
	if(root == NULL){
		return false;
	}
	if(index == word.length() - 1){
		if(root->isTerminating){
			bool flag = false;
			for(int i = 0; i < N; i++){
				if(root->children[i] != NULL){
					flag = true;
					break;
				}
			}
			// there are someother words in the children
			if(flag == true){
				root->isTerminating = false;
				return false;
			}
			// delete the root;
			delete root;
			return true;
		}else{
			return false;
		}
	}
	bool result = deleteWord(root->children[word.at(index + 1) - 'a'], word, index + 1);
	if(result == true){
		root->children[word.at(index + 1) - 'a'] = NULL;
		bool flag = false;
		for(int i = 0; i < N; i++){
			if(root->children[i] != NULL){
				flag = true;
		       		break;
		       }
		}
		if(flag == true){
			return false;
		}
		// delete the root;
		delete root;
		return true;
	}
	return false;
}

int main(){
	Tries **root;
	root = new Tries*[N];
	for(int i = 0; i < N; i++){
		root[i] = NULL;
	}
	int option;
	string str;
	string strprint;
	Tries *searchRoot;
	bool deleteResult;
	cout<<"****************** Plese provide the string from a to z in lower case only *************************"<<endl;
	do{
		cout<<"\n\n1. Insert\n2. Search Exact\n3. Suggestions\n4. Delete(if word present )or otherwise it won't do anything\n5. Print all the words\n6. Exit\nEnter the option : "; 
		cin>>option;
		switch(option){
			case 1:
				cout<<"Enter the string : ";
				cin>>str;
				cout<<"\n     [Inserting.....]\n";
				insert(&root[str.at(0) - 'a'], str, 0);
				cout<<"\n     [Inserted]\n";
				break;
			case 2: cout<<"Enter the string : ";
				cin>>str;
				cout<<"\n       [Searching...]\n";
				searchRoot = searchExact(root[str.at(0) - 'a'], str, 0);
				
				if(searchRoot != NULL){
					cout<<"\n       [Match Found]"<<endl;
				}else{
					cout<<"\n       [Match Not Found]"<<endl;
				}

				break;
			case 3: cout<<"Enter the string : ";
				cin>>str;
				cout<<"\n\n+------------------------------------------+\n\n";	
				suggestion(root[str.at(0) - 'a'], str, strprint, 0);
				cout<<"\n+------------------------------------------+\n";	
				break;
			case 4: cout<<"Enter the string : " ;
				cin>>str;
				cout<<"\n    [Deleting...]\n";
				deleteResult = deleteWord(root[str.at(0) - 'a'], str, 0);
				// if there is only word in that chain
				if(deleteResult == true){
					root[str.at(0) - 'a'] = NULL;
				}
				cout<<"\n     [Deleted]\n";
				break;
			case 5: cout<<"\n\n+------------------------------------------+\n\n";	
				for(int i = 0; i < N; i++){
					printTries(root[i],strprint);	
				}
				cout<<"\n+------------------------------------------+\n";	
				break;
			case 6: cout<<"\n       Exiting......"<<endl;
				break;
			default:cout<<"Enter the correct option"<<endl;
				break;
		}
	}while(option != 6);
	return 0;
}
