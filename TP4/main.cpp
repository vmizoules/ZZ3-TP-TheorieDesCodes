#include <iostream>       
#include <fstream>
#include <string>        
#include <vector>
#include <map>
#include <exception>
#include <bitset>        
#include <cstdlib>
#include <ctime>

const int N=2;
const int K=1;
const int R=4;
const int NbMot = 3;

#define DEBUG

using namespace std;


////////////////////////////////////////////////////////////
//      template<int bits> bitset<bits> randBitset()      //
//                                                        //
//               Generate random bitset                   //
////////////////////////////////////////////////////////////

template<int bits> bitset<bits> randBitset()
{ 
    bitset<bits> r(rand());
    for(int i = 0; i < bits/16 - 1; i++) {
        r <<= 16;
        r |= bitset<bits>(rand()); 
    }
    return r;
} 

////////////////////////////////////////////////////////////
// vector< bitset<N> > GSM_code(vector< bitset<K> > mess) //
//                                                        //
//     Convolutional coding of a message (GSM norm)       //
////////////////////////////////////////////////////////////

vector< bitset<N> > GSM_code(vector< bitset<K> > mess)
{
    int i=0, g0, g1;
    vector< bitset<N> > mess_out;

    bitset<N> cod_out; 
    bitset<R+1> G0(25);
    bitset<R+1> G1(27); 
    bitset<R+1> reg; 
    reg.reset();

    #ifdef DEBUG
    cout << "-------------------- Debug Informations (Coding) --------------------" << endl << endl;
    cout << "Initial register ( u(i-4)  u(i-3)  u(i-2)  u(i-1)  u(i)  ): " << reg << endl;
    cout << "Polynom G0       ( g0(i-4) g0(i-3) g0(i-2) g0(i-1) g0(i) ): " << G0 << endl;
    cout << "Polynom G1       ( g1(i-4) g1(i-3) g1(i-2) g1(i-1) g1(i) ): " << G1 << endl << endl;
    #endif

    for (vector<bitset<K> >::iterator it = mess.begin(); it != mess.end(); ++it) {
        reg = reg<<1; // register shift
        reg.set(0,(*it).count());

        g0 = (reg&G0).count()%2; // modulo 2 addition
        g1 = (reg&G1).count()%2;

        cod_out.reset();
        cod_out.set(0,g0);
        cod_out.set(1,g1);

        mess_out.push_back(cod_out);

        #ifdef DEBUG
        cout << "Block number: " << ++i << " - In frame: "<< *it << endl; 
        cout << "\t Current status of registers: "<< reg << endl;
        cout << "\t Out : " << cod_out << endl;
        #endif
    }
    #ifdef DEBUG
    cout << "------------------------------------------------------------------" << endl << endl;
    #endif

    return mess_out;
}

/////////////////////////////////////////////////////////////////////////
// vector< bitset<N> >  GSM_transmission(vector< bitset<N> > mess_cod) //
//                                                                     //
//         Simulation of a transmission channel => adding errors       //
/////////////////////////////////////////////////////////////////////////

vector< bitset<N> >  GSM_transmission(vector< bitset<N> > mess_cod)
{
    vector< bitset<N> > mess_tra = mess_cod;

    //TODO: Code here

    return mess_tra;
}

/////////////////////////////////////////////////////////////////////////
//                                                                     //
// 						        Other		       					   //
//                                                                     //
/////////////////////////////////////////////////////////////////////////

/* Class State */
class State {
	private :
		vector< bitset<K> > input; 
		int distance;
        int difference;
        bitset<R> state_name ;
        bool is_new;
    
    public :
        State () {}
        State (vector< bitset<K> > in, int dist, int diff, bitset<R> name)
            : input(in),
            distance(dist),
            difference(diff),
            state_name(name),
            is_new(true)
            {}
        // getter / setter
        bitset<R> getStateName() const {return state_name;}
        vector< bitset<K> > getInput() const {return input;}
        void addInputValue(int value) {
			input.push_back(bitset<K>(value));
		}
		bool isNew() const { return is_new; }
		bool isNotNew() const { return ! is_new; }
		void setNotNew() {is_new = false;}

		// other
		bitset<N> getResultByEntry(int entry) {
			bitset<R+1> G0(25);
			bitset<R+1> G1(27); 
			bitset<R+1> reg;
			
			reg = bitset<R+1>(state_name.to_ulong());
			reg = reg<<1; // move to left
			reg.set(0, entry); // entry at right (pos 0)
			
			int g0 = (reg&G0).count()%2; // modulo 2 addition
			int g1 = (reg&G1).count()%2;
			
			bitset<N> out;
			out.set(0,g0);
			out.set(1,g1);
			
			return out;
		}
		bitset<R> getNextStateNameByEntry(int entry) {
			bitset<R> temp_state_name;
			temp_state_name = getStateName();
			temp_state_name = temp_state_name<<1;
			temp_state_name.set(0, entry); // set to ENTRY(0 or 1) the first bit
			
			return temp_state_name;
		}
		
		void display() {
			cout << "[" << state_name << "] - input=";
			
			for (vector<bitset<K> >::iterator it = input.begin() ; it != input.end(); ++it) {
				cout << *it;
			}
			
			cout << " dst=" << distance << " - diff=" << difference <<  endl;
		}
		
};

void insertStateInList(map< unsigned long, State > & state_list, State new_state) {
	state_list.insert(pair< unsigned long , State >(new_state.getStateName().to_ulong(), new_state));
}

void computeNewState(map< unsigned long, State > & state_list, State * actual_state, bitset<N> actual_encoded_entry, int entry)
{
	// compute next state name
	bitset<R> temp_state_name = actual_state->getNextStateNameByEntry(entry);
	
	// check if futur state already exists
	if( state_list.count(temp_state_name.to_ulong()) == 1 ) {
		// TODO competition entre les distances et si = lancer piece...
		cout << "			CAS A TRAITER - " << temp_state_name << endl;
	} else {
		// create new state

		// copy input
		vector< bitset<K> > temp_input = vector< bitset<K> >(actual_state->getInput());
		
		// compute potential output
		bitset<N> potential_output = actual_state->getResultByEntry(entry);
		
		// count distance between potential output and actual mess_tra
		int difference = (potential_output^actual_encoded_entry).count();
		
		State temp_state = State(temp_input, 0, difference, temp_state_name);
		// add entry in input
		temp_state.addInputValue(entry);
		
		// insert in the list
		insertStateInList(state_list, temp_state);
	}

}

//////////////////////////////////////////////////////////////////
// vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra) //
//                                                              //
//     Convolutional decoding of a message (GSM norm)           //
//////////////////////////////////////////////////////////////////

vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra)
{
    // -- init vars
    vector< bitset<K> > mess_dec;
    map< unsigned long, State > state_list;
    
    
    // -- initialize decoding process
    // first state
    
    State first_state =  State (vector< bitset<K> >(), 0, -1, bitset<R>(0));// input:"" - dist:0 - diff:-1 - statename:0000
    insertStateInList(state_list, first_state); // add first state in list
	// actual state
    State * actual_state;
    
    // for each group of 2 bits of mess_tra
    for(vector< bitset<N> >::iterator it = mess_tra.begin() ; it != mess_tra.end(); ++it) {
		
		cout << "----group 2 bits ----" << endl;
		
		map< unsigned long, State >::iterator last_elt = state_list.end();
		
		// for each existing states
		for (map< unsigned long, State >::iterator actual_state_it = state_list.begin() ; actual_state_it != state_list.end(); ++actual_state_it) {
			cout << "	";
			cout << "----1 state ----" << endl;
			
			actual_state = & actual_state_it->second;
			
			if(actual_state->isNotNew()) {
				// compute new if input bit was 0
				computeNewState(state_list, actual_state, *it, 0);
				// compute new if input bit was 1 
				computeNewState(state_list, actual_state, *it, 1);
			}
			
			cout << "		";
			actual_state->display();
			cout << "	";
			cout << "----end 1 state ----" << endl;
			
		}
		
		// for each state
        for (map< unsigned long, State >::iterator actual_state_it = state_list.begin() ; actual_state_it != state_list.end(); ++actual_state_it) {
			actual_state = & actual_state_it->second;
			// update distance
			
			// set to normal
			actual_state->setNotNew();
			
		}
		
		cout << "----end group 2 bits ----" << endl;
        
        
        //actual_state->getStateName()<<1

        //actual_state->input.push_back(bitset<K>(1));
        


        // if input bit was 0
        // actual_state->input.push_back(bitset<K>(0));
        
       
        
    }

    // ...
    //TODO: Code here
    // ..
   
    // end -> it stays only 1 state    
    // read input attribut
    
    




    /////////// TO DELETE AND MODIFY ///////////
    for(unsigned int i=0;i<mess_tra.size();++i) {
        mess_dec.push_back(randBitset<K>());
    }
    ////////////////////////////////////////////

    return mess_dec;
}

//////////////////////////////////////////////////////////////////
//                             MAIN                             //
//////////////////////////////////////////////////////////////////

int main()
{
    vector< bitset<K> > mess;
    vector< bitset<N> > mess_cod;
    vector< bitset<N> > mess_tra;
    vector< bitset<K> > mess_dec;

    // Random initialization message
    srand( (unsigned)time( NULL ) );
    for(int i=0;i<NbMot;++i) {
        mess.push_back(randBitset<K>());
    }
    for(int i=0;i<R;++i) {
        mess.push_back(bitset<K>(0));
    }

    // Coding of the message => mess_cod
    mess_cod = GSM_code(mess);

    // Simulation of a transmission (errors) => mess_tra
    mess_tra = GSM_transmission(mess_cod);

    // Decoding of the transmitted message => mess_dec
    mess_dec = GSM_decode(mess_tra);

    cout << "Source Message   : ";
    for (vector<bitset<K> >::iterator it = mess.begin() ; it != mess.end(); ++it) {
        cout << ' ' << *it;
    }
    cout << '\n';

    cout << "Coded Message    : ";
    for (vector<bitset<N> >::iterator it = mess_cod.begin() ; it != mess_cod.end(); ++it) {
        cout << ' ' << *it;
    }
    cout << '\n';

    cout << "Received Message : ";
    for (vector<bitset<N> >::iterator it = mess_tra.begin() ; it != mess_tra.end(); ++it) {
        cout << ' ' << *it;
    }
    cout << '\n';

    cout << "Decoded Message  : ";
    for (vector<bitset<K> >::iterator it = mess_dec.begin() ; it != mess_dec.end(); ++it) {
        cout << ' ' << *it;
    }
    cout << '\n';
}

