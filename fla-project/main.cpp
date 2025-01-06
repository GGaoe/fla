#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
using namespace std;

bool dealType=0;//0表示PDA，1表示TM

//全局变量
int stateNum=0;
string state[10000];

int inputAlphabetNum=0;
char inputAlphabet[10000];

int stackAlphabetNum=0;
char stackAlphabet[10000];

string startState;

char startStack;

int acceptStateNum=0;
string acceptingStates[10000];

int transitionNum=0;
string transitions[10000][5];
//可能把状态转化为结构体会更好一点（？

void clean_Annotation(string* s){//去掉注释
    size_t start = s->find(";");
    if (start != string::npos) {
        *s = s->substr(0, start);
    }
}

void parse_string(string s, string* arr, int* Num){
    size_t start = s.find('{');
    size_t end = s.find('}');
    if (start != string::npos && end != string::npos && start < end) {
        string statesStr = s.substr(start + 1, end - start - 1);
        stringstream ss(statesStr);
        string state;
        while (getline(ss, state, ',')) {
            arr[(*Num)++] = state;
        }
    }
}

void parse_char(string s, char* arr, int* num){
    size_t start = s.find('{');
    size_t end = s.find('}');
    if (start != string::npos && end != string::npos && start < end) {
        string statesStr = s.substr(start + 1, end - start - 1);
        stringstream ss(statesStr);
        string Alphabet;
        while (getline(ss, Alphabet, ',')) {
            arr[(*num)++] = Alphabet[0];
        }
    }
}

bool parsePDA(string fileContent){
    istringstream fileStream(fileContent);
    string line;
    while (getline(fileStream, line)) {
        clean_Annotation(&line);
        if (line.find("#Q") == 0) {
            parse_string(line, state, &stateNum);
        }
        else if(line.find("#S") == 0) {
            parse_char(line, inputAlphabet, &inputAlphabetNum);
        }
        else if(line.find("#G") == 0) {
            parse_char(line, stackAlphabet, &stackAlphabetNum);
        }
        else if(line.find("#q0") == 0) {
            startState = line.substr(6);
        }
        else if(line.find("#z0") == 0) {
            startStack = line[6];
        }
        else if(line.find("#F") == 0) {
            parse_string(line, acceptingStates, &acceptStateNum);
        }
        else if(line.empty()) {
            continue;
        }
        else{
            stringstream ss(line);
            string transition[5];
            int i = 0;
            while (getline(ss, transition[i], ' ')) {
                i++;
            }
            for (int j = 0; j < 5; j++) {
                transitions[transitionNum][j] = transition[j];
            }
            transitionNum++;
        }
    }

    return true;
}

bool parseTM(string fileContent){
    cout<<fileContent<<endl;
    return true;
}

void read_test(){
    //READ TEST
    cout<<"STATE:"<<endl;
    for(int i=0; i<stateNum; i++){
        cout<<state[i]<<endl;
    }
    cout<<endl;
    cout<<"INPUT ALPHABET:"<<endl;
    for(int i=0; i<inputAlphabetNum; i++){
        cout<<inputAlphabet[i]<<endl;
    }
    cout<<endl;
    cout<<"STACK ALPHABET:"<<endl;
    for(int i=0; i<stackAlphabetNum; i++){
        cout<<stackAlphabet[i]<<endl;
    }
    cout<<endl;
    cout<<"START STATE:"<<endl;
    cout<<startState<<endl;
    cout<<endl;
    cout<<"START STACK:"<<endl;
    cout<<startStack<<endl;
    cout<<endl;
    cout<<"ACCEPTING STATES:"<<endl;
    for(int i=0; i<acceptStateNum; i++){
        cout<<acceptingStates[i]<<endl;
    }
    cout<<endl;
    cout<<"TRANSITIONS:"<<endl;
    for(int i=0; i<transitionNum; i++){
        for(int j=0; j<5; j++){
            cout<<transitions[i][j]<<" ";
        }
        cout<<endl;
    }
}

int main(int argc, char* argv[]){
    if(argc != 3){
        return 1;//参数错误
    }
    //读取PDA或者TM文件
    ifstream file(argv[argc-2]);
    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    //根据文件名判断是PDA还是TM
    if(argv[argc-2][strlen(argv[1])-1] == 'a'){
        //cout<<"PDA"<<endl;
        dealType = 0;
    }
    else {
        //cout<<"TM"<<endl;
        dealType = 1;
    }

    //读取输入字符串
    string inputString = argv[argc-1];

    //PDA解析器
    if(dealType == 0)
        parsePDA(fileContent);

    //TM解析器
    else{
        parseTM(fileContent);
    }

    //读取测试
    read_test();

    
    return 0;
}