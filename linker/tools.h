#ifndef tools_h
#define tools_h
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<bitset>
#include<map>
using namespace std;

string to_low(string &a);

char get_hex(int v);

string to_hex(int v);

bool is_label(string &label);

void read_comments();

bool numerical(string &s);

int string_to_int(string &s);

bool uses_label(int op);

bool uses_data(int op);

void treat_offset(int &code, int offset);

void treat(int &code1, int &code2, int instruction_offset, int data_offset);

#endif
