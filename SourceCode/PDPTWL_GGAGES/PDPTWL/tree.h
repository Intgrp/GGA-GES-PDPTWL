
#pragma once

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include "uti.h"
//#include "Solution.h"

struct Node {
	//bool canuse=false;
private:
	std::list<Node>::iterator last_iter;//ָ����һ������ʹ�õĵط�
public:
	int level;
	int num;
	Node* father;
	std::list<Node> adj;
	struct content {
		bool eject_itjp = 0;
		bool isprune = 0;
		int psum;
		int countp;
		int countd;
		int cus_id;
		int q_new = 0;
		double atime_new = 0;//insert/eject ��ȥ��ǰһ����atime

		std::list<pthelm>::iterator eject_itr;//ppj�е�λ�� ������
		std::list<pthelm>::iterator itr; //insert insert��ȥ��ǰһ��
										 //eject ָ��eject��ǰһ��
		pthelm* pej;//eject����һ��
		
		content(nullptr_t ptr) :psum(0), countp(0), countd(0) {

		}
		content(Node* father) :psum(father->cont.psum), countp(father->cont.countp), countd(father->cont.countd)
		{
		}
		void init(Node* father) {
			eject_itjp = isprune = 0;
			psum = father->cont.psum;
			countp = father->cont.countp;
			countd = father->cont.countd;
			q_new = 0;
			atime_new = 0;
		}
		void init(nullptr_t) {
			eject_itjp = isprune = 0;
			psum = 0;
			countp = 0;
			countd = 0;
			q_new = 0;
			atime_new = 0;
		}
		//string c;
	}cont;
	
	Node(int level, int num, nullptr_t ptr) :level(level), num(num), father(ptr), cont(ptr) {
		last_iter = adj.end();
	}
	Node(int level, int num, Node* fth) :level(level), num(num), father(fth), cont(father) {
		last_iter = adj.end();
	}
	
	void clear() {//clear �൱�ڰ��α����begin;
		last_iter = adj.begin();
	}
	std::list<Node>::iterator end() {
		return last_iter;
	}
	bool empty() {
		return adj.empty() || last_iter == adj.begin();
	}
	void init(int level, int num, Node* fth) {
		this->level = level; this->num = num; this->father = fth;
		last_iter = adj.begin();
		//canuse = true;
		cont.init(father);
	}
	void init(int level, int num, nullptr_t fth) {
		this->level = level; this->num = num; this->father = fth;
		last_iter = adj.begin();
		//canuse = true;
		cont.init(fth);
	}
	Node& putpack_getlast(int level, int num, Node* fth) {
		if (last_iter == adj.end() || adj.empty()) {//���û����,�������һ��
			adj.emplace_back(Node(level, num, fth));
			last_iter = prev(adj.end());
		}
		else {//����У����³�ʼ��
			_assert(!adj.empty());
			last_iter->init(level, num, fth);
		}
		auto tmp = last_iter;
		++last_iter;
		return *(tmp);
	}
};