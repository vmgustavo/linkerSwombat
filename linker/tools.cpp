#include "tools.h"
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

bool uses_label(int op)
{
  return(op == 7 or op == 8 or op == 9 or op == 19);
}

bool uses_data(int op)
{
  return (op == 1 or op == 2);
}

void treat_offset(int &code, int offset)
{
  code += offset;
}
void treat(int &code1, int &code2, int instruction_offset, int data_offset)
{
  if(uses_label(code1>>3))
  {
    treat_offset(code2,instruction_offset);
  }
  else if(uses_data(code1>>3) and code2 != 254 and code2 != 255)
  {
    treat_offset(code2, -data_offset);
  }
}
