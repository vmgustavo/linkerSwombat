#include<iostream>
#include<stdlib.h>
#include<vector>
#include<bitset>
#include<map>
using namespace std;

string to_low(string &a)
{
  string ret = a;
  for(int i = 0;i < ret.size();i++)
  {
    if(ret[i] >= 'A' and ret[i] <= 'Z')
    {
      ret[i] = ret[i] - 'A' + 'a';
    }
  }
  return ret;
}

char get_hex(int v)
{
  if(v<10) return (char)('0' + v);
  else return (char)('A'+v-10);
}


string to_hex(int v)
{
  string ret;
  ret.resize(2);
  ret[0] = get_hex(v/16);
  ret[1] = get_hex(v%16);
  return ret;
}

class Swombat{
  private:

    map<string,int> index_of_registers;//cada registrador tem um indice correspondente (uso geral de 0 a 7)
    vector<int> memory;//conteudo da memoria
    int mem_size;//tamanho da memoria
    map<string,int> data_to_pos;
    map<string,int> label_to_pos;
    int last_free;

  public:

    int next_instruction;
    void generate_indexes()
    {
      for(int i = 0;i <= 7;i++)
      {
        string tmp = "A";
        tmp.push_back('0' + i);
        index_of_registers[tmp] = index_of_registers[to_low(tmp)] = i;
      }
      data_to_pos["IO"] = 254;
      data_to_pos["io"] = 254;
    }

    void get_started()
    {
      next_instruction = 0;
      mem_size = 256;
      last_free = mem_size - 3;
      generate_indexes();
      memory.resize(mem_size,0);
    }

    bool islabel(string &s)
    {
      return label_to_pos.count(s);
    }

    bool isdata(string &s)
    {
      return data_to_pos.count(s);
    }

    int get_reg_index(string &s)
    {
      return index_of_registers[s];
    }

    int get_addr_from_data(string &s)
    {
      if(!isdata(s)) cerr<<s<<" is not a data"<<endl;
      return data_to_pos[s];
    }

    int get_addr_from_label(string &s)
    {
      if(!islabel(s)) cerr<<s<<" is not a label"<<endl;
      return label_to_pos[s];
    }

    void store_instruction(string &label, int graphic)
    {
      if(!label.empty())
      {
        label_to_pos[label] = next_instruction;
      }
      memory[next_instruction] = (graphic>>8);
      memory[next_instruction+1] = graphic%(1<<8);
      next_instruction += 2;
    }
    
    void solve_pendencies(vector< pair< int, string> > &label_pendencies, vector< pair< int, string> > &data_pendencies)
    {
      for(auto pend : label_pendencies)
      {
        memory[pend.first+1] += get_addr_from_label(pend.second);
      }
      
      for(auto pend : data_pendencies)
      {
        cerr<<"instr = "<<pend.first<<" need "<<pend.second<<endl;
        memory[pend.first+1] += get_addr_from_data(pend.second);
      }
    }

    void allocate(string &label, int num_bytes, int value)
    {
      if(num_bytes == 0) return;
      else if(num_bytes == 1)
      {
        data_to_pos[label] = last_free;
      }
      memory[last_free--] = value%(1<<8);
      allocate(label, num_bytes-1,(value>>8));
    }

    void print()
    {
      cout<<"DEPTH = "<<mem_size<<";"<<endl;
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
};

bool is_label(string &label){ return (label[0] == '_');}

void read_comments()
{
  string tmp;
  getline(cin,tmp);
  cerr<<"removendo comentario : "<<tmp<<endl;
}

bool numerical(string &s)
{ 
  for(int i = 0;i < s.size();i++)
  {
    if(s[i] < '0' or s[i] > '9') return false; 
  }
  return true;
}

int string_to_int(string &s)
{
  int ret = 0;
  for(int i = 0;i < s.size();i++)
  {
    ret *= 10;
    ret += s[i] - '0';
  }
  return ret;
}

void assemble(Swombat &OurMachine, string &label, string &instr, vector< pair< int, string > > &label_pendencies, vector< pair< int, string > > &data_pendencies)
{
  cerr<<"instruction is "<<instr<<endl;
  int graphic = 0,op = 0;
  if(instr == "exit") graphic = 0;
  else if(instr == "loadi" or instr == "storei")
  {
    string tmp,reg;
    int addr=0;
    op = 1;

    if(instr == "storei") op = 2;

    cin>>reg;
    cin>>tmp;

    if(numerical(tmp)) addr = string_to_int(tmp);
    else
    {
      if(OurMachine.isdata(tmp))
      {
        addr = OurMachine.get_addr_from_data(tmp);
      }
      else data_pendencies.push_back(make_pair(OurMachine.next_instruction,tmp));
    }

    graphic = (op<<11) + (OurMachine.get_reg_index(reg)<<8) + addr;
  }
  else if(instr == "jmpz" or instr == "jmpn")
  {
    string tmp,reg;
    int addr = 0;
    op = 8;

    if(instr == "jmpn") op = 9;

    cin>>reg;
    cin>>tmp;

    if(numerical(tmp)) addr = string_to_int(tmp);
    else
    {
      if(OurMachine.islabel(tmp))
      {
        addr = OurMachine.get_addr_from_label(tmp);
      }
      else label_pendencies.push_back(make_pair(OurMachine.next_instruction,tmp));
    }

    graphic = (op<<11) + (OurMachine.get_reg_index(reg)<<8) + addr;
  }
  else if(instr == "add" or instr == "subtract" or instr == "multiply" or instr == "divide" or instr == "move" or instr == "load" or instr == "store" or instr == "negate")
  {
    string reg1,reg2;
    op = 3;
    if(instr == "subtract") op = 4;
    else if(instr == "multiply") op = 5;
    else if(instr == "divide") op = 6;
    else if(instr == "move") op = 10;
    else if(instr == "load") op = 11;
    else if(instr == "store") op = 12;
    else if(instr == "negate") op = 15;

    cin>>reg1>>reg2;
    graphic = (op<<11) + (OurMachine.get_reg_index(reg1)<<8) + (OurMachine.get_reg_index(reg2)<<5);
  }
  else if(instr == "jump")
  {
    string tmp;
    int addr = 0;
    op = 7;

    cin>>tmp;

    if(numerical(tmp)) addr = string_to_int(tmp);

    else
    {
      if(OurMachine.islabel(tmp))
      {
        addr = OurMachine.get_addr_from_label(tmp);
      }
      else label_pendencies.push_back(make_pair(OurMachine.next_instruction,tmp));
    }
    graphic = (op<<11) + addr;
  }
  else if(instr == "loadc" or instr == "addi")
  {
    int constant_value;
    string reg;
    op = 13;
    if(instr == "addi") op = 18;

    cin>>reg;
    cin>>constant_value;

    graphic = (op<<11) + (OurMachine.get_reg_index(reg)<<8) + constant_value;

  }
  else if(instr == "clear" or instr == "push" or instr == "pop")
  {
    string reg;
    op = 14;
    if(instr == "push") op = 16;
    else if(instr == "pop") op = 17;
    
    cin>>reg;

    graphic = (op<<11) + (OurMachine.get_reg_index(reg)<<8);
  }
  else if(instr == "call")
  {
    string tmp;
    op = 19;
    int addr = 0;
    cin>>tmp;
    if(OurMachine.islabel(tmp))
    {
      addr=OurMachine.get_addr_from_label(tmp);
    }
    else
    {
      label_pendencies.push_back(make_pair(OurMachine.next_instruction,tmp));
    }
    graphic = (op<<11) + addr;
  }
  else if(instr == "return")
  {
    op = 20;
    graphic = (op<<11);
  }
  else if(instr == ".data")
  {
    int num_bytes,value;
    cin>>num_bytes>>value;
    OurMachine.allocate(label,num_bytes,value);
    read_comments();
    return;
  }
  else cerr<<instr<<" nao eh uma instrucao valida.\n";
  read_comments();
  OurMachine.store_instruction(label,graphic);
  
}

bool read_instruction(Swombat &OurMachine, vector< pair< int, string > > &label_pendencies, vector< pair< int, string> > &data_pendencies)
{
  string label,instr;
  cin>>label;
  if(cin.eof()) return false;
  if(is_label(label))
  {
    label = label.substr(0,label.size() - 1);//trata o label
    cin>>instr;
  }
  else
  {
    instr = label;//se nao houver label, a primeira palavra eh instrucao
    label.clear();
  }
  assemble(OurMachine,label,instr,label_pendencies,data_pendencies);
  return true;
}


int main(int argc, char* argv[])
{
  if(argc!=3)
  {
    cerr<<"Usage:"<<endl;
    cerr<<argv[0]<<" [input file name] [output file name]"<<endl;
    return 1;
  }
  string output_filename = argv[2];
  output_filename += ".mif";
  freopen(argv[1], "r", stdin);
  freopen(output_filename.c_str(), "w", stdout);
  Swombat OurMachine;
  OurMachine.get_started();
  vector< pair< int, string > > label_pendencies, data_pendencies;
  while(read_instruction(OurMachine,label_pendencies,data_pendencies));//le as instrucoes enquanto houver
  OurMachine.solve_pendencies(label_pendencies,data_pendencies);
  OurMachine.print();
  return 0;
}
