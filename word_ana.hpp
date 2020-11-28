/*词法分析*/
#include <iostream>
#include <fstream>
#include <string>
#include <cstdio>
#define maxn 10000
using namespace std;
string trans(char c)
{
	char s[2] = {c, '\0'};
	string s1 = s;
	return s1;
}
/*种类编码*/
/*
标识符-1
常量-2
const
var
procedure
begin
end
odd
if
then
call
while
do
read
write
( )
= := + - * / # < <= > >=
, ;
*/
void deal_cnt(string& cnt_word, string (&word_list)[maxn], int& word_num, string& word_res)
{
    if (cnt_word == "const" || cnt_word == "var" || cnt_word == "procedure" || cnt_word == "begin" || cnt_word == "end" || cnt_word == "odd" || cnt_word == "if" || cnt_word == "then" || cnt_word == "call" || cnt_word == "while" || cnt_word == "do" || cnt_word == "read" || cnt_word == "write")
    {
		word_list[word_num++] = "(" + cnt_word + ",-)";
        word_res += "(" + cnt_word + ",-)\n";
    }
    else
    {
        int cnt_len = cnt_word.length();
        int j = 0;
        bool flag = true;
        while (j < cnt_len)
        {
            char c = cnt_word[j];
            if (c < '0' || c > '9')
            {
                flag = false;
                break;
            }
            j++;
        }
        if (flag)
        {
        	word_list[word_num++] = "(2," + cnt_word + ")";
            word_res += "(2," + cnt_word + ")\n";
        }
        else
        {
        	word_list[word_num++] = "(1," + cnt_word + ")";
            word_res += "(1," + cnt_word + ")\n";
        }
    }
    cnt_word = "";
}
void word_ana(string& word_res, string (&word_list)[maxn], int& word_num, const string& source)
{
	string cnt_word = "";
    int i = 0;
    int len = source.length();
    
    while (i < len)
    {
    	if (cnt_word.length() > 10)
    	{
    		cout << "ERROR80" << endl;
    		break;
		}
        char c = source[i];
        if (c == ',' || c == ';' || c == '=' || c == '+' || c == '-' || c == '*' || c == '/' || c == '#' || c == '(' || c == ')')
        {
        	if (cnt_word != "")
        		deal_cnt(cnt_word, word_list, word_num, word_res);
        	word_list[word_num++] = "(" + trans(c) + ",-)";
            word_res += "(" + trans(c) + ",-)\n";
        }
        else if (c == ':')
        {
            if (source[i + 1] == '=')
            {
            	if (cnt_word != "")
            		deal_cnt(cnt_word, word_list, word_num, word_res);
            	word_list[word_num++] = "(:=,-)";
                word_res += "(:=,-)\n";
                i++;
            }
            else
            {
                cout << "ERROR103" << endl;
            }
        }
        else if (c == '<' || c == '>')
        {
        	if (cnt_word != "")
        		deal_cnt(cnt_word, word_list, word_num, word_res);
            if (source[i + 1] == '=')
            {
            	word_list[word_num++] = "(" + trans(c) + "=,-)";
                word_res += "(" + trans(c) + "=,-)\n";
                i++;
            }
            else
            {
            	word_list[word_num++] = "(" + trans(c) + ",-)";
                word_res += "(" + trans(c) + ",-)\n";
            }
        }
        else if (c == ' ' || c == '\n')
        {
        	if (cnt_word != "") 
            	deal_cnt(cnt_word, word_list, word_num, word_res);
        }
        else if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9'))
        {
            cnt_word += source[i];
        }
        else
        { 
        	cout << "ERROR133" << endl;
		}
        i++; 
    }
}
