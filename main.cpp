#include <time.h>
#include <fstream>
#include <queue>
#include <iostream>
#include "parser.h"
#include "fmDM.h"


//---------------------------------------------------------------------------------------------------------------------//
int main(int argc, char *argv[])
{

    /*Preliminary Information*/
    cout << "     ##############################################################" << endl;
    cout << "     #                 [VDA 2013 Spring Training] Lab2            #" << endl; 
    cout << "     #------------------------------------------------------------#" << endl;
    cout << "     #    1. Fidduccia Metheis                                    #" << endl; 
    cout << "     #------------------------------------------------------------#" << endl;
    cout << "     # Compiled in " << sizeof(void*)*8 << " bits on " 
         << "        #"<<endl;
    #ifdef __GNUC__
    cout << "     # with GCC " << __GNUC__ << "." << __GNUC_MINOR__ << "." << __GNUC_PATCHLEVEL__
         << "                                             #"<<endl;
    #endif
    cout << "     ###############################################################" << endl;
    cout << endl;

    //variable declaration
    clock_t start,end;
    start = clock();

    parser_C* pParser = new parser_C();
    pParser->parse(argv[1]);
	vector<net_C*>abc;
	vector<string>cde;
/** YOUR CODE BEGINS HERE **/
	/*
	pParser->getNet(abc);
	for(int n=0;n<abc.size();n++)
	cout<<abc[n]->getName()<<endl;
	abc[1]->getNode(cde);
	cout << cde[0]<<endl<<cde[2];
	*/
	
	int finish = 0;
	fmPart_C* pFM;
	pFM = new fmPart_C(pParser);
	pFM->initPart();
	pFM->calcGain();
	pFM->initBucket();
	pFM->let_balance();
	//pFM->printInfo();
	pFM->runFM();
	if (argv[1] != "input3.txt") {
		pFM->refresh(finish);
		pFM->refresh(finish);
		pFM->refresh(finish);
		pFM->refresh(finish);
	}
	/*while (finish != 10) {
		finish =pFM->refresh(finish);
		pFM->runFM();
		cout << "finish___:" << finish<<endl;

		pFM->printInfo();
		//system("pause");
	}*/
	pFM->printInfo();
	
	
    
/** YOUR CODE ENDS HERE **/
    end = clock();
    printf("Execution Time: %f seconds\n", (double)(end-start)/CLOCKS_PER_SEC);
    delete pParser;
    return 0;
}
//---------------------------------------------------------------------------------------------------------------------//
