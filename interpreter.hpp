#include <iostream>
#include <string>
#include <cstdio>
#define maxn 10000
using namespace std;
struct ins{
    int add = 0;
    string name = "";
    int sour = 0;
    int tar = 0;
};
ins get_ins(string& l)
{
    ins my_ins;
    int len = l.length();
    int flag = 0;
    for (int i = 0; i < len; i++)
    {
        if (l[i] == ' ')
        {
            flag++;
            continue;
        }
        if (flag == 0)
        {
            my_ins.add = my_ins.add * 10 + l[i] - '0';
        }
        else if (flag == 1)
        {
            my_ins.name = my_ins.name + l[i];
        }
        else if (flag == 2)
        {
            my_ins.sour = my_ins.sour * 10 + l[i] - '0';
        }
        else
        {
            my_ins.tar = my_ins.tar * 10 + l[i] - '0';
        }
    }
    return my_ins;
}
int get_sl(int b, int lev_diff, int (&S)[maxn])
{
    int sl = b;
    while (lev_diff > 0)
    {
        sl = S[sl];
        lev_diff -= 1;
    }
    return sl;
}
void interpreter(int code_num, string (&code_list)[maxn])
{
    int S[maxn];
    string l;
    int p = 0, t = 0, b = 0;
    S[0] = 0;
    S[1] = 0;
    S[2] = 0;
    while (true) {
        l = code_list[p++];
        ins l_ins = get_ins(l);
        string name = l_ins.name;
        if (name == "jmp")
        {
            p = l_ins.tar;
        }
        else if (name == "jpc")
        {
            if (S[t--] == 0)
            {
                p = l_ins.tar;
            }
        }
        else if (name == "lit")
        {
            S[++t] = l_ins.tar;
        }
        else if (name == "lod")
        {
            int sl = get_sl(b, l_ins.sour, S);
            S[++t] = S[sl + l_ins.tar];
        }
        else if (name == "sto")
        {
            int sl = get_sl(b, l_ins.sour, S);
            S[sl + l_ins.tar] = S[t];
        }
        else if (name == "cal")
        {
            S[t + 1] = get_sl(b, l_ins.sour, S);
            S[t + 2] = b;
            S[t + 3] = p;
            b = t + 1;
            p = l_ins.tar;
        }
        else if (name == "int")
        {
            t += l_ins.tar;
        }
        else if (name == "opr")
        {
            if (l_ins.tar == 0)
            {
                t = b - 1;
                p = S[t + 3];
                b = S[t + 2];
            }
            else if (l_ins.tar == 1)
            {
                S[t] = -S[t];
            }
            else if (l_ins.tar == 2)
            {
                int tar = S[t--];
                int sour = S[t--];
                S[++t] = sour + tar;
            }
            else if (l_ins.tar == 3)
            {
                int tar = S[t--];
                int sour = S[t--];
                S[++t] = sour - tar;
            }
            else if (l_ins.tar == 4)
            {
                int tar = S[t--];
                int sour = S[t--];
                S[++t] = sour * tar;
            }
            else if (l_ins.tar == 5)
            {
                int tar = S[t--];
                int sour = S[t--];
                S[++t] = sour / tar;
            }
            else if (l_ins.tar == 6)
            {
                S[t] = S[t] % 2;
            }
            else if (l_ins.tar == 7)
            {
                //do nothing
            }
            else if (l_ins.tar == 8)
            {
                int tar = S[t--];
                int sour = S[t--];
                if (sour == tar)
                {
                    S[++t] = 1;
                }
                else{
                    S[++t] = 0;
                }
            }
            else if (l_ins.tar == 9)
            {
                int tar = S[t--];
                int sour = S[t--];
                if (sour != tar)
                {
                    S[++t] = 1;
                }
                else
                {
                    S[++t] = 0;
                }
            }
            else if (l_ins.tar == 10)
            {
                int tar = S[t--];
                int sour = S[t--];
                if (sour < tar)
                {
                    S[++t] = 1;
                }
                else
                {
                    S[++t] = 0;
                }
            }
            else if (l_ins.tar == 11)
            {
                int tar = S[t--];
                int sour = S[t--];
                if (sour >= tar)
                {
                    S[++t] = 1;
                }
                else
                {
                    S[++t] = 0;
                }
            }
            else if (l_ins.tar == 12)
            {
                int tar = S[t--];
                int sour = S[t--];
                if (sour > tar)
                {
                    S[++t] = 1;
                }
                else
                {
                    S[++t] = 0;
                }
            }
            else if (l_ins.tar == 13)
            {
                int tar = S[t--];
                int sour = S[t--];
                if (sour <= tar)
                {
                    S[++t] = 1;
                }
                else
                {
                    S[++t] = 0;
                }
            }
            else if (l_ins.tar == 14)
            {
                cout << S[t--];
            }
            else if (l_ins.tar == 15)
            {
                cout << endl;
            }
            else if (l_ins.tar == 16)
            {
                cin >> S[++t];
            }
        }
        if (p == 0) {
            return;
        }
    }
}