#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>

using namespace std;


// Leafs and nodes
class Symbol {
    public:
    	// nom
		string name;
		// frequence
		double freq;
		// codage
		string code;
		// est-ce une feuille ?
		bool leaf;
		// lien gauche / droit
		Symbol *left, *right;

		// constructeur par défaut
		Symbol(): name(""),freq(0),code(""),leaf(true), left(NULL), right(NULL){}
		// constructeur avec arguments
		Symbol(
			string newName,
			double newFreq,
			string newCode="",
			bool newLeaf=true,
			Symbol* newLeft=NULL,
			Symbol* newRight=NULL)
			:name(newName), freq(newFreq),code(newCode), leaf(newLeaf), left(newLeft), right(newRight)
			{}
	 
	 	// To Do: Code Here
	  	// Complete the class
};


class CompareSymbol {
	public:
		bool operator() (Symbol* symb1, Symbol* symb2) {
			// compare probability		
			bool ret = false;
			if(symb1->freq > symb2->freq) {
				ret = true;
			}
			// return result of comparaison
			return ret;
		}
};


// recursive function
void assignCodeOnChildren(Symbol* root){
	//if left child exists
	if(root->left != NULL) {
		// assign code on left child
		root->left->code = root->code + "0";
		
		// re-run on left child
		assignCodeOnChildren(root->left);
	}

	//if right child exists
	if(root->right != NULL) {
		// assign code on left child
		root->right->code = root->code + "1";
		
		// re-run on left child
		assignCodeOnChildren(root->right);
	}
}

// Create Huffman tree and return root
Symbol* CreateHuffmanCode(vector<Symbol*>& alphabet)
{
	Symbol* symb1;
	Symbol* symb2;
	Symbol* newSymb;
	
	// create a priority queue
	priority_queue<Symbol*, vector<Symbol*>, CompareSymbol> sortedAlphabet;
	
	// insert element into priority queue from vector
	for(vector<Symbol*>::iterator i = alphabet.begin(); i != alphabet.end() ; ++i ) {
		sortedAlphabet.push(*i);
	}
	
	// continue while there is symbols
	while(sortedAlphabet.size() != 1) {
	
		// lowest 1
		symb1 = sortedAlphabet.top();
		sortedAlphabet.pop();
		// lowest 2
		symb2 = sortedAlphabet.top();
		sortedAlphabet.pop();

		// create new symb
		newSymb = new Symbol(
			symb1->name + "" + symb2->name,
			symb1->freq + symb2->freq,
			"",
			false,
			symb1, //left
			symb2 // right
			);
			
		// insert it (it's a node)
		sortedAlphabet.push(newSymb);
	}
	
	// assign code on root
	sortedAlphabet.top()->code = "";
	assignCodeOnChildren( sortedAlphabet.top() );

	// return root
	return sortedAlphabet.top();
}


// crée un alphabet (liste des symboles/proba associé)
void CreateAlphabet(vector<Symbol*>& alphabet, bool Proba=true)
{
	if(Proba)
	{
		// Probability of french letters
		alphabet.push_back(new Symbol("a",8.11));
		alphabet.push_back(new Symbol("b",0.81));
		alphabet.push_back(new Symbol("c",3.38));
		alphabet.push_back(new Symbol("d",4.28));
		alphabet.push_back(new Symbol("e",17.69));
		alphabet.push_back(new Symbol("f",1.13));
		alphabet.push_back(new Symbol("g",1.19));
		alphabet.push_back(new Symbol("h",0.74));
		alphabet.push_back(new Symbol("i",7.24));
		alphabet.push_back(new Symbol("j",0.18));
		alphabet.push_back(new Symbol("k",0.02));
		alphabet.push_back(new Symbol("l",5.99));
		alphabet.push_back(new Symbol("m",2.29));
		alphabet.push_back(new Symbol("n",7.68));
		alphabet.push_back(new Symbol("o",5.2));
		alphabet.push_back(new Symbol("p",2.92));
		alphabet.push_back(new Symbol("q",0.83));
		alphabet.push_back(new Symbol("r",6.43));
		alphabet.push_back(new Symbol("s",8.87));
		alphabet.push_back(new Symbol("t",7.44));
		alphabet.push_back(new Symbol("u",5.23));
		alphabet.push_back(new Symbol("v",1.28));
		alphabet.push_back(new Symbol("w",0.06));
		alphabet.push_back(new Symbol("x",0.53));
		alphabet.push_back(new Symbol("y",0.26));
		alphabet.push_back(new Symbol("z",0.12));
	}
	else
	{

		// To Do: Code Here
		// 1/ Take the probabilty of letters in french langage (default code)
		// 2/ Compute the probabilty by the text to encode (text.txt)

	}
} 

void DeleteMemory(vector<Symbol*>& alphabet, Symbol* root)
{
  // To Do: Code Here
  // Clear the memory	
}

// MAIN
int main()
{
	// Init vars
	vector<Symbol*> alphabet;

	// Compute the frequencies of the symbol
	CreateAlphabet(alphabet);

	// Build the Huffman code tree 
	Symbol* root = CreateHuffmanCode(alphabet);

	// Display the result
	for(size_t i=0;i<alphabet.size();++i)
		cout << "Name : " << alphabet[i]->name << " ; Code : " << alphabet[i]->code << endl;

	// Clear the memory
	DeleteMemory(alphabet,root);
	
	// Exit
	return 0;
}

