#include<iostream>
#include<stdlib.h>
#include<vector>
#include<bitset>
#include<map>
#include"tools.h"
#include"Swombat.h"
using namespace std;

int main(int argc, char* argv[])
{
  if(argc!=3)
  {
    cerr<<"Usage:"<<endl;
    cerr<<argv[0]<<" [input file name] [output file name]"<<endl;
    return 1;
  }
  freopen(argv[1], "r", stdin);
  freopen(argv[2], "w", stdout);
  Swombat OurMachine;
  OurMachine.get_started();
  vector< pair< int, string > > label_pendencies, data_pendencies;
  while(read_instruction(OurMachine,label_pendencies,data_pendencies));//le as instrucoes enquanto houver
  OurMachine.solve_pendencies(label_pendencies,data_pendencies);
  OurMachine.print();
  return 0;
}
