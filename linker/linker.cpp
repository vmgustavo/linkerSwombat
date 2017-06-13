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
  for(auto p : extern_pendencies)//resolve as pendencias externas
  {
    memory[p.first] += label_to_pos[p.second];
  }
}

void Linker::print(ofstream &fout)
{

  fout<<"DEPTH = 256;"<<endl;
  fout<<"WIDTH = 8;"<<endl;
  fout<<"ADDRESS_RADIX = HEX;"<<endl;
  fout<<"DATA_RADIX = BIN;"<<endl;
  fout<<"CONTENT"<<endl<<"BEGIN"<<endl<<endl;
  for(int i = 0;i < mem_size;i++)
  {
    int v = memory[i];
    fout<<to_hex(i)<<" : ";
    bitset<8> to_print(v);
    fout<<to_print<<";";
    fout<<" --"<<endl;
  }
  fout<<"END;"<<endl;
}

void run_module(Linker &linker, ifstream &fin)
{
  cerr<<"running module"<<endl;
  int instr_size;
  int data_size;
  char aux;
  string s;
  fin>>s;
  while(!numerical(s))//adiciona as referencias externas
  {
    string tmp;
    int pos;
    string label;
    fin>>pos>>label;
    pos += linker.instruction_offset;
    linker.extern_reference(pos, label);
    fin>>s;
  }
  instr_size = string_to_int(s);
  fin>>data_size;//pega quantas instrucoes foram usadas e quantos dados foram alocados
  for(int i = 0;i < linker.mem_size;i++)
  {
    int code1, code2;
    fin>>code1;
    treat(code1, code2, linker.instruction_offset, linker.data_offset);//faz os offsets necessarios nas funcoes em que se faz necessario
    if(i < instr_size)
    {
      fin>>code2;
      linker.memory[linker.instruction_offset + i] = code1;//coloca na posicao deslocada ja o code1 tratado
      linker.memory[linker.instruction_offset + i + 1] = code2;
      i++;
    }
    else if(i > linker.mem_size - 3 - data_size and i < 254)
    {
      linker.memory[i - linker.data_offset] = code1;//tratando o offset dos dados
    }
  }
  string label;
  while(fin>>label)
  {
    int position;
    fin>>position;
    position += linker.instruction_offset;
    linker.add_label(label,position);//criando a tabela de simbolos completa
  }
  linker.instruction_offset += instr_size;
  linker.data_offset += data_size;
}
