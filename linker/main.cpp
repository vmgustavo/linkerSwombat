#include<iostream>
#include<fstream>
#include<map>
#include<vector>
#include<stdlib.h>
#include<string.h>
#include "tools.h"
#include "linker.h"
#include<cassert>
using namespace std;

int main(int argc, char* argv[])
{
  string output_file = argv[1];
  output_file += ".mif";
  ofstream fout;
  fout.open(output_file.c_str());

  Linker linker;
  linker.get_started();

  for(int i = 2;i < argc;i++)
  {
    ifstream fin;
    fin.open(argv[i]);//lendo os .o
    cerr<<"abrindo "<<argv[i]<<endl;
    run_module(linker, fin);
  }
  linker.solve_pendencies();//resolvendo as pendencias externas
  linker.print(fout);//imprime o estado da memoria final
  return 0;
}





