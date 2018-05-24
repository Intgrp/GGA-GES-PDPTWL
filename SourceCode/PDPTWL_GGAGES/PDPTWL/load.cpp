//#include "stdafx.h"
#include <stdio.h>
#include <string>
#include <string.h>
#include <sstream>
#include <iostream>
#include <math.h>
#include <set>
#include <vector>
#include <list>

#include "load.h"
#include"Solution.h"

#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
using namespace std;

int g_data::load(const char *openfile)
{
	//FILE *f1 = _fsopen(openfile, "r", _SH_DENYNO);
	FILE *f1 = fopen(openfile, "r");
	if (!f1)
	{
		printf("file f1 read error! %s\n", openfile);
		return 0;
	}

	fscanf(f1, "%d %d %d", &vnum, &cap, &speed);
	if (speed!=1)
	{
		speed = 1;
		
	}

	customer *c = allcustomer;
	while (fscanf(f1, "%d %d %d %d %d %d %d %d %d",
		&c->cusid, &c->x, &c->y, &c->demand, &c->etime, &c->ltime, &c->stime, &c->pid, &c->did) > 0)

	{
		if (c->cusid == 0)//depote
		{
			c->p = 2;
			c->d = 2;
		}
		if (c->demand > 0)
		{
			c->p = 1;
			c->d = 0;
		}
		else
		{
			c->p = 0;
			c->d = 1;
		}
		c++;
		cus_num++;
	}
	//creat_order();

	//������룬���������ص�֮��ľ���,pickup��delivery֮��ľ���
	for (int i = 0; i <= cus_num; i++)//0��ʾ�ֿ�
	{
		for (int j = 0; j <= i; j++)
		{
			dist[i][j] = sqrt(pow((allcustomer[i].x - allcustomer[j].x), 2) + pow((allcustomer[i].y - allcustomer[j].y), 2));
			dist[j][i] = dist[i][j];
			// 			if (dist[i][j]>maxdist)
			// 			{
			// 				maxdist = dist[i][j];
			// 			}
		}
	}
	fclose(f1);
	//::dist = this->dist;
	//::cap = this->cap;
	creat_order();
	return 1;
}



void g_data::creat_order()//�����������
{
	//od.reserve(g_d->cus_num / 2 + 1);

	//auto& pdd = g_d->pdd;
	pdd[pdd_index].id = pdd_index;
	pdd[pdd_index].pp = &allcustomer[0];//ָ�룬ȡֵ����
	pdd[pdd_index].pd = &allcustomer[0];
	allcustomer[0].porder = &pdd[pdd_index];
	//	allcustomer[allcustomer[i].did].porder = &pdd[pdd_index];
	//od.push_back(pdd + pdd_index);

	for (int i = 1; i < cus_num; i++)//��1��ʼ��0�ǲֿ⣬������order��� cus_num=107
	{
		if (allcustomer[i].demand > 0)
		{
			pdd_index++;//������Ŵ�1��ʼ������
			pdd[pdd_index].id = pdd_index;
			pdd[pdd_index].pp = &allcustomer[i];//ָ�룬ȡֵ����
			pdd[pdd_index].pd = &allcustomer[allcustomer[i].did];
			allcustomer[i].porder = &pdd[pdd_index];
			allcustomer[allcustomer[i].did].porder = &pdd[pdd_index];
			//od.push_back(pdd + pdd_index);
		}
	}
	pdd_index++;
	//cout <<"d";
}

// void creat_order( )//�����������
// { //vector<order*> pd;
//   //order *pd = new order[cus_num / 2];
// 	int pd_index = 0;
// 	for (int i = 1; i <= cus_num; i++)//��1��ʼ��0�ǲֿ⣬������order���
// 	{
// 		if (allcustomer[i].demand > 0)
// 		{
// 			pd_index++;//������Ŵ�1��ʼ������
// 			pd[pd_index].id = pd_index;
// 			pd[pd_index].pp = &allcustomer[i];//ָ�룬ȡֵ����
// 			pd[pd_index].pd = &allcustomer[allcustomer[i].did];
// 			allcustomer[i].porder = &pd[pd_index];
// 			allcustomer[allcustomer[i].did].porder = &pd[pd_index];
// 			//s.cheatod.push_back(pd + pd_index);
// 		}
//
// 	}
// }
int Solution::updata(Riterator itr) {
	for (auto thss = itr->path.begin(); thss != itr->path.end(); thss++)//lst��һ�ڵ㣬 ths��һ�ڵ�
	{
		itr->snum = snum;
		thss->proute = itr;//�ó�Ա�������ڵĶ��� Route
	}
	return itr->updata();
}

/********************************************
* ��������: Route::update
* ����˵��: �����·����violation��Ŀ��dis, twv, cv
* ����˵��: this.path         //customer,depot info
* ��������: None
����ÿ�� ·��Ԫ��pthelm �ĵ���ʱ�� �뿪ʱ�� �뿪ʱcost
Route::path
***************************************************/
int Route::updata()
{
	stack<int> slifo;
	double distanc = 0;
	cv = 0; twdiff = 0.0; twwait = 0.0, twv = 0, tvt = 0.0;
	path_length = 0;
	if (path.empty()) return 0;
	int Load = 0;
	customer* pthsc;

	//using Pit = std::list<pthelm>::iterator;
	std::list<pthelm>::iterator lst, ths;
	lst = path.begin();

	pthsc = lst->pcus;
	//dis = 0;
	dis = dist[0][pthsc->cusid];//��0�ֿ⿪ʼ ��0�ֿ⵽·����һ��custom֮��ľ���
	if (dis < pthsc->etime)
	{
		twwait += (pthsc->etime - dis);
		dis = pthsc->etime;
		//����ʱ�䣿������������������������������������������������������
	}//else����ֱ�ӷ�������ȴ�dis=dis��

	lst->st = dis;
	dis += pthsc->stime;
	lst->et = dis;//pthelm::et �뿪ʱ��
	Load += pthsc->demand;
	lst->eld = Load;//pthelm::eld�뿪ʱ����
	lst->lcost = cost();//dis+100twv+1000*cv��·��Ԫ�ص�pthelm::lcost
						//һ��orderָ������customer��������pthelm���ֱ���pp pd  �໥��������
						//pthsc->p == 1 ? lst->porder->pelmp[snum] = &*lst : lst->porder->pelmd[snum] = &*lst;///////////////////////////////////
	pthsc->p == 1 ? lst->porder->pelmp[snum] = lst : lst->porder->pelmd[snum] = lst;///////////////////////////////////

	path_length++;

	for (ths = ++path.begin(); ths != path.end(); ths++, lst++)//lst��һ�ڵ㣬 ths��һ�ڵ�
	{
		pthsc = ths->pcus;
		//#define CalculateDistance(a,b) (g_dmat[a->locid][b->locid])
		ths->disfrmlst = dist[(lst->pcus)->cusid][pthsc->cusid];//CalculateDistance((lst->pc), pthsc);//����һ�㵽��һ���ʱ��
		distanc += dist[(lst->pcus)->cusid][pthsc->cusid];
		dis += ths->disfrmlst;
		tvt += ths->disfrmlst;//travel time
		ths->st = dis;//����ʱ�� 
		if (dis <= pthsc->etime)
		{
			twwait += pthsc->etime - dis;
			dis = pthsc->etime;
		}
		else if (dis > pthsc->ltime)
		{
			return 0;
		}

		dis += pthsc->stime;
		ths->et = dis;//pthelm::et
		Load += pthsc->demand;
		ths->eld = Load;//pthelm::eld�뿪ʱ����
		if (Load > MAXCAP)
		{
			return 0;
		}
		if (Load > MAXCAP || Load < 0)	cv += 1;
		ths->lcost = cost();//pthelm::lcost �뿪ʱ����  dis+100twv+1000*cv
							//pthsc->p == 1 ? (*ths->porder).pelmp[snum] = &*ths : (*ths->porder).pelmd[snum] = &*ths;
		pthsc->p == 1 ? (*ths->porder).pelmp[snum] = ths : (*ths->porder).pelmd[snum] = ths;
		if (pthsc->p == 1 && pthsc->cusid != 0)//pickup ��ջ
		{
			slifo.push(pthsc->cusid);
		}
		if (pthsc->d == 1 && pthsc->cusid != 0)//delivery ��ջ
		{
			if (slifo.empty())
			{
				return 0;
			}
			if (slifo.top() == pthsc->pid)
			{
				slifo.pop();//ɾ��ջ��
			}
			else
			{
				return 0;//infeasible
			}

		}
		path_length++;

	}
	if (!slifo.empty())
	{
		return 0;
	}

	dis += dist[0][pthsc->cusid];//���ֿ�0���� ���һ��custom���ֿ�0֮��ľ���
	distanc += dist[0][pthsc->cusid];//�������ȴ�ʱ��

	route_cost = cost();
	route_dist = distanc;
	return 1;

}

void Solution::updata(Riterator itr, int &violation) {
	for (auto thss = itr->path.begin(); thss != itr->path.end(); thss++)//lst��һ�ڵ㣬 ths��һ�ڵ�
	{
		thss->proute = itr;//�ó�Ա�������ڵĶ��� Route
	}
	itr->updata(violation);
}
void Route::updata(int &violation)//û��LIFO��� û�гɶԼ��
{
	double distanc = 0;
	cv = 0; twdiff = 0.0; twwait = 0.0, twv = 0, tvt = 0.0;
	path_length = 0;
	dis = 0;
	if (path.empty()) return;
	int Load = 0;
	customer* pthsc;

	//using Pit = std::list<pthelm>::iterator;
	std::list<pthelm>::iterator lst, ths;
	lst = path.begin();
	//lst->proute = this;//�ó�Ա�������ڵĶ��� Route
	pthsc = lst->pcus;
	//dis = 0;
	dis = dist[0][pthsc->cusid];//��0�ֿ⿪ʼ ��0�ֿ⵽·����һ��custom֮��ľ���
	lst->st = dis;//����ʱ��
	if (dis <= pthsc->etime)
	{
		twwait += (pthsc->etime - dis);
		dis = pthsc->etime;

	}//else����ֱ�ӷ�������ȴ�dis=dis��

	dis += pthsc->stime;
	lst->et = dis;//pthelm::et �뿪ʱ��
	Load += pthsc->demand;
	lst->eld = Load;//pthelm::eld�뿪ʱ����
	lst->lcost = cost();//dis+100twv+1000*cv��·��Ԫ�ص�pthelm::lcost
						//һ��orderָ������customer��������pthelm���ֱ���pp pd  �໥��������
						//pthsc->p == 1 ? lst->porder->pelmp[snum] = &*lst : lst->porder->pelmd[snum] = &*lst;
	pthsc->p == 1 ? lst->porder->pelmp[snum] = lst : lst->porder->pelmd[snum] = lst;
	path_length++;

	for (ths = ++path.begin(); ths != path.end(); ths++, lst++)//lst��һ�ڵ㣬 ths��һ�ڵ�
	{
		//ths->proute = this;
		pthsc = ths->pcus;
		//#define CalculateDistance(a,b) (g_dmat[a->locid][b->locid])
		ths->disfrmlst = dist[(lst->pcus)->cusid][pthsc->cusid];//CalculateDistance((lst->pc), pthsc);//����һ�㵽��һ���ʱ��
		distanc += dist[(lst->pcus)->cusid][pthsc->cusid];
		dis += ths->disfrmlst;
		tvt += ths->disfrmlst;//travel time
		ths->st = dis;//����ʱ��
		if (dis < pthsc->etime)
		{
			twwait += pthsc->etime - dis;
			dis = pthsc->etime;
		}
		else if (dis > pthsc->ltime)
		{
			twdiff += dis - pthsc->ltime;//soft
			twv += 1;//hard
					 //����ȴ� ֱ�ӿ�ʼ dis=dis;
		}
		dis += pthsc->stime;
		ths->et = dis;//pthelm::et
		Load += pthsc->demand;
		ths->eld = Load;//pthelm::eld�뿪ʱ����
		if (Load > MAXCAP || Load < 0)	cv += 1;
		ths->lcost = cost();//pthelm::lcost �뿪ʱ����  dis+100twv+1000*cv
							//	pthsc->p == 1 ? (*ths->porder).pelmp[snum] = &*ths : (*ths->porder).pelmd[snum] = &*ths;
		pthsc->p == 1 ? (*ths->porder).pelmp[snum] = ths : (*ths->porder).pelmd[snum] = ths;

		path_length++;
	}
	dis += dist[0][pthsc->cusid];//���ֿ�0���� ���һ��custom���ֿ�0֮��ľ���
	distanc += dist[0][pthsc->cusid];
	route_cost = cost();
	route_dist = distanc;
	violation = twv + cv;
}

int Route::updata_lcheck(double &coss)const
{
	stack<int> slifo;
	if (path.empty()) return 0;
	int Load = 0;
	double diss = 0;
	double twait = 0;
	double distanc;
	customer* pthsc;

	std::list<pthelm>::const_iterator lst, ths;
	lst = path.cbegin();

	pthsc = lst->pcus;

	diss = dist[0][pthsc->cusid];//��0�ֿ⿪ʼ ��0�ֿ⵽·����һ��custom֮��ľ���
	distanc = dist[0][pthsc->cusid];
	if (diss < pthsc->etime)
	{
		twait += (pthsc->etime - diss);
		diss = pthsc->etime;

	}//else����ֱ�ӷ�������ȴ�dis=dis��


	diss += pthsc->stime;

	Load += pthsc->demand;

	for (ths = ++path.cbegin(); ths != path.cend(); ths++, lst++)//lst��һ�ڵ㣬 ths��һ�ڵ�
	{
		pthsc = ths->pcus;

		diss += dist[(lst->pcus)->cusid][pthsc->cusid];
		distanc += dist[(lst->pcus)->cusid][pthsc->cusid];

		if (diss <= pthsc->etime)
		{
			twait += (pthsc->etime - diss);
			diss = pthsc->etime;
		}
		else if (diss > pthsc->ltime)
		{
			return 0;
		}

		diss += pthsc->stime;

		Load += pthsc->demand;

		if (Load > MAXCAP)
		{
			return 0;
		}
		if (pthsc->p == 1 && pthsc->cusid != 0)//pickup ��ջ
		{
			slifo.push(pthsc->cusid);
		}
		if (pthsc->d == 1 && pthsc->cusid != 0)//delivery ��ջ
		{
			if (slifo.empty())
			{
				return 0;
			}
			if (slifo.top() == pthsc->pid)
			{
				slifo.pop();//ɾ��ջ��
			}
			else
			{
				return 0;//infeasible
			}

		}
	}
	if (!slifo.empty())
	{
		return 0;
	}
	coss = cost(distanc, twait);
	return 1;
}

int Route::updata_check(int violation)const//û��LIFO��� û�гɶԼ��
{
	if (path.empty()) return 0;
	int Load = 0;
	double diss = 0;
	customer* pthsc;

	std::list<pthelm>::const_iterator lst, ths;
	lst = path.cbegin();

	pthsc = lst->pcus;

	diss = dist[0][pthsc->cusid];//��0�ֿ⿪ʼ ��0�ֿ⵽·����һ��custom֮��ľ���
	if (diss < pthsc->etime)
	{
		diss = pthsc->etime;
	}//else����ֱ�ӷ�������ȴ�dis=dis��

	diss += pthsc->stime;
	Load += pthsc->demand;
	for (ths = ++path.cbegin(); ths != path.cend(); ths++, lst++)//lst��һ�ڵ㣬 ths��һ�ڵ�
	{
		pthsc = ths->pcus;
		diss += dist[(lst->pcus)->cusid][pthsc->cusid];
		if (diss <= pthsc->etime)
		{
			diss = pthsc->etime;
		}
		else if (diss > pthsc->ltime)
		{
			return 0;
		}
		diss += pthsc->stime;

		Load += pthsc->demand;

		if (Load > MAXCAP)
		{
			return 0;
		}

	}

	return 1;
}

void Route::lastpossible()
{
	//int Etime = 0, Ltime = INT_MAX;//depote�Ŀ�ʼʱ�� ����ʱ�� ��һ�������ʱ��
	if (path.empty()) return;
	std::list<pthelm>::iterator lst, ths;
	ths = prev(path.end());//���һ��·��Ԫ�� depote 0
						   //ths->proute = this;
	customer* pthsc;
	pthsc = ths->pcus;
	ths->lpt = pthsc->ltime;//zi=lpt
	lst = prev(ths);
	//pthsc = ths->pcus;
	double a;
	while (1)
	{
		a = (ths->lpt - dist[ths->pcus->cusid][lst->pcus->cusid] - lst->pcus->stime);
		lst->lpt = min(a, lst->pcus->ltime);
		if (lst == path.begin()) break;
		--lst;
		--ths;

	}
}

/*
int Route::HillClimbing(int thrHill)
{

std::vector<order*> vo;
vo.reserve(path.size());//һ��·���ϵ�����path����  order
// 		for (auto it=path.begin();it!=path.end();it++)
// 		{
// 			vo.push_back((*it).porder);
// 		}
for (auto &it : path)
{
vo.push_back(it.porder);
}
//updata ����·����Ϣ��dis twv cv twdiff
updata();
double mincost = cost();
double newcost = mincost;
bool changed = 0;
do
{
changed = 0;
for (auto iti = vo.begin(); iti != vo.end(); iti++)
{
for (auto itj = next(iti); itj != vo.end(); itj++)
{
auto oi = *iti;
customer* pci = iti == std::find(vo.begin(), vo.end(), oi) ? oi->pp : oi->pd;//��һ��Ϊpp���ڶ���Ϊpd�����ص�һ���ҵ���λ��
auto oj = *itj;
customer* pcj = itj == std::find(vo.begin(), vo.end(), oj) ? oj->pp : oj->pd;
if (oi != oj && (pcj->ltime < pci->etime))//CalculateDistance(pcj,vo[0]->pp)<CalculateDistance(pci, vo[0]->pp)
{
std::swap(*iti, *itj);

//updata();//ֻ�ܸ��¼���Route::path�е�����
newcost = getcostonly(vo);//����vector<order*> vo �е�����

if (newcost < mincost)
{
mincost = newcost;
changed = 1;
}
else
{
std::swap(*iti, *itj);
}

}
}
}

} while (changed == true);//stop when no improvement achieved in the previous pass


//judge the route infeasible or feasible
// cv>0 or twv>0  hard time window
// cv>0 or twdiff>��ֵ  soft time window
// Route::path2--vo-------ptmp--Route::path2
// if (changed_total==0) ����Ҫ���²���

std::list<pthelm> ptmp;
std::set<order*> so;////����� ƽ������� �������� �����Ըı��ֵ
for (auto &it : vo)
{
if (so.find(it) != so.end())//�ҵõ���˵��pp order�Ѿ����ȥ��
{
ptmp.emplace_back(pthelm{ it->pd,it,this });
}
else
{
so.insert(it);
ptmp.emplace_back(pthelm{ it->pp,it,this });
}
}
path.swap(ptmp);
if (updata() == 0)//��Route::path updata
//if (cv > 0 || twv > 0)//infeasible
{
path.swap(ptmp);
return 1;
}
else
{
return 0;//feasible
}
}*/
double Route::cost() const
{
	//return dis / twwait;
	return route_dist + twwait;//hard time window
							
}
double Route::cost(double diss, double twait) const
{
	//return diss / twait;
	return diss + twait;
	
}

// double Route::getcostonly(std::vector<order*> &vo)const//����
// {
// 
// 	double dis = 0.0, twwait = 0.0;
// 	int capload = 0;
// 	double twdiff = 0.0;
// 	int twv = 0, cv = 0;
// 
// 	std::vector<order*>::iterator lst, ths;
// 	{//�׽ڵ�
// 	 /*		so.insert(vo[0]);*/
// 		if (dis<vo[0]->pp->etime)
// 		{
// 
// 			twwait += vo[0]->pp->etime - dis;
// 			dis = vo[0]->pp->etime;
// 		}
// 		dis += vo[0]->pp->stime;
// 		capload += vo[0]->pp->demand;
// 
// 	}
// 	int i = 1;
// 	int lst_cusid;
// 	int ths_cusid;
// 	customer* ths_cus;
// 	lst = vo.begin();
// 	lst_cusid = std::find(vo.begin(), vo.end(), vo[0]) == lst ? vo[0]->pp->cusid : vo[0]->pd->cusid;//
// 	for (ths = ++vo.begin(); ths != vo.end(); ths++, i++)
// 	{
// 
// 		ths_cus = std::find(vo.begin(), vo.end(), vo[i]) == ths ? vo[i]->pp : vo[i]->pd;//
// 		ths_cusid = ths_cus->cusid;
// 		dis += dist[lst_cusid][ths_cusid];//travel time
// 		lst_cusid = ths_cusid;
// 		if (dis<ths_cus->etime)
// 		{
// 			twwait += ths_cus->etime - dis;
// 			dis = ths_cus->etime;
// 		}
// 		else if (dis > ths_cus->ltime)
// 		{
// 			twv++;
// 			twdiff += dis - ths_cus->ltime;
// 		}
// 		dis += ths_cus->stime;
// 		capload += ths_cus->demand;
// 		if (capload>MAXCAP)
// 		{
// 			cv++;//ֻ��һ��·�� cv���Ϊ1
// 		}
// 	}
// 
// 	return (dis)+(1000 * twv) + (2000 * cv);/////////////////////cost
// 
// }
// double Route::getcostonly(std::list<pthelm>::const_iterator &iterp, std::list<pthelm>::const_iterator &iterd) const////���ܸı�����ָ���Ԫ�ص�ֵ
// {
// 	//һ��·��route��ɾ��iterp��iterd��������ָ���  pthelm  ���cost
// 	if (path.size() == 0) return 0;
// 	double dis = 0.0, twwait = 0.0;
// 	int capload = 0;
// 	double twdiff = 0.0;
// 	int twv = 0, cv = 0;
// 
// 
// 	std::list<pthelm>::const_iterator lst, ths;
// 	{//�׽ڵ�
// 		lst = path.begin();
// 		while (lst == iterp || lst == iterd) lst++;
// 		if (lst == path.end()) return 0;
// 
// 		if (dis < lst->pcus->etime)
// 		{
// 
// 			twwait += lst->pcus->etime - dis;
// 			dis = lst->pcus->etime;
// 		}
// 		if (dis>lst->pcus->ltime)
// 		{
// 			twv += 1;
// 			twdiff += (dis - lst->pcus->ltime);
// 		}
// 		dis += lst->pcus->stime;
// 		capload += lst->pcus->demand;
// 
// 	}
// 
// 	int lst_cusid;
// 	int ths_cusid;
// 	customer* ths_cus;
// 
// 	//	lst_cusid = std::find(vo.begin(), vo.end(), vo[0]) == lst ? vo[0]->pp->cusid : vo[0]->pd->cusid;//
// 	for (ths = ++path.begin(); /*ths != vo.end()*/; ths++, lst++)
// 	{
// 
// 		while (ths == iterp || ths == iterd) ths++;
// 		while (lst == iterp || lst == iterd) lst++;
// 		if (ths == path.end()) break;
// 
// 		//ths_cus = std::find(vo.begin(), vo.end(), vo[i]) == ths ? vo[i]->pp : vo[i]->pd;//
// 		ths_cusid = ths->pcus->cusid;
// 		lst_cusid = lst->pcus->cusid;
// 		dis += dist[lst_cusid][ths_cusid];//travel time
// 										  //lst_cusid = ths_cusid;
// 		ths_cus = ths->pcus;
// 		if (dis < ths_cus->etime)
// 		{
// 			twwait += ths_cus->etime - dis;
// 			dis = ths_cus->etime;
// 		}
// 		else if (dis > ths_cus->ltime)
// 		{
// 			twv++;
// 			twdiff += dis - ths_cus->ltime;
// 		}
// 		dis += ths_cus->stime;
// 		capload += ths_cus->demand;
// 		if (capload > cap)
// 		{
// 			cv++;//ֻ��һ��·�� cv���Ϊ1
// 		}
// 	}
// 
// 	return (dis)+(1000 * twv) + (2000 * cv);/////////////////////cost
// 
// }

double Route::getcostonly() const ////���ܸı�����ָ���Ԫ�ص�ֵ
{

	if (path.size() == 0) return 0;
	double dis = 0.0, twwait = 0.0;
	int q = 0;
	double distanc = 0.0;
	std::list<pthelm>::const_iterator lst, ths;
	{//�׽ڵ�
		lst = path.begin();

		if (lst == path.end()) return dis;

		if (dis < lst->pcus->etime)
		{

			twwait += lst->pcus->etime - dis;
			dis = lst->pcus->etime;
		}

		dis += lst->pcus->stime;
		q += lst->pcus->demand;

	}

	int lst_cusid;
	int ths_cusid;
	customer* ths_cus;

	for (ths = ++path.begin(); /*ths != vo.end()*/; ths++, lst++)
	{

		if (ths == path.end()) break;


		ths_cusid = ths->pcus->cusid;
		lst_cusid = lst->pcus->cusid;
		dis += dist[lst_cusid][ths_cusid];//travel time
		distanc += dist[lst_cusid][ths_cusid];
		ths_cus = ths->pcus;
		if (dis < ths_cus->etime)
		{
			twwait += ths_cus->etime - dis;
			dis = ths_cus->etime;
		}

		dis += ths_cus->stime;
		q += ths_cus->demand;

	}

	return (cost(distanc, twwait));/////////////////////cost

}

