#include <fstream>
#include <cstring>
#include <cassert>
#include "parser.h"

//----------------------------------------------------------------------------------------------------------------------//
bool parser_C::parse(char* p_pInput)
{
    ifstream input;
    input.open(p_pInput);
    printf("[Lab2] - Parsing File:%s\n",p_pInput);
    net_C* pNet(0);
    int nNumNet(0), nNumNode(0), nTotalNode(0);        
    string strNetName, strNodeName;
    input>>nNumNet;
    for(int nNetIter=0;nNetIter<nNumNet;++nNetIter)
    {
        input>>strNetName>>nNumNode;
        pNet = new net_C(strNetName);
        for(int nNodeIter=0;nNodeIter<nNumNode;++nNodeIter)
        {
            input>>strNodeName;
            pNet->addNode(strNodeName);
            nTotalNode ++;
        }
        m_vNet.push_back(pNet);
    }
    printf("[Lab2] - Num Net:%d\n",nNumNet);
    printf("[Lab2] - Total Num Node:%d\n",nTotalNode);
    return true;
}
//----------------------------------------------------------------------------------------------------------------------//
parser_C::~parser_C()
{
    for(unsigned int nNetIter=0;nNetIter<m_vNet.size();++nNetIter)
    {
        delete m_vNet[nNetIter];
    }
}
//----------------------------------------------------------------------------------------------------------------------//

