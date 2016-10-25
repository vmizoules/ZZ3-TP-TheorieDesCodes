#include <iostream>      
#include <fstream>
#include <string>         
#include <bitset>        
#include <vector>
#include <stdlib.h>
#include <time.h> 

/**
 * Debug macros
 **/ 
#define DEBUG_RF true // Debug Information: Read File
#define DEBUG_HE true // Debug Information: Hamming Encoding
#define DEBUG_HD true // Debug Information: Hamming Decoding
#define DEBUG_EI true // Debug Information: Error Injection
#define DEBUG_CD true // Debug Information: Code Distance

/**
 * Hamming (7,4)
 **/ 
#define HAMMING_BEFORE 4
#define HAMMING_AFTER 7

/**
 * Define max error to inject per bitset
 **/ 
#define MAX_ERROR_PER_BITSET 1

/**
 * Helper
 **/ 
#define M1 inBuffer[0] // bit de poids faible - LSB (droite)
#define M2 inBuffer[1]
#define M3 inBuffer[2]
#define M4 inBuffer[3] // bit de poids fort - MSB (gauche)

#define C1 inBuffer[0] // bit de poids faible - LSB (droite)
#define C2 inBuffer[1]
#define C3 inBuffer[2]
#define C4 inBuffer[3]
#define C5 inBuffer[4]
#define C6 inBuffer[5]
#define C7 inBuffer[6] // bit de poids fort - MSB (gauche)

using namespace std;

bool randInitialized = false;

/* ----------------- getRand ----------------- */
int getRand(int min, int max) {
	if(! randInitialized) {
		srand (time(NULL));
		randInitialized = true;
	}

	return rand() % (max+1) + min;
}
/* ----------------- End getRand ----------------- */

/* ----------------- readFile ----------------- */
/**
 * vector<bitset<HAMMING_BEFORE> > readFile(string filename)
 * Read a file in binary and create a vector of bitset wih a width of 4 for each bitset
 * Return a vector bitset
 **/ 
vector<bitset<HAMMING_BEFORE> > readFile(string filename)
{
	vector<bitset<HAMMING_BEFORE> > content;
	ifstream reader;
	char buffer;
	reader.open(filename.c_str(), ios::binary|ios::in);

	if(DEBUG_RF) {
		cout << "Read :" << endl;
	}

	if(reader != NULL && reader.is_open())
	{
		while(!reader.eof())
		{
			reader.read(&buffer, 1);
			bitset<HAMMING_BEFORE> bsBufferLSB(buffer);
			bitset<HAMMING_BEFORE> bsBufferMSB(buffer>>4);
			
			content.push_back(bsBufferMSB);
			content.push_back(bsBufferLSB);
	
			if(DEBUG_RF) {
				cout << " | " << bsBufferMSB.to_string();
				cout << " | " << bsBufferLSB.to_string();
			}
		}
	}
	
	if(DEBUG_RF) {
		cout << endl;
	}

	reader.close();
	return content;
}
/* ----------------- End readFile ----------------- */

/* ----------------- bitsetHammingEncoding ----------------- */
bitset<HAMMING_AFTER> bitsetHammingEncoding(const bitset<HAMMING_BEFORE> & inBuffer) {
	bitset<HAMMING_AFTER> outBuffer;

	// c : codage haming / G : matrice génératrice / M : message
	// c = G(transposé) . M

	// XOR :
	// 0 ^ 0 = 0
	// 0 ^ 1 = 1
	// 1 ^ 0 = 1
	// 1 ^ 1 = 0
	
	// transfert bit to outBuffer
	// right bit (poids faible - LSB)
	outBuffer[0] = M1 ^ M2 ^ M4; //m1 + m2 + m4
	outBuffer[1] = M1 ^ M3 ^ M4;
	outBuffer[2] = M1;
	outBuffer[3] = M2 ^ M3 ^ M4 ;
	outBuffer[4] = M2;
	outBuffer[5] = M3; // m3 = inBuffer[1]
	outBuffer[6] = M4; // m4 = inBuffer[0]
	// left bit (poids fort - MSB)

	return outBuffer;
}
/* ----------------- End bitsetHammingEncoding ----------------- */

/* ----------------- HammingEncoding ----------------- */
/**
 * vector<bitset<HAMMING_AFTER> > HammingEncoding(vector<bitset<HAMMING_BEFORE> > bitsetVector)
 * Convert a vector of bitset<4> into a hamming vector of bitset<7>
 **/ 
vector<bitset<HAMMING_AFTER> > HammingEncoding(vector<bitset<HAMMING_BEFORE> > bitsetVector)
{
	vector<bitset<HAMMING_AFTER> > encodedVectorBitset;
	
	if(DEBUG_HE) {
		std::cout << "Encode :" << endl;
	}

	for(vector<bitset<HAMMING_BEFORE> >::iterator i = bitsetVector.begin(); i != bitsetVector.end();++i)
	{
		// get bitset
		bitset<HAMMING_BEFORE> inBuffer = *i;
		// encode
		bitset<HAMMING_AFTER> outBuffer = bitsetHammingEncoding(inBuffer);
		// add sample to encodedVectorBitset
		encodedVectorBitset.push_back(outBuffer);

		// If debug, display result
		if(DEBUG_HE) {
			cout << " | " << outBuffer.to_string();
		}
	}
	
	if(DEBUG_HE) {
		cout << endl;
	}

	return encodedVectorBitset;
}
/* ----------------- End HammingEncoding ----------------- */

/* ----------------- injectError ----------------- */
vector<bitset<HAMMING_AFTER> > injectError(vector<bitset<HAMMING_AFTER> > & input)
{
	// init vars
	int pos;
	vector<bitset<HAMMING_AFTER> > output;
	
	if(DEBUG_EI) {
		cout << endl << "-- Starting injection --" << endl;
	}
	
	// for each bitset in the vector
	for(vector<bitset<HAMMING_AFTER> >::iterator i = input.begin(); i != input.end();++i) {
		// get sample
		bitset<HAMMING_AFTER> buffer = *i;

		// there is between 0 and MAX_ERROR_PER_BITSET in a bitset
		for(int i = 0 ; i < getRand(0,MAX_ERROR_PER_BITSET) ; ++i) {
			// compute position
			pos = getRand(0, HAMMING_AFTER-1);
			// inject error in position pos
			buffer[pos] = buffer[pos] ^ 1;
			
			if(DEBUG_EI) {
				cout << "Injecting error in position : " << pos+1 << endl;
			}
		}
		
		// add modified sample to output
		output.push_back(buffer);
	}
	
	if(DEBUG_EI) {
		cout << "After injection :" << endl;
	
		for(vector<bitset<HAMMING_AFTER> >::iterator i = output.begin(); i != output.end();++i) {
			bitset<HAMMING_AFTER> buff = *i;
			cout << " | " << buff.to_string();
		}
	
		cout << endl << "-- End injection --" << endl;
	}
	
	return output;
}
/* ----------------- End injectError ----------------- */

/* ----------------- verifyBitset ----------------- */
int verifyBitset(bitset<HAMMING_AFTER> inBuffer)
{
	bitset<3> position;
	int pos = 0;
	
	// left bit (poids fort - MSB)
	position[2] = C4 ^ C5 ^ C6 ^ C7;
	position[1] = C2 ^ C3 ^ C6 ^ C7;
	position[0] = C1 ^ C3 ^ C5 ^ C7;
	// right bit (poids faible - LSB)

	// convert bitset to int
	pos = (int) (position.to_ulong());

	return pos;
}
/* ----------------- End verifyBitset ----------------- */

/* ----------------- bitsetHammingDecoding ----------------- */
bitset<HAMMING_BEFORE> bitsetHammingDecoding(bitset<HAMMING_AFTER> & inBuffer) {
	bitset<HAMMING_BEFORE> outBuffer;

	// verify syndrom
	int errorPosition = verifyBitset(inBuffer);
	if(errorPosition != 0) {
		// there is an error, let's deal with
		inBuffer[errorPosition-1] = inBuffer[errorPosition-1] ^ 1; // inverse symbol
		cout << "Error corrected in position : " << errorPosition << endl;
	}
	
	// transfert bit to outBuffer		
	// right bit (poids faible - LSB)
	outBuffer[0] = C3 ; // m1 = c3
	outBuffer[1] = C5 ; // m2 = c5
	outBuffer[2] = C6 ; // m3 = c6 = outBuffer[5]
	outBuffer[3] = C7 ; // m4 = c7 = outBuffer[6]
	// left bit (poids fort - MSB)

	return outBuffer;
}
/* ----------------- End bitsetHammingDecoding ----------------- */

/* ----------------- HammingDecoding ----------------- */
vector<bitset<HAMMING_BEFORE> > HammingDecoding(vector<bitset<HAMMING_AFTER> > & bitsetVector)
{
	vector<bitset<HAMMING_BEFORE> > decodedVectorBitset;
	
	if(DEBUG_HD) {
		cout << endl << "-- Starting decoding --" << endl;
	}

	for(vector<bitset<HAMMING_AFTER> >::iterator i = bitsetVector.begin(); i != bitsetVector.end();++i)
	{
		// get bitset
		bitset<HAMMING_AFTER> inBuffer = *i;
		// decode
		bitset<HAMMING_BEFORE> outBuffer = bitsetHammingDecoding(inBuffer);
		// add sample to decodedVectorBitset
		decodedVectorBitset.push_back(outBuffer);
	}
	
	if(DEBUG_HD) {
		cout << "After decoding :" << endl;
	
		for(vector<bitset<HAMMING_BEFORE> >::iterator i = decodedVectorBitset.begin(); i != decodedVectorBitset.end();++i) {
			bitset<HAMMING_BEFORE> buff = *i;
			cout << " | " << buff.to_string();
		}
	
		cout << endl << "-- End decoding --" << endl;
	}

	return decodedVectorBitset;
}
/* ----------------- End HammingDecoding ----------------- */

/* ----------------- getHammingDistBetweenInt ----------------- */
int getHammingDistBetweenInt(int a, int b) {
	// compute Hamming distance between 2 words (converted in int)
	int distance = 0;
	char valeur = a^b;
	while(valeur) {
		++distance; 
		valeur &= valeur - 1;
	}
	return distance;
}
/* ----------------- End getHammingDistBetweenInt ----------------- */

/* ----------------- getGlobalHammingDistance ----------------- */
int getGlobalHammingDistance() {
	// search minimal distance in all words possible

	// var
	int min, max, actual, next, dist, distanceMin = 255;
	bitset<HAMMING_BEFORE> mybitset;
	bitset<HAMMING_AFTER> actualBitset, nextBitset;

	// compute min & max
	min = 0; // default
	// set all bits to 1 in mybitset
	mybitset.set();
	// get value
	max = (int) mybitset.to_ulong();

	if(DEBUG_CD) {
		cout << endl << "-- Starting computing hamming distance --" << endl;
	}

	// for left between [0-14]
	for(int i=min ; i<max ; ++i) {
		// get hamming code for i
		actualBitset = bitsetHammingEncoding(bitset<HAMMING_BEFORE>(i));
		actual = (int) bitsetHammingEncoding(bitset<HAMMING_BEFORE>(i)).to_ulong();
		
		// for right between [i-15]
		for(int j=i+1 ; j<(max+1) ; ++j) {
			// hamming code for j
			nextBitset = bitsetHammingEncoding(bitset<HAMMING_BEFORE>(j));
			next = (int) bitsetHammingEncoding(bitset<HAMMING_BEFORE>(j)).to_ulong();
			
			// compute distance between actual and next
			dist = getHammingDistBetweenInt(actual, next);
			if(dist < distanceMin) {
				distanceMin = dist;
			}
			
			if(DEBUG_CD) {
				cout << actualBitset.to_string() << " vs " << nextBitset.to_string() << " | dist -> " << dist << endl;
			}
		}

	}
	
	if(DEBUG_CD) {
		cout << "-- End compute hamming distance --" << endl;
	}

	
	return distanceMin;
}
/* ----------------- End getGlobalHammingDistance ----------------- */

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                     Main                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
	vector< bitset<HAMMING_BEFORE> > input_data;
	vector< bitset<HAMMING_BEFORE> > decoded_data;
	vector< bitset<HAMMING_AFTER> > encoded_data;
	vector< bitset<HAMMING_AFTER> > modified_data;
	int distance;

	// Read data to encode
	input_data = readFile("input2.txt");

	// Encode by Hamming (7,4) coding
	encoded_data = HammingEncoding(input_data);

	// Inject error
	modified_data = injectError(encoded_data);

	// Decode
	decoded_data = HammingDecoding(modified_data);
	
	// Hamming Distance
	distance = getGlobalHammingDistance();
	cout << endl << "Distance of Hamming code (" << HAMMING_AFTER << "," << HAMMING_BEFORE <<  ") : " << distance << endl;
}



