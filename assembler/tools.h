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

#endif
