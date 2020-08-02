#include<iostream>
using namespace std;
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BYEL   "\x1B[01;33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

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
		cout<< YEL << str << RESET<<endl;
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
		cout<< YEL << str << RESET <<endl;
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
	cout<< RED "****************** Plese provide the string from a to z in lower case only *************************" RESET <<endl;
	do{
		cout<< "\n\n1. Insert\n2. Search Exact\n3. Suggestions\n4. Delete(if word present )or otherwise it won't do anything\n5. Print all the words\n6. Exit\n" BYEL "Enter the option : " ;
		cin>>option;
		cout<< RESET ;
		switch(option){
			case 1:
				cout<< BYEL "Enter the string : ";
				cin>>str;
				cout<< RESET ;
				cout<< YEL "\n     [Inserting.....]\n" RESET;
				insert(&root[str.at(0) - 'a'], str, 0);
				cout<<CYN "\n     [Inserted]\n" RESET;
				break;
			case 2: cout<< BYEL "Enter the string : ";
				cin>>str;
				cout<< RESET ;
				cout<<YEL "\n       [Searching...]RESET\n";
				searchRoot = searchExact(root[str.at(0) - 'a'], str, 0);
				
				if(searchRoot != NULL){
					cout<<GRN "\n       [Match Found]" RESET<<endl;
				}else{
					cout<<RED "\n       [Match Not Found]" RESET<<endl;
				}

				break;
			case 3: cout<< BYEL "Enter the string : ";
				cin>>str;
				cout<< RESET ;
				cout<< MAG "\n\n+------------------------------------------+\n\n" RESET;
				suggestion(root[str.at(0) - 'a'], str, strprint, 0);
				cout<< MAG "\n+------------------------------------------+\n" RESET;	
				break;
			case 4: cout<< BYEL "Enter the string : " ;
				cin>>str;
				cout<< RESET ;
				cout<< YEL "\n    [Deleting...]\n" RESET;
				deleteResult = deleteWord(root[str.at(0) - 'a'], str, 0);
				// if there is only word in that chain
				if(deleteResult == true){
					root[str.at(0) - 'a'] = NULL;
				}
				cout<< CYN " \n     [Deleted]\n" RESET;
				break;
			case 5: cout<< MAG "\n\n+------------------------------------------+\n\n" RESET;	
				for(int i = 0; i < N; i++){
					printTries(root[i],strprint);	
				}
				cout<< MAG "\n+------------------------------------------+\n" RESET;	
				break;
			case 6: cout<<YEL "\n       Exiting......" RESET<<endl;
				break;
			default:cout<<RED "Enter the correct option" RESET<<endl;
				break;
		}
	}while(option != 6);
	return 0;
}
