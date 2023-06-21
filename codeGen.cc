#include <iostream>
#include <sstream>
#include <utility>
#include <map>
#include <cstdlib>
#include <vector>
using namespace std;

class Tree {
public:
  string rules;
  vector< string > tokens;
  vector< Tree > children;
};

//SYMBOL TABLE: data structure: map
//map allVar: for variables and their types 
//key: variable name; value: vector<type, location wrt $29)
//vector <string> varProp; //vector for variable properties
 map <string, vector<string>> allVar; //map for variables in a procedure 

//Signature: pair
// vector<string> : for the parameters 
// allVar: symbol table for variables used in the procedure
 pair<vector<string>, map<string, string>> signatTab; //signature of a procedure 

//All procedures & their signatures 
//string: procedure name
//signatTab: signatures of procedures 
 map<string, pair<vector<string>, map<string, string>>> allProce; //for all procedures 

//some declarations
int labelCount=1;


//check if its a terminal
bool isTerminal(string word){
    //cout<<"Word: "<<word<<endl;
    string terminals[]={
        "BOF", "EOF", "INT", "WAIN", "LPAREN",
        "COMMA", "RPAREN", "LBRACE", "RETURN", "SEMI" ,
        "RBRACE", "STAR",  "ID", "NUM", "NULL", 
        "BECOMES","NEW", "LBRACK","RBRACK", "AMP",
        "PLUS","MINUS","SLASH", "PCT","IF",
        "ELSE", "WHILE","PRINTLN","DELETE", "EQ",
        "NE", "LT", "LE", "GE", "GT"
    };
    for (int i=0; i<35; i++){
        if(word==terminals[i]){
            return true;
        }
        
    }
    return false;
}

void push(int regNum,int varLocwrt){
  cout<<"sw $"<<regNum<<", "<<varLocwrt<<"($29)"<<endl;
  
}

void pop(int varLocwrt){
  cout<<"lw $3,"<<varLocwrt<<"($29)"<<endl;
}

void pushsystk(){
  cout<<"sw $3, -4($30)"<<endl;
  cout<<"sub $30, $30, $4"<<endl;
}

void popBinOp(){
  cout<<"add $30, $30, $4"<<endl;
  cout<<"lw $5, -4($30)"<<endl;
}

//void code()

int countWords(string tokens){
    if(tokens.empty()){
        return 0;
    }
    bool inWord=false;
    int count=0;
    for(char c: tokens){
        if(c==' ' ){
            inWord=false;
        }
        else if(c==':'){
          break;
        }
        else if(!inWord){
            inWord=true;
            count++;
        }
    }
    return count;
}

//build tree
Tree createNode(string root){
    Tree newNode;
    newNode.rules=root;
    newNode.tokens.push_back("");
    //newNode.tokens.push_back("");
    return newNode;
}

Tree buildTree(vector<string> &invec,Tree parseTree){
    
  string line=invec[0];
  string root, nodes, indvword;
  istringstream input(line);
  input >> root;
  nodes=line.substr(root.length()+1, line.length());
  parseTree=createNode(root);
  //cout<<"parseTree: "<<parseTree.rules<<endl;
  //cout<<"root: "<<root<<endl;
  //cout<<"nodes: "<<nodes<<endl;
  if(isTerminal(root)){
    parseTree.tokens[0]=nodes;
    //cout<<endl;
    return parseTree;
  }
  else{
    int numloop=countWords(nodes); 
    //the problem is here!!!!1 the count nodes include :int too
    istringstream indvtok(nodes);
    //cout<<"nodes: "<<nodes<<endl;
    for(int i=0; i<numloop;i++){
      indvtok>>indvword;
       //cout<<"indvword: "<<indvword<<endl;
       if(nodes.find(":")!=string::npos){
        //has a type 
        string nodeType=nodes.substr(indvword.length()+1,nodes.length());
       // cout<<"type: "<<nodeType<<endl;
        parseTree.tokens[0]=nodeType;
       }
      if(indvword==".EMPTY"){
        parseTree.tokens[0]=".EMPTY";
        return parseTree;
      }
      Tree connectNode;
      invec.erase(invec.begin(),invec.begin()+1);
      connectNode=buildTree(invec,parseTree);
      parseTree.children.push_back(connectNode);

      
      
    }
    //cout<<endl;
     
    return parseTree;
    
  }
 // cout<<endl;
 //cout<<"parseTree root: "<<parseTree.rules<<endl;
  return parseTree;

}

//print Tree
void printChildren(Tree childtoken){
    //childtoken=checkType(childtoken);
    //cout<<"numchild: "<<childtoken->children.size()<<endl;
    //cout<<childtoken.children[2].rules<<childtoken.tokens[1]<<endl;
    for(int i=0; i<childtoken.children.size();i++){
    //childtoken=checkType(childtoken);
     cout<<childtoken.children[i].rules<<" "<<childtoken.tokens[0];
       // 
       
       

    }
  // cout<<childtoken.tokens[0];
   
}

vector<string> all_id;
bool error;
void preorderTree(Tree root){
    cout<<root.rules<<" ";
    int numchild=root.children.size();
    //cout<<"num child: "<<numchild<<endl;
    //cout<<root.rules<<" ";
    //cout<<"reach? "<<endl;
    //root=checkType(root);
    //cout<<"?? "<<endl;
    //error=checkSem(root, all_id);
    
    /*if(error){
        cerr<<"ERROR"<<endl;
    }*/
    //cout<<"# ids: "<<all_id.size()<<endl;
    if((root.rules=="dcls" || root.rules=="statements" || root.rules=="params") && root.children.size()==0){
        cout<<".EMPTY"<<endl;
        return;
    }
    if(isTerminal(root.rules)){
        
        cout<<root.tokens[0];
        /*if(root.rules=="NUM"){
            cout<<root.tokens[1];
        }
        else if(root.rules=="ID"){
            cout<<root.tokens[1];
        }
        else if(root.rules == "NULL"){
            cout<<root.tokens[1];
        }*/
        cout<<endl;
        return;
    }

    
    
    printChildren(root);
    cout<<endl;
    if(numchild==0 && !isTerminal(root.rules)){
        return;
    }
    for(int i=0;i<numchild;i++){
        //cout<<root.children[i].rules<<" ";
        preorderTree(root.children[i]);
        
    }
    //cout<<endl;
    
}

int locwrt29=4;
int avReg=5;
void buildSymbT(Tree root, int &locwrt29){
  string varName, varType, varLoca, nodeToken;
  vector <string> varProp;
  int location;
  int numNodes=root.children.size();
  if(isTerminal(root.rules)){
    return;
  }
  
  else if(root.rules=="dcl"){
    //cout<<"comes in here dcl"<<endl;
    nodeToken=root.children[1].tokens[0];
    varName=nodeToken.substr(0,nodeToken.length()-6);
    varType="int";
    locwrt29=locwrt29-4;
    //cout<<varName<<":"<<locwrt29<<endl;
    varLoca=to_string(locwrt29);
    varProp.push_back(varType);
    varProp.push_back(varLoca);
    allVar.insert(pair<string, vector<string>>(varName, varProp));
    //push(5,locwrt29);
    return;
  }
  for(int i=0; i<numNodes;i++){
      buildSymbT(root.children[i],locwrt29);
    }
  
  
  
  
}



void GenCode(Tree root){
  string varReturn, temp, varStore;
  int varPos;
  vector <string> tempvec;
    //we only wanna process non terminals
    int numNodes=root.children.size();
    if(isTerminal(root.rules)){
      return;
    }
    /*else if(root.rules=="dcl"){
      //first two parameters, build symbol table
      buildSymbT(root,locwrt29);
      return;
    }*/
    else if(root.rules=="expr"){
      if(root.children.size()>1){
        if(root.children[0].rules=="expr" && root.children[2].rules=="term"){
          
          GenCode(root.children[0]); //for expr 
          pushsystk();
          GenCode(root.children[2]); //for term
          popBinOp();
          if(root.children[1].rules=="PLUS"){
            cout<<"add $3, $5, $3"<<endl;
          }
          else if(root.children[1].rules=="MINUS"){
            cout<<"sub $3, $5, $3"<<endl;
          }
        }
        return;
      }
      
    }
    else if(root.rules=="term"){
      //cout<<"size: "<<root.children.size()<<endl;
      if(root.children.size()>1){
        if(root.children[0].rules=="term" && root.children[2].rules=="factor"){
          GenCode(root.children[0]); //for term
          pushsystk();
          GenCode(root.children[2]); //for factor
          popBinOp();
          if(root.children[1].rules=="STAR"){
            cout<<"mult $3, $5"<<endl;
            cout<<"mflo $3"<<endl;
          }
          else if(root.children[1].rules=="SLASH"){
            cout<<"div $5, $3"<<endl;
            cout<<"mflo $3"<<endl;
          }
          else if(root.children[1].rules=="PCT"){
            cout<<"div $5, $3"<<endl;
            cout<<"mfhi $3"<<endl;
          }
        }
        return;
      }
      
    }

    //factor
    else if(root.rules=="factor"){
      if(root.children[0].rules=="NUM"){
        temp=root.children[0].tokens[0];
        varReturn=temp.substr(0,temp.length()-6);
        cout<<"lis $3"<<endl;
        cout<<".word "<<varReturn<<endl;
        return;
      }
      else if(root.children[0].rules=="ID"){
        //check for the variable to return 
        temp=root.children[0].tokens[0];
        varReturn=temp.substr(0,temp.length()-6);
        //check if the variable exists in the symbol table 
        if(allVar.find(varReturn)!=allVar.end()){
          //exists
          //check for position
          tempvec=allVar[varReturn];
          varPos=abs((stoi(tempvec[1])/4))+1;
          //cout<<"varpos: "<<stoi(tempvec[1])<<endl;
          //push(3,stoi(tempvec[1]));
          pop(stoi(tempvec[1]));
          //cout<<"add $3, $"<<varPos<<", $0"<<endl;
          return;
        }
        
      }
      else if(root.children.size()>1){
        //cout<<"comes in here"<<endl;
        //factor → LPAREN expr RPAREN
          GenCode(root.children[1]);
          return;
      }

    }

    //dcls → dcls dcl BECOMES NUM SEMI
    else if(root.rules=="dcls" ){
      if(root.children.size()==0){
        return;
      }
      else if(root.children.size()>0 && root.children[0].rules=="dcls"&& root.children[1].rules=="dcl"
          && root.children[2].rules=="BECOMES" && root.children[3].rules=="NUM"){
            GenCode(root.children[0]); //for dcls
            //dcl processing
            temp=root.children[1].children[1].tokens[0];
            varStore=temp.substr(0,temp.length()-6);
            //find in symbol table to get offset
            tempvec=allVar[varStore];
            int offset=stoi(tempvec[1]);

            //cout for num
            cout<<"lis $3"<<endl;
            temp=root.children[3].tokens[0];
            cout<<".word "<<temp.substr(0,temp.length()-6)<<endl;
            push(3,offset);
            return;
            
      }
    }
    
    //statements 
    else if(root.rules=="statements" && root.children.size()>0){
      if(root.children.size()>0){
        if(root.children[0].rules=="statements" && root.children[1].rules=="statement"){
          //statements → statements statement
        GenCode(root.children[0]); //for statements
        GenCode(root.children[1]); //for statements
        return;
        }
        

      }
      
      
      //do we really need this tho? 
      
      else if(root.children.size()==0){
        return;
      }
    }

    //statement 
    else if(root.rules=="statement"){
      if(root.children[0].rules=="lvalue" && root.children[1].rules=="BECOMES"
        && root.children[2].rules=="expr" && root.children[3].rules=="SEMI"){
          //statement → lvalue BECOMES expr SEMI
          //GenCode(root.children[0]); //for lvalue
          GenCode(root.children[2]); //for expr
          //cout<<"size: "<<root.children[0].children.size()<<endl;
          if(root.children[0].children.size()==1){
            //lvalue → ID
            //get variable for assignment 
            temp=root.children[0].children[0].tokens[0];
            //cout<<temp<<endl;
            varStore=temp.substr(0,temp.length()-6);
            //cout<<varStore<<endl;
            tempvec=allVar[varStore];
            varPos=(stoi(tempvec[1]));
            push(3,varPos);

          }
          else{
            GenCode(root.children[0]);
            
          }
          
          //get id
          
          return;
        }
        else if(root.children[0].rules=="IF" && root.children[1].rules=="LPAREN"){
          //no need to check the whole thing lah
          //cout<<"labelcount: "<<labelCount<<endl;
          int tempIflabel=labelCount;
          GenCode(root.children[2]); //for test
          cout<<"beq $3, $0, L"<<labelCount<<endl;
          cout<<endl;
          labelCount+=2;
          GenCode(root.children[5]); //for stmts 1
          //cout<<"temp label: "<<templabel<<endl;
          cout<<"beq $0, $0, L"<<tempIflabel+1<<endl;
          cout<<endl;
          cout<<"L"<<tempIflabel<<":"<<endl;
          GenCode(root.children[9]); //for stmts2
          //labelCount++;
          cout<<endl;
          cout<<"L"<<tempIflabel+1<<":"<<endl;
          //labelCount=templabel+2; //next available label
          return;

        }
        else if(root.children[0].rules=="WHILE" && root.children[1].rules=="LPAREN"){
          int tempWlabel=labelCount;
          cout<<"L"<<labelCount<<":"<<endl;
          GenCode(root.children[2]); //for test
          cout<<endl;
          labelCount+=2;
          cout<<"beq $3, $0, L"<<tempWlabel+1<<endl;
          GenCode(root.children[5]); //for statements
          cout<<endl;
          cout<<"beq $0, $0, L"<<tempWlabel<<endl;
          //labelCount++;
          cout<<"L"<<tempWlabel+1<<":"<<endl;
          //labelCount++; //next available label
          return;
        }

        else if(root.children[0].rules=="PRINTLN" && root.children[1].rules=="LPAREN"
                && root.children[2].rules=="expr" && root.children[3].rules=="RPAREN"){
                GenCode(root.children[2]); //for expr
                cout<<"add $1, $3, $0"<<endl;
                cout<<"lis $10"<<endl;
                cout<<".word print"<<endl;
                cout<<"sw $31, -4($30)"<<endl;
                cout<<"sub $30, $30, $4"<<endl;
                cout<<"jalr $10"<<endl;
                cout<<"add $30, $30, $4"<<endl;
                cout<<"lw $31, -4($30)"<<endl;
                return;
        }


    }

    //lvalue
    else if(root.rules=="lvalue"){
      if(root.children.size()==1 &&  root.children[0].rules=="ID"){
        temp=root.children[0].tokens[0];
        varReturn=temp.substr(0,temp.length()-6);
        //check if the variable exists in the symbol table 
        if(allVar.find(varReturn)!=allVar.end()){
          //exists
          //check for position
          tempvec=allVar[varReturn];
          varPos=abs((stoi(tempvec[1])/4))+1;
          //cout<<"varpos: "<<stoi(tempvec[1])<<endl;
          //push(3,stoi(tempvec[1]));
          pop(stoi(tempvec[1]));
          //cout<<"add $3, $"<<varPos<<", $0"<<endl;
          return;
        }
      }
      else if(root.children.size()==3 && root.children[0].rules=="LPAREN"
              && root.children[1].rules=="lvalue" && root.children[2].rules=="RPAREN"){
                GenCode(root.children[1]); //for lvalue
                return;
              }
    }

    //test
    /*test → expr EQ expr
      test → expr NE expr
      test → expr LT expr
      test → expr LE expr
      test → expr GE expr
      test → expr GT expr*/
    else if(root.rules=="test" && root.children[0].rules=="expr"
            && root.children[2].rules=="expr"){
              //processing for $3 & $5
              GenCode(root.children[0]); //for expr #1
              pushsystk(); //push $3
              GenCode(root.children[2]); //for expr #2
              popBinOp(); //$5= first val
              if(root.children[1].rules=="LT"){
                cout<<"slt $3, $5, $3"<<endl;
              }
              else if(root.children[1].rules=="GT"){
                cout<<"slt $3, $3, $5"<<endl;
              }
              else if(root.children[1].rules=="GE"){
                cout<<"slt $3, $5, $3"<<endl;
                cout<<"sub $3, $11, $3"<<endl;
              }
              else if(root.children[1].rules=="LE"){
                cout<<"slt $3, $3, $5"<<endl;
                cout<<"sub $3, $11, $3"<<endl;
              }
              else if(root.children[1].rules=="NE"){
                cout<<"slt $6, $3, $5"<<endl;
                cout<<"slt $7, $5, $3"<<endl;
                cout<<"add $3, $6, $7"<<endl;
                //need further processing?
              }
              else if(root.children[1].rules=="EQ"){
                cout<<"slt $6, $3, $5"<<endl;
                cout<<"slt $7, $5, $3"<<endl;
                cout<<"add $3, $6, $7"<<endl;
                cout<<"sub $3, $11, $3"<<endl;
              }
        return;
    }



    for(int i=0; i<numNodes;i++){
      GenCode(root.children[i]);
    }


}

//print the symbol table
void printSymbT(map<string,vector<string>> symbT){
    for(auto const &pair: allVar){
      vector<string> temp=pair.second;
        cout<<pair.first<<" : "<<temp[0]<<" "<<temp[1]<<endl;

    }
}

int main(){
    Tree parseTree;
    
    istream& in =cin;
    string s;
    //string line;
    vector <string> infile;
    
    istringstream line(s);
    while(getline(in,s)){
        //store input to a vector
        infile.push_back(s);
    }
    //debug the vector
    /*for(auto i: infile){
        cout<<i<<endl;
    }*/
    //cout<<"here"<<endl;
     parseTree=buildTree(infile, parseTree);
     //prologue
    cout<<";;prologue"<<endl;
    cout<<".import print"<<endl;
    cout<<"lis $11"<<endl;
    cout<<".word 1"<<endl;
    cout<<"lis $4"<<endl;
    cout<<".word 4"<<endl;
    cout<<"sub $29, $30, $4"<<endl; //init fp
    
    
    buildSymbT(parseTree,locwrt29);
    int numVar=allVar.size()*4;
     cout<<"lis $5"<<endl; //push
    cout<<".word "<<numVar<<endl; //stack
    cout<<"sub $30, $30, $5"<<endl; //frame
     
    push(1,0); //store first parameter
    push(2,-4);//store second parameter
    cout<<endl;

    GenCode(parseTree);
    cout<<endl;
    cout<<";;epilogue"<<endl;
    cout<<"add $30, $29, $4"<<endl; //pop stack frame
     cout<<"jr $31"<<endl;
     //cout<<"done"<<endl;
     //preorderTree(parseTree);
     //printSymbT(allVar);
     //buildSymbT(parseTree);
    
}

//todo: sw and lw 