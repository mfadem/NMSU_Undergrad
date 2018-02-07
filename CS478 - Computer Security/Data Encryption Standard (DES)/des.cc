/*
Brett Pelkey
Michael Fadem
2-27-16

Programming Assignment 1
CS478
Computer Security

des.cc is the file that provides all the function the decrypt and encrypt a given file
*/

#include "des.h"
#include <iostream>
#include <stdio.h>
#include <bitset>
#include <stdlib.h>
#include <cstring>

using namespace std;
/*constructor*/
des::des(string filename, string key){
	//cout<< filename << endl;
	KEY = key;
	read(filename);
	
}

/*
read will read the ciphertext file and put the entire contents into a buffer called
fileBuffer. It will also init the filesize global variable which tells the length of the
file. 
*/
void des::read(string filename){
	// cout << "the filename param is " << filename << endl;
	
	FILE *f = fopen(filename.c_str(), "rb");	// open file
	fseek(f, 0, SEEK_END);						// seek to end
	filesize = ftell(f);						// get size of file
	if(filesize%8 != 0)
		filesize += 8-(filesize%8);
	cout << "size = " << filesize << endl;
	fseek(f, 0, SEEK_SET);						// set the seek
	
	fileBuffer = (char*) malloc(filesize + 1);	// set size of buffer
	fread(fileBuffer, filesize, 1, f);			// read file into buffer
	fclose(f);									// close file
	fileBuffer[filesize] = 0;					// add terminating character
	//cout << fileBuffer;
	//print out fileBuffer in binary (prints the end of line char too)
	int count = 0;
	//char block[8];
	// init block to all A's to check later
	for(int i= 0; i < 8; i++)
		block[i] = 'A';
	// print the block array for testing
	//for(int i= 0; i < 8; i++)
		//cout << block[i] << endl;
	//int tmpFilesize = filesize;
	genKeys(KEY);
	for(int i= 0; i < filesize; i++){
		block[count] = fileBuffer[i];
		count++;
		//cout << bitset<8>(fileBuffer[i]) << endl;
		if(count == 8 || i == filesize - 1){
			blockToBits();
			//initPerm();
			for(int i= 0; i < 8; i++)
				block[i] = '0';
			count = 0;
		}
	}
}

void des::initPerm(){
	cout << "initPerm called and block = " << endl;
	int iPerm[64];
	for(int i = 0; i < 64; i++)
		iPerm[i] = 0;
	for (int i = 0; i < 64; i++) {
		iPerm[i] = iPerm[IP[i] - 1];
	}

	for (int i = 0; i < 32; i++)
		L[i] = iPerm[i];
	for (int i = 32; i < 64; i++)
		R[i - 32] = iPerm[i];
	cout << "L[]= ";
	for (int i = 0; i < 32; i++)
		cout << L[i];
	cout << "\nR[]= ";
	for (int i = 0; i < 32; i++)
		cout << R[i];
	cout << endl;
	//most can be removed as of 3-30-16 3:15
	/*int block3[64];
	for(int i = 0; i < 64; i++)
		block3[i] = '0';
	int tmp;
	for(int i = 0; i < 8; i++){
		if(block[i] != '0'){
			tmp = block[i];
			//cout << (char) tmp << endl;
			// convert char to int array in binary form called block2
			int count2 = 0;
			for (int j = 7; j >= 0; --j) { 
				int x = i*8+count2;
				//cout << x << endl;
				block3[x] = tmp & (1 << j) ? 1 : 0;
				//block[x] = tmp & (1 << j) ? 1 : 0;
				//cout << block3[x];
				//cout  << block2[j];
				count2++;
			}
			//cout << endl;
	 		//cout << bitset<8>(block[i]) << endl;
			//cout << tmp << endl;
		}
	}
	//print 64 bit block for testing
	for(int i = 0; i < 64; i++)
		cout << block3[i];
	cout << endl;*/
}

void des::blockToBits(){
	//clear bits back to 0 before applying block to bits
	for(int i = 0; i < 64; i++)
		bits[i] = 0;
	int tmp;
	for(int i = 0; i < 8; i++){
		tmp = block[i];
		//cout << (char) tmp << endl;
		// convert char to int array in binary form called keyBits
		int count = 0;
		for (int j = 7; j >= 0; --j) { 
			int x = i*8+count;
			bits[x] = tmp & (1 << j) ? 1 : 0;
			count++;
		}	
	}
}

void des::genKeys(string s){
	cout << "genKeys called" << endl;
	cout << "and key is " << s << endl;
	//convert string to char array
	strcpy(block, s.c_str());
	blockToBits();
	//print 64 bit block for testing
	for(int i = 0; i < 64; i++)
		cout << bits[i];
	cout << endl;
	
	//permute the keyBits into a new array with PC1
	int permutedKeyBits[56];
	for(int i = 0; i < 56; i++)
		permutedKeyBits[i] = bits[PC1[i]-1];
	
	//print new permutedKeyBits
	cout << "permuted keyBits is" << endl;
	for(int i = 0; i < 56; i++)
		cout << permutedKeyBits[i];
	cout << endl;
	
	//convert keybits back to original text
	char _key[8];
	for(int i = 0; i < 8; i++){
		unsigned char byte = 0;
		for(int j = i*8; j < 8+i*8; j++){
			if(bits[j]==1)
				byte |= 1<<(7-j%8);
		}
		_key[i] = byte;
	}
	for(int i = 0; i < 8; i++)
		cout << _key[i];
	cout << endl;
	
	//int C[28];//left half of PC1 key
	//int D[28];//right half of PC1 key
	for(int i = 0; i < 28; i++)
		C[i]=permutedKeyBits[i];
	for(int i = 28; i < 56; i++)
		D[i-28]=permutedKeyBits[i];
	cout << "C[]= ";
	for(int i = 0; i < 28; i++)
		cout << C[i];
	cout << "\nD[]= ";
	for(int i = 0; i < 28; i++)
		cout << D[i];
	cout << endl;

	for(int i = 1; i <= 16; i++)
		genSubKey(i);

	//for(int i = 0; i < 56; i++)
		//cout << k16[i];
	//cout << endl;

	
}

void des::genSubKey(int num){
	int test[56];
	for(int i = 0; i < 28; i++){
		switch (num){
			case 1:
				test[i] = C[(i+1)%28];
				test[i+28] = D[(i+1)%28];
				C[i] = C[(i+1)%28];
				D[i] = D[(i+1)%28];
				//apply pc2 to k1
				for(int j = 0; j < 48; j++)
					k1[j] =test[PC2[j]-1];
				break;
			case 2:
				k2[i] = C[(i+1)%28];
				k2[i+28] = D[(i+1)%28];
				C[i] = C[(i+1)%28];
				D[i] = D[(i+1)%28];
				//for(int i = 0; i < 48; i++)
					//k2[i] = k2[PC2[i]-1];
				break;
			case 3:
				k3[i] = C[(i+2)%28];
				k3[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k3[i] = k3[PC2[i]-1];
				break;
			case 4:
				k4[i] = C[(i+2)%28];
				k4[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k4[i] = k4[PC2[i]-1];
				break;
			case 5:
				k5[i] = C[(i+2)%28];
				k5[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k5[i] = k5[PC2[i]-1];
				break;
			case 6:
				k6[i] = C[(i+2)%28];
				k6[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k6[i] = k6[PC2[i]-1];
				break;
			case 7:
				k7[i] = C[(i+2)%28];
				k7[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k7[i] = k7[PC2[i]-1];				
				break;
			case 8:
				k8[i] = C[(i+2)%28];
				k8[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k8[i] = k8[PC2[i]-1];
				break;
			case 9:
				k9[i] = C[(i+1)%28];
				k9[i+28] = D[(i+1)%28];
				C[i] = C[(i+1)%28];
				D[i] = D[(i+1)%28];
				//for(int i = 0; i < 48; i++)
					//k9[i] = k9[PC2[i]-1];
				break;
			case 10:
				k10[i] = C[(i+2)%28];
				k10[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k10[i] = k10[PC2[i]-1];
				break;
			case 11:
				k11[i] = C[(i+2)%28];
				k11[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k11[i] = k11[PC2[i]-1];
				break;
			case 12:
				k12[i] = C[(i+2)%28];
				k12[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k12[i] = k12[PC2[i]-1];
				break;
			case 13:
				k13[i] = C[(i+2)%28];
				k13[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k13[i] = k13[PC2[i]-1];
				break;
			case 14:
				k14[i] = C[(i+2)%28];
				k14[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k14[i] = k14[PC2[i]-1];
				break;
			case 15:
				k15[i] = C[(i+2)%28];
				k15[i+28] = D[(i+2)%28];
				C[i] = C[(i+2)%28];
				D[i] = D[(i+2)%28];
				//for(int i = 0; i < 48; i++)
					//k15[i] = k15[PC2[i]-1];
				break;
			case 16:
				k16[i] = C[(i+1)%28];
				k16[i+28] = D[(i+1)%28];
				C[i] = C[(i+1)%28];
				D[i] = D[(i+1)%28];
				int t[56];
				
				break;
		}//end switch
	}//end for
	for(int j = 0; j < 56; j++)
		cout << k1[j];
	cout << endl; 
}


