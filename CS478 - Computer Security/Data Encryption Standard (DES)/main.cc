/* 
Brett Pelkey
Michael Fadem
2-27-16

Programming Assignment 1
CS478
Computer Security

This is the driver file to run the program. It will get the command line arguments
and user input to run what the user would like to
*/
#include <iostream>
#include <termios.h>
#include <unistd.h>
#include <stdlib.h>
#include <cstring>
#include "des.h"

using namespace std;

void usage();

/*
main driver function
*/
int main(int argc, char** argv){
	if(argc != 1)
		usage();
	
	//a.read();
	char option; // option for Enc or Dec
	string file; // name of file to Enc or Dec
	string key; // key to Enc or Dec
	string iv; // initializing vector
	while(option != 'D' && option != 'd' && option != 'E'
			&& option != 'e'){
		cout << "Enter E to encrypt or D to decrypt" << endl;
		cin >> option;
	}
	if(option == 'E' || option == 'e'){
		cout << "You selected E for encryption" << endl;
	}else if (option == 'D' || option == 'd'){
		cout << "You selected D for decryption" << endl;
	}
	if(option == 'E' || option == 'e')
		cout << "Enter the path of the file to Ecrypt" << endl;
	if(option == 'D' || option == 'd')
		cout << "Enter the path of the file to Decrypt" << endl;
	cin >> file;
	//cout << file << endl;
	cout << "Enter the key for encryption/decryption" << endl;
	// hide terminal input
	termios tty;
    tcgetattr(STDIN_FILENO, &tty);
    /* we want to disable echo */
    tty.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &tty);
    cin >> key;
    //cout << key << endl;
	cout << "Enter the iv for encryption/decryption or type skip generate random iv" << endl;
	cin >> iv;
	if(iv.compare("skip") == 0)
		cout << "no iv given so we will create one for you" << endl;

	des a(file, key);
	//a.read(file);

    return 0;
}

/*
usage will give the usage if the user does not use the makefile and does not run the main
file in accordance to what it expects
*/
void usage(){
	cout << "Usage: ./des" << endl;
	//cout << "Example: ./des ciphertext.txt" << endl;
	exit(EXIT_FAILURE);
}
