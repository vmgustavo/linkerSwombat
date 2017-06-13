#include<iostream>
#include<map>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include "tools.h"

using namespace std;

class Linker{
  private:
    vector< pair<int, string> > extern_pendencies;
    map<string, int> label_to_pos;
	public:
		int instruction_offset;
    vector<int> memory;
		int data_offset;
		int mem_size;

		void get_started();

		void extern_reference(int position, string &label);

		void add_label(string &label, int position);

		void solve_pendencies();

		void print();
};

void run_module(Linker &linker);
