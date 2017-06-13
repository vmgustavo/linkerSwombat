#include<iostream>
#include<map>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include "tools.h"
#include "linker.h"
using namespace std;

int main(int argc, char* argv[])
{
  string output_file = argv[1];
  output_file += ".mif";
  freopen(output_file.c_str(),"w",stdout);
  
  Linker linker;
  linker.get_started();
   
  for(int i = 2;i < argc;i++)
  {
    freopen(argv[i],"r",stdin);
    cerr<<"abrindo "<<argv[i]<<endl;
    run_module(linker);
  }
  linker.solve_pendencies();
  linker.print();
  return 0;
}





