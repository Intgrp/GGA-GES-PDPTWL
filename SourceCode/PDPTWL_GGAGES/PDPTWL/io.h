#pragma once

//#define MAX_CUS_NUM    (2000 * 20)
#include <string>
//��ȡ�ļ������������buff��
//buffΪһ��ָ�����飬ÿһ��Ԫ����һ���ַ�ָ�룬��Ӧ�ļ���һ�е����ݡ�
//specΪ������������������
extern int read_file(char ** const buff, const unsigned int spec, const char * const filename);

//��result�������е�����д���ļ���д�뷽ʽΪ����д��
extern void write_result(const char * const buff, const char * const filename);

//�ͷŶ��ļ��Ļ�����
extern void release_buff(char ** const buff, const int valid_item_num);

extern void write_file(const bool cover, const char * const buff, const char * const filename);
extern void write_file(const bool cover, const std::string& buff, const char * const filename);
void print_time(const char *  head);

