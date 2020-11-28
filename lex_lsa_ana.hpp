/*以下是PL0文法*/
/*
〈程序〉→〈分程序〉。
〈分程序〉→ [<常量说明部分>][<变量说明部分>][<过程说明部分>]〈语句〉
 <常量说明部分> → CONST<常量定义>{ ,<常量定义>}；
 <常量定义> → <标识符>=<无符号整数>
 <无符号整数> → <数字>{<数字>}
 <变量说明部分> → VAR<标识符>{ ,<标识符>}；
 <标识符> → <字母>{<字母>|<数字>}
 <过程说明部分> → <过程首部><分程度>；{<过程说明部分>}
 <过程首部> → procedure<标识符>；
 <语句> → <赋值语句>|<条件语句>|<当型循环语句>|<过程调用语句>|<读语句>|<写语句>|<复合语句>|<空>
 <赋值语句> → <标识符>:=<表达式>
 <复合语句> → begin<语句>{ ；<语句>}<end>
 <条件> → <表达式><关系运算符><表达式>|odd<表达式>
 <表达式> → [+|-]<项>{<加减运算符><项>}
 <项> → <因子>{<乘除运算符><因子>}
 <因子> → <标识符>|<无符号整数>|(<表达式>)
 <加减运符> → +|-
 <乘除运算符> → *|/
 <关系运算符> → =|#|<|<=|>|>=
 <条件语句> → if<条件>then<语句>
 <过程调用语句> → call<标识符>
 <当型循环语句> → while<条件>do<语句>
 <读语句> → read(<标识符>{ ，<标识符>})
 <写语句> → write(<标识符>{，<标识符>})
 <字母> → a|b|c…x|y|z
 <数字> → 0|1|2…7|8|9

*/
/*以下是字母表示*/
/*
<程序>-P
<分程序>-SP
<常量说明部分>-CP
<变量说明部分>-VP
<过程说明部分>-PP
<语句>-S
<常量定义>-CD
<标识符>-ID
<无符号整数>-UI
           <数字>-NUM
           <字母>-CHR
<过程首部>-PRH
<赋值语句>-ASS
<条件语句>-CON
<条件>-COS 
<当型循环语句>-WHI
<过程调用语句>-CAL
<读语句>-RED
<写语句>-WRI
<复合语句>-COM
              <空>-NON
<表达式>-EXP
<项>-TER
             <关系运算符>-REL
           <加减运算符>-ADD
            <乘除运算符>-MUL
<因子>-FAC
*/
/*语法分析*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#include <queue>
#define maxn 10000
using namespace std;
struct tab_t
{
	string name;
	string kind;
	int val;
	int level;
	int adr;
	int size;
	int in_add;
	tab_t(string name, string kind, int val, int level, int adr, int size, int in_add)
	{
		this->name = name;
		this->kind = kind;
		this->val = val;
		this->level = level;
		this->adr = adr;
		this->size = size;
		this->in_add = in_add;
	}
};	
struct node
{
	string word;
	int depth;
	int child_num;
	node* next;
	node* first_child;
	node* fa;
	node (string word, int depth, node* fa)
	{
		this->word = word;
		this->depth = depth;
		this->fa = fa;
		this->child_num = 0;
		this->next = NULL;
		this->first_child = NULL; 
	}
	node* add_child(node* child)
	{
		this->child_num++;
		node* last_child = this->find_last_child();
		if (last_child == NULL)
		{
			this->first_child = child;
		}
		else
		{
			last_child->next = child;
		}
	}
	node* find_last_child()
	{
		node* child = this->first_child;
		if (child == NULL)
		{
			return NULL;
		}
		while (child->next != NULL)
		{
			child = child->next;
		}
		return child;
	}
};
string get_id(const string& word)
{
	int i = 0;
	int len = word.length();
	while(word[i] != ',')
	{
		i++;
	}
	i++;
	return word.substr(i, len - i - 1);
}
int get_ui(const string& word)
{
	int i = 0;
	int len = word.length();
	int ui = 0;
	while (word[i] != ',')
	{
		i++;
	}
	for (i++; i < len - 1; i++)
	{
		ui = ui * 10 + word[i] - '0';
	}
	return ui;
}
string gen(int code_num, const string& f, int l, int a)
{
	return to_string(code_num) + " " + f + " " + to_string(l) + " " + to_string(a);
}
pair<int, int> find_char(string word, int lev, const int& tab_num, tab_t* (&t_list)[maxn])
{
    cout << lev << " " << word << endl;
	int i = tab_num - 1;
	int cnt_lev = lev;
	for (; i>=0 ;i--)
    {
	    if (t_list[i]->kind == "PROCEDURE" && t_list[i]->level < cnt_lev)
        {
	        cnt_lev--;
            continue;
        }
	    if (t_list[i]->level == cnt_lev && t_list[i]->name == word)
        {
            if (t_list[i]->kind == "CONSTANT")
            {
                return make_pair(t_list[i]->val, -1);
            }
            else if (t_list[i]->kind == "VARIABLE")
            {
                return make_pair(lev - t_list[i]->level, t_list[i]->adr);
            }
        }
    }
	cout << "ERROR187" << endl;
}
node* P(node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* SP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx);
node* CP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* VP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx);
node* PP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx);
node* S(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* CD(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* ID(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* PRH(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx);
node* ASS(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* CON(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* WHI(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* CAL(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* RED(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* WRI(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* COM(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* EXP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* UI(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* COS(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* TER(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
node* FAC(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev);
void lex_ana(node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn])
{
	node* root = P(node_list, node_num, word_list, tab_num, t_list, code_num, code_list, -1);
	queue<node*> q;
	q.push(root);
	int cnt_depth = 0;
	while (!q.empty())
	{
		node* cnt = q.front();
		q.pop();
		if (cnt->depth > cnt_depth)
		{
			cnt_depth = cnt->depth;
			cout << endl;
		}
		if (cnt->fa!=NULL && cnt->fa->first_child == cnt)
		{
			cout << "(";
		}
		if (cnt->fa != NULL)
			cout << "[" << cnt->fa->word <<"] ";
		cout << cnt->word << " ";
		if (cnt->fa != NULL && cnt->next == NULL)
		{
			cout << ")";
		}
		node* child = cnt->first_child;
		while(child != NULL)
		{
			q.push(child);
			child = child->next;	
		}	
	} 
	for (int i = 0; i < tab_num; i++)
	{
		std::cout << "NAME:" + t_list[i]->name + " " + "KIND:" + t_list[i]->kind + " " + "VAL:" + to_string(t_list[i]->val) + " " + "LEVEL:" + to_string(t_list[i]->level) + " "+ "ADR:" + to_string(t_list[i]->adr) + " " + "SIZE:" + to_string(t_list[i]->size) + " " + "IN:" + to_string(t_list[i]->in_add) << endl;
	}
	for (int i = 0; i < code_num; i++)
	{
		std::cout << code_list[i] << std::endl;
	}
}
node* P(node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* P = new node("P", 0, NULL);	
	node_list[node_num++] = P; 
	int sym = 0;
	int dx = 3;
    t_list[tab_num++] = new tab_t("main", "PROCEDURE", 0, lev, dx++, 1, 0);
    int temp_dx = dx;
    int temp_tab_num = tab_num;
    dx = 3;
	P->add_child(SP(sym, 0 + 1, P,node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev + 1, dx));
    dx = temp_dx;
    t_list[temp_tab_num - 1]->size = dx - 3;
	return P;
}
node* SP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx)
{
    if (lev >= 4)
    {
        cout << "ERROR" << endl;
    }
	node* SP = new node("SP", depth, fa);
	node_list[node_num++] = SP;
	int temp_code_num = code_num;
	int pro_tab_num = tab_num;
	code_list[code_num] = gen(code_num, "jmp", 0, 0);
	code_num++;
	int len = word_list[sym].length();
	if (len >= 6 && word_list[sym].substr(1, 5) == "const")//常量说明部分 
	{
		SP->add_child(CP(sym, depth + 1, SP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));		
	}
	int temp_tab_num = tab_num;
	len = word_list[sym].length();
	if (len >= 4 && word_list[sym].substr(1, 3) == "var")//变量说明部分
	{
		SP->add_child(VP(sym, depth + 1, SP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev, dx));
	}
	int temp_tab_end = tab_num;
	len = word_list[sym].length();
	if (len >= 10 && word_list[sym].substr(1, 9) == "procedure")//过程说明部分 
	{
		SP->add_child(PP(sym, depth + 1, SP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev, dx));
	}
	t_list[pro_tab_num - 1]->in_add = code_num;
	code_list[temp_code_num] = gen(temp_code_num, "jmp", 0, code_num);
    code_list[code_num] = gen(code_num, "int", 0, temp_tab_end - temp_tab_num + 3);
    code_num++;
	SP->add_child(S(sym, depth + 1, SP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));

	code_list[code_num] = gen(code_num, "opr", 0, 0);
	code_num++;
	return SP;
}
node* CP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* CP = new node("CP", depth, fa);
	node_list[node_num++] = CP;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 6 && word_list[sym].substr(1, 5) == "const")
	{
		CP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, CP));
		sym++;
	}
	else
	{
		cout << "ERROR213" << endl;
	}
	
	CP->add_child(CD(sym, depth + 1, CP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	cnt_word = word_list[sym];
	while (cnt_word[1] == ',')
	{
		CP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, CP));
		sym++;
		CP->add_child(CD(sym, depth + 1, CP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		cnt_word = word_list[sym];		
	}
	if (cnt_word[1] == ';')
	{
		CP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, CP));
		sym++;
	}
	else
	{
		cout << "ERROR232" << endl;
	}
	return CP;
}
node* VP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx)
{
	node* VP = new node("VP", depth, fa);
	node_list[node_num++] = VP;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 4 && cnt_word.substr(1, 3) == "var")
	{
		VP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, VP));
		sym++;
	}
	else
	{
		cout << "ERROR249" << endl;
	}
	VP->add_child(ID(sym, depth + 1, VP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	t_list[tab_num++] = new tab_t(get_id(VP->find_last_child()->first_child->word), "VARIABLE", 0, lev, dx++, 0, 0);
	cnt_word = word_list[sym];
	while(cnt_word[1] == ',')
	{
		VP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, VP));
		sym++;
		VP->add_child(ID(sym, depth + 1, VP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		t_list[tab_num++] = new tab_t(get_id(VP->find_last_child()->first_child->word), "VARIABLE", 0, lev, dx++, 0, 0);
		cnt_word = word_list[sym];	
	}	
	if (cnt_word[1] == ';')
	{
		VP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, VP));
		sym++;
	}
	else
	{
		cout << "ERROR267" << endl;
	}
	return VP;
}
node* PP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx)
{
	node* my_PP = new node("PP", depth, fa);
	node_list[node_num++] = my_PP;
	my_PP->add_child(PRH(sym, depth + 1, my_PP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev, dx));
	int temp_dx = dx;
	int temp_tab_num = tab_num;
	dx = 3;
	my_PP->add_child(SP(sym, depth + 1, my_PP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev + 1, dx));
	string cnt_word = word_list[sym];
	if (cnt_word[1] == ';')
	{
		my_PP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, my_PP));
		sym++;
	}
	else
	{
		cout << "ERROR285" << endl;	
	}
    dx = temp_dx;
	t_list[temp_tab_num - 1]->size = dx - 3;
	cnt_word = word_list[sym];
	int len = cnt_word.length();
	while (len >= 10 && cnt_word.substr(1, 9) == "procedure")
	{
		my_PP->add_child(PP(sym, depth + 1, my_PP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev, dx));
		dx++;
		cnt_word = word_list[sym];
		len = cnt_word.length();
	}
	return my_PP;
}
node* S(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* S = new node("S", depth, fa);
	node_list[node_num++] = S;
	string cnt_word = word_list[sym + 1];
	int len = cnt_word.length();
	if (len >= 3 && cnt_word.substr(1,2) == ":=")
	{
		S->add_child(ASS(sym, depth + 1, S, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	}
	else
	{
		cnt_word = word_list[sym];
		len = cnt_word.length();
		if (len >= 3 && cnt_word.substr(1,2) == "if")
		{
			S->add_child(CON(sym, depth + 1, S, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		}
		else if(len >= 6 && cnt_word.substr(1,5) == "while")
		{
			S->add_child(WHI(sym, depth + 1, S, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		}
		else if(len >= 4 && cnt_word.substr(1,4) == "call")
		{
			S->add_child(CAL(sym, depth + 1, S, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		}
		else if(len >= 5 && cnt_word.substr(1,4) == "read")
		{
			S->add_child(RED(sym, depth + 1, S, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));	
		}
		else if(len >= 6 && cnt_word.substr(1,5) == "write")
		{
			S->add_child(WRI(sym, depth + 1, S, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		}
		else if(len >= 6 && cnt_word.substr(1,5) == "begin")
		{
			S->add_child(COM(sym, depth + 1, S, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		}
		else
		{
			//do nothing
		}
	}
 	return S;	
}
node* CD(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* CD = new node("CD", depth, fa);
	node_list[node_num++] = CD;
	CD->add_child(ID(sym, depth + 1, CD, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	string cnt_word = word_list[sym];
	if (cnt_word[1] == '=')
	{
		CD->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, CD));
		sym++;	
	}
	else
	{
		cout << "ERROR355" << endl; 
	}
	CD->add_child(UI(sym, depth + 1, CD, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	t_list[tab_num++] = new tab_t(get_id(CD->first_child->first_child->word), "CONSTANT", get_ui(CD->find_last_child()->first_child->word), lev , 0, 0, 0);
	return CD;
}
node* ID(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* ID = new node("ID", depth, fa);
	node_list[node_num++] = ID;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 2 && cnt_word[1] == '1')
	{
		ID->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, ID));
		sym++;
	}
	else
	{
		cout << "ERROR373" << endl;
	}
	return ID;
}
node* PRH(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev, int& dx)
{
	node* PRH = new node("PRH",depth,fa);
	node_list[node_num++] = PRH;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 10 && cnt_word.substr(1, 9) == "procedure")
	{
		PRH->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, PRH));	
		sym++;
	}
	else
	{
		cout << "ERROR390" << endl;
	}
	PRH->add_child(ID(sym, depth + 1, PRH, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	t_list[tab_num++] = new tab_t(get_id(PRH->find_last_child()->first_child->word), "PROCEDURE", 0, lev, dx++, 0, 0);
	cnt_word = word_list[sym];
	len = cnt_word.length();
	if (len >= 2 && cnt_word[1] == ';')
	{
		PRH->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, PRH));
		sym++;
	}
	else
	{
		cout << "ERROR402" << endl;
	}
	return PRH;
}
node* ASS(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* ASS = new node("ASS", depth, fa);
	node_list[node_num++] = ASS;
	ASS->add_child(ID(sym, depth + 1, ASS, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	pair<int,int> my_id_code = find_char(get_id(ASS->find_last_child()->first_child->word),lev,tab_num,t_list);
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 3 && cnt_word.substr(1, 2) == ":=")
	{
		ASS->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, ASS));
		sym++;	
	}	
	else
	{
		cout << "ERROR420" << endl;
	}
	ASS->add_child(EXP(sym, depth + 1, ASS, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	code_list[code_num] = gen(code_num, "sto", my_id_code.first, my_id_code.second);
	code_num++;
	return ASS; 	
}
node* CON(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* CON = new node("CON", depth, fa);
	node_list[node_num++] = CON;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 3 && cnt_word.substr(1,2) == "if")
	{
		CON->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, CON));
		sym++; 
	}
	else
	{
		cout << "ERROR438" << endl;
	}
	CON->add_child(COS(sym, depth + 1, CON, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	int temp_num = code_num;
	code_list[code_num] = gen(code_num, "jpc", 0, -1);
	code_num++;
	cnt_word = word_list[sym];
	len = cnt_word.length();
	if (len >= 5 && cnt_word.substr(1,4) == "then")
	{
		CON->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, CON));
		sym++;
	}
	else
	{
		cout << "ERROR450" << endl;
	}
	CON->add_child(S(sym, depth + 1, CON, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	code_list[temp_num] = gen(temp_num, "jpc", 0, code_num);
	return CON;
}
node* WHI(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* WHI = new node("WHI", depth, fa);
	node_list[node_num++] = WHI;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 6 && cnt_word.substr(1,5) == "while")
	{
		WHI->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, WHI));
		sym++;	
	} 
	else
	{
		cout << "ERROR468" << endl;
	}
	int ret_num = code_num;
	WHI->add_child(COS(sym, depth + 1, WHI, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	int temp_num = code_num;
	code_list[code_num] = gen(code_num, "jpc", 0, -1);
	code_num++;
	cnt_word = word_list[sym];
	len = cnt_word.length();
	if (len >= 3 && cnt_word.substr(1,2) == "do")
	{
		WHI->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, WHI));
		sym++;
	}
	else
	{
		cout << "ERROR480" << endl;
	}
	WHI->add_child(S(sym, depth + 1, WHI, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	code_list[code_num] = gen(code_num , "jmp", 0, ret_num );
	code_num++;
	code_list[temp_num] = gen(temp_num , "jpc", 0, code_num );
	return WHI; 
} 
node* CAL(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* CAL = new node("CAL", depth, fa);
	node_list[node_num++] = CAL;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 5 && cnt_word.substr(1,4) == "call")
	{
		CAL->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, CAL));
		sym++;	
	}
	else
	{
		cout << "ERROR498" << endl;
	}
	CAL->add_child(ID(sym, depth + 1, CAL, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	string pro_name = get_id(CAL->find_last_child()->first_child->word);
	pair<int,int> pro_info;
	int cnt_lev = lev;
	int i;
	bool flag = false;
	for (i = tab_num - 1; i>=0; i--)
    {
        if (t_list[i]->kind == "PROCEDURE")
        {
            if (flag == false && t_list[i]->level == lev - 1)
            {
                flag = true;
            }
            if (flag)
            {
                if (t_list[i]->level < cnt_lev)
                {
                    cnt_lev--;
                }
                if (t_list[i]->level <= cnt_lev && t_list[i]->name == pro_name)
                {
                    pro_info.first = t_list[i]->level;
                    pro_info.second = t_list[i]->in_add;
                    break;
                }
            }
            else
            {
                if (t_list[i]->name == pro_name)
                {
                    pro_info.first = t_list[i]->level;
                    pro_info.second = t_list[i]->in_add;
                    break;
                }
            }
        }
    }
	if (i == -1)
    {
	    cout << "ERROR" << endl;
    }
	else {
        code_list[code_num] = gen(code_num, "cal", lev - pro_info.first, pro_info.second);
        code_num++;
    }
	return CAL;
}
node* RED(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* RED = new node("RED", depth, fa);
	node_list[node_num++] = RED;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 5 && cnt_word.substr(1,4) == "read")
	{
		RED->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, RED));
		sym++;
	}
	else
	{
		cout << "ERROR516" << endl;
	}
	cnt_word = word_list[sym];
	len = cnt_word.length();
	if (len >= 2 && cnt_word[1] == '(')
	{
		RED->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, RED));
		sym++;
	}
	else
	{
		cout << "ERROR527" << endl;
	}
	RED->add_child(ID(sym, depth + 1, RED, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev)); 
	code_list[code_num] = gen(code_num, "opr", 0, 16);
	code_num++;
	pair<int,int> my_id_code = find_char(get_id(RED->find_last_child()->first_child->word), lev, tab_num, t_list);
	if (my_id_code.second == -1)
	{
		cout << "ERROR" << endl;
	}
	else
	{
		code_list[code_num] = gen(code_num, "sto", my_id_code.first, my_id_code.second);
		code_num++;
	}
	cnt_word = word_list[sym];
	len = cnt_word.length();
	while (len >= 2 && cnt_word[1] == ',')
	{
		RED->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, RED));
		sym++;
		RED->add_child(ID(sym, depth + 1, RED, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		code_list[code_num] = gen(code_num, "opr", 0, 16);
		code_num++;
		my_id_code = find_char(get_id(RED->find_last_child()->first_child->word),lev, tab_num, t_list);
		if (my_id_code.second == -1)
		{
			cout << "ERROR" << endl;
		}
		else
		{
			code_list[code_num] = gen(code_num, "sto", my_id_code.first, my_id_code.second);
			code_num++;
		}
		cnt_word = word_list[sym];
		len = cnt_word.length();			
	}
	if (len >= 2 && cnt_word[1] == ')')
	{
		RED->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, RED));
		sym++;		 	
	}
	else
	{
		cout << "ERROR547" << endl;
	}
	return RED;
}
node* WRI(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* WRI = new node("WRI", depth, fa);
	node_list[node_num++] = WRI;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 6 && cnt_word.substr(1,5) == "write")
	{
		WRI->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, WRI));
		sym++;
	}
	else
	{
		cout << "ERROR564" << endl;
	}
	cnt_word = word_list[sym];
	len = cnt_word.length();
	if (len >= 2 && cnt_word[1] == '(')
	{
		WRI->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, WRI));
		sym++;
	}
	else
	{
		cout << "ERROR575" << endl;
	}
	WRI->add_child(ID(sym, depth + 1, WRI, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev)); 
	pair<int,int> my_id_code = find_char(get_id(WRI->find_last_child()->first_child->word), lev, tab_num, t_list);
	if (my_id_code.second == -1)
	{
		code_list[code_num] = gen(code_num , "lit", 0, my_id_code.first);
		code_num++;
		code_list[code_num] = gen(code_num , "opr", 0, 14);
		code_num++;
		code_list[code_num] = gen(code_num , "opr", 0, 15);
		code_num++;
	}
	else
	{
		code_list[code_num] = gen(code_num , "lod", my_id_code.first, my_id_code.second);
		code_num++;
		code_list[code_num] = gen(code_num , "opr", 0, 14);
		code_num++;
        code_list[code_num] = gen(code_num , "opr", 0, 15);
        code_num++;
	}
	cnt_word = word_list[sym];
	len = cnt_word.length();
	while (len >= 2 && cnt_word[1] == ',')
	{
		WRI->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, WRI));
		sym++;
		WRI->add_child(ID(sym, depth + 1, WRI, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		my_id_code = find_char(get_id(WRI->find_last_child()->first_child->word), lev, tab_num, t_list);
		if (my_id_code.second == -1)
		{
			code_list[code_num] = gen(code_num, "lit", 0, my_id_code.first);
			code_num++;
			code_list[code_num] = gen(code_num, "opr", 0, 14);
			code_num++;
            code_list[code_num] = gen(code_num, "opr", 0, 15);
            code_num++;
		}
		else
		{
			code_list[code_num] = gen(code_num, "lod", my_id_code.first, my_id_code.second);
			code_num++;
			code_list[code_num] = gen(code_num , "opr", 0, 14);
			code_num++;
            code_list[code_num] = gen(code_num , "opr", 0, 15);
            code_num++;
		}
		cnt_word = word_list[sym];
		len = cnt_word.length();			
	}
	if (len >= 2 && cnt_word[1] == ')')
	{
		WRI->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, WRI));
		sym++;		 	
	}
	else
	{
		cout << "ERROR595" << endl;
	}
	return WRI;
}
node* COM(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* COM = new node("COM", depth, fa);
	node_list[node_num++] = COM;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 6 && cnt_word.substr(1,5) == "begin")
	{
		COM->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, COM));
		sym++;	
	}
	else
	{
		cout << "ERROR612" << endl;
	}
	COM->add_child(S(sym, depth + 1, COM, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	cnt_word = word_list[sym];
	len = cnt_word.length();
	while (len >= 2 && cnt_word[1] == ';')
	{
		COM->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, COM));
		sym++;
		COM->add_child(S(sym, depth + 1, COM, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		cnt_word = word_list[sym];
		len = cnt_word.length();
	}
	if (len >= 4 && cnt_word.substr(1, 3) == "end")
	{
		COM->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, COM));
		sym++;
	}
	else
	{
		cout << "ERROR632" << endl;
	}
	return COM;
}
node* UI(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* UI = new node("UI", depth, fa);
	node_list[node_num++] = UI;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 2 && cnt_word[1] == '2')
	{
		UI->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, UI));
		sym++;
	}
	else
	{
		cout << "ERROR649" << endl;
	}
	return UI;
}
node* EXP(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* EXP = new node("EXP", depth, fa);
	node_list[node_num++] = EXP;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 2 && ((cnt_word[1] == '+') || (cnt_word[1] == '-')))
	{
		EXP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, EXP));
		sym++;
	}
	EXP->add_child(TER(sym, depth + 1, EXP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
	if (len >=2 && cnt_word[1] == '-')
	{
		code_list[code_num] = gen(code_num, "opr", 0, 1);
		code_num++;
	}
	cnt_word = word_list[sym];
	len = cnt_word.length();
	while (len >= 2 && ((cnt_word[1] == '+') || (cnt_word[1] == '-')))
	{
		EXP->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, EXP));
		sym++;
		EXP->add_child(TER(sym, depth + 1, EXP, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));	
		if (cnt_word[1] == '+')
		{
			code_list[code_num] = gen(code_num, "opr", 0, 2);
			code_num++;
		}
		else if(cnt_word[1] == '-')
		{
			code_list[code_num] = gen(code_num, "opr", 0, 3);
			code_num++;
		}
		cnt_word = word_list[sym];
		len = cnt_word.length();	
	}
	return EXP;
}
node* COS(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* COS = new node("COS", depth, fa);
	node_list[node_num++] = COS;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 4 && cnt_word.substr(1, 3) == "odd")
	{
		COS->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, COS));
		sym++;
		COS->add_child(EXP(sym, depth + 1, COS, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));	
		code_list[code_num] = gen(code_num, "opr", 0, 6);
		code_num++;
	}
	else
	{
		COS->add_child(EXP(sym, depth + 1, COS, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		cnt_word = word_list[sym];
		len = cnt_word.length();
		if (len >= 3 && (cnt_word.substr(1,2) == "<=" || cnt_word.substr(1,2)== ">="))
		{
			COS->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, COS)); 
			sym++;
		}
		else if (len >= 2 && (cnt_word[1] == '=' || cnt_word[1] == '#' || cnt_word[1] == '<' || cnt_word[1] == '>'))	
		{
			COS->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, COS));
			sym++;
		}
		else
		{
			cout << "ERROR706" << endl;
		}
		COS->add_child(EXP(sym, depth + 1, COS, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		if (len >= 3 && cnt_word.substr(1,2) == "<=")
		{
			code_list[code_num] = gen(code_num, "opr", 0, 13);
			code_num++;
		}
		else if (len >= 3 && cnt_word.substr(1,2) == ">=")
		{
			code_list[code_num] = gen(code_num, "opr", 0, 11);
			code_num++;
		}
		else if(len >= 2 && cnt_word[1] == '<')
		{
			code_list[code_num] = gen(code_num, "opr", 0, 10);
			code_num++;
		}
		else if(len >=2 && cnt_word[1] == '>')
		{
			code_list[code_num] = gen(code_num, "opr", 0, 12);
			code_num++;
		}
		else if(len >=2 && cnt_word[1] == '=')
		{
			code_list[code_num] = gen(code_num , "opr", 0, 8);
			code_num++;
		}
		else if(len >=2 && cnt_word[1] == '#')
		{
			code_list[code_num] = gen(code_num, "opr", 0, 9);
			code_num++;
		}
	}
	return COS;
}
node* TER(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* TER = new node("TER", depth, fa);
	node_list[node_num++] = TER;
	TER->add_child(FAC(sym, depth + 1, TER, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));  
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	while (len >= 2 && (cnt_word[1] == '*' || cnt_word[1] == '/'))
	{
		TER->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, TER));
		sym++;
		TER->add_child(FAC(sym, depth + 1, TER, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));	
		if (cnt_word[1] == '*')
		{
			code_list[code_num] = gen(code_num, "opr", 0, 4);
			code_num++;		
		}		
		else
		{
			code_list[code_num] = gen(code_num, "opr", 0, 5);
			code_num++;
		}
		cnt_word = word_list[sym];
		len = cnt_word.length();
	}
	return TER;
} 
node* FAC(int& sym, int depth, node* fa,node* (&node_list)[maxn], int& node_num, string (&word_list)[maxn], int& tab_num, tab_t* (&t_list)[maxn], int& code_num, string (&code_list)[maxn], int lev)
{
	node* FAC = new node("FAC", depth, fa);
	node_list[node_num++] = FAC;
	string cnt_word = word_list[sym];
	int len = cnt_word.length();
	if (len >= 2 && cnt_word[1] == '(')
	{
		FAC->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, FAC));
		sym++;
		FAC->add_child(EXP(sym, depth + 1, FAC, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev));
		cnt_word = word_list[sym];
		int len = cnt_word.length();
		if (len >= 2 && cnt_word[1] == ')')
		{
			FAC->add_child(node_list[node_num++] = new node(word_list[sym], depth + 1, FAC));
			sym++;
		}
		else
		{
			cout << "ERROR749" << endl;	
		}	
	}
	else if (len >= 2 && cnt_word[1] == '1')
	{
		FAC->add_child(ID(sym, depth + 1, FAC, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev)); 
		pair<int,int> my_id_code = find_char(get_id(FAC->find_last_child()->first_child->word), lev, tab_num, t_list);
		if (my_id_code.second == -1)
		{
			code_list[code_num] = gen(code_num, "lit", 0, my_id_code.first);
			code_num++;
		}
		else
		{
			code_list[code_num] = gen(code_num, "lod", my_id_code.first, my_id_code.second);
			code_num++;
		}
	} 
	else if (len >= 2 && cnt_word[1] == '2')
	{
		FAC->add_child(UI(sym, depth + 1, FAC, node_list, node_num, word_list, tab_num, t_list, code_num, code_list, lev)); 
		code_list[code_num] = gen(code_num, "lit", 0, get_ui(FAC->find_last_child()->first_child->word));
		code_num++;
	}
	else
	{
		cout << "ERROR762" << endl;
	}
	return FAC;
}
