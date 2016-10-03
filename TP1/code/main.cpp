#include <vector>
#include <queue>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <map>
#include <stdexcept>

using namespace std;

class FileDetails {
	public:
		string name;
		int char_nb; // <equiv> size+1

        FileDetails():name(""), char_nb(-1) {}
        FileDetails(string newName):name(newName), char_nb(-1) {}
        FileDetails(string newName, int nb):name(newName), char_nb(nb) {}
};

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


// Recursive function to assign huffman code
void assignCodeOnChildren(Symbol* root)
{
	//if left child exists
	if(root->left != NULL) {
		// assign code on left child
		root->left->code = root->code + "0";
		
		// re-run on left child
		assignCodeOnChildren(root->left);
	}

	//if right child exists
	if(root->right != NULL) {
		// assign code on right child
		root->right->code = root->code + "1";
		
		// re-run on right child
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

// create alphabet (list of symbols/proba associated)
void CreateAlphabet(vector<Symbol*>& alphabet, bool useProba=true, FileDetails* fileToLoad=NULL)
{
    if(useProba) {

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
    } else {

        // Add all french letters in alphMap
        map<char,double> alphMap;
        alphMap.insert(pair<char,double>('a',0));
        alphMap.insert(pair<char,double>('b',0));
        alphMap.insert(pair<char,double>('c',0));
        alphMap.insert(pair<char,double>('d',0));
        alphMap.insert(pair<char,double>('e',0));
        alphMap.insert(pair<char,double>('f',0));
        alphMap.insert(pair<char,double>('g',0));
        alphMap.insert(pair<char,double>('h',0));
        alphMap.insert(pair<char,double>('i',0));
        alphMap.insert(pair<char,double>('j',0));
        alphMap.insert(pair<char,double>('k',0));
        alphMap.insert(pair<char,double>('l',0));
        alphMap.insert(pair<char,double>('m',0));
        alphMap.insert(pair<char,double>('n',0));
        alphMap.insert(pair<char,double>('o',0));
        alphMap.insert(pair<char,double>('p',0));
        alphMap.insert(pair<char,double>('q',0));
        alphMap.insert(pair<char,double>('r',0));
        alphMap.insert(pair<char,double>('s',0));
        alphMap.insert(pair<char,double>('t',0));
        alphMap.insert(pair<char,double>('u',0));
        alphMap.insert(pair<char,double>('v',0));
        alphMap.insert(pair<char,double>('w',0));
        alphMap.insert(pair<char,double>('x',0));
        alphMap.insert(pair<char,double>('y',0));
        alphMap.insert(pair<char,double>('z',0));

        // open file in read mode
        ifstream file(fileToLoad->name.c_str(), ios::in); 

        // Check file 
        if(file) {

            // Get file content
            string content;
            getline(file,content);
            fileToLoad->char_nb = content.size() + 1;

            // Counts the number of iteration by letter
            for (unsigned int i = 0; i < content.size(); i++){
                (*alphMap.find(content[i])).second = (*alphMap.find(content[i])).second + 1;
            }

            for (map<char,double>::iterator it=alphMap.begin(); it!=alphMap.end(); ++it){
                
                // Calculating the probability by letter
                it->second = (it->second*100)/(content.size()+1);

                // Add letter and its probability to the alphabet
                if (it->second!=0) {
                    // letter - string(1,it->first)
                    // probability - it->second;
                    alphabet.push_back(new Symbol( string(1,it->first) , it->second ));
                }
            }

            // close file
            file.close();  
        } else {
      		// can't open the file ? -> exception !
            throw runtime_error("Failed open file !");
        }
    }
} 

void DeleteNodeAndChildren(Symbol* root)
{
    //if left child exists
    if(root->left != NULL) {
        // re-run on left child
        DeleteNodeAndChildren(root->left);
    }

    //if right child exists
    if(root->right != NULL) {   
        // re-run on right child
        DeleteNodeAndChildren(root->right);
    }

    delete(root);
}

void DeleteMemory(Symbol* root)
{
    // Clear the memory
    DeleteNodeAndChildren(root);
}

void printExportedNode(Symbol * sym){
	
	cout <<  sym->name << " [shape=none, margin=0, label=<<TABLE><TR><TD>" << sym->name <<"</TD><TD>" << sym->freq << "</TD></TR><TR><TD COLSPAN=\"2\">" << sym->code << "</TD></TR></TABLE>>];" << endl;
}

void exportTreeToFile(FileDetails* fileToExport, vector<Symbol*>& alphabet, Symbol* root) {
	// Open the file in write mode (and delete old content)
	ofstream exportFile(fileToExport->name.c_str(), ios::out | ios::trunc);

	if(exportFile) {
		// init gv file
		exportFile << "digraph html {" << endl;
	
		// write all final leaf into file
		for(size_t i=0;i<alphabet.size();++i) {
			cout << alphabet[i]->name << endl;
		}
		
		// write all links
		//while()
		
		// end gv file
		exportFile << "}" << endl;
		
		// close file
		exportFile.close();
	} else {
		// can't open the file ? -> exception !
		throw runtime_error("Failed open file !");
	}
}

// MAIN
int main()
{
	// Init vars
	vector<Symbol*> alphabet;
	float antropie = 0.0;
	FileDetails * fileToLoad = new FileDetails("text.txt");
	FileDetails * fileToExport = new FileDetails("output.gv");

	// Compute the frequencies of the symbol
	CreateAlphabet(alphabet,false,fileToLoad);

	// Build the Huffman code tree 
	Symbol* root = CreateHuffmanCode(alphabet);

	// Display the result
	for(size_t i=0;i<alphabet.size();++i)
		cout << "Name : " << alphabet[i]->name << " ; Code : " << alphabet[i]->code << endl;
		
	// Export Display
	printExportedNode(root);

	// Antropie calcul
	for(size_t i=0;i<alphabet.size();++i) {
		antropie =
			antropie +
			(float)alphabet[i]->code.size() *
			(float)alphabet[i]->freq / (float)100
			;
	}
	cout << endl << "Entropy: " << antropie << endl;
	
	// [Rate] = [final volume] / [initial volume]
	float finalSize = antropie * fileToLoad->char_nb;
	float initialSize = 7 * fileToLoad->char_nb;
	// display
	cout << endl << "Applied to file '" << fileToLoad->name << "'" << endl;
    cout << "Initial size (in ASCII): " << initialSize << " bits" << endl;
    cout << "Final size (using Huffman): " << finalSize << " bits" << endl;
    cout << "Compression rate: " << finalSize/initialSize << endl;
    
    // export des données dans un fichier gv (pour graph dot)
    exportTreeToFile(fileToExport, alphabet, root);
    
	// Clear the memory
	DeleteMemory(root);
	
	// Exit
	return 0;
}

