#include "linker.h"

using namespace std;

void Linker::get_started()
{
  mem_size = 256;
  memory.resize(mem_size,0);
  instruction_offset = 0;
  data_offset = 0;
}

void Linker::extern_reference(int position, string &label)
{
  extern_pendencies.push_back(make_pair(position, label));
}

void Linker::add_label(string &label, int position)
{
  label_to_pos[label] = position;
}

void Linker::solve_pendencies()
{
  for(auto p : extern_pendencies)
  {
    memory[p.first] += label_to_pos[p.second];
  }
}

void Linker::print()
{

  cout<<"DEPTH = 256;"<<endl;
  cout<<"WIDTH = 8;"<<endl;
  cout<<"ADDRESS_RADIX = HEX;"<<endl;
  cout<<"DATA_RADIX = BIN;"<<endl;
  cout<<"CONTENT"<<endl<<"BEGIN"<<endl<<endl;
  for(int i = 0;i < mem_size;i++)
  {
    int v = memory[i];
    cout<<to_hex(i)<<" : ";
    bitset<8> to_print(v);
    cout<<to_print<<";";
    cout<<" --"<<endl;
  }
  cout<<"END;"<<endl;
}

void run_module(Linker &linker)
{
  int instr_size;
  int data_size;
  cin>>instr_size>>data_size;
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
  for(int i = 0;i < linker.mem_size;i += 2)
  {
    int code1, code2;
    cin>>code1>>code2;
    treat(code1, code2, linker.instruction_offset, linker.data_offset);
    linker.memory[linker.instruction_offset + i] = code1;
    linker.memory[linker.instruction_offset + i + 1] = code2;
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
  linker.data_offset += data_size;
}





