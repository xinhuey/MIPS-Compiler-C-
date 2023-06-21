#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>    //need for vector find
#include <map>
#include <stack>
#include <set>
#include <ctype.h>
#include "wlp4data.h"

using namespace std;
//define TrieNode
struct TrieNode{
    string token;
    vector <TrieNode> children;
};


const std::string CFG = ".CFG"; 
const std::string INPUT = ".INPUT";
const std::string TRANSITIONS =".TRANSITIONS"; 
const std::string REDUCTIONS =".REDUCTIONS";
const std::string EMPTY = ".EMPTY";
const std::string END = ".END";

//using namespace std;  // ahc added cannot tahan ::

//helper function to insert into the trie tree 
TrieNode createNode(string word){
    TrieNode newNode;
    newNode.token=word;
    return newNode;
    
}

//helper function to check if its a terminal
bool isTerminal(string y){
    for(char c:y){
        if(!isupper(c)){
            return false;
        }

    }
    return true;
}

int countWords(string rule){
    if(rule.empty()){
        return 0;
    }
    bool inWord=false;
    int count=0;
    for(char c: rule){
        if(c==' '){
            inWord=false;
        }
        else if(!inWord){
            inWord=true;
            count++;
        }
    }
    return count;
}

void printString(string inputseq, string reducseq){
    if(inputseq==""){
        //cout<<"input sequence is empty"<<endl;
        cout<<reducseq+" ."<<endl;
    }
    else if(reducseq==""){
        cout<<"."+inputseq<<endl;
    }
    else{
    cout<<reducseq+" ." +inputseq<<endl;
    }
    
}

string peekToken(vector<vector<string>>tokens){
    string nextsym;
    string temp;
    if(tokens.empty()){
        return "";
    }
    nextsym=tokens[1][0];
    return nextsym;
}

auto popStuff(auto &s, int len){
    if(s.empty()){
        return s;
    }
    else{
        for(int i=0;i<len;i++){
        s.pop();
    }
    }
    return s;
    
}
 
void printChildren(TrieNode childtoken){
    for(int i=0; i<childtoken.children.size();i++){
       cout<<childtoken.children[i].token<<" ";
    }
    //cout<<endl;
} 

void preorderTrie(TrieNode root, vector<string> &lex){
    int numchild=root.children.size();
    if(root.token==".EMPTY"){
        return;
    }
    cout<<root.token<<" ";
    if(isTerminal(root.token)){
        cout<<lex[0];
        lex.erase(lex.begin(),lex.begin()+1);
    }
    printChildren(root);
    cout<<endl;
    for(int i=0; i<numchild;i++){
        
        preorderTrie(root.children[i],lex);
    }
    //cout<<endl; 
    
}


int main() {
   
    istream& in = cin;    
    string s;    
    //input section, store into string
    string inputseq="";
    string kind;
    string lexeme;
    vector<vector<string>> tokens;
    vector<string> tokenlk;
    //string reducseq="";
    //2d vector:
    // < <kind, lexeme> >
    //push in BOF
    tokenlk.push_back("BOF");
    tokenlk.push_back("BOF");
    tokens.push_back(tokenlk);
    tokenlk.clear();
    while(getline(in,s)){
       // vector<string> tokenlk;
        istringstream line(s);
        //cout<<"s: "<<s<<endl;
        line >> kind;
        //cout<<"kind: "<<kind<<endl;
        tokenlk.push_back(kind);
        line >> lexeme;
        //cout<<"lexeme: "<<lexeme<<endl;
        tokenlk.push_back(lexeme);
        //cout<<tokenlk[0]<<" "<<tokenlk[1]<<endl;
        tokens.push_back(tokenlk);
        tokenlk.clear();
        
    }
    //cout<<"number of tokens: "<<tokens.size()<<endl;
    //insert EOF
    tokenlk.push_back("EOF");
    tokenlk.push_back("EOF");
    tokens.push_back(tokenlk);

    /*debug input 
    for(int i=0; i<tokens.size();i++){
        //cout<<"i: "<<i<<endl;
        cout<<tokens[i][0]<<" "<<tokens[i][1]<<endl;
    }*/
    

    getline(in, s); // Transitions section (skip header)
     //CFG data structure: 2D vector
    //< <non-terminal (char), terminal(String)>, ...>
    istringstream lineCFG(WLP4_CFG);
    vector <vector<string>>allCfgRules;
    while(getline(lineCFG,s)){
        vector<string> rule;
        if(s==".CFG"){
            continue;
            //we're going down the file, so cfg will be encountered before input
        }
        else{
            //nested vector:
            
            std::string lhsNt, rhsTmn;
            std::istringstream extStr(s);
            extStr>>lhsNt;
            //std::cout<<"left: "<<lhsNt<<std::endl;
            //std::cout<<"S: "<<s<<std::endl;
            size_t eraseterm=s.find(lhsNt);
            if(eraseterm!=string::npos){
                s.erase(eraseterm, lhsNt.length()+1);
            }
            //rhsTmn=s.erase(0, lhsNt.length());
            rule.push_back(lhsNt);
            rule.push_back(s);
            
        }
        allCfgRules.push_back(rule);
        
    } 
/*debugging cfg vector
int s1=allCfgRules.size();
for(int i=0;i<s1;i++){
//std::cout<<"size of s2: "<<s2<<std::endl;
std::cout<<allCfgRules[i][0]<<" ->"<<allCfgRules[i][1]<<endl;
}
cout<<endl;*/

  
/*input section, store into string
string inputseq="";
//string reducseq="";
while(getline(in,s)){
    if(s==TRANSITIONS){
        break;
    }
    inputseq=inputseq+" "+s;
}*/
//cout<<"Input sequence: "<<inputseq<<endl;

//transitions data structure 
multimap<int,vector<string>> trnMap;  //transition multimap-duplicate keys
//data structure for trnmap:
//<rule #, [inp string, next rule num]>
istringstream lineTrans(WLP4_TRANSITIONS);
while(getline(lineTrans, s)) { 
      vector<string> trnstep;
     if (s == ".TRANSITIONS") {  
        // start of reduction info         
        continue;
    }else {  
        int fromRule;
         string toRule;
         istringstream line(s);
         line>>fromRule;
         while(line >> s){
             if(line.peek()==EOF){
                 toRule=s;
                 trnstep.push_back(toRule);
             }
             else{
                 trnstep.push_back(s);
             }
        }       
         trnMap.insert(pair<int,vector<string>>(fromRule,trnstep));
     } //getline(in,s)-trnMap
 }
/*debugging transitions
for(auto const &pair:trnMap){
  std::cout<<"{"<<pair.first <<" ";
  vector<string> temp=pair.second;
  for(int i=0; i<temp.size();i++){
    
    cout<<temp[i]<<" ";
  }
  cout<<endl;
}  
cout<<endl;*/

/*** reduction map ****/
set<int> redustates;
multimap<int,vector<string>> reduMap;  //reduction multimap-duplicate keys
istringstream lineReduc(WLP4_REDUCTIONS);
while(getline(lineReduc, s)) { 
    vector<string> reducstep;
    if (s == ".REDUCTIONS") {  
        // end of input File         
        continue;
    } else {  
        int prevrule;
        string nxtrule, lkahd;
        istringstream reducline(s);
        reducline >> prevrule;
        redustates.insert(prevrule);
        while(reducline >> s){
            if(reducline.peek()==EOF){
                lkahd=s;
                reducstep.push_back(lkahd);
            }
            else{
                //nxt rule
                reducstep.push_back(s);
            }
        }
        reduMap.insert(pair<int, vector<string>>(prevrule,reducstep));
    } 
}
/*debugging reduction
for(auto const &pair:reduMap){
  std::cout<<"{"<<pair.first <<" ";
  vector<string> temp=pair.second;
  
    
    cout<<temp[0]<<" "<<temp[1];
  
  cout<<endl;
}  
cout<<endl;*/
/*cout<<"all reducing states"<<endl;
for(auto s: redustates){
    cout<<s<<endl;
}
cout<<endl;*/

//processing part  T_T
//RMB: convert the torules to int (cuz they're stored as string in the vector)

stack<int> statestk;
stack<string> symbstk;
stack<TrieNode> treestk;

//initialise the stacks
auto init=trnMap.begin();
int prevstatenum=init->first;
vector <string> temp=init->second;
symbstk.push("");

treestk.push(createNode(""));
statestk.push(prevstatenum);
//initialise symbol stack with bof too

string startsymb=allCfgRules[0][0];
string reducseq="";
int countsym=1;
string nxtIn;
bool notrans=false;
bool noredrule=true;
//int i=0;
//print initial string
//printString(inputseq, reducseq);
//inputseq=inputseq.substr(4,inputseq.length());
//cout<<"remove bof: "<<inputseq<<endl;
//reducseq="BOF";
//printString(inputseq, reducseq);
//int numSym=countWords(inputseq);
//cout<<"number of symbols: "<<numSym<<endl;
 string reduceRule;
 string nxtSym;
 //string nxtIn;
//cout<<"d"<<endl;
//remark:
//remove tokens from vector each time of shift 
//int noTokens=tokens.size();
vector <string> lexemes;
TrieNode redNode;
TrieNode emptyNode;
for(int i=0;i<tokens.size();i++){
    //cout<<"comes in here 1"<<endl;
    //cout<<"Top of tree stack symbol: "<<treestk.top().token<<endl;
    while(treestk.top().token!=startsymb){
        //consume input 
        //cout<<"comes in here 2"<<endl;
    if(tokens.empty()){
        //cout<<"??"<<endl;
        nxtIn=".ACCEPT";
       // cout<<"nxtinput: "<<nxtIn<<endl;
    }
    else{
        nxtIn=tokens[0][0];
        //cout<<"Next token: "<<nxtIn<<endl;
        countsym++;
        //cout<<"next input symbol: "<<nxtIn<<endl;
    }
    int lentopop;
   int j=0;
    //cout<<"top of state stack: "<<statestk.top()<<endl;
    while(redustates.find(statestk.top())!=redustates.end() ){
        //check if it's a reduce state 
        j++;
       //cout<<"is it okay here"<<endl;
        pair<multimap<int, vector<string>>::iterator,multimap<int, vector<string>>::iterator> findcfgrule;
        findcfgrule=reduMap.equal_range(statestk.top());
        for(multimap<int, vector<string>>::iterator it=findcfgrule.first;it!=findcfgrule.second;++it){
            vector<string> tempcfgrule= it->second;
            if(tempcfgrule[1]==nxtIn){
                //cout<<"does it at least come in here"<<endl;
                //its a reduce state and the next symbol matches 
                int toreduceNum=stoi(tempcfgrule[0]);
                reduceRule=allCfgRules[toreduceNum][0];
                string initialRule=allCfgRules[toreduceNum][1];
                if(initialRule==".EMPTY"){
                    
                //cout<<"comes in here-empty"<<endl;
                emptyNode=createNode(".EMPTY");
                lentopop=0;
                }else{
                lentopop=countWords(initialRule);
                }
                statestk=popStuff(statestk,lentopop);
                //cout<<"top of state stack: "<<statestk.top()<<endl;
                //create new node for the reduced rule 
                redNode=createNode(reduceRule);
                if(lentopop==0){
                    //.EMPTY case
                    redNode.children.insert(redNode.children.begin(),emptyNode);
                }
                //loop to link the children to the new rednode
                for(int j=0; j<lentopop; j++){
                    TrieNode tempNode=treestk.top();
                    redNode.children.insert(redNode.children.begin(),tempNode);
                    //cout<<"TempNode: "<<tempNode.token<<endl;
                    treestk.pop();
                }
                //push the new node to the tree stack 
                treestk.push(redNode);
                //preorderTrie(redNode);
                //cout<<"Token from reducing: "<<treestk.top().token<<endl;
                //update the vector 
                //cout<<"token here: "<<tokens[0][0]<<endl;
                //tokens.erase(tokens.begin(), tokens.begin()+1);
                //cout<<"token here: "<<tokens[0][0]<<endl;
                //check for transition 
                pair<multimap<int, vector<string>>::iterator,multimap<int, vector<string>>::iterator> ret;
            ret=trnMap.equal_range(statestk.top());
            for(multimap<int, vector<string>>::iterator it=ret.first;it!=ret.second;++it){
                vector<string> chktemp;
                chktemp=it->second;
                string ruleSymb=chktemp[0];
                if(ruleSymb==treestk.top().token){
                    //get next state number 
                    int nxtrule=stoi(chktemp[1]);
                    statestk.push(nxtrule);
                    notrans=false;
                    break;
                }
                else{
                    notrans=true;
                }
            }
            noredrule=false;
            break;




            }
            else{
                //not reducible
                noredrule=true;
            }
        }
        if(noredrule){
            //cout<<"1does it come in here"<<endl;
            break;
        }

 }
    //cout<<"current state: "<<statestk.top()<<endl;
    //shift
    
    if(noredrule || (redustates.find(statestk.top())==redustates.end() && treestk.top().token!=startsymb)){
        treestk.push(createNode(nxtIn));
        //cout<<"Token from shift: "<<treestk.top().token<<endl;
        //update the vector 
       // cout<<"lexeme: "<<tokens[0][1]<<endl;
        lexemes.push_back(tokens[0][1]);
        //cout<<"here1"<<endl;
       
        tokens.erase(tokens.begin(), tokens.begin()+1);
        //cout<<"next token: "<<tokens[0][1]<<endl;
        //cout<<"top of token stack: "<<treestk.top().token<<endl;
        //cout<<"top of state stack s: "<<statestk.top()<<endl;
        if(trnMap.find(statestk.top())!=trnMap.end()){
            pair<multimap<int, vector<string>>::iterator,multimap<int, vector<string>>::iterator> ret;
            ret=trnMap.equal_range(statestk.top());
           // cout<<"Ddd"<<endl;
            for(multimap<int, vector<string>>::iterator it=ret.first;it!=ret.second;++it){
                //cout<<"2"<<endl;
                vector<string> chktemp;
                chktemp=it->second;
                string ruleSymb=chktemp[0];
                if(ruleSymb==treestk.top().token){
                    //get next state number 
                    int nxtrule=stoi(chktemp[1]);
                    //cout<<"nxtrule: "<<nxtrule<<endl;
                    statestk.push(nxtrule);
                    notrans=false;
                    break;
                }
                else{
                    notrans=true;
                    //cout<<"come in here"<<endl;
                }
        }
        /*if(notrans){
            cerr<<"ERROR at "<<countsym-2<<endl;
            break;
        }*/
        }
        else{
            notrans=true;
            cerr<<"ERROR at "<<countsym-2<<endl;
            break;
        }
        
    }
    
    
    if(notrans && tokens.size()!=0){
        cerr<<"ERROR at "<<countsym-2<<endl;
        //cout<<"comes down? "<<endl;
       break;
        
    }
//cout<<endl;
}
if(notrans ){
            //cout<<"here? "<<endl;
            break;
        }
    
    
//cout<<endl;

}
if(notrans){
    preorderTrie(redNode,lexemes);
}



}