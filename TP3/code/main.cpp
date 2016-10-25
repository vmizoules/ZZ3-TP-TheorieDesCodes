#include <iostream>      
#include <fstream>
#include <string>         
#include <bitset>        
#include <vector>

/**
 * Hamming (7,4)
 **/ 
#define N 4
#define HAMMING_7 7

/**
 * Debug macros
 **/ 
#define DEBUG_RF true // Debug Information: Read File
#define DEBUG_HE true // Debug Information: Hamming Encoding

#define M1 inBuffer[3] // bit de poids faible - LSB (droite)
#define M2 inBuffer[2]
#define M3 inBuffer[1]
#define M4 inBuffer[0] // bit de poids fort - MSB (gauche)

using namespace std; 

/**
 * vector<bitset<N> > readFile(string filename)
 * Read a file in binary and create a vector of bitset wih a width of 4 for each bitset
 * Return a vector bitset
 **/ 
vector<bitset<N> > readFile(string filename)
{
	vector<bitset<N> > content;
	ifstream reader;
	char buffer;
	reader.open(filename.c_str(), ios::binary|ios::in);

	if(DEBUG_RF) {
		cout << "Read : \t";
	}

	if(reader != NULL && reader.is_open())
	{
		while(!reader.eof())
		{
			reader.read(&buffer, 1);
			bitset<N> bsBufferLSB(buffer);
			bitset<N> bsBufferMSB(buffer>>4);
			
			content.push_back(bsBufferMSB);
			content.push_back(bsBufferLSB);
	
			if(DEBUG_RF) {
				cout << " |" << bsBufferMSB.to_string();
				cout << " |" << bsBufferLSB.to_string();
			}
		}
	}
	
	if(DEBUG_RF) {
		cout << endl;
	}

	reader.close();
	return content;
} 

/**
 * vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
 * Convert a vector of bitset<4> into a hamming vector of bitset<7>
 **/ 
vector<bitset<HAMMING_7> > HammingEncoding(vector<bitset<N> > bitsetVector)
{
	vector<bitset<HAMMING_7> > encodedBitset;
	
	if(DEBUG_HE) {
		std::cout << "Encode : \t";
	}

	for(vector<bitset<N> >::iterator i = bitsetVector.begin(); i != bitsetVector.end();++i)
	{
		// Code to modify (sample)		
		bitset<N> inBuffer = *i;
		bitset<HAMMING_7> outBuffer;
		
		// c : codage haming / G : matrice génératrice / M : message
		// c = G(transposé) . M

		// M4 bit de poid fort (gauche), M1 bit de poid faible (droite)

		// XOR :
		// 0 ^ 0 = 0
		// 0 ^ 1 = 1
		// 1 ^ 0 = 1
		// 1 ^ 1 = 0

		// right bit (poids faible - LSB)
		outBuffer[0] = M1 ^ M2 ^ M4; //m1 + m2 + m4
		outBuffer[1] = M1 ^ M3 ^ M4;
		outBuffer[2] = M1;
		outBuffer[3] = M2 ^ M3 ^ M4 ;
		outBuffer[4] = M2;
		outBuffer[5] = M3; // m3 = inBuffer[1]
		outBuffer[6] = M4; // m4 = inBuffer[0]
		// left bit (poids fort - MSB)

		
		if(DEBUG_HE) {
			cout << " | " << outBuffer.to_string();
		}
		
		encodedBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HE) {
		cout << endl;
	}

	return encodedBitset;
}

vector<bitset<HAMMING_7> > & injectError(vector<bitset<HAMMING_7> > & bitsetVector)
{
	
	// TODO

	return bitsetVector;
}

vector<bitset<N> > HammingDecoding(vector<bitset<HAMMING_7> > & bitsetVector)
{
	vector<bitset<N> > decodedBitset;
	
	if(DEBUG_HE) {
		std::cout << "Decode : \t";
	}

	for(vector<bitset<HAMMING_7> >::iterator i = bitsetVector.begin(); i != bitsetVector.end();++i)
	{
		// Code to modify (sample)		
		bitset<HAMMING_7> inBuffer = *i;
		
		/*
		bitset<HAMMING_7> outBuffer;
		
		// c : codage haming / G : matrice génératrice / M : message
		// c = G(transposé) . M

		// M4 bit de poid fort (gauche), M1 bit de poid faible (droite)

		// XOR :
		// 0 ^ 0 = 0
		// 0 ^ 1 = 1
		// 1 ^ 0 = 1
		// 1 ^ 1 = 0

		// right bit (poids faible - LSB)
		outBuffer[0] = M1 ^ M2 ^ M4; //m1 + m2 + m4
		outBuffer[1] = M1 ^ M3 ^ M4;
		outBuffer[2] = M1;
		outBuffer[3] = M2 ^ M3 ^ M4 ;
		outBuffer[4] = M2;
		outBuffer[5] = M3; // m3 = inBuffer[1]
		outBuffer[6] = M4; // m4 = inBuffer[0]
		// left bit (poids fort - MSB)
*/
		
		if(DEBUG_HE) {
			cout << " | " << inBuffer.to_string();
		}
		
		//encodedBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HE) {
		cout << endl;
	}

	return decodedBitset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                     Main                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	vector< bitset<N> > input_data;
	vector< bitset<N> > decoded_data;
	vector< bitset<HAMMING_7> > encoded_data;
	vector< bitset<HAMMING_7> > modified_data;

	// Read data to encode
	input_data = readFile("test2.txt");

	// Encode by Hamming (7,4) coding
	encoded_data = HammingEncoding(input_data);

	// Inject error
	modified_data = injectError(encoded_data);

	// Decode
	decoded_data = HammingDecoding(modified_data);
}



