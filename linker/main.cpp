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
  //freopen(output_file.c_str(),"w",stdout);

  Linker linker;
  linker.get_started();

  for(int i = 2;i < argc;i++)
  {
    ifstream fin;
    fin.open(argv[i]);
    //freopen(argv[i],"r",stdin);
    cerr<<"abrindo "<<argv[i]<<endl;
    /*cerr<<"running module"<<endl;
    int instr_size;
    int data_size;
    cin>>instr_size>>data_size;
    cerr<<"instr size = "<<instr_size<<", data_size = "<<data_size<<endl;
    while(cin.peek() == 'e')
    {
      string tmp;
      cin>>tmp;
      int pos;
      string label;
      cin>>pos>>label;
      cerr<<pos<<" needs "<<label<<endl;
      pos += linker.instruction_offset;
      linker.extern_reference(pos, label);
    }
    for(int i = 0;i < linker.mem_size;i++)
    {
      int code1, code2;
      cin>>code1;
      treat(code1, code2, linker.instruction_offset, linker.data_offset);
      if(i < instr_size)
      {
        cerr<<"atualizando "<<i<<endl;
        cin>>code2;
        i++;
        linker.memory[linker.instruction_offset + i] = code1;
        linker.memory[linker.instruction_offset + i + 1] = code2;
      }
      else if(i > linker.mem_size - 3 - data_size and i < 254)
      {
        cerr<<"atualizando data "<<i<<endl;
        linker.memory[i - linker.data_offset] = code1;
      }
    }
    string label;
    while(cin>>label)
    {
      int position;
      cin>>position;
      cerr<<label<<" directs to "<<position<<endl;
      position += linker.instruction_offset;
      linker.add_label(label,position);
    }
    linker.instruction_offset += instr_size;
    linker.data_offset += data_size;*/
    run_module(linker, fin);
    fin.close();
  }
  linker.solve_pendencies();
  linker.print(fout);
  return 0;
}





