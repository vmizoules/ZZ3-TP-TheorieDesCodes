#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "CImg.h"

#define ROUND( a ) ( ( (a) < 0 ) ? (int) ( (a) - 0.5 ) : (int) ( (a) + 0.5 ) )

using namespace std;
using namespace cimg_library;

/*--------------- DCT ----------------*/
CImg<double> applyDctOnBlock(CImg<unsigned char> block){
    // init vars
    int step = block.width(); // step = width or height of block
    double Ci,Cj;
    double total, sub_total, sub_sub_total;
    CImg<double> dctBlock(step,step,1,1,0);

    // for each pixel in bloc in width    
    for(int i=0 ; i<step ; ++i) {
        // for each pixel in bloc in hight
        for(int j=0 ; j<step ; ++j) {

            // fix Ci and Cj values
            Ci = 1;
            if(i==0) {
                Ci = M_SQRT1_2; // 1/sqrt(2)
            }
            Cj = 1;
            if(j==0) {
                Cj = M_SQRT1_2;
            }
            
            // run calcul
            sub_total = 0.0;
            for(int x=0 ; x<step ; ++x) {
                for(int y=0 ; y<step ; ++y) {
                    // calcul multiplication
                    sub_sub_total = block(x,y);
                    sub_sub_total *= cos(((2.0*x+1.0)*i*M_PI)/(2.0*step));
                    sub_sub_total *= cos(((2.0*y+1.0)*j*M_PI)/(2.0*step));

                    // get sum
                    sub_total += sub_sub_total;
                }
            }

            total = (2.0/step) * Ci * Cj * sub_total;

            // affect value to pixel in DCT block
            dctBlock(i,j) = total;
        }
    }

    return dctBlock;
}
/*--------------- /DCT ----------------*/

/*--------------- DCT-1 ----------------*/
CImg<unsigned char> applyInvertedDctOnDCTBlock(CImg<double> dctBlock){
    // init vars
    int step = dctBlock.width(); // step = width or height of block
    double Ci,Cj;
    double total, sub_total, sub_sub_total;
    CImg<unsigned char> block(step,step,1,1,0);

    // for each pixel in dctbloc in width    
    for(int x=0 ; x<step ; ++x) {
        // for each pixel in dctbloc in hight
        for(int y=0 ; y<step ; ++y) {

            // run calcul
            sub_total = 0.0;
            for(int i=0 ; i<step ; ++i) {
                for(int j=0 ; j<step ; ++j) {

                    // fix Ci and Cj values
                    Ci = 1;
                    if(i==0) {
                        Ci = M_SQRT1_2; // 1/sqrt(2)
                    }
                    Cj = 1;
                    if(j==0) {
                        Cj = M_SQRT1_2;
                    }
            
                    // calcul multiplication
                    sub_sub_total = (2./step) * Ci * Cj * dctBlock(i,j);
                    sub_sub_total *= cos(((2.0*x+1.0)*i*M_PI)/(2.0*step));
                    sub_sub_total *= cos(((2.0*y+1.0)*j*M_PI)/(2.0*step));
                    // get sum
                    sub_total += sub_sub_total;
                }
            }
            total =  sub_total;
            
            // affect value to pixel in DCT block
            block(x,y) = (unsigned char)total;
        }
    }

    return block;
}
/*--------------- /DCT-1 ----------------*/

/*--------------- JPEGENCODER ----------------*/
CImg<double> JPEGEncoder(CImg<unsigned char> image, const CImg<> & Q)
{
	// init vars
    CImg<double> compressed_image(image.width(),image.height(),1,1,0);
    int step = 8; // bloc 8*8
    CImg<unsigned char> image_block;
    CImg<double> dct_block;

    // for each bloc in width    
    for(int i=0 ; i<image.width()/step ; ++i) {
        // for each bloc in hight
        for(int j=0 ; j<image.height()/step ; ++j) {
            // get one bloc of 8*8 px
            image_block = image.get_crop( i*8, j*8, i*8+(step-1), j*8+(step-1));
            
            // apply dct on block
            dct_block = applyDctOnBlock(image_block);

			// quantification
			dct_block = dct_block.get_div(Q).get_round();

            // affect bloc in final image
            
            // foreach pixel in bloc width
            for(int ii=0 ; ii<step ; ++ii) {
                // foreach pixel in bloc height
                for(int jj=0 ; jj<step ; ++jj){
                    compressed_image(i*8+ii, j*8+jj)= dct_block(ii,jj);
                }
            }
        }
    }
    return compressed_image;
}
/*--------------- /JPEGENCODER ----------------*/

/*--------------- JPEGDECODER ----------------*/
CImg<unsigned char> JPEGDecoder(CImg<double> image, const CImg<> & Q)
{
	// init vars
    CImg<unsigned char> uncompressed_image(image.width(),image.height(),1,1,0);
    int step = 8; // bloc 8*8
    CImg<unsigned char> new_image_block;
    CImg<double> dct_block;

    // for each bloc in width    
    for(int i=0 ; i<image.width()/step ; ++i) {
        // for each bloc in hight
        for(int j=0 ; j<image.height()/step ; ++j) {
            // get one bloc of 8*8 px
            dct_block = image.get_crop( i*8, j*8, i*8+(step-1), j*8+(step-1));

            // reverse quantification
            dct_block = dct_block.get_mul(Q);
            
            // apply dct-1 on dctblock
            new_image_block = applyInvertedDctOnDCTBlock(dct_block);

            // affect bloc in final image
            // foreach pixel in bloc width
            for(int ii=0 ; ii<step ; ++ii) {
                // foreach pixel in bloc height
                for(int jj=0 ; jj<step ; ++jj){
                    uncompressed_image(i*8+ii, j*8+jj)= new_image_block(ii,jj);
		            
                }
            }
        }
    }
    return uncompressed_image;
}
/*--------------- /JPEGDECODER ----------------*/

/*--------------- COMPUTEDISTORSION ----------------*/
double computeDistorsion(const CImg<unsigned char> & imageBefore, const CImg<unsigned char> & imageAfter) {
	double distorsion;
	
	// use lib -> Mean Square Error
	distorsion = imageBefore.MSE(imageAfter);
	
	return distorsion;
}
/*--------------- /COMPUTEDISTORSION ----------------*/

/*--------------- INITMATRIX ----------------*/
CImg<> initQuantizationMatrix(double quality) {
    // Quantization matrix
    CImg<> Q(8,8);
    Q(0,0)=16;   Q(0,1)=11;   Q(0,2)=10;   Q(0,3)=16;   Q(0,4)=24;   Q(0,5)=40;   Q(0,6)=51;   Q(0,7)=61;
    Q(1,0)=12;   Q(1,1)=12;   Q(1,2)=14;   Q(1,3)=19;   Q(1,4)=26;   Q(1,5)=58;   Q(1,6)=60;   Q(1,7)=55;
    Q(2,0)=14;   Q(2,1)=13;   Q(2,2)=16;   Q(2,3)=24;   Q(2,4)=40;   Q(2,5)=57;   Q(2,6)=69;   Q(2,7)=56;
    Q(3,0)=14;   Q(3,1)=17;   Q(3,2)=22;   Q(3,3)=29;   Q(3,4)=51;   Q(3,5)=87;   Q(3,6)=80;   Q(3,7)=62;
    Q(4,0)=18;   Q(4,1)=22;   Q(4,2)=37;   Q(4,3)=56;   Q(4,4)=68;   Q(4,5)=109;  Q(4,6)=103;  Q(4,7)=77;
    Q(5,0)=24;   Q(5,1)=35;   Q(5,2)=55;   Q(5,3)=64;   Q(5,4)=81;   Q(5,5)=104;  Q(5,6)=113;  Q(5,7)=92;
    Q(6,0)=49;   Q(6,1)=64;   Q(6,2)=78;   Q(6,3)=87;   Q(6,4)=103;  Q(6,5)=121;  Q(6,6)=120;  Q(6,7)=101;
    Q(7,0)=72;   Q(7,1)=92;   Q(7,2)=95;   Q(7,3)=98;   Q(7,4)=112;  Q(7,5)=100;  Q(7,6)=103;  Q(7,7)=99;
    // Quality
    Q *= (float)quality;

    return Q;
}
/*--------------- /INITMATRIX ----------------*/

/*--------------- MAIN ----------------*/
int main()
{
    double quality;

    // Ask quality
	cout << "Quality (1.0 low compression / 30.0 strong compression) : " << endl;
    if (!(cin >> quality)){
        quality = 1.0;
        cout << "Bad value (using default : " << quality << ")" << endl;
    }
	
    // Init matrix
	CImg<> Q = initQuantizationMatrix(quality);

    // Read the image "lena.bmp"
    CImg<unsigned char> my_image("datas/lena.bmp");
	// Take the luminance information 
    my_image.channel(0);

    // Compress to JPEG
    CImg<double> compressed_image = JPEGEncoder(my_image, Q);
    // Uncompress
    CImg<unsigned char> uncompressed_image = JPEGDecoder(compressed_image, Q);

    /* Display */
    // bmp file
    CImgDisplay main_disp(my_image,"Initial Image");
    // compressed (by dct)
    CImgDisplay comp_disp(compressed_image,"Compressed Image");
    // uncompressed (by dct-1)
    CImgDisplay uncomp_disp(uncompressed_image,"Uncompressed Image");
	// distorsion
	cout << "Distorsion rate : " << computeDistorsion(my_image, uncompressed_image) << endl;

	/* Wait */
    while (!main_disp.is_closed()) {
        main_disp.wait();
    }
}
/*--------------- /MAIN ----------------*/

/* ---- explications -----

Image coords : (i,j)
------------------------- i
| (0,0) (1,0)
| (0,1)
|
j

*/




