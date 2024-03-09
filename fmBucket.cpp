#include "fmDM.h"
//----------------------------------------------------------------------------------------------------------------------//
fmBucket_C::fmBucket_C() :m_nMax(0) {}
void fmBucket_C::setSize(int p_max)
{
	m_nMax = p_max;
	m_nsize = p_max;
	m_vPositive.resize(p_max + 1);
	m_vNegative.resize(p_max + 1);
}
void fmBucket_C::setMax(int p_max)
{
	m_nMax = p_max;
}
void fmBucket_C::pushNode(fmNode_C* p_Node)
{
	if (p_Node->getGain() >= 0)
		m_vPositive[p_Node->getGain()].push_back(p_Node);
	else
		m_vNegative[-(p_Node->getGain())].push_back(p_Node);
}
void fmBucket_C::clear()
{
	int a = m_nsize;
	int b = -a;
	for (a; a >= b; a--) {
		getList(a).clear();
	}
}
void fmBucket_C::printbucket()
{
	int a = m_nsize;
	int b = -a;
	cout << "bucket--------------------------------------------------------------------------------------" << endl;
	for (a; a >= b; a--)
	{
		cout << a << ": ";
		for (int x = 0; x < getList(a).size(); x++)
		{
			cout << getList(a)[x]->getName()<<" ";
		}
		cout<<"   "<<getList(a).size() << endl;
	}
}
//----------------------------------------------------------------------------------------------------------------------//

