#include <vector>
#include <map>
#include <string>
#include <set>
#include <list>
#include "parser.h"

using std::string;
using std::vector;
using std::map;
using std::set;
using std::list;
class fmNet_C;
class fmNode_C;

//----------------------------------------------------------------------------------------------------------------------//
class fmNode_C
{
    public:
        // Enumerator
        enum part_E {ePartA=0, ePartB=1, ePartC=2, ePartD=3};

        // Constructor
        fmNode_C(string p_strName);
        // Accessor
        string getName();
        part_E getPart();
        int    getGain();
        bool   isLock();
        int    getNetNum();
        fmNet_C* getNet(int);
        list<fmNode_C*>::iterator getIter();
        // Modifier
        void addGain(int);
        void addNet(fmNet_C*);
        void setPart(part_E);
        void setGain(int);
        void setLock(bool);
        void setIter(list<fmNode_C*>::iterator);

    private:
        int m_nGain;
        string m_strName;
        vector<fmNet_C*> m_vNet;
        part_E m_ePart;
        bool m_bLock;
        bool m_bSwap;
        list<fmNode_C*>::iterator m_itList;
};
//----------------------------------------------------------------------------------------------------------------------//
class fmNet_C
{
    public:
        // Constructor
        fmNet_C(string m_strName);
        // Accessor
        string    getName();
        int       getNetWeight();
        int       getNodeNum();
        fmNode_C* getNode(int);
        int       getPart(int);
        // Modifier
        void addNode(fmNode_C*);
        void setPart(int,int);
        void addPart(int,int);
    private:
        string m_strName;						//�u���W��
        vector<fmNode_C*> m_vNode;				//������Node
        int m_nNetWeight;						//�u���v��
        vector<int> m_vPart;					//�b��Ϫ�node�Ӽ�
};
//----------------------------------------------------------------------------------------------------------------------//
class fmBucket_C
{
    public:
        fmBucket_C();
        ~fmBucket_C();
        vector<fmNode_C*>& getList(int p_nId) {
			if (p_nId >= 0) return getPos(p_nId);
			else return getNeg(p_nId);
		}
		vector<fmNode_C*>& getNeg(int p_nId) { return m_vNegative[-p_nId]; };
        vector<fmNode_C*>& getPos(int p_nId) { return m_vPositive[p_nId]; };
		void pushNode(fmNode_C*);
        void clear();
        void setSize(int);
		void setMax(int);
		void printbucket();
        int getMax() {return m_nMax;}
		int getSize() { return m_nsize; }
    private:
        vector< vector<fmNode_C*> > m_vPositive;
        vector< vector<fmNode_C*> > m_vNegative;
        int m_nMax;									//��bucket�� �ثe��Node���̤jgain��
		int m_nsize;								//gain�̤j�i�H�h��
};
//----------------------------------------------------------------------------------------------------------------------//
class fmPart_C
{
    public:
        // Constructor
        fmPart_C(parser_C*);											//�]�t��l��bucket
        void addNet(string p_strNet, vector<string>& p_rvNode);

        //
        void runFM();
        void onePass();
        // Function
        void initPart();												//��l�ƦU��node�b����bucket
        void setPart(vector<string>&, vector<string>&);
        void dump();

        int refresh(int);
		bool updatecut(fmNode_C*);											//movenode���scut�Ʀr
		int  getCut() { return m_cost; };								//�o��cut number
        void calcGain();												//��l��gain
        void calcPartNum(fmNet_C*,int&,int&,fmNode_C*&,fmNode_C*&);
        void initBucket();												//�N��l�Ƨ�gain�᪺Node��Jbucket
        void buildBucket();
		bool isBalance() ;		
		bool morebalance(fmNode_C* p_node);
		bool isBalance(fmNode_C::part_E);								//�洫��O�_balance
		bool diff_cut();								//�O�_�����p��cutsize
        void moveNode(fmNode_C*);
        void updateMaxIter(int,int,int&,int&);
        void updateGain(fmNode_C*);								//�洫node�� ��sGain
        void updateMaxGain();											//��s����̰���gain
        void savePart(vector<fmNode_C::part_E>&);
		void let_balance();												//��l��balance
        void restorePart(vector<fmNode_C::part_E>&);
		void changegain(vector<fmNode_C*>, int, fmNode_C::part_E);		//�N�����n�n�@��n���gain��vector�@���ʪ��@swapnode
        void swapNode(fmNode_C* ,int, fmNode_C::part_E);				//�N������gain��node �bbucket�����洫
		void remove_node_frombucket(fmNode_C*);							//����move����node������bucket
        void printInfo();
		fmNode_C* choose_Node();
        //
        void dumpBucket(int);
    private:
        //
        map<string,fmNode_C*> m_mapNode;
        map<string,fmNet_C*>  m_mapNet;
        //
        vector<fmNode_C*>        m_vNode;
        vector<fmNet_C*>         m_vNet;
        vector<fmBucket_C*>      m_vBucket;
        vector<int>              m_vMaxGain;	//�����U��bucket�����̰�gain
        vector<int>              m_vSize;		
		vector<int>				 m_vpart;		//�����̦n��cut�C��node�b�����part
		vector<vector<int> >		 m_vvpart;		
		vector<vector<int> >		 m_temppart;
		vector<int>				 m_vcost;
		vector<int>				 m_tempcost;
		int						 m_cost;
		int						 m_nowcost;		//�����{���q��cost
		int						 A;				//partA���X��Node
		int						 m_jump;
        double					 m_dBalance;
};
//----------------------------------------------------------------------------------------------------------------------//

