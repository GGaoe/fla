#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sstream>
#include <stack>
using namespace std;

bool dealType=0;//0表示PDA，1表示TM

bool verbose = false;

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

char blank;

int tapeNum=0;

std::stack<char> PDAstack;

int transitionNum=0;
string transitions[10000][5];

char tape[11][100000];

const int mid = 50000;

int tapePointer[11][2];//0表示左端点，1表示右端点

int pointer[11];

void clean_Annotation(string* s){//去掉注释
    size_t start = s->find(";");
    if (start != string::npos) {
        *s = s->substr(0, start);
    }
    int len = s->length()-1;
    for(int i=len; i>=0; i--){
        if((*s)[i] == ' '){
            *s = s->substr(0, i-1);
        }
        else{
            break;
        }
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
        else if(line.find("#B") == 0) {
            blank = '_';
        }
        else if(line.find("#F") == 0) {
            parse_string(line, acceptingStates, &acceptStateNum);
        }
        else if(line.find("#N") == 0){
            tapeNum = line[5]-'0';
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
    cout<<"STACK ALPHABET/TAPE ALPHABET:"<<endl;
    for(int i=0; i<stackAlphabetNum; i++){
        cout<<stackAlphabet[i]<<endl;
    }
    cout<<endl;
    cout<<"START STATE:"<<endl;
    cout<<startState<<endl;
    cout<<endl;
    if(!dealType){
        cout<<"START STACK:"<<endl;
        cout<<startStack<<endl;
    }
    else{
        cout<<"BLANK:"<<endl;
        cout<<blank<<endl;
        cout<<endl;
        cout<<"TAPE NUM:"<<endl;
        cout<<tapeNum<<endl;
        cout<<endl;
    }
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

int input_check(string inputString){
    for(int i=0; i<inputString.length(); i++){
        bool flag = false;
        for(int j=0; j<inputAlphabetNum; j++){
            if(inputString[i] == inputAlphabet[j]){
                flag = true;
                break;
            }
        }
        if(!flag){
            return i;
        }
    }
    return -1;
}

void PDAid(string inputString, int pointer, string currentState){
    cout<<"STEP:"<<pointer<<endl;
    cout<<"STATE:"<<currentState<<endl;
    cout<<"INPUT:";
    for(int i=pointer; i<inputString.length(); i++){
        cout<<inputString[i];
    }
    cout<<endl;
    //输出栈的内容
    cout<<"STACK:";
    std::stack<char> temp;
    // 遍历栈
        while (!PDAstack.empty()) {
            char topElement = PDAstack.top();
            PDAstack.pop();
            std::cout << topElement;

            // 将元素推入临时栈中
            temp.push(topElement);
        }
        cout<<endl;

        // 如果需要，将元素从临时栈移回原始栈
        while (!temp.empty()) {
            PDAstack.push(temp.top());
            temp.pop();
        }
    cout<<"---------------------------------------------"<<endl;
}

bool PDA_run(string inputString){
    PDAstack.push(startStack);
    string currentState = startState;
    for(int pointer = 0; pointer <= inputString.length(); pointer++){
        //输出一个即时描述
        if(verbose)
            PDAid(inputString, pointer, currentState);
        char currentInput = inputString[pointer];
        if(pointer == inputString.length()){
            currentInput = '_';
        }
        bool flag = false;
        if(PDAstack.empty()){
            return false;
        }
        for(int i=0; i<transitionNum&&!flag; i++){
            if(transitions[i][0] == currentState && transitions[i][1][0] == currentInput && transitions[i][2][0] == PDAstack.top()){
                flag = true;
                currentState = transitions[i][3];
                PDAstack.pop();
                if(transitions[i][4] == "_"){
                    continue;
                }
                else{ 
                    for(int j=transitions[i][4].length()-1; j>=0; j--){
                        PDAstack.push(transitions[i][4][j]);
                    }
                }
            }
        }
        if(!flag){
            return false;
        }
    }
    if(verbose)
        PDAid(inputString, inputString.length()+1, currentState);
    for(int i=0; i<acceptStateNum; i++){
        if(currentState == acceptingStates[i]){
            return true;
        }
    }
    return false;
}

int abs(int a){
    return a>0?a:-a;
}

void TMid(string state,int step){
    //输出一个即时描述

    //输出当前步数
    cout<<"Step   : "<<step<<endl;

    //输出当前状态
    cout<<"State  : "<<state<<endl;

    for(int i=0; i<tapeNum; i++){
        
        //输出当前磁带索引
        cout<<"Index"<<i<<" : ";
        if(tapePointer[i][0] > tapePointer[i][1]){
            cout<<abs(pointer[i]-mid)<<endl;
        }
        else{
            if(pointer[i] < tapePointer[i][0]){
                for(int j=pointer[i]; j<=tapePointer[i][1]; j++){
                    cout<<abs(j-mid)<<" ";
                }
                cout<<endl;
            }
            else if(pointer[i] > tapePointer[i][1]){
                for(int j=tapePointer[i][0]; j<=pointer[i]; j++){
                    cout<<abs(j-mid)<<" ";
                }
                cout<<endl;
            }
            else{
                for(int j=tapePointer[i][0]; j<=tapePointer[i][1]; j++){
                    cout<<abs(j-mid)<<" ";
                }
                cout<<endl;
            }
        }


        //输出当前磁带
        cout<<"Tape"<<i<<"  : ";
        if(tapePointer[i][0] > tapePointer[i][1]){
            cout<<blank<<endl;
        }
        else{
            if(pointer[i] < tapePointer[i][0]){
                for(int j=pointer[i]; j<=tapePointer[i][1]; j++){
                    cout<<tape[i][j]<<" ";
                }
                cout<<endl;
            }
            else if(pointer[i] > tapePointer[i][1]){
                for(int j=tapePointer[i][0]; j<=pointer[i]; j++){
                    cout<<tape[i][j]<<" ";
                }
                cout<<endl;
            }
            else{
                for(int j=tapePointer[i][0]; j<=tapePointer[i][1]; j++){
                    cout<<tape[i][j]<<" ";
                }
                cout<<endl;
            }
        }

        //输出当前磁头位置
        cout<<"Head"<<i<<"  : ";
        if(tapePointer[i][0] > tapePointer[i][1]){
            cout<<'^'<<endl;
        }
        else{
            for(int j=tapePointer[i][0]; j<pointer[i]; j++){
                cout<<"  ";
            }
            cout<<"^"<<endl;
        }
    }
    cout<<"---------------------------------------------"<<endl;
}

void TM_run(string inputString){
    int step = 0;

    //初始化所有磁带
    for(int i=0; i<tapeNum; i++){
        for(int j=0; j<100000; j++){
            tape[i][j] = blank;
        }
        tapePointer[i][0] = mid+1;
        tapePointer[i][1] = mid;
        pointer[i] = mid;
    }
    //先把输入字符串写到第一条磁带上
    for(int i=0; i<inputString.length(); i++){
        tape[0][i+mid] = inputString[i];
    }
    tapePointer[0][0] = mid;
    tapePointer[0][1] = mid+inputString.length()-1;

    bool flag = true;//还能找到一个合法转移
    string currentState = startState;//当前状态

    while(flag){
        if(verbose)
            TMid(currentState, step);
        flag = false;
        for(int i=0; i<transitionNum && !flag; i++){
            bool flag1 = (transitions[i][0] == currentState);
            bool flag2 = true;
            for(int j=0; j<tapeNum; j++){
                if((transitions[i][1][j]=='*'&& tape[j][pointer[j]] == blank) ||(transitions[i][1][j]!='*'&&tape[j][pointer[j]] != transitions[i][1][j])){
                    flag2 = false;
                    break;
                }
            }
            if(flag1 && flag2){
                flag = true;
                currentState = transitions[i][4];//转移到下一个状态
                for(int j=0; j<tapeNum; j++){//对每一条磁带进行操作
                    if(transitions[i][2][j] == '*'||transitions[i][2][j] == tape[j][pointer[j]]){
                        continue;
                    }
                    else if(transitions[i][2][j] == '_' && tape[j][pointer[j]] != blank){
                        tape[j][pointer[j]] = transitions[i][2][j];
                        if(pointer[j] == tapePointer[j][0]){//如果等于左端点
                            for(int k = tapePointer[j][0]; k<=tapePointer[j][1]; k++){
                                if(tape[j][k] != blank){
                                    break;
                                }
                                tapePointer[j][0]++;
                            }
                        }
                        else if(pointer[j] == tapePointer[j][1]){//如果等于右端点
                            for(int k = tapePointer[j][1]; k>=tapePointer[j][0]; k--){
                                if(tape[j][k] != blank){
                                    break;
                                }
                                tapePointer[j][1]--;
                            }
                        }
                    }
                    else if(transitions[i][2][j] != '_'){
                        tape[j][pointer[j]] = transitions[i][2][j];
                        if(tapePointer[j][0] > tapePointer[j][1]){
                            tapePointer[j][0] = pointer[j];
                            tapePointer[j][1] = pointer[j];
                        }
                        else if(pointer[j] < tapePointer[j][0]){
                            tapePointer[j][0] = pointer[j];
                        }
                        else if(pointer[j] > tapePointer[j][1]){
                            tapePointer[j][1] = pointer[j];
                        }
                    }
                }
                //移动磁头
                // for(int j=0; j<tapeNum; j++){
                //     cout<<"place0:"<<pointer[j]-mid<<endl;
                // }
                for(int j=0; j<tapeNum; j++){
                    if(transitions[i][3][j] == 'l'){
                        pointer[j]--;
                    }
                    else if(transitions[i][3][j] == 'r'){
                        pointer[j]++;
                    }
                    else if(transitions[i][3][j] == '*'){
                        continue;
                    }
                }
                // for(int j=0; j<tapeNum; j++){
                //     cout<<"place1:"<<pointer[j]-mid<<endl;
                // }
            }
        }
        step++;
    }

    //check the state
    if(tapePointer[0][0] > tapePointer[0][1]){
        cout<<"No stdout"<<endl;
    }
    else{
        if(verbose)cout<<"Result: ";
        for(int i=tapePointer[0][0]; i<=tapePointer[0][1]; i++){
            cout<<tape[0][i];
        }
        cout<<endl;
        if(verbose)cout<<"==================== END ===================="<<endl;
    }
}

bool Parameter_Check(int argc, char* argv[]){
    string s1 = argv[1];
    string s2 = argv[2];
    if(argc == 2){
        if(s1=="--help" || s1=="-h")return true;
        else return false;
    }
    else if (argc == 3){
        //第二个参数是.pda或者.tm,第三个参数是输入字符串
        if ((strlen(argv[1])>=4 && argv[1][strlen(argv[1])-1] == 'a' && argv[1][strlen(argv[1])-2] == 'd' 
        && argv[1][strlen(argv[1])-3] == 'p' && argv[1][strlen(argv[1])-4] == '.')||
        (strlen(argv[1])>=3 && argv[1][strlen(argv[1])-1] == 'm' && argv[1][strlen(argv[1])-2] == 't' 
        && argv[1][strlen(argv[1])-3] == '.'))return true;
        else return false;
    }
    else if (argc == 4){
        //第二个参数是[-h|--help]或者是[-v|--verbose],第三个参数是.pda或者.tm,第四个参数是输入字符串
        if(s1!="--help" && s1!="--verbose" && s1!="-h" && s1!="-v")return false;
        if ((strlen(argv[2])>=4 && argv[2][strlen(argv[2])-1] == 'a' && argv[2][strlen(argv[2])-2] == 'd' 
        && argv[2][strlen(argv[2])-3] == 'p' && argv[2][strlen(argv[2])-4] == '.')||
        (strlen(argv[2])>=3 && argv[2][strlen(argv[2])-1] == 'm' && argv[2][strlen(argv[2])-2] == 't' 
        && argv[2][strlen(argv[2])-3] == '.'))return true;
        else return false;
    }
    else if (argc == 5){
        //第二个参数是[-v|--verbose],第三个参数是[-h|--help],第四个参数是.pda或者.tm,第五个参数是输入字符串
        if(s1!="--verbose" && s1!="-v")return false;
        if(s2!="--help" && s2!="-h")return false;
        if ((strlen(argv[3])>=4 && argv[3][strlen(argv[3])-1] == 'a' && argv[3][strlen(argv[3])-2] == 'd'
        && argv[3][strlen(argv[3])-3] == 'p' && argv[3][strlen(argv[3])-4] == '.')||
        (strlen(argv[3])>=3 && argv[3][strlen(argv[3])-1] == 'm' && argv[3][strlen(argv[3])-2] == 't'
        && argv[3][strlen(argv[3])-3] == '.'))return true;
        else return false;
    }
    else return false;
}

int main(int argc, char* argv[]){
    
    //参数检查
    if(!Parameter_Check(argc, argv)){
        cout<<"usage: fla [-h|--help] <pda> <input>"<<endl;
        cout<<"       fla [-v|--verbose] [-h|--help] <tm> <input>"<<endl;
        return 1;
    }
    
    string s1 = argv[1];
    string s2 = argv[2];

    if(s1=="--help" || s1=="-h"|| s2=="--help" || s2=="-h"){
        cout<<"usage: fla [-h|--help] <pda> <input>"<<endl;
        cout<<"       fla [-v|--verbose] [-h|--help] <tm> <input>"<<endl;
        if(argc==2)return 0;
    }

    if(s1=="--verbose" || s1=="-v"){
        verbose = true;
    }

    //读取PDA或者TM文件
    ifstream file(argv[argc-2]);
    string fileContent((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
    file.close();

    //根据文件名判断是PDA还是TM
    if(argv[argc-2][strlen(argv[argc-2])-1] == 'a'){
        dealType = 0;
    }
    else {
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
    //read_test();

    //输入检查
    int check= input_check(inputString);
    if(verbose){
        if (check != -1){
            cout<<"illegal input"<<endl;
            cout<<"Input: "<<inputString<<endl<<"==================== ERR ===================="<<endl;
            cout<<"error: '"<<inputString[check];
            cout<<"' was not declared in the set of input symbols"<<endl<<"Input: ";
            cout<<inputString<<endl;
            for(int i=0; i<check+7; i++){
                cout<<" ";
            }
            cout<<"^"<<endl;
            cout<<"==================== END ===================="<<endl;
            return 1;
        }
        else{
            cout<<"Input: "<<inputString<<endl;
            cout<<"==================== RUN ===================="<<endl;
        }
    }
    else{
        if (check != -1){
            cout<<"illegal input"<<endl;
            return 1;
        }
    }

    //PDA运行
    if(dealType == 0){
        if(PDA_run(inputString)){
            cout<<"true"<<endl;
        }
        else{
            cout<<"false"<<endl;
        }
        if(verbose)cout<<"==================== END ===================="<<endl;
    }

    //TM运行
    else{
        TM_run(inputString);
    }

    
    return 0;
}