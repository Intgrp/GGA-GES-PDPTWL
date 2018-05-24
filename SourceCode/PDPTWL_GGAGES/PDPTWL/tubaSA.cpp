//#include "stdafx.h"
#include "Solution.h"
#include "ejection.h"
using namespace std;
//extern int cus_num;
//extern int offset;
//extern char *each_problem_buff;//����ȫ�ֱ���

void Solution::tabuList()
{
	//S0��һ�����н�
	//Sbest��¼���ſ��н�
	int cita = 100;
	int to = 20;
	int isrestart = 0;
	vector<int> tabulist;
	tabulist.resize((g_d.cus_num - 1) / 2);
	for (int i = 0; i < tabulist.size(); i++)
	{
		tabulist[i] = 0;
	}
	std::list<Route> R_best_child;
	for (auto it = R.begin(); it != R.end(); it++)
	{
		R_best_child.push_back(**it);
	}


	for (int k = 1; k <= cita&&isrestart <= to; k++)
	{//��ǰ�������
		localSearch(tabulist, R_best_child, k, isrestart);

	}

}


void  Solution::localSearch(vector<int> &tabulist, std::list<Route> &R_best_child, int k, int &isrestart)
{
	std::list<pthelm>::iterator it_pthelmp, it_pthelmd;
	Route* itroute;

	double ccost = 0;
	checkR((removed.size()), 1);
	for (auto it = R.begin(); it != R.end(); it++)
	{
		ccost += (*it)->route_cost;
	}
	double bestcost = ccost;

	int rand_order;
	while (1) {
		rand_order = rand() % od.size();
		if (rand_order == 0 || removed.end() != find_if(removed.begin(), removed.end(),
			[&rand_order](const removepool& rp) {return rp.remove_order->id == rand_order; }))
			continue;//��removed�����ҵ���

		else break;
	}
	itroute = od[rand_order]->pelmp->proute;//��ѡrequest����·��
	order* rorder = od[rand_order];

	it_pthelmp = find_if(itroute->path.begin(), itroute->path.end(),
		[&rorder](pthelm &rp) {return rorder->pelmp->porder->id == rp.porder->id; });//��һ���ҵ�����pickup
	it_pthelmd = next(it_pthelmp);
	it_pthelmd = find_if(it_pthelmd, itroute->path.end(),
		[&rorder](pthelm &rp) {return rorder->pelmp->porder->id == rp.porder->id; });//�ڶ����ҵ�����dilivery
	vector<Perturb> record_perturb;
	record_perturb.reserve(100);
	double perR = rand() / double(RAND_MAX);


	if (perR < 0.5)//// simple relocate   Pex
	{


		record_perturb.clear();
		for (auto rit = R.begin(); rit != R.end(); rit++)
		{
			if ((*rit) == itroute) continue;
			//ȫ��feasible
			perturbPD(*rit, itroute, it_pthelmp, it_pthelmd, record_perturb);//ritһֱ�ڱ�
			it_pthelmp->proute = itroute;
			it_pthelmd->proute = itroute;
		}
		if (!record_perturb.empty())
		{

			sort(record_perturb.begin(), record_perturb.end(),
				[](Perturb &i, Perturb &j) {return i.ccost < j.ccost; });
		}



		for (int randselct = 0; randselct < record_perturb.size(); randselct++)
		{

			Perturb r1 = record_perturb[randselct];
			if (tabulist[rand_order] == 0)//��order���ڷǽ���״̬
			{

				(r1.itroute)->path.splice(r1.it_pthelmp, itroute->path, it_pthelmp);
				(r1.itroute)->path.splice(r1.it_pthelmd, itroute->path, it_pthelmd);
				it_pthelmp->proute = (r1.itroute);
				it_pthelmd->proute = (r1.itroute);
				int ret = updata(r1.itroute);
				if (ret == 0)//infeasible
				{
					my_ERROR
					g_d.each_problem_output << "route error13!!!\n";
				}
				ret = updata(itroute);
				if (ret == 0)//infeasible
				{
					my_ERROR
					g_d.each_problem_output << "route error13!!!\n";
				}
				checkR((removed.size()), 1);
				ccost = 0;
				for (auto it = R.begin(); it != R.end(); it++)
				{
					ccost += (*it)->route_cost;
				}
				// 					if (ccost != r1.ccost)
				// 					{
				// 						printf("ccost != r1.ccost\n");
				// 						my_ERROR
				// 					}
				if (r1.ccost < bestcost)
				{//�������Ž�

					bestcost = r1.ccost;
					for (auto it = R.begin(); it != R.end(); it++)
					{
						R_best_child.push_back((**it));
					}
					isrestart = 0;

				}
				else
				{
					isrestart++;
				}

				if (isrestart >= sqrt(k))
				{//s=sbest
					for (auto it = R_best_child.begin(); it != R_best_child.end(); it++)
					{
						R.push_back(&(*it));
					}
					ccost = bestcost;
					for (int i = 0; i < tabulist.size(); i++)
					{
						tabulist[i] = 0;
					}
					isrestart = 0;
				}
				else
				{
					//���µ�ǰ��R
					tabulist[rand_order] = 1;

				}
				break;
			}
			else
			{
				if (r1.ccost < bestcost)
				{
					(r1.itroute)->path.splice(r1.it_pthelmp, itroute->path, it_pthelmp);
					(r1.itroute)->path.splice(r1.it_pthelmd, itroute->path, it_pthelmd);
					it_pthelmp->proute = (r1.itroute);
					it_pthelmd->proute = (r1.itroute);
					ret = updata(r1.itroute);
					if (ret == 0)//infeasible
					{
						my_ERROR
						g_d.each_problem_output << "route error13!!!\n";
					}
					ret = updata(itroute);
					if (ret == 0)//infeasible
					{
						my_ERROR
						g_d.each_problem_output << "route error13!!!\n";
					}
					checkR((removed.size()), 1);
					ccost = 0;
					for (auto it = R.begin(); it != R.end(); it++)
					{
						ccost += (*it)->route_cost;
					}
					// 						if (ccost != r1.ccost)
					// 						{
					// 							printf("ccost != r1.ccost\n");
					// 							my_ERROR
					// 						}
					bestcost = r1.ccost;
					if (r1.ccost < bestcost)
					{//�������Ž�

						bestcost = r1.ccost;
						for (auto it = R.begin(); it != R.end(); it++)
						{
							R_best_child.push_back((**it));
						}
						isrestart = 0;

					}
					else
					{
						isrestart++;
					}
					break;
				}
				else
				{
					isrestart++;
					continue;
				}

			}
		}



	}
	// 		else //if (Pex <= perR &&perR <= Pex2)
	// 			 // extended exchange
	// 		{//�����ѡһ��order
	// 			record_perturb.clear();
	//
	// 			int rand_order2;
	// 			while (1) {
	// 				rand_order2 = rand() % od.size();
	// 				if (rand_order2 == rand_order || rand_order2 == 0 || od[rand_order2]->pelmp->proute == itroute || removed.end() != find_if(removed.begin(), removed.end(),
	// 					[&rand_order2](const removepool& rp) {return rp.remove_order->id == rand_order2; }))
	// 					continue;//��removed�����ҵ���
	// 				else break;
	// 			}
	// 			auto itroute2 = od[rand_order2]->pelmp->proute;//��ѡrequest����·��
	// 			order* rorder2 = od[rand_order2];
	//
	// 			auto it_pthelmp2 = find_if(itroute2->path.begin(), itroute2->path.end(),
	// 				[&rorder2](pthelm &rp) {return rorder2->pelmp->porder->id == rp.porder->id; });//��һ���ҵ�����pickup
	// 			auto it_pthelmd2 = next(it_pthelmp2);
	// 			it_pthelmd2 = find_if(it_pthelmd2, itroute2->path.end(),
	// 				[&rorder2](pthelm &rp) {return rorder2->pelmp->porder->id == rp.porder->id; });//�ڶ����ҵ�����dilivery
	// 			Route tmp2;
	// 			auto tmpn2 = next(it_pthelmp2);
	// 			auto tmdn2 = next(it_pthelmd2);
	// 			tmp2.path.splice(tmp2.path.begin(), itroute2->path, it_pthelmp2);
	// 			tmp2.path.splice(tmp2.path.end(), itroute2->path, it_pthelmd2);
	//
	// 			Route tmp;
	// 			auto tmpn = next(it_pthelmp);
	// 			auto tmdn = next(it_pthelmd);
	// 			tmp.path.splice(tmp.path.begin(), itroute->path, it_pthelmp);
	// 			tmp.path.splice(tmp.path.end(), itroute->path, it_pthelmd);
	//
	// 			//��it_pthelmp �� it_pthelmd insert��itroute2->path
	// 			//�ٽ�tmp2 �� it_pthelmp2 �� it_pthelmd2 splice ��  itroute1->path
	// 			perturbPD(itroute2, &tmp, it_pthelmp, it_pthelmd, record_perturb);
	// 			// 			it_pthelmp->proute = itroute;
	// 			// 			it_pthelmd->proute = itroute;
	// 			vector<Perturb> record_perturb2;
	// 			if (!record_perturb.empty())//�к��ʵ�λ��
	// 			{//�ٽ�tmp2 �� it_pthelmp2 �� it_pthelmd2 splice ��  itroute1->path
	//
	// 				perturbPD(itroute, &tmp2, it_pthelmp2, it_pthelmd2, record_perturb2);
	// 				// 				it_pthelmp2->proute = itroute2;
	// 				// 				it_pthelmd2->proute = itroute2;
	// 				if (!record_perturb2.empty() && !record_perturb.empty())
	// 				{//��record_perturb2 �� record_perturb �� ��ѡһ��
	//
	// 					sort(record_perturb.begin(), record_perturb.end(),
	// 						[](Perturb &i, Perturb &j) {return i.ccost < j.ccost; });
	// 					double q = rand() / (double)RAND_MAX;
	// 					//int randselct = 0;
	// 					int randselct = floor(q*q*q*q*record_perturb.size());
	// 					if (randselct >= record_perturb.size())
	// 					{
	// 						randselct = record_perturb.size() - 1;
	// 					}
	// 					Perturb r1 = record_perturb[randselct];
	//
	// 					//Perturb r1 = record_perturb[rand() % record_perturb.size()];
	//
	// 					itroute2->path.splice(r1.it_pthelmp, tmp.path, it_pthelmp);
	// 					itroute2->path.splice(r1.it_pthelmd, tmp.path, it_pthelmd);
	// 					it_pthelmp->proute = itroute2;
	// 					it_pthelmd->proute = itroute2;
	//
	// 					sort(record_perturb2.begin(), record_perturb2.end(),
	// 						[](Perturb &i, Perturb &j) {return i.ccost < j.ccost; });
	// 					double q2 = rand() / (double)RAND_MAX;
	// 					//int randselct2 = 0;
	// 					int randselct2 = floor(q2*q2*q2*q2*q2*q2*q2*record_perturb2.size());
	// 					if (randselct2 >= record_perturb2.size())
	// 					{
	// 						randselct2 = record_perturb2.size() - 1;
	// 					}
	// 					Perturb r2 = record_perturb2[randselct2];
	//
	// 					//Perturb r2 = record_perturb2[rand() % record_perturb2.size()];
	//
	// 					itroute->path.splice(r2.it_pthelmp, tmp2.path, it_pthelmp2);
	// 					itroute->path.splice(r2.it_pthelmd, tmp2.path, it_pthelmd2);
	//
	// 					it_pthelmp2->proute = itroute;
	// 					it_pthelmd2->proute = itroute;
	// 				}
	// 				else
	// 				{
	// 					itroute2->path.splice(tmdn2, tmp2.path, it_pthelmd2);
	// 					itroute2->path.splice(tmpn2, tmp2.path, it_pthelmp2);
	// 					it_pthelmp2->proute = itroute2;
	// 					it_pthelmd2->proute = itroute2;
	// 					itroute->path.splice(tmdn, tmp.path, it_pthelmd);
	// 					itroute->path.splice(tmpn, tmp.path, it_pthelmp);
	// 					it_pthelmp->proute = itroute;
	// 					it_pthelmd->proute = itroute;
	// 					continue;
	// 				}
	//
	// 			}
	// 			else//û�к��ʵ�λ��
	// 			{
	// 				itroute2->path.splice(tmdn2, tmp2.path, it_pthelmd2);
	// 				itroute2->path.splice(tmpn2, tmp2.path, it_pthelmp2);
	// 				it_pthelmp2->proute = itroute2;
	// 				it_pthelmd2->proute = itroute2;
	// 				itroute->path.splice(tmdn, tmp.path, it_pthelmd);
	// 				itroute->path.splice(tmpn, tmp.path, it_pthelmp);
	// 				it_pthelmp->proute = itroute;
	// 				it_pthelmd->proute = itroute;
	// 				continue;
	// 			}
	// 			//printf("exchange\n");
	// 		}

}
