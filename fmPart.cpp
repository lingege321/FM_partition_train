#include "fmDM.h"
#include <fstream>
ofstream fout;
//----------------------------------------------------------------------------------------------------------------------//
void fmPart_C::addNet(string p_strNet, vector<string>& p_rvNode)
{
	m_mapNet[p_strNet] = new fmNet_C(p_strNet);
	for (int i = 0; i < p_rvNode.size(); i++)
	{
		if (m_mapNode[p_rvNode[i]] == NULL) { m_mapNode[p_rvNode[i]] = new fmNode_C(p_rvNode[i]); }
		m_mapNode[p_rvNode[i]]->addNet(m_mapNet[p_strNet]);
		m_mapNet[p_strNet]->addNode(m_mapNode[p_rvNode[i]]);
	}

}
fmPart_C::fmPart_C(parser_C* p_parser)
{
	fout.open("out.txt");
	int max = 0;
	vector<net_C*>p_parserNet;
	vector<string>p_parserNode;
	p_parser->getNet(p_parserNet);
	for (int i = 0; i < p_parserNet.size(); i++)
	{
		p_parserNet[i]->getNode(p_parserNode);
		addNet(p_parserNet[i]->getName(), p_parserNode);
	}
	/*初始化bucket*/
	for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it) {
		if (max < it->second->getNetNum())
			max = it->second->getNetNum();
	}
	m_vBucket.resize(2);
	m_vBucket[0] = new fmBucket_C();
	m_vBucket[1] = new fmBucket_C();
	m_vBucket[0]->setSize(max+1);
	m_vBucket[1]->setSize(max+1);
	/*初始化bucket*/
	m_jump = m_mapNode.size();
	while (1) {
		if (m_jump < 100)
			break;
		m_jump = m_jump / 2;
	}
	////cout << "hello" << m_vBucket[0]->getMax() << endl;
}
int fmPart_C::refresh(int finish)
{
	m_vBucket[0]->clear();
	m_vBucket[1]->clear();
	m_temppart = m_vvpart;
	m_tempcost = m_vcost;
	m_vvpart.clear();
	m_vcost.clear();
	
	for (int num = 0; num < m_temppart.size()&&num<3; num++) {
		int x = 0;
		A = 0;
		int choose_v;
		//cout << "-------------------mvvpartsize" << m_vvpart.size() <<m_temppart.size()<< endl;
		/*for (int i = 0; i < m_vvpart.size(); i++)
			//cout << m_vcost[i] << endl;
		while (1) {
			choose_v = rand() % m_vvpart.size();
			//cout << m_cost << " " << choose_v << " " << m_vcost[choose_v] << endl;
			if (m_vcost[choose_v] == m_cost) {
				m_vpart = m_vvpart[0];
				m_vvpart.clear();
				m_vvpart.push_back(m_vpart);
				break;
			}
		}
		*/
		//cout << "-------------------mvvpartsize" << m_vvpart.size() << endl;
		m_vpart = m_temppart[num];
		m_cost = m_tempcost[num];
		/*if(m_vcost.size()==0) {
			m_vvpart.push_back(m_vpart);
			m_vcost.push_back(m_cost);
		}
		if (m_vcost.size() != 0)
		{
			if (m_cost <= m_vcost[0]) {
				m_vvpart.push_back(m_vpart);
				m_vcost.push_back(m_cost);
			}
		}*/
		for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it) {
			it->second->setLock(false);
			////cout << m_vpart[x] << " ";
			if (m_vpart[x] == 0) {
				it->second->setPart(fmNode_C::ePartA);
				it->second->setGain(0);
				A++;
			}
			else {
				it->second->setPart(fmNode_C::ePartB);
				it->second->setGain(0);
			}
			x++;
		}
		calcGain();
		m_cost = m_nowcost;
		x = 0;
		initBucket();
		//cout << "--------------------------------------------------refresh--------------------------------------------------------------" << endl;
		//cout << "bucketA__:" << m_vBucket[0]->getMax() << "   bucketB__:" << m_vBucket[1]->getMax() << endl;
		runFM();
		
	}
	for (int nnnn = 0; nnnn < m_vcost.size(); nnnn++) {
		//for (int a = 0; a < m_vpart.size(); a++)
			//cout << m_vpart[a];
		//cout << "cost___:" << m_vcost[nnnn] << endl;
	}
	
	if (m_vBucket[0]->getMax() <= 0 && m_vBucket[1]->getMax() <= 0)
	{
		return finish + 1;
	}
	else return 0;
	
}
void fmPart_C::printInfo()
{	
	int x = 0;
	for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it)
	{
		fout << it->first << "   " << m_vvpart[0][x]*100 << "   " << m_vvpart[0][x] * 100 << "   " << ":N"<< endl;
		x++;
	}
	/*cout << "cutsize: " << m_cost << endl;
	x = 0;
	for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it) {
		cout << it->first << " ";
		cout << m_vpart[x] << "  ";
		cout << it->second->getGain();
		cout << "lock:" << it->second->isLock();
		cout<< "   " << x << endl;
		x++;
	}
	cout << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << A;*/
	/*
	for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it)
	{
	//cout << it->first<<"  "<<it->second->getNet(0)->getName()<<"  "<<it->second->getNetNum()<<"  "<<it->second->getPart()<<"  "<<it->second->getGain()<<endl;
	}
	//cout << endl;
	//cout << "------------------------";
	int n = 0;
	for (map<string, fmNet_C*>::iterator it = m_mapNet.begin(); it != m_mapNet.end(); ++it)
	{
	n++;
	//cout << it->first << " "<<it->second->getNodeNum()<<endl;
	}
	//cout << "--" << n << "--";*/
}
void fmPart_C::initPart()
{
	A = 0;
	int whichpart = 0;
	m_vpart.resize(m_mapNode.size());

	if (m_mapNode.size() > 1000) {
		for (map<string, fmNet_C*>::iterator it = m_mapNet.begin(); it != m_mapNet.end(); ++it) {
			if (it->second->getNodeNum() < 4)
			{
				for (int i = 0; i < it->second->getNodeNum(); i++) {
					it->second->getNode(i)->setPart(fmNode_C::ePartA);
				}
			}
			else
			{
				for (int i = 0; i < it->second->getNodeNum(); i++) {
					it->second->getNode(i)->setPart(fmNode_C::ePartB);
				}
			}
		}
		for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it) {
			if (it->second->getPart() == fmNode_C::ePartA)
			{
				A++;
				m_vpart[whichpart] = 0;
			}
			else
				m_vpart[whichpart] = 1;
			whichpart++;
		}

	}
       /* for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it)
                //cout<<it->second->getName()<<" A: "<<A<<endl;

        for(map<string, fmNet_C*>::iterator it = m_mapNet.begin(); it != m_mapNet.end(); ++it)
                //cout<<it->first<<"nodenum:  "<<it->second->getNodeNum()<<endl;

        for(int x=0;x<m_vpart.size();x++)
                //cout<<m_vpart[x]<<" ";

        //cout<<"A__:"<<A<<"  total:__"<<m_mapNode.size();*/
	else
	{
		for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it)
		{
			if (whichpart % 2 == 0) {
				it->second->setPart(it->second->ePartA);
				m_vpart[whichpart] = 0;
				A++;
			}
			else {
				it->second->setPart(it->second->ePartB);
				m_vpart[whichpart] = 1;
			}

			whichpart++;
		}
	}
	/*for (int i = 0; i < m_vpart.size(); i++)
	//cout << m_vpart[i] << " ";
	//cout << endl;*/

}
void fmPart_C::calcGain()
{
	m_cost = 0;

	for (map<string, fmNet_C*>::iterator it = m_mapNet.begin(); it != m_mapNet.end(); ++it)
	{
		vector<fmNode_C*>partA;
		vector<fmNode_C*>partB;
		for (int i = 0; i < it->second->getNodeNum(); i++)
		{
			if (it->second->getNode(i)->getPart() == fmNode_C::ePartA)
				partA.push_back(it->second->getNode(i));
			else
				partB.push_back(it->second->getNode(i));
		}
		if (it->second->getNodeNum() != 1)
		{
			if (partA.size() == 0) {
				for (int x = 0; x < partB.size(); x++)
					partB[x]->setGain(partB[x]->getGain() - 1);
			}
			else if (partB.size() == 0) {
				for (int x = 0; x < partA.size(); x++)
					partA[x]->setGain(partA[x]->getGain() - 1);
			}
			else if (partA.size() == 1 && partB.size() == 1) {
				partA[0]->setGain(partA[0]->getGain() + 1);
				partB[0]->setGain(partB[0]->getGain() + 1);
				m_cost++;
			}
			else if (partA.size() == 1) {
				partA[0]->setGain(partA[0]->getGain() + 1);
				m_cost++;
			}
			else if (partB.size() == 1) {
				partB[0]->setGain(partB[0]->getGain() + 1);
				m_cost++;
			}
			else {
				m_cost++;
			}
		}
	}
	m_nowcost = m_cost;
	if (m_vvpart.size() == 0) {
		m_vvpart.push_back(m_vpart);
		m_vcost.push_back(m_nowcost);
	}
	////cout << "cost--->" << m_nowcost << "A--->" <<A<<"total---->"<<m_mapNode.size()<< endl;
}
void fmPart_C::initBucket()
{
	m_vBucket[0]->setMax(-m_vBucket[0]->getSize());
	m_vBucket[1]->setMax(-m_vBucket[1]->getSize());
	for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it)
	{
		if (it->second->getPart() == 0) {
			m_vBucket[0]->pushNode(it->second);
		}
		else {
			m_vBucket[1]->pushNode(it->second);
		}
	}
	int  i = m_vBucket[0]->getMax();
	int y = -i;
	for (i; i<y + 1; i++)
	{
		if (m_vBucket[0]->getList(i).size() != 0)
			m_vBucket[0]->setMax(i);
		if (m_vBucket[1]->getList(i).size() != 0)
			m_vBucket[1]->setMax(i);
	}
	/*//cout << "partA max gain" << m_vBucket[0]->getMax() << "partB max gain" << m_vBucket[1]->getMax()<<endl;
	for (int n=0;n<m_vBucket[1]->getList(-2).size();n++)
	{
	//cout << m_vBucket[1]->getList(-2)[n]->getName() << " " << m_vBucket[1]->getList(-2)[n]->getGain() << endl;
	}*/
}
void fmPart_C::let_balance()
{
	m_cost = m_mapNode.size();
	double a = A;
	double b = m_mapNode.size();
	double ba;
	ba = a / b;
	if (ba >= 0.52)
	{
		fmNode_C *p;
		while (1) {
			p = m_vBucket[0]->getList(m_vBucket[0]->getMax())[0];
			moveNode(p);
			updateGain(p);
			updatecut(p);
			a = A;
			ba = a / b;
			if (ba<=0.52)
				break;
		}
	}
	else if (ba <= 0.48) {
		fmNode_C *p;
		while (1) {
			p = m_vBucket[1]->getList(m_vBucket[1]->getMax())[0];
			moveNode(p);
			updateGain(p);
			updatecut(p);
			a = A;
			ba = a / b;
			if (ba>=0.48)
				break;
		}
	}
	//int x = 0;
	for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it) {
		it->second->setLock(0);
		it->second->setGain(0);
		/*cout << it->second->getPart() << " " << m_vpart[x]<<endl;
		x++;*/
	}
	m_vBucket[0]->clear();
	m_vBucket[1]->clear();
	calcGain();
	initBucket();
}
bool fmPart_C::isBalance()
{
	double p_dbalance;
	double a = A;
	double b = m_mapNode.size();
	p_dbalance = a / b;
	////cout << "balance:" << p_dbalance<<"A:A"<<A<<"AA"<<a<<"mmap"<<m_mapNode.size()<<endl;
	if (p_dbalance <= 0.55&&p_dbalance >= 0.45)
		return true;
	else
		return false;
}
bool fmPart_C::isBalance(fmNode_C::part_E part)
{
	if (part == fmNode_C::ePartA)
	{
		double a = A - 1;
		double b = m_mapNode.size();
		double p_dbalance;
		p_dbalance = a / b;
		if (p_dbalance <= 0.6&&p_dbalance >= 0.4)
			return true;
		else {

			return false;
		}
	}
	else {
		double a = A + 1;
		double b = m_mapNode.size();
		double p_dbalance;
		p_dbalance = a / b;
		if (p_dbalance <= 0.6&&p_dbalance >= 0.4)
			return true;
		else
			return false;
	}

}
bool fmPart_C::morebalance(fmNode_C* p_node)
{
	double a = A;
	double b = m_mapNode.size();
	double ba;
	ba = a / b;
	if (ba >= 0.45&&ba <= 0.55)
		return true;
	else
	{
		if (p_node->getPart() == fmNode_C::ePartA) {
			if (ba >= 0.5)
				return true;
			else return false;
		}
		else {
			if (ba < 0.5)
				return true;
			else return false;
		}
	}
}
bool fmPart_C::diff_cut()
{
	if (m_cost >= m_nowcost) {
		m_cost = m_nowcost;
		return true;
	}
	else return false;
}
bool fmPart_C::updatecut(fmNode_C* p_node)
{
	if (isBalance())
	{
		////cout << "isbalance" << endl;
		if (diff_cut())
		{
			
			int x = 0;
			for (map<string, fmNode_C*>::iterator it = m_mapNode.begin(); it != m_mapNode.end(); ++it) {
				if (it->second->getPart() == p_node->ePartA)
					m_vpart[x] = 0;
				else
					m_vpart[x] = 1;
				x++;
			}
			if (m_vcost[0] == m_cost)
			{
				m_vvpart.push_back(m_vpart);
				m_vcost.push_back(m_cost);
			}
			else if (m_vcost[0] > m_cost)
			{
				m_vvpart.clear();
				m_vcost.clear();
				m_vvpart.push_back(m_vpart);
				m_vcost.push_back(m_cost);
			}
			
			return true;
		}
	}
	return false;
}
void fmPart_C::updateMaxGain()
{
	int a, b;
	a = m_vBucket[0]->getSize();
	b = -a;
	for (a; a > b; a--)
	{
		if (m_vBucket[0]->getList(a).size() != 0) {
			m_vBucket[0]->setMax(a);
			/*//cout << "----------" << endl;
			//cout << "partA:" << a << "   " << endl;
			//cout << "partA:" << b << "   " << endl;
			//cout << "getlist" << m_vBucket[0]->getList(a).size() << endl;
			//cout << "----------" << endl;*/
			break;
		}
		m_vBucket[0]->setMax(b);
	}
	a = m_vBucket[0]->getSize();
	b = -a;
	for (a; a > b; a--) {
		if (m_vBucket[1]->getList(a).size() != 0) {
			m_vBucket[1]->setMax(a);
			/*//cout << "----------" << endl;
			//cout << "partB:" << a << "   " << endl;
			//cout << "partB:" << b << "   " << endl;
			//cout << "----------" << endl;*/
			break;
		}
		m_vBucket[1]->setMax(b);
	}
}
void fmPart_C::updateGain(fmNode_C* p_fmNode)
{
	for (int i = 0; i < p_fmNode->getNetNum(); i++) {
		vector<fmNode_C*>temp_fmNodeA;
		vector<fmNode_C*>temp_fmNodeB;
		for (int x = 0; x < p_fmNode->getNet(i)->getNodeNum(); x++) {
			if (p_fmNode->getNet(i)->getNode(x)->getPart() == p_fmNode->ePartA)
				temp_fmNodeA.push_back(p_fmNode->getNet(i)->getNode(x));
			else
				temp_fmNodeB.push_back(p_fmNode->getNet(i)->getNode(x));
		}
		if (p_fmNode->getPart() == p_fmNode->ePartA)
		{
			////cout << "updategainA" << "partBmaxgain" << m_vBucket[1]->getMax() << "Again" << m_vBucket[0]->getMax() << endl;
			if (temp_fmNodeA.size() <= 2 || temp_fmNodeB.size() <= 1) {
				////cout << "*******************************************************************" << endl;
				
				 if (temp_fmNodeA.size() == 1)
					changegain(temp_fmNodeB, 1, p_fmNode->ePartB);
				if (temp_fmNodeA.size() == 2)
					changegain(temp_fmNodeA, -1, p_fmNode->ePartA);
				if (temp_fmNodeB.size() == 0)
					changegain(temp_fmNodeA, -1, p_fmNode->ePartA);
				if (temp_fmNodeB.size() == 1)
					changegain(temp_fmNodeB, 1, p_fmNode->ePartB);
				////cout << "*******************************************************************" << endl;
			}
		}
		else
		{
			////cout << "updategainB" <<"partBmaxgain"<<m_vBucket[1]->getMax()<<"Again"<<m_vBucket[0]->getMax()<< endl;
			if (temp_fmNodeB.size() <= 2 || temp_fmNodeA.size() <= 1) {
				////cout << "*******************************************************************" << endl;
				
				 if (temp_fmNodeB.size() == 1)
					changegain(temp_fmNodeA, 1, p_fmNode->ePartA);
				 if (temp_fmNodeB.size() == 2)
					changegain(temp_fmNodeB, -1, p_fmNode->ePartB);
				 if (temp_fmNodeA.size() == 0)
					changegain(temp_fmNodeB, -1, p_fmNode->ePartB);
				 if (temp_fmNodeA.size() == 1)
					changegain(temp_fmNodeA, 1, p_fmNode->ePartA);
				////cout << "*******************************************************************" << endl;
			}
		}
	}
	remove_node_frombucket(p_fmNode);
	updateMaxGain();
}
void fmPart_C::swapNode(fmNode_C* p_fmnode, int change, fmNode_C::part_E epart)
{
	vector<fmNode_C*> temp;
	if (epart == p_fmnode->ePartA) {
		temp = m_vBucket[0]->getList(p_fmnode->getGain());							//取得含有p_fmnode那個bucket的vector

		for (int x = 0; x < temp.size(); x++) {
			if (temp[x] == p_fmnode) {
				////cout << p_fmnode->getName()<<"  at  "<<p_fmnode->getPart()<< "  from  " << temp[x]->getGain() << "  to  ";
				temp.erase(temp.begin() + x);
				break;
			}
		}
		temp.swap(m_vBucket[0]->getList(p_fmnode->getGain()));
		p_fmnode->addGain(change);
		////cout<<p_fmnode->getGain()<<endl;
		if (p_fmnode->getGain() > m_vBucket[0]->getMax()) {
			m_vBucket[0]->setMax(p_fmnode->getGain());
		}
		temp = m_vBucket[0]->getList(p_fmnode->getGain());
		temp.push_back(p_fmnode);
		temp.swap(m_vBucket[0]->getList(p_fmnode->getGain()));
	}
	else {
		temp = m_vBucket[1]->getList(p_fmnode->getGain());							//取得含有p_fmnode那個bucket的vector
		for (int x = 0; x < temp.size(); x++) {
			if (temp[x] == p_fmnode) {
				////cout << p_fmnode->getName() << "  at  " << p_fmnode->getPart() << "  from  " << temp[x]->getGain() << "  to  ";
				temp.erase(temp.begin() + x);
				break;
			}
		}
		temp.swap(m_vBucket[1]->getList(p_fmnode->getGain()));
		p_fmnode->addGain(change);
			////cout << p_fmnode->getGain() << endl;
		if (p_fmnode->getGain() > m_vBucket[1]->getMax()) {
			m_vBucket[1]->setMax(p_fmnode->getGain());
		}
		temp = m_vBucket[1]->getList(p_fmnode->getGain());
		temp.push_back(p_fmnode);
		temp.swap(m_vBucket[1]->getList(p_fmnode->getGain()));
	}
}
void fmPart_C::changegain(vector<fmNode_C*> p_vfmnode, int change, fmNode_C::part_E epart)
{
	for (int x = 0; x < p_vfmnode.size(); x++)
	{
		if (!p_vfmnode[x]->isLock()) {
			swapNode(p_vfmnode[x], change, epart);
		}
	}
}
void fmPart_C::remove_node_frombucket(fmNode_C* p_fmnode)
{
	if (p_fmnode->getPart() == p_fmnode->ePartA)
	{
		vector<fmNode_C*>temp;
		temp = m_vBucket[1]->getList(p_fmnode->getGain());
		for (int x = 0; x < temp.size(); x++)
		{
			if (temp[x] = p_fmnode)
			{
				////cout<<"                                                              "<<temp[x]->getName();
				temp.erase(temp.begin() + x);
				////cout<<"                                                              "<<temp[x]->getName()<<endl;
				break;
			}
		}
		temp.swap(m_vBucket[1]->getList(p_fmnode->getGain()));
	}
	else
	{
		vector<fmNode_C*>temp;
		temp = m_vBucket[0]->getList(p_fmnode->getGain());
		for (int x = 0; x < temp.size(); x++)
		{
			if (temp[x] = p_fmnode)
			{

				////cout << "remove" << endl;
				temp.erase(temp.begin() + x);
				break;
			}
		}
		temp.swap(m_vBucket[0]->getList(p_fmnode->getGain()));
	}
}
void fmPart_C::moveNode(fmNode_C* p_fmNode)											//把挑選出來的node做交換
{
	m_nowcost = m_nowcost - p_fmNode->getGain();
	////cout << "m_nowcost:" << m_nowcost << endl;
	if (p_fmNode->getPart() == p_fmNode->ePartA) {
		////cout << "movea" << endl;
		A--;
		p_fmNode->setPart(p_fmNode->ePartB);
		for (int x = 0; x < p_fmNode->getNetNum(); x++)
		{
			p_fmNode->getNet(x)->addPart(0, -1);
			p_fmNode->getNet(x)->addPart(1, 1);
		}
	}
	else {
		////cout << "moveb" << endl;
		A++;
		p_fmNode->setPart(p_fmNode->ePartA);
		for (int x = 0; x < p_fmNode->getNetNum(); x++)
		{
			p_fmNode->getNet(x)->addPart(0, 1);
			p_fmNode->getNet(x)->addPart(1, -1);
		}
	}
	p_fmNode->setLock(true);
}
fmNode_C* fmPart_C::choose_Node()													//挑選出要交換的node
{
	int score_Parta = 0;
	int score_Partb = 0;
	if (m_vBucket[0]->getMax() > m_vBucket[1]->getMax())
		score_Parta = score_Parta + 4;
	else if (m_vBucket[0]->getMax() < m_vBucket[1]->getMax())
		score_Partb = score_Partb + 4;
	if (A < m_mapNode.size() - A)
		score_Partb = score_Partb + 2;
	else if (A > m_mapNode.size() - A)
		score_Parta = score_Parta + 2;
	if (m_vBucket[0]->getList(m_vBucket[0]->getMax()).size() > m_vBucket[1]->getList(m_vBucket[1]->getMax()).size())
		score_Parta++;
	else if (m_vBucket[1]->getList(m_vBucket[1]->getMax()).size() > m_vBucket[0]->getList(m_vBucket[0]->getMax()).size())
		score_Partb++;
	//cout << "now gain a" << m_vBucket[0]->getMax() << "  B  " << m_vBucket[1]->getMax() << endl;
	//cout << "scoreA" << score_Parta << "__scoreB" << score_Partb<<endl;
	//cout << "sizeA:___";
	for (int x = m_vBucket[0]->getSize(); x >= -m_vBucket[0]->getSize(); x--)
	{
	//cout  << m_vBucket[0]->getList(x).size()<<" ";
	}
	//cout << "sizeB:___";
	for (int x = m_vBucket[0]->getSize(); x >= -m_vBucket[0]->getSize(); x--)
	{
	//cout << m_vBucket[1]->getList(x).size() << " ";
	}
	//cout << endl;
	if (score_Parta > score_Partb)
	{
		//cout << "A______" << A << endl;
		if (isBalance(fmNode_C::ePartA))
			return m_vBucket[0]->getList(m_vBucket[0]->getMax())[0];
		else
			return m_vBucket[1]->getList(m_vBucket[1]->getMax())[0];
	}
	else
	{
		//cout << "A_______" << A << endl;
		if (isBalance(fmNode_C::ePartB))
			return m_vBucket[1]->getList(m_vBucket[1]->getMax())[0];

		else
			return m_vBucket[0]->getList(m_vBucket[0]->getMax())[0];
	}
}
void fmPart_C::runFM()
{
	int fin = 0;
	fmNode_C *changenode;
	while (fin != m_mapNode.size()) {
		/*//cout << "A";
		m_vBucket[0]->printbucket();
		//cout << "B";
		m_vBucket[1]->printbucket();*/

		////cout << "cutsize: " << m_cost << "con___:" << con << endl;
		changenode = choose_Node();
		////cout << "chosenode" << changenode->getName() << "gain:" << changenode->getGain() << "part:__" << changenode->getPart() << endl;
		moveNode(changenode);
		updateGain(changenode);
		updatecut(changenode);
		if(m_mapNode.size()>10000){
		if (m_vBucket[0]->getMax() < -1 && m_vBucket[1]->getMax() < -1)
			break;
		}
		fin++;
		//cout << "cutsize: " << m_cost <<"con___:"<<con<<"fin" <<fin<<endl;
		////cout << "jump__:" << m_jump << "  con___:" << con << "  fin___:" << fin << " size___:" << m_mapNode.size() << endl;
	}
}
//----------------------------------------------------------------------------------------------------------------------//
  
