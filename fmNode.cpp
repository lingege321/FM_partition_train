#include "fmDM.h"

//----------------------------------------------------------------------------------------------------------------------//
fmNode_C::fmNode_C(string p_strName) : m_nGain(0), m_strName(p_strName), m_ePart(fmNode_C::ePartA), m_bLock(false) 
{

}
//----------------------------------------------------------------------------------------------------------------------//
string fmNode_C::getName()
{
    return m_strName;
}
//----------------------------------------------------------------------------------------------------------------------//
void fmNode_C::addNet(fmNet_C* p_pNet)
{
    m_vNet.push_back(p_pNet);
}
//----------------------------------------------------------------------------------------------------------------------//
void fmNode_C::addGain(int p_nGain)
{
    m_nGain += p_nGain;
}
//----------------------------------------------------------------------------------------------------------------------//
void fmNode_C::setPart(part_E p_ePart)
{
    m_ePart = p_ePart;
}
//----------------------------------------------------------------------------------------------------------------------//
int fmNode_C::getGain()
{
    return m_nGain;
}
//----------------------------------------------------------------------------------------------------------------------//
fmNode_C::part_E fmNode_C::getPart()
{
    return m_ePart;
}
//----------------------------------------------------------------------------------------------------------------------//
void fmNode_C::setGain(int p_nGain)
{
    m_nGain = p_nGain;
}
//----------------------------------------------------------------------------------------------------------------------//
void fmNode_C::setLock(bool p_bLock)
{
    m_bLock = p_bLock;
}
//----------------------------------------------------------------------------------------------------------------------//
void fmNode_C::setIter(list<fmNode_C*>::iterator p_itList)
{
    m_itList = p_itList;
}
//----------------------------------------------------------------------------------------------------------------------//
list<fmNode_C*>::iterator fmNode_C::getIter()
{
    return m_itList;
}
//----------------------------------------------------------------------------------------------------------------------//
int fmNode_C::getNetNum()
{
    return m_vNet.size();
}    
//----------------------------------------------------------------------------------------------------------------------//
fmNet_C* fmNode_C::getNet(int p_nIdx)
{
    return m_vNet[p_nIdx];
}
//----------------------------------------------------------------------------------------------------------------------//
bool fmNode_C::isLock()
{
    return m_bLock;
}
//----------------------------------------------------------------------------------------------------------------------//