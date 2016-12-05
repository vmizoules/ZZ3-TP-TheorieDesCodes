//
//  TP6_RSA
//  

#include <stdio.h>
#include <iostream>
#include <gmp.h>
#include <sstream>
#define MESSAGE_SIZE 10
#define BITSTRENGTH  14              /* size of modulus (n) in bits */
#define PRIMESIZE (BITSTRENGTH / 2)  /* size of the primes p and q  */

/* declare global variables */

mpz_t d,e,n;
mpz_t M,c;
gmp_randstate_t r_state;

// check parity of a mpz number
bool isEven(const mpz_t & num) {
	mpz_t r;
	mpz_init(r);
	bool ret = true;
	
	// compute modulo 2
	mpz_cdiv_r_ui(r, num, (unsigned int) 2);
	
	// check if rest is == 0 or not
	if(mpz_cmp_ui(r, 0)) {
		ret = false;
	}
	
	// clear
	mpz_clear(r);
	
	return ret;
}

// m ≡ g k mod p
// mpz_powm(c, M, e, n);	
// algorithm 3 Exponentiation by squaring : Require: three integers g, k and p ;
// compute m
void powm(mpz_t & m, const mpz_t & M, const mpz_t & e, const mpz_t & p ){
	// init vars
	bool kNull = false;
	mpz_t y,g,k;
	// keep vars M and e 
	mpz_init_set(g, M);
	mpz_init_set(k, e);

	#ifdef DEBUGPOWM
		std::cout << std::endl << " <<<<< POWM >>>>>" << std::endl;	
		char k_str[1000]; char g_str[1000]; char p_str[1000];char m_str[1000]; char y_str[1000];
	
		mpz_get_str(k_str,10,k);mpz_get_str(g_str,10,g);mpz_get_str(p_str,10,p);mpz_get_str(m_str,10,m);
	
		std::cout << " m:" << m_str << " g:" << g_str <<" k:" << k_str  << " p:" << p_str << std::endl;
	#endif 

	// if k < 0 then
	if(mpz_sgn(k)==-1){
		
		#ifdef DEBUGPOWM
			std::cout << "	Premier if Comparateur : " << mpz_cmp_ui(k, (unsigned long int) 0) << mpz_sgn(k) << std::endl;
		#endif
		
		// g <- 1/g
		mpz_invert(g, g, p); 
		// k <- -k
		mpz_neg(k,k);
		#ifdef DEBUGPOWM
			mpz_get_str(k_str,10,k);mpz_get_str(g_str,10,g);mpz_get_str(p_str,10,p);mpz_get_str(m_str,10,m);
			std::cout << "	Fin premier if m:" << m_str << " g:" << g_str <<" k:" << k_str  << " p:" << p_str << std::endl;
		#endif 
		
	}
	// if k = 0 then
	if(mpz_cmp_ui (k, (unsigned long int) 0) == 0){
		#ifdef DEBUGPOWM
			std::cout << "	Deuxieme if m:" << std::endl;
		#endif 
		
		
		// return m = 1
		mpz_set_ui (m, (unsigned long int) 1);	
		kNull = true;
		#ifdef DEBUGPOWM
			mpz_get_str(k_str,10,k);mpz_get_str(g_str,10,g);mpz_get_str(p_str,10,p);mpz_get_str(m_str,10,m);
			std::cout << "	Fin deuxieme if m:" << m_str << " g:" << g_str <<" k:" << k_str  << " p:" << p_str << std::endl;
		#endif
	}

	// y <- 1
	mpz_init_set_str(y, "1", 0);
	
	#ifdef DEBUGPOWM	
		mpz_get_str(k_str,10,k);mpz_get_str(g_str,10,g);mpz_get_str(p_str,10,p);mpz_get_str(m_str,10,m);mpz_get_str(y_str,10,y);
		std::cout << "Avant TQ if m:" << m_str << " g:" << g_str <<" k:" << k_str  << " p:" << p_str << " y:" << y_str << std::endl;
	#endif
	
	// while k > 1 do
	while ((mpz_cmp_ui(k, (unsigned long int) 1)>0)){
		
		#ifdef DEBUGPOWM
			std::cout << "	Dans TQ" << std::endl;
		#endif
		
		// if k is even then
		if(isEven(k)){
			#ifdef DEBUGPOWM
				std::cout << "		Troisieme if "<< std::endl;
			#endif
			// g ← g × g mod p
			mpz_mul (g, g, g); // g * g
			mpz_mod (g, g, p); // g mod p
			
			// k ← k/2
			mpz_cdiv_q_ui(k, k, (unsigned long int) 2); 
			#ifdef DEBUGPOWM
				mpz_get_str(k_str,10,k);mpz_get_str(g_str,10,g);mpz_get_str(p_str,10,p);mpz_get_str(m_str,10,m);mpz_get_str(y_str,10,y);
				std::cout << "		Fin if troisieme m:" << m_str << " g:" << g_str <<" k:" << k_str  << " p:" << p_str << " y:" << y_str << std::endl;
			#endif
		} else {
			#ifdef DEBUGPOWM
				std::cout << "		Dans Else "  << std::endl;
			#endif
			// y ←g×y
			mpz_mul (y, g, y);
			
			// g ←g×g
			mpz_mul (g, g, g);

			
			// k ← (k − 1)/2 
			mpz_sub_ui(k, k, (unsigned long int) 1); // k ← (k − 1)
			
			mpz_cdiv_q_ui (k, k, (unsigned long int) 2); // k ← k/2
			#ifdef DEBUGPOWM
				mpz_get_str(k_str,10,k);mpz_get_str(g_str,10,g);mpz_get_str(p_str,10,p);mpz_get_str(m_str,10,m);mpz_get_str(y_str,10,y);
				std::cout << "		Fin else m:" << m_str << " g:" << g_str <<" k:" << k_str  << " p:" << p_str << " y:" << y_str << std::endl;
			#endif
		}
	} 

	// return m = g × y
	if(!kNull){
		mpz_mul (m, g, y);
		mpz_mod (m, m, p); // m mod p
	}
	#ifdef DEBUGPOWM
		std::cout << "Fin de la fonction "  << std::endl;
		mpz_get_str(k_str,10,k);mpz_get_str(g_str,10,g);mpz_get_str(p_str,10,p);mpz_get_str(m_str,10,m);mpz_get_str(y_str,10,y);
		std::cout << "Fin if m:" << m_str << " g:" << g_str <<" k:" << k_str  << " p:" << p_str << " y:" << y_str << std::endl << std::endl;
	#endif	
}

// compute random message 
std::string getRandomMessage(int taille){
	// init vars
	std::string random_out_message; 
	std::ostringstream keep_the_rand_integer;
	int rand_integer; 
	// initialisation de rand
	srand(time(NULL));
	for (int i; i<taille;i++){
		// rand integer 0 to 9 
		rand_integer = 1+ (rand() % 9);
		// keep the rand integer in the out message  
		keep_the_rand_integer << rand_integer;
		random_out_message = keep_the_rand_integer.str();
	}
	return random_out_message;
} 

// compute random between min (include) and max (include)
void setToRandom(mpz_t & rand_Num, const unsigned int min, const unsigned int max) {

	// init vars 
	unsigned long rand_min_ui = min; // random between min (include)
	unsigned long rand_max_ui = max; // and max (include)
	mpz_t rand_max;
	mpz_init(rand_max);
	// compute randmax to make rand between 0 and rand_max (not included)
	mpz_init_set_ui(rand_max, (unsigned long)(rand_max_ui +1 -rand_min_ui ));
	
	// get random between 0 and rand_max-rand_min
	mpz_urandomm(rand_Num,r_state,rand_max);
	
	// add rand_min to have random between rand_min and rand_max
	mpz_add_ui(rand_Num, rand_Num, rand_min_ui);
	
	// for DEBUG
	//gmp_printf("%Zd\n", rand_Num);
	
	return;
}

/* Main subroutine */
int main()
{
	/* Init GMP random */
    unsigned long int seed = time(0);
    gmp_randinit_default (r_state);
    gmp_randseed_ui(r_state, seed);
	
    /* Initialize the GMP integers */
    mpz_init(d);
    mpz_init(e);
    mpz_init(n);
    
    mpz_init(M);
    mpz_init(c);
    
 
    /* This function creates the keys. The basic algorithm is...
     *
     *  1. Generate two large distinct primes p and q randomly
     *  2. Calculate n = pq and x = (p-1)(q-1)
     *  3. Select a random integer e (1<e<x) such that gcd(e,x) = 1
     *  4. Calculate the unique d such that ed = 1(mod x)
     *  5. Public key pair : (e,n), Private key pair : (d,n)
     *
     */
    
    
    
    
    /*
     *  Step 1 : Get two large primes.
     */
    mpz_t p,q;
    mpz_init(p);
    mpz_init(q);
    
	// generate random (BITSTRENGTH bits) for p
    mpz_urandomb(p, r_state, BITSTRENGTH);
    // make it prime
    mpz_nextprime(p, p);

    // generate random for q
    mpz_urandomb(q, r_state, BITSTRENGTH);
    // make it prime
    mpz_nextprime(q, q);

	// For testing -> Fix p & q
    /*mpz_init_set_str(p, "47", 0); 
    mpz_init_set_str(q, "71", 0);*/    
    
    // display p
    char p_str[1000];
    mpz_get_str(p_str,10,p);
    std::cout << "Random Prime 'p' = " << p_str <<  std::endl;
    // display q
    char q_str[1000];
    mpz_get_str(q_str,10,q);
    std::cout << "Random Prime 'q' = " << q_str <<  std::endl;
    
    
    
    
    
    /*
     *  Step 2 : Calculate n (=pq) ie the 1024 bit modulus
     *  and x (=(p-1)(q-1)).
     */
    char n_str[1000];
    mpz_t x;
    mpz_init(x);

    /* Calculate n... */
    mpz_mul(n,p,q);
    
    // display n
    mpz_get_str(n_str,10,n);
    std::cout << "\t n = " << n_str << std::endl;
    
    
    /* Calculate x(phi)*/
    // init vars
    mpz_t p_minus_1,q_minus_1;
    mpz_init(p_minus_1);
    mpz_init(q_minus_1);

	// do substraction
    mpz_sub_ui(p_minus_1,p,(unsigned long int)1);
    mpz_sub_ui(q_minus_1,q,(unsigned long int)1);
	// do multiplication
    mpz_mul(x,p_minus_1,q_minus_1);
    
    // display phi
    char phi_str[1000];
    mpz_get_str(phi_str,10,x);
    std::cout << "\t phi(n) = " << phi_str << std::endl;





    /*
     *  Step 3 : Get small odd integer e such that gcd(e,x) = 1.
     */
    // set random e between 2 and phi-1
    setToRandom(e, 2, mpz_get_ui(x)-1);
    // mpz_init_set_str(e, "79", 0);  
    // check if e is even
    if(isEven(e)) {
		// change the parity (+1)
		mpz_add_ui(e,e,(unsigned long int)1);
	}
	// check gcd
    mpz_t pgcd;
    mpz_init(pgcd);
    mpz_gcd(pgcd, e, x);
    // if pgcd is not equal to 1
    while(mpz_cmp_ui(pgcd, 1) != 0) {
		// do +2 and re-compute
		mpz_add_ui(e, e, (unsigned long int)2);
		
		mpz_gcd(pgcd, e, x);
	}

	// display pgcd
    char pgcd_str[1000];
    mpz_get_str(pgcd_str,10,pgcd);
    mpz_clear(pgcd);
    std::cout << "\t gcd(e,x) = " << pgcd_str << std::endl;
    // display e
    // DEBUG : mpz_init_set_str(e, "79", 0);
    char e_str[1000];
    mpz_get_str(e_str,10,e);
    std::cout << "\t e = " << e_str << std::endl;


    

    /*
     *  Step 4 : Calculate unique d such that ed = 1(mod x)
     */
    // compute invert of e mod x
    mpz_invert(d, e, x);
    
    // display d
    char d_str[1000];
    mpz_get_str(d_str,10,d);
    std::cout << "\t d = " << d_str << std::endl << std::endl;
    
    
    

    /*
     *  Step 5 : Print the public and private key pairs...
     */
    std::cout << "Public Keys  (e,n): ( " << e_str <<" , " << n_str << " )" << std::endl;
    std::cout << "Private Keys (d,n): ( " << d_str <<" , " << n_str << " )" << std::endl;




    /*
     *  Step 6 : Encrypt the message
     */

    // init vars
    mpz_t decrypt_block_mpz;
	mpz_init(decrypt_block_mpz);
	//std::string original_msg = "6882326879666683";
	
	// message random 
	std::string original_msg = "";
	original_msg = getRandomMessage(MESSAGE_SIZE);
	std::cout << "Message Random : " << original_msg << std::endl;
	
	std::string encrypted_msg;
	std::string decrypted_msg;
	int temp_msg_block_index = 0;
    char temp_msg_block_str[1000];
    char buffer[1000];
    
    // get n size 
    std::string n_s = (std::string)n_str;
    int block_size = (n_s.size()-1);
    
	// get message size 
    int message_size = original_msg.size();
    std::cout << "Message size :" << message_size << std::endl;
    
    // for each message blocks
    for(int i=0 ; i<message_size ; i = i+block_size) {
		// for each char in block
		for(int j=i ; j<i+block_size ; j++) {
			// copy char
			temp_msg_block_str[temp_msg_block_index] = (char)original_msg[j];
			// increment index
			temp_msg_block_index++;
			
			#ifdef DEBUG
				std::cout << "Je rentre j:"<< j << "i:"<< i ;
				std::cout <<"m:" << (char)original_msg[j] << "";
			#endif 
		}
		temp_msg_block_index = 0;
		
		// copy temp_msg_block_str to M
		mpz_init_set_str(M, temp_msg_block_str, 0);

		// encrypt with exponential function mpz_pown(mpz_t rop, mpz_t base, mpz_t exp, mpz_t mod)
		//mpz_powm(c, M, e, n);
		powm(c, M, e, n); 
		
		// save encrypted block
		mpz_get_str(buffer, 10, c);
		encrypted_msg += buffer;
		
		#ifdef DEBUG
			// display encrypted block
			char C_str[1000];
			mpz_get_str(C_str,10,c);
			std::cout << "\t Encrypted = " << C_str << std::endl << std::endl;
		#endif
		
		// decrypt block
		//mpz_powm(decrypt_block_mpz, c, d, n);
		powm(decrypt_block_mpz, c, d, n); 
	
		// save decrypted block
		mpz_get_str(buffer, 10, decrypt_block_mpz); 
		decrypted_msg += buffer;
		
		#ifdef DEBUG
			// display decrypted block
			char decrypt_message_str[1000];
			mpz_get_str(decrypt_message_str,10,decrypt_block_mpz);  
			std::cout << "\t Decrypted = " << decrypt_block_mpz << std::endl << std::endl;
		#endif		
	}

	// display original message
	std::cout << "Original message = " << original_msg << std::endl;

	// display encrypted message
	std::cout << "Encrypted message = " << encrypted_msg << std::endl;
	
	// display decrypted message
	std::cout << "Decrypted message = " << decrypted_msg << std::endl;


    /* Clean up the GMP integers */
    mpz_clear(p_minus_1);
    mpz_clear(q_minus_1);
    mpz_clear(x);
    mpz_clear(p);
    mpz_clear(q);
    
    mpz_clear(d);
    mpz_clear(e);
    mpz_clear(n);
    
    mpz_clear(M);
    mpz_clear(c);
    
    // clean rand state
    gmp_randclear(r_state);
}

