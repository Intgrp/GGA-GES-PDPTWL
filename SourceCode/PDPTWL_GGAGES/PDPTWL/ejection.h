#pragma once
#include <cstddef>
#include <list>
#include "uti.h"
#include "Solution.h"
#include "tree.h"
#include "load.h"
//using Pcit=std::list<pthelm>::iterator ;


struct ejectpij_best
{
	int pin, din;//pickup_insert delivery_insert
	//din=0 û�н���insert����
	std::list<pthelm>::iterator itjpn;
	int rindex;//�ڼ���·��
	std::list<int> ejection;//��¼��eject�ĵ�����
	std::list<std::list<pthelm>::iterator> it_ejection;//��eject���Ǹ��Ǹ�Ԫ����ppj�е�λ�õ�����
	
};

class EjectionSearch
{
public:
	Paramenter para;
	int kmax=para.kmax;//���eject kmax��request
	int jd=1;//deliveryλ�� [1,2*kmax+1]
	//order tempod;
	int rindex;
	int itjp_index;//pin��λ�ñ��
	//struct Node;
	std::list<pthelm>::iterator itjp;
	Route* tmpr;
	Solution *pS0;
	std::vector<ejectpij_best> record_eject;//��¼���е�ejectin&insertion��ϣ�ѡpsum��С��
	std::list<pthelm> *ptmp_path;
	std::list<pthelm> tmppath;
	removepool *temporder;
	g_data *g_d;
	EjectionSearch(g_data *g_d):g_d(g_d) {
		//int jd = 1; int Psum = 0; int Pbest = INT_MAX; int kmax = 2;
		kmax = para.kmax;
		record_eject.reserve(2 * kmax + 2); 
	}
	EjectionSearch(int kmax, g_data *g_d)
		:kmax(kmax),
		g_d(g_d)/*,Psum(Psum),Pbest(Pbest)*/{
		record_eject.reserve(2*kmax+2); //��¼ejectin&insertion��ϣ����2*kmax+1��Ԫ�� ��1��ʼ����[1,2*kmx+2]
	}
// 	void insert_route(std::list<pthelm>& ptmp_path, 
// 		removepool & temporder, Route * pr_origin, std::vector<insertpij>& record_insert, int route_index);
	//void lexicographic_iterativebacktrack();
	void eject_route(Node& root/*std::list<pthelm> &ptmp_path, removepool &temporder*/);
	void lexicographic_iterativebacktrack(int n, Node &root);
	//int insert_chck(std::list<pthelm>::iterator it_pthelmp, std::list<pthelm>::iterator it_pthelmd, int increment);
	bool constraint(const Node &head);
	//return 0 ����������ϴ���
	//return -1 �Լ����Ӵ���������
	//return -2 /��֮����ֵ� �� �Լ����ֵܵ��Ӵ�����������
	//retun 1  ��ȷ
	int prune(Node& head, int n, bool increment );
	void traverse(Node& head, int level, int startnum, int endnum);
	void traverse_purn(Node& head, int level, int startnum, int endnum);
	void Search(Node& head, int level, int n);
};

//extern void lexicographic_iterativebacktrack();
