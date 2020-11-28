#include "word_ana.hpp"
#include "lex_lsa_ana.hpp"
#include "interpreter.hpp"
#define maxn 10000
string word_res = "";
string word_list[maxn];
int word_num = 0;
node* node_list[maxn];
int node_num = 0;
string source;
tab_t* t_list[maxn];
int tab_num = 0;
string code_list[maxn];
int code_num = 0;
void my_read()
{
    ifstream s_file("../prog.pas");
    string temp;
    while (getline(s_file, temp))
    {
        source += temp + "\n";
    }
    s_file.close();
}
void my_write()
{
    ofstream c_file("../word_list.txt",ios::out);
    c_file << word_res;
    c_file.close();
}
int main()
{
    my_read();
    word_ana(word_res, word_list, word_num, source);
    my_write();
    lex_ana(node_list, node_num, word_list, tab_num, t_list, code_num, code_list);
    interpreter(code_num, code_list);
    return 0;
}

