#include <iostream>       
#include <fstream>
#include <string>        
#include <vector>
#include <bitset>        
#include <cstdlib>
#include <ctime>

const int N=2;
const int K=1;
const int R=4;
const int NbMot = 12;

#define DEBUG

using namespace std; 

/* Class State */
class State {
	private :
		vector< bitset<K> > input; 
		int distance;
        int difference;
        bitset<R> state_name ;  
    
    public :
        State () {}
        State (vector< bitset<K> > in, int dist, int diff, bitset<R> name)
            : input(in),
            distance(dist),
            difference(diff),
            state_name(name)
            {}

};



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

//////////////////////////////////////////////////////////////////
// vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra) //
//                                                              //
//     Convolutional decoding of a message (GSM norm)           //
//////////////////////////////////////////////////////////////////

vector< bitset<K> > GSM_decode(vector< bitset<N> > mess_tra)
{
    // init vars
    vector< bitset<K> > mess_dec;
    vector< State > state_list;

    //list< vector > ???

    // initialize decoding process
    
    // first state
    State first_state =  State (vector< bitset<K> >(), 0, -1, bitset<R>(0));// state: 0000
    
    state_list.push_back(first_state);

 
    // deal with others states
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
    for(int i=0;i<NbMot;++i)
        mess.push_back(randBitset<K>());
    for(int i=0;i<R;++i)
        mess.push_back(bitset<K>(0));

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
