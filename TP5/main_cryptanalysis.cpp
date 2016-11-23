#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <array>
#include <fstream>
#include <streambuf>

//#define DEBUG
#define IC_THRESHOLD 0.057

using namespace std;
 
typedef array<pair<char, double>, 26> FreqArray;

char decipher(char input, char key) {
	return (char) ('a' + ((input - key +26) %26));
}


class VigenereCryptanalysis
{
	private:
		array<double, 26> targets;
		array<double, 26> sortedTargets;
	 
	public:
		VigenereCryptanalysis(const array<double, 26>& targetFreqs) 
		{
			targets = targetFreqs;
			sortedTargets = targets;
			sort(sortedTargets.begin(), sortedTargets.end());
		}

		pair<string, string> analyze(string input) 
		{
			// init vars
			int key_size = 0;
			int sub_string_size, nb_letter;
			float IC = 0.0;
			vector<float> IC_avg_list;
			
			// while size of key is not suffisant to have a normal IC
			while(IC < IC_THRESHOLD) {
				key_size++;
				
				IC = 0;
				// compute IC for this key_size and for all start_ind
				for(int start_ind = 0 ; start_ind < key_size ; ++start_ind) {
					
					// for each letter in alphabet
					for(char c = 'a' ; c <= 'z' ; c=(char)c+1 ) {
						// reset string size & nb_letter
						sub_string_size = 0;
						nb_letter = 0;
						
						// count letter and size
						for(int ind = start_ind ; ind < input.length() ; ind = ind+key_size) {
							sub_string_size++;
							
							// if it's the good character, increment
							if(input[ind] == c) {
								//cout << "lettre trouvée"<< endl;
								nb_letter++;
							}
						}
						
						IC = IC + ((float)nb_letter*((float)nb_letter-1))/((float)sub_string_size*((float)sub_string_size-1));
					}
				
					#ifdef DEBUG
					cout << "IC for key lenght " << key_size << " start_ind " << start_ind << " - "  << IC << endl;
					#endif
				}
				// compute IC avg
				IC = IC / key_size;
			}
			
			#ifdef DEBUG
			cout << "Key lenght found - " << key_size << endl;
			#endif
			
			// reset vars
			nb_letter=0;
			double nb_expected, chi2, min_chi2;
			char char_found;
			string key = "";

			// -- Find the Key --
			// foreach letter of the key
			for(int start_ind = 0 ; start_ind<key_size ; ++start_ind) {
				min_chi2 = 100000000.0;
				
				// foreach letter of the alphabel
				for(char potential_letter_key = 'a' ; potential_letter_key <= 'z' ; potential_letter_key=(char)potential_letter_key+1 ) {
					// -- compute chi2 of potential letter --
					// reset vars
					chi2 = 0.0;
					
					// for each letter in alphabet
					for(char c = 'a' ; c <= 'z' ; c=(char)c+1 ) {
						// -- compute partial chi2			
						nb_letter=0;
						sub_string_size = 0;
					
						// count letter
						for(int ind = start_ind ; ind < input.length() ; ind = ind+key_size) {
							// count size of sub string
							sub_string_size++;
							
							// if it's the good character, increment
							if(decipher(input[ind], potential_letter_key) == c) {
								nb_letter++;
							}
						}
					
						// compute partial CHI2
						nb_expected = this->targets[c - 'a'] * sub_string_size;
						chi2 = chi2 + (nb_letter - nb_expected)*(nb_letter - nb_expected) / nb_expected;
					}
					
					#ifdef DEBUG
					cout << "potential letter key : " << potential_letter_key << " chi2 " << chi2 << endl;
					#endif
					
					// keep the minimal chi2 (and associated letter)
					if(chi2 < min_chi2){
						#ifdef DEBUG
						cout << "change mini letter by " << potential_letter_key << " with ki2=" << chi2 << " and miniki2=" << min_chi2 << endl;
						#endif
						
						min_chi2 = chi2;
						char_found = potential_letter_key;
					}
				}
				
				key+= char_found;
			}
			

			// decipher msg
			string result = "";
			char keyCharacter;
			for(unsigned int a = 0; a < input.length(); ++a) {
				// get key character 
				keyCharacter = (char) key[a % key.length()];
				result += decipher(input[a], keyCharacter);
			}

			return make_pair(result, key);
		}
};
 
int main()
{
	string file_name="ciphertext_3.txt";
	
	ifstream t(file_name);
	string input((istreambuf_iterator<char>(t)),istreambuf_iterator<char>());
	
	array<double, 26> english = {
	0.08167, 0.01492, 0.02782, 0.04253, 0.12702, 0.02228,
	0.02015, 0.06094, 0.06966, 0.00153, 0.00772, 0.04025,
	0.02406, 0.06749, 0.07507, 0.01929, 0.00095, 0.05987,
	0.06327, 0.09056, 0.02758, 0.00978, 0.02360, 0.00150,
	0.01974, 0.00074};

	array<double, 26> french = {
	0.0811,  0.0081,  0.0338,  0.0428,  0.1769,  0.0113,
	0.0119,  0.0074,  0.0724,  0.0018,  0.0002,  0.0599, 
	0.0229,  0.0768,  0.0520,  0.0292,  0.0083,  0.0643,
	0.0887,  0.0744,  0.0523,  0.0128,  0.0006,  0.0053,
	0.0026,  0.0012};

	/**/
	VigenereCryptanalysis vc_en(english);
	pair<string, string> output_en = vc_en.analyze(input);

	cout << "Key: "  << output_en.second << endl;
	cout << "Text: " << output_en.first << endl;
	/**/

	/**/
	VigenereCryptanalysis vc_fr(french);
	pair<string, string> output_fr = vc_fr.analyze(input);

	cout << "Key: "  << output_fr.second << endl;
	cout << "Text: " << output_fr.first << endl;
	/**/

}
