/*
Michael Fadem
Project 3: Virtual Memory Translation
CS 474
11/22/16
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Constant variables for the sizes of the memory spaces
const int pageSize = 256;
const int virtualMemSize = 256;
const int mainMemSize = 128;
const int TLBSize = 16;
signed char value[256];

//Function to search the backing store for the new page 
int mainStorage(int pageNum){
	
	FILE* fptr;
	unsigned char buff[256];
	int newPage = 0;
	int i = 0;
	
	//Searching through the backing store binary file for new page
	fptr = fopen("BACKING_STORE.bin", "rb");
	if(fptr == NULL){
		printf("Error opening file");
		return -1;
	}
	
	//Searching for new page in BACKING_STORE.bin
	fseek(fptr,pageNum*256,SEEK_CUR);
	fread(buff, sizeof(char),1,fptr);
	
	//Filling signed byte value array to get value later
	for(i = 0; i < sizeof(buff); i++){
		value[i]=buff[i];
	}
	
	//Assigning new page
	newPage = atoi(buff) & 255;
	fclose(fptr);
	return newPage;
	
}

//Main function to go through addresses.txt
int main(int argc, char *argv[]){
	
	//Making sure addresses are supplied
	if(argc != 2){
		printf("Insuffcient number of args");
		return -1;
	}
	
	//File pointer, Physical/virtual mem, and TLB
	FILE* fptr;	
	int PM[mainMemSize];
	int VM[virtualMemSize][2];
	int TLB[TLBSize][2];
	int i = 0;
	
	//Initalizing virtual memory
	for(i = 0; i < virtualMemSize; i++){
		if(i > mainMemSize - 1){
			VM[i][0] = -1;
		}
		else{
			VM[i][0] = i;
		}
		
		if (i > mainMemSize - 1){
			VM[i][1] = -1;
		}
		else{
			VM[i][1] = mainMemSize - i;
		}
	}
	
	//Initalizing Physical memory 
	for(i = 0; i < mainMemSize; i++){
		PM[i]= i;
	}
	
	//Initalizing TLB 
	for(i = 0; i < TLBSize; i++){
		if(i > TLBSize - 1){
			TLB[i][0] = -1;
		}
		else{
			TLB[i][0] = i;
		}
		
		if(i > TLBSize - 1){
			TLB[i][1] = -1;
		}
		else{
			TLB[i][1] = TLBSize - i;
		}
	}
	
	//Open addresses file
	fptr = fopen(argv[1], "r");
	if(fptr == NULL){
		printf("Error opening file");
		return -1;
	}
	
	//Address values
	int physicalAddress = 0;
	int translatedAddresses = 0;
	
	//Page variables
	int pageNum = 0;
	int pageHit = 0;
	int pageFault = 0;
	double pageFaultRate = 0.0;
	
	//TLB variables
	int TLBHit = 0;
	int TLBFault = 0;
	int TLBHitCount = 0;
	int TLBReplacement = 0;
	double TLBHitRate = 0.0;

	//File variables
	ssize_t input;
	size_t length = 0;
	char *lineptr = NULL;

	printf("Address Translations:\n\n");
	
	//Reading the addresses from the addresses.txt file and 
	while((input = getline(&lineptr,&length,fptr)) != -1){
		//Finding virtual address, page, and page table number
		int OGAddress = atoi(lineptr);
		int virtualAddress = atoi(lineptr) & 255;
		//int virtualAddress = mainStorage(atoi(lineptr));
		int page = atoi(lineptr) & 65280;
		//int page = mainStorage(virtualAddress);
		int pageTabNum = page >> 8;
		TLBReplacement = pageTabNum % 15;
		TLBHit = 0;
		
		//Checking TLB for address
		for(i = 0; i < TLBSize; i++){
			if(TLB[i][0] == pageTabNum){
				TLBHit = 1;
				TLBHitRate++;
				TLBHitCount++;
				break;
			}
			else if(i == TLBSize){
				TLBFault++;
				if((VM[pageTabNum][0] == pageTabNum) && (VM[pageTabNum][1] != -1)){
					pageHit++;
					TLB[TLBReplacement][0] = pageTabNum;
					TLB[TLBReplacement][1] = 1;		
				}
			}
		}
		
		//Getting new page and adjusting TLB and Page Table
		if(VM[pageTabNum][0] < 0 && !TLBHit){
			pageFault++;
			pageFaultRate++;
			int newPage = mainStorage(pageTabNum);
			VM[pageTabNum][0] = newPage;
			VM[pageTabNum][1] = 1;			
		}
		
		//Calculating physical address
		physicalAddress = (PM[VM[pageTabNum][0]] * pageSize) + virtualAddress;

		//Print translation
		printf("Virtual Address: %d Physical Address: %d Signed Byte Value: %hhd \n", page, physicalAddress, value[virtualAddress]);
		
		//Increasing total number of translated addresses
		translatedAddresses++;
	}
	
	//Closing addresses file
	fclose(fptr);

	//Statistic print statements 
	printf("Number of Translated Addresses: %d \n", translatedAddresses);
	printf("Page Faults: %d \n", pageFault);
	printf("Page Fault Rate: %f% \n", pageFaultRate / 1000.0*100);
	printf("TLB Hits: %d \n", TLBHitCount);
	//printf("TLB Faults: %d \n", TLBFault);
	printf("TLB Hit Rate: %f% \n", TLBHitRate/ 1000.0*100);
	return 0;
}
