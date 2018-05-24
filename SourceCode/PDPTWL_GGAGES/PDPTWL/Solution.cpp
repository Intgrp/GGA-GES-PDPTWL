//

//#include "stdafx.h"
//#pragma once
#include <set>
#include <vector>
#include <map>
#include <unordered_map>
#include <string.h>
#include <stack>
#include "load.h"
#include "Solution.h"
using namespace std;
void Solution::construction_sequential()
{
	/**************************��ԭʼ�ĳ�ʼ·����һ��·��һ��order************************/
	for (int i = 1; i <g_d->pdd_index; i++)//��1��ʼ
	{
		Route tmp(g_d,snum);
		tmp.no = i;
		tmp.path = {
				pthelm(od[0].pp, od),
				pthelm(od[i].pp, &od[i]),
				pthelm(od[i].pd, &od[i]),
				pthelm(od[0].pd, od) 
		};

		R.emplace_back(tmp);
		updata(prev(R.end()));
	}
}


void Solution::checkR(size_t removesize, bool istwv)
{
	int totalp = 0;
	for (auto it = R.begin(); it != R.end(); it++)
	{
		int rr = updata(it);

		if (rr == 0)
		{
			my_ERROR
		}
		int cd = 0;
		auto itd = (*it).path.end();
		itd--;
		if (itd->pcus->cusid != 0)
		{
			my_ERROR
				g_d->each_problem_output << "route error11!!!\n";
		}

		for (auto iit = next((*it).path.begin()); iit != itd; iit++)
		{
			totalp++;
			if (iit->proute != (it))
			{
				my_ERROR
					g_d->each_problem_output << "route error12!!!\n";
			}
			if (iit->pcus->cusid == 0)
			{
				cd++;
			}
			if (cd != 0)
			{
				my_ERROR
			}
		}
	}
	if (istwv == 1)
	{
		for (auto it = R.begin(); it != R.end(); it++)
		{
			if ((*it).twv != 0 || (*it).cv != 0)
			{
				for (auto u = (*it).path.begin(); u != (*it).path.end(); u++)
				{
					printf("%d ", u->pcus->cusid);
				}
				printf("route error!!!");
				my_ERROR
					g_d->each_problem_output << "route error13!!!\n";
				//offset += sprintf(each_problem_buff + offset, "route error13!!!");
			}
		}

	}
	if (totalp != (g_d->cus_num - 1 - 2 * static_cast<int>(removesize)))
	{
		printf("route error!!!");
		my_ERROR
			g_d->each_problem_output << "route error14!!!\n";
		//offset += sprintf(each_problem_buff + offset, "route error14!!!");
	}
	total_cost = 0;
	total_dist = 0;
	for (auto it = R.begin(); it != R.end(); it++)
	{
		total_cost += (*it).route_cost;
		total_dist += (*it).route_dist;
	}
}


	constexpr bool xiaoyu(const pair< double, std::list<Route>::iterator>& _Left,
		const pair< double, std::list<Route>::iterator>& _Right)
{	 
	return (_Left.first < _Right.first );
}
	constexpr bool dayu(const double _Left,
		const double & _Right)
	{
		return (_Left > _Right);
	}
void cross(const Solution &s1,Solution &s2 )
{
	int s1snum=s2.snum;
	std::list<Route>::iterator r1, r2;
	std::list<Route>::iterator e1, l1;
	std::list<pthelm>::iterator p1, p2;
	Solution s12(s1, SN + 1);
	int sRnum = s12.R.size();
	//ѡ�����ڽ���� route
	int snum = 0;//ѡ�񼸸��أ�
	if ((int)(s2.g_d->mostp*sRnum)==0 || (int)(s2.g_d->leastp*sRnum)==0)
	{
		snum = 1;
	}
	else
	snum = rand() % (int)(s2.g_d->mostp*sRnum) + (int)(s2.g_d->leastp*sRnum);//[5%-15%]
	if (snum<=0)//������ LNS ����distance
	{
		//ѡrequest ��� ���ѡ
// 		s2.order_perturb(100);
// 		s2.checkR(0, 1);//����ע��
		snum = 1;
	}
	else
	{//�������


		//ѡ��õ� distance/path.size()  
		vector<pair< double, std::list<Route>::iterator>> scor;
		scor.reserve(sRnum);
		scor.clear();
		r1 = s12.R.begin();
		for (int i = 0; i < sRnum; i++)
		{
			scor.emplace_back(make_pair((r1->route_cost) / (double)r1->path_length, r1));
			advance(r1, 1);
		}
		sort(scor.begin(), scor.end(), xiaoyu);
		int goodnum = (int)snum*s2.g_d->goodcross + 1;//����ȡ��
		for (int i = 0; i < goodnum; i++)/////////////////////////////////////////////////////////
		{
			auto it = scor[i].second;
			for (auto itt = next(it->path.begin()); itt != prev(it->path.end()); itt++)
			{
				if (itt->pcus->p == 1)
				{
					list<pthelm>::iterator p = itt->porder->pelmp[s2.snum];
					list<pthelm>::iterator d = itt->porder->pelmd[s2.snum];
					p->proute->path.erase(p);//erase��p���

					d->proute->path.erase(d);
				}
			}
			r2 = s2.R.begin();
			advance(r2, rand() % s2.R.size());
			s2.R.splice(r2, s12.R, it);//����Ҹ�λ��insert��ȥ
		}
		//s2.snum = s1snum;
		s2.checkR(0, 1);

		//���ѡ�񼸸���
		for (int i = 0; i < (int) snum-goodnum; i++)
		{
			auto it = s12.R.begin();
			const int  a = rand() % s12.R.size();
			advance(it, a);

			for (auto itt = next(it->path.begin()); itt != prev(it->path.end()); itt++)
			{
				if (itt->pcus->p == 1)
				{
					list<pthelm>::iterator p = itt->porder->pelmp[s2.snum];
					list<pthelm>::iterator d = itt->porder->pelmd[s2.snum];
					p->proute->path.erase(p);//erase��p���

					d->proute->path.erase(d);
				}
			}
			r2 = s2.R.begin();
			advance(r2, rand() % s2.R.size());
			s2.R.splice(r2, s12.R, it);//����Ҹ�λ��insert��ȥ
		}
		for (auto it = s2.R.begin(); it != s2.R.end(); )
		{
			if (it->path.size() == 2)
			{
				s2.R.erase(it++);
			}
			else
			{
				it++;
			}
		}

		s2.snum = s1snum;
		s2.checkR(0, 1);

		//��s2ѡ����eject
			//ÿ��ejectһ��·������������һ��

		vector<double> scorr;
		sRnum = s2.R.size();
		scorr.reserve(sRnum);
		for (int i = 0; i < (int) (snum * s2.g_d->ejmut); i++)//*1.5 ʵ����snum*3
		{
			for (int h=0;h<s2.g_d->badmut;h++)
			{
				//bad1
				sRnum = s2.R.size();
				scorr.clear();
				r1 = s2.R.begin();
				for (int i = 0; i < sRnum; i++)
				{

					scorr.push_back((r1->route_cost) / (double)r1->path_length);
					advance(r1, 1);
				}
				vector<double>::iterator its = max_element(scorr.begin(), scorr.end());
				auto it = s2.R.begin();
				advance(it, its - scorr.begin());
				s2.ejection_cross(it, 1);

			}
			
		
		
		
		//���ѡ
		auto itr = s2.R.begin();
		const int  a = rand() % s2.R.size();
		advance(itr, a);
		s2.ejection_cross(itr,0);
		}
	}
}


void cross_route(const Solution &s1, Solution &s2)
{
	std::list<Route>::iterator r1, r2;
	std::list<Route>::iterator e1, l1;
	std::list<pthelm>::iterator p1, p2;
	Solution s12(s1, SN + 1);
	r1 = s12.R.begin();

		advance(r1, rand() % s12.R.size());
//	}
	
		
	e1 = r1;
	l1 = next(e1, 1);
	if (l1!=s12.R.end())
	{
		if (rand() / (double)RAND_MAX <= 0.5)
		{
			l1 = next(e1, 1);
		}
	}
	
	


	for (auto it = e1; it != l1; it++)//r1:size()-1
	{
		for (auto itt = next(it->path.begin()); itt != prev(it->path.end()); itt++)
		{
			if (itt->pcus->p == 1)
			{

				list<pthelm>::iterator p = itt->porder->pelmp[s2.snum];
				list<pthelm>::iterator d = itt->porder->pelmd[s2.snum];
				p->proute->path.erase(p);//erase��p���

				d->proute->path.erase(d);
			}

		}
	}
	for (auto it = s2.R.begin(); it != s2.R.end(); )
	{
		if (it->path.size() == 2)
		{

			s2.R.erase(it++);
		}
		else
		{
			it++;
		}

	}

	s12.snum = s2.snum;
	s2.R.splice(s2.R.end(), s12.R, e1, l1);

	s2.checkR(0, 1);
	//�ٰ�e1������route.no eject
// 	list<Route>::iterator se = find_if(s2.R.begin(), prev(s2.R.end()), [&e1](const Route &p) {return p.no == e1->no; });
// 	if (prev(s2.R.end()) != se)
// 	{//�ҵ���
// 		s2.ejection_cross(se);//��se���ڵ�·��ɾ��
// 	}

}
void CROSS(Solution &s1, Solution &s2)
{
	Solution s21(s2, SN);
	cross_route(s1, s2);//s2���ˣ�s1û��
	cross_route(s21, s1);
}
void CROSS_large(Solution &s1, Solution &s2)
{
	Solution s21(s2, SN);
	 
	cross(s1, s2);//s2���ˣ�s1û��
	 
	cross(s21, s1);
// 	s1.ejection_insertion(2);
// 	s1.perturb(100);
// 	s1.checkR(0, 1);
}


