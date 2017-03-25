#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif // !_CRT_SECURE_NO_WARNINGS

#include "lib_io.h"
#include "lib_time.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/timeb.h>
#include <iostream>
#include <fstream>
#include <string>

#define MAX_LINE_LEN 55000

using namespace std;

void write_file(const bool cover, const char * const buff, const char * const filename);

void print_time(const char *head)
{
#ifdef _DEBUG
	struct timeb rawtime;
	struct tm * timeinfo;
	ftime(&rawtime);
	timeinfo = localtime(&rawtime.time);

	static int ms = rawtime.millitm;
	static unsigned long s = rawtime.time;
	int out_ms = rawtime.millitm - ms;
	unsigned long out_s = rawtime.time - s;
	ms = rawtime.millitm;
	s = rawtime.time;

	if (out_ms < 0)
	{
		out_ms += 1000;
		out_s -= 1;
	}
	printf("%s date/time is: %s \tused time is %lu s %d ms.\n", head, asctime(timeinfo), out_s, out_ms);
#endif
}

int read_file(char ** const buff, const unsigned int spec, const char * const filename)
{
	ifstream fin(filename);
	if (!fin.is_open())
	{
		cout << "Fail to open file" << filename << ", " << strerror(errno) << "." << endl;
		return 0;
	}
	cout << "Open file " << filename << " OK." << endl;

	string line;
	unsigned int cnt = 0;
	while ((cnt < spec) && (getline(fin, line)))
	{
		buff[cnt] = new char[MAX_LINE_LEN + 2];
		strncpy(buff[cnt], line.c_str(), MAX_LINE_LEN + 2 - 1);
		buff[cnt][MAX_LINE_LEN + 1] = 0;
		++cnt;
	}
	fin.close();
	cout << "There are " << cnt << " lines in file " << filename << "." << endl;

	return cnt;
}

void write_result(const char * const buff, const char * const filename)
{
	write_file(1, buff, filename);
}

void release_buff(char ** const buff, const int valid_item_num)
{
	for (int i = 0; i < valid_item_num; ++i)
	{
		delete(buff[i]);
	}
}

void write_file(const bool cover, const char * const buff, const char * const filename)
{
	if (buff == nullptr)
	{
		return;
	}

	const char *write_type = cover ? "w" : "a"; //1:覆盖写文件，0:追加写文件
	ofstream fout(filename);
	if (!fout.is_open())
	{
		cout << "Fail to open file " << filename << ", " << strerror(errno) << "." << endl;
		return;
	}
	cout << "Open file " << filename << " OK." << endl;
	fout << buff << endl;
	fout.close();
}