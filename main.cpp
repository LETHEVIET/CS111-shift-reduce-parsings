#include <bits/stdc++.h>
#include<conio.h>
//ditmemay
using namespace std;

struct grammer{
    string p;
    vector<string> prod;
};
vector<grammer> g;

struct Row{
    vector<string> stk;
    string currSym;
    vector<string> remains;
    string action;
    string reduceNum;
};

vector<string> input;

vector<string> split (string s){
    vector<string> line;
    string c = "";
    for (int i = 0; i < s.size(); i++){
        if (s[i] == ' '){
            if (c != ""){
                line.push_back(c);
                c = "";
            }
        }else{
            c+=s[i];
        }
    }line.push_back(c);

    for (int i = 0; i < input.size(); i++){
        cout<<line[i];
    }

    return line;
}

void read_grammar()
{
    ifstream f("grammar.txt");
    string s;
    while (!f.eof())
    {
        getline(f, s);

        if(s=="") continue;
        vector<string> line = split(s);
        string p = line[0];

        vector<string> prod;

        for (int i = 2; i< line.size(); i++){
            if (line[i] == "|"){
                if (prod.size() != 0){
                    grammer gr;
                    gr.p = p;
                    gr.prod = prod;
                    g.push_back(gr);
                    while(prod.size()){
                        prod.pop_back();
                    }
                }
            }else{
                prod.push_back(line[i]);
            }
        }

        grammer gr;
        gr.p = p;
        gr.prod = prod;
        g.push_back(gr);

    }


    /*
    for (int i = 0; i < g.size(); i++){
        cout<< g[i].p << "->";
        for (int j = 0; j< g[i].prod.size(); j++){
                cout << g[i].prod[j] << " ";
        }
        cout<< endl;
    }
    */
}

void read_input(){
    freopen("input.txt", "r", stdin);
    string s;
    getline(cin, s);
    string c = "";
    for (int i = 0; i < s.size(); i++){
        //cout<< i << " " << s[i] << "  " << c << endl;
        if (s[i] == ' '){
            if (c != ""){
                input.push_back(c);
                c = "";
            }
        }else{
            c+=s[i];
        }
    }input.push_back(c);
}

void print (vector<string> str){
    for (int i = 0; i < str.size(); i++){
        cout<<str[i] << " " ;
    }
}

int main()
{
    read_grammar();

    read_input();


    vector<string> stk;

    string currSym = input[0];
    input.erase(input.begin());

    //string action = "shift";

    vector<Row> table;
    int reduceCount = 0;
    while (true){
        Row line;
        line.stk = stk;
        line.currSym = currSym;
        line.remains = input;
        line.reduceNum = "";

        grammer replacement;
        bool reduce = false;

        int c = 0;
        bool gotR = false;
        int bk;

        for (int k = 0; k < stk.size(); k++){
            vector<string> tmp;

            for (int i = k; i < stk.size(); i++){
                tmp.push_back(stk[i]);
            }
            for (int m = 0; m < g.size(); m++){
                if (g[m].prod.size() >= tmp.size()){
                    bool matched = true;
                    for (int i = 0; i< tmp.size(); i++){
                        bool d = tmp[i] == g[m].prod[i];
                        if (tmp[i] != g[m].prod[i]){
                            matched = false;
                        }
                    }
                    if (matched){
                        if (g[m].prod.size() == tmp.size() && !gotR){
                            //c++;
                            replacement = g[m];
                            gotR = true;
                            bk = k;
                        }else{
                            if (g[m].prod[tmp.size()] == currSym){
                                c++;
                            }
                        }
                    }
                }
            }
        }
        if ((c == 0 && gotR) || (currSym == "" && gotR)){
            reduce = true;
            while (stk.size() > bk){
                stk.pop_back();
            }

            stk.push_back(replacement.p);
            //break;
        }

        if (reduce){
            line.action = "Reduce: " + replacement.p + "->";
            for (int i = 0; i < replacement.prod.size(); i++){
                line.action += replacement.prod[i];
            }

            reduceCount ++;
            line.reduceNum  = to_string(reduceCount);
        }else{
            if (currSym ==""){

                table.push_back(line);
                break;
            }
            stk.push_back(currSym);
            if (input.empty()){
                currSym = "";
            }else{
                currSym = input[0];
                input.erase(input.begin());
            }
            line.action = "Shift";
            //action = "shift";
        }
        table.push_back(line);
    }

    freopen("output.txt", "w", stdout);

    if (stk.size() == 1 && stk[0] == g[0].p){
        cout<< "A";
    }else{
        cout<< "F";
    }
    cout<< endl;
    cout<< "Stack" << "\t\t\t\t"
    << "CurrSym" << "\t\t"
    << "Rest of Input" << "\t\t\t\t"
    << "Action" << "\t\t"
    << "Reduction number" << endl;

    for (int i = 0; i< table.size(); i++){
        print(table[i].stk);
        cout<< "\t\t\t\t";
        cout<< table[i].currSym;
        cout<< "\t\t";
        print(table[i].remains);
        cout<< "\t\t\t\t";
        cout<< table[i].action;
        cout<< "\t\t";
        cout<< table[i].reduceNum;
        cout<< endl;
    }

    return 0;
}
