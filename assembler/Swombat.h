#ifndef swombat_h
#define swombat_h
#include<iostream>
#include<stdlib.h>
#include<vector>
#include<bitset>
#include<map>
#include"tools.h"
using namespace std;

class Swombat{
  private:

    map<string,int> index_of_registers;//cada registrador tem um indice correspondente (uso geral de 0 a 7)
    vector<int> memory;//conteudo da memoria
    int mem_size;//tamanho da memoria
    map<string,int> data_to_pos;//posicao da memoria relacionada a uma alocacao em .data
    map<string,int> label_to_pos;//posicao da memoria relacionada a um label
    int last_free;//proxima posicao livre para alocacao (decrescente)

  public:

    int next_instruction;//proxima posicao livre para guardar instrucao (crescente)
    void generate_indexes();
    
    void get_started();

    bool islabel(string &s);

    bool isdata(string &s);

    int get_reg_index(string &s);

    int get_addr_from_data(string &s);

    int get_addr_from_label(string &s);

    void store_instruction(string &label, int graphic);
    
    void solve_pendencies(vector< pair< int, string> > &label_pendencies, vector< pair< int, string> > &data_pendencies);

    void allocate(string &label, int num_bytes, int value);
    
    void print();
};

void assemble(Swombat &OurMachine, string &label, string &instr, vector< pair< int, string > > &label_pendencies, vector< pair< int, string > > &data_pendencies);

bool read_instruction(Swombat &OurMachine, vector< pair< int, string > > &label_pendencies, vector< pair< int, string> > &data_pendencies);

#endif
