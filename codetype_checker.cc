#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <utility>
#include <algorithm>
using namespace std;

class Tree{
    public:
    
        string rules;
        vector<string> tokens;
        vector<Tree> children;

       // Tree();
};

/*struct Tree{
    string rules;
    vector <string> tokens;
    vector<Tree> children;
}*/

//some declarations 

//SYMBOL TABLE: data structure: map
//map allVar: for variables and their types 
//key: variable name; value: variable data type
 map <string, string> allVar; //map for variables in a procedure 

//Signature: pair
// vector<string> : for the parameters 
// allVar: symbol table for variables used in the procedure
 pair<vector<string>, map<string, string>> signatTab; //signature of a procedure 

//All procedures & their signatures 
//string: procedure name
//signatTab: signatures of procedures 
 map<string, pair<vector<string>, map<string, string>>> allProce; //for all procedures 



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


int countWords(string tokens){
    if(tokens.empty()){
        return 0;
    }
    bool inWord=false;
    int count=0;
    for(char c: tokens){
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


void buildSignature(Tree root){
    //Signature: pair
    //pair<vector<string>, map<string, string>> signatTab;
    // vector<string> : for the parameters 
    // allVar: symbol table for variables used in the procedure
    pair<vector<string>, map<string, string>> currProc;
    map<string, string> varInProc;
    //cout<<"here"<<endl;
    vector <string> params;
    string procName;
    Tree temp;
    if(root.rules=="procedures" && root.children.size()>1){
        //cout<<"pass?"<<endl;
        //cout<<":"<<root.children[].rules<<endl;
        if(root.children[0].rules=="procedure" 
        && root.children[1].rules=="procedures"){
            //cout<<"pass1?"<<endl;
            temp=root.children[0]; //temp=procedure
            //get procedure name 
            procName=temp.children[1].tokens[0];
            //cout<<"Procedure name: "<<procName<<endl;
            //get the parameters 
            if(temp.children[3].children.size()==0){
                //cout<<"gets in here"<<endl;
                //empty parameters 
                params.push_back("");

            }
            else{
                //non empty parameters
            }

            if(root.children[0].children[6].children.size()>0 && root.children[0].children[6].rules=="dcls"){
                
                    //variable declarations in the procedure 
                    temp=root.children[6]; //temp=dcls branch
                
                

            }
            else if(root.children[0].children[6].children.size()==0 && root.children[0].children[6].rules=="dcls"){
                //insert params and variable symbol table into signature pair 
                pair<vector<string>, map<string, string>>currProc=make_pair(params, varInProc);
            }
            /*if(temp.children[7].tokens[0]!=": int"){
                //expr derived from procedures must have type int 
                cerr<<"1ERROR"<<endl;
            }*/

            //insert into signature into global map 
            //allVar.insert(pair<string, string>(varName, varType));
            allProce.insert(pair<string,pair<vector<string>,map<string, string>>>(procName,currProc));
    
    
    }
    } 

}




//print the symbol table
void printSymbT(map<string, string> symbT){
    for(auto const &pair: allVar){
        cout<<pair.first<<" : "<<pair.second<<endl;
    }
}

void buildSymbT(Tree root){
    //printSymbT(allVar);
    //cout<<endl;
    //data structure: maps 
    //map allVar: for variables and their types 
    //key: variable name; value: variable data type
    //map <string, string> allVar;
    //cout<<root.rules<<endl;
    string varName, idTok, varType;
    int lenId, lenType;
    if(root.rules == "dcl" && root.children[0].rules=="type" 
        && root.children[1].rules == "ID"){
            //check for nodes with the rule dcl -> type ID

            //get ID name
            varName=root.children[1].tokens[0];
            //cout<<"Varname: "<<varName<<endl;
            //get ID type 
             varType=root.children[1].tokens[1].substr(3, root.children[1].tokens[1].length());
             //cout<<"VarType: "<<varType<<endl;
            //lenType=idTok.length()-varName.length();
            //varType=varName.substr(lenType, varName.length());
            //cout<<varName<<" : "<<varType<<endl;;
            //check if this variable alr exists
            if(allVar.find(varName)==allVar.end()){
                //variable is new, add to the map 
                allVar.insert(pair<string, string>(varName, varType));
            }
            else{
                //cout<<varName<<endl;
                cerr<<"1ERROR"<<endl;
            }
        //printSymbT(allVar);
        return;
    }
    
    //traverse the tree
    /*for(int i=0; i<root.children.size();i++){
        buildSymbT(root.children[i]);
    }*/

}






bool checkSem(Tree curr, vector<string> &all_id){
   //cout<<"called"<<endl;
   bool error=false;
    if(curr.rules == "dcl" && curr.children[0].rules=="type" && 
    curr.children[1].rules=="ID"){
        if(all_id.empty()){
           // cout<<"here"<<endl;
            all_id.push_back(curr.children[1].tokens[0]);
            
        }
        else{
            vector<string> ::iterator it= find(all_id.begin(),all_id.end(),curr.children[1].tokens[0]);
            if(it==all_id.end()){
                 //cout<<"1"<<curr.children[1].tokens[0]<<endl;
                 all_id.push_back(curr.children[1].tokens[0]);
                 //cout<<"# id: "<<all_id.size()<<endl;
            }
            else{
                //cout<<"does it come in here"<<endl;
                //cerr<<"ERROR"<<endl;
                error=true;
            }
        }
            
    }
     if(curr.rules == "dcl" && all_id.size()==2){
       // cout<<"??"<<endl;
        //cout<<curr.children[0].children[0].rules<<endl;
        if(curr.children[0].children.size()==2 && curr.children[0].children[1].rules=="STAR"){
            error=true;
        }
    }
    
    return error;
    //return all_id;
}

Tree checkType(Tree &curr){
    //cout<<"here"<<endl;
    Tree temp=curr;
    Tree imm;
    string nodeType;
    string vName, vType;
    string typeExpr, typeTerm, typeFact, typeLval;
    if(curr.rules == "dcl" && curr.children[0].rules=="type"){
         //cout<<"correct?"<<endl;
        // cout<<curr.children[0].rules<<endl;
        //curr=curr.children[0]; //w "type as a rule"
        //cout<<"ehh"<<endl;
        //dcl → type ID
        if(curr.children[0].rules == "type"){
            //cout<<"hmm?"<<endl;
            //cout<<"s "<<curr.children[0].children[0].rules<<endl;

            if(curr.children[0].children.size()==2 
                && curr.children[0].children[0].rules=="INT" 
                && curr.children[0].children[1].rules=="STAR"){
                    //go down the other ID branch 
                //curr=temp->children[1]; //ID branch
                //curr->token[0]=curr->token[0]+" : int";
                    //cout<<"comes in here"<<endl;
                    
                    temp.children[1].tokens[1]= " : int*";
                }
            else if(curr.children[0].children.size()==1 &&
                curr.children[0].children[0].rules=="INT" ){
                
                temp.children[1].tokens[1]= " : int";
    
                
            }
            //printSymbT(allVar);
            buildSymbT(temp);
            
        }
        else{
            cerr<<"2ERROR"<<endl;
        }

    }

    //expr
    else if(curr.rules == "expr" ){
        //expr only used for return statements 
        //expr → term
        if(curr.children[0].rules=="term"){
            imm=checkType(curr.children[0]);
            temp.children[0]=imm; //connect the updated nodes
            nodeType=imm.tokens[0];
            //cout<<nodeType<<endl;
           // return type not int - to fix 
            if (nodeType != ": int" && curr.children[0].children[0].children[0].rules=="STAR"){
                cerr<<"3ERROR"<<endl;
            }
            temp.tokens[0]=nodeType;
        }

        //expr → expr PLUS term && expr → expr MINUS term
        else if(curr.children[0].rules =="expr"  
                && curr.children[2].rules=="term"){
                   // cout<<"comes ine"<<endl;
            string overType;
            imm=checkType(curr.children[0]); //for expr
            temp.children[0]=imm;
            typeExpr=imm.tokens[0];
            imm=checkType(curr.children[2]); //for term
            temp.children[2]=imm;
            typeTerm=imm.tokens[0];
            //type checking 
            if(curr.children[1].rules=="PLUS"){
                if(typeExpr==typeTerm && typeExpr==": int"){
                overType=typeExpr;   
            }
            else if(typeExpr==": int*" && typeTerm==": int"){
                overType=": int*";
            }
            else if(typeExpr==": int" && typeTerm==": int*"){
                overType=": int*";
            }
            else{
                cerr<<"4ERROR"<<endl;
            }
            }
            else if(curr.children[1].rules=="MINUS"){
                if(typeExpr==typeTerm && typeExpr==": int"){
                overType=typeExpr;   
                }
                else if(typeExpr==": int*" && typeTerm==": int"){
                overType=": int*";
                }
                else if(typeExpr==typeTerm && typeExpr==": int*"){
                    overType=": int";
                }
                else{
                    cerr<<"5ERROR"<<endl;
                }

            }
            
            temp.tokens[0]=overType;

            
        }

    }

    //term
    else if(curr.rules=="term"){

        ////term → factor 
        if(curr.children[0].rules=="factor"){
            imm=checkType(curr.children[0]);
            temp.children[0]=imm;
            nodeType=temp.children[0].tokens[0];
            
            //cout<<"type:"<<nodeType<<endl;
            temp.tokens[0]=nodeType;
        }

        //term → term (smtg) factor && term → term STAR factor && term → term SLASH factor
        //&& term → term PCT factor
        else if(curr.children[0].rules == "term" && curr.children[2].rules=="factor"){
                imm=checkType(curr.children[0]); // for term
                temp.children[0]=imm;
                typeTerm=imm.tokens[0];
                //type checking here
                imm=checkType(curr.children[2]); //for factor 
                temp.children[2]=imm;
                typeFact=imm.tokens[0];
                temp.tokens[0]=": int"; //type must be int 
                if(typeFact!=temp.tokens[0] || typeTerm!=temp.tokens[0]){
                    cerr<<"6ERROR"<<endl;
                }


        }
        
    }

        //dcls  to fix for imm and trees
        else if(curr.rules=="dcls"){
            if(curr.children.size()>0){
 //curr=curr.children[0];
            //cout<<"1"<<curr.children[0].rules<<endl;
            //cout<<"finally"<<endl;
            //check 
            string rightType;
            string ruleType;
            if(curr.children[0].rules == "dcls" && curr.children[1].rules =="dcl"
                && curr.children[2].rules =="BECOMES" && curr.children[4].rules=="SEMI"){
                    //cout<<"here?"<<endl;
                    imm=checkType(curr.children[0]);    //for dcls
                    temp.children[0]=imm;
                    //cout<<"1"<<imm.children[1].rules<<endl;
                    //check if the correct type is assigned 
                    imm=checkType(curr.children[1]);
                    temp.children[1]=imm;
                    if(temp.children[1].children[0].children.size()==2){
                        //type: int star*
                        rightType=" int*";
                    }
                    else{
                        rightType=" int";
                    }
                    //string rightType=curr.children[1].children[1].tokens[1];
                    //cout<<"type: "<<rightType<<endl;
                    //cout<<"done"<<endl;
                    //cout<<curr.children[1].rules<<endl;
                    //imm=checkType(curr.children[1]);
                    //cout<<"dones"<<endl;
                    //check for NUM/NULL

                    //dcls → dcls dcl BECOMES NUM SEMI
                    if(temp.children[3].rules == "NUM"){
                        //cout<<temp.children[3].rules<<endl;
                        temp.children[3].tokens[1]=" : int";
                        ruleType=" int";
                        
                    }
                    //dcls → dcls dcl BECOMES NULL SEMI
                    else if(temp.children[3].rules == "NULL"){
                        temp.children[3].tokens[1]=" : int*";
                        ruleType=" int*";
                    }

                    if(ruleType!=rightType){
                        cerr<<"7ERROR"<<endl;
                    }
            }
            else{
                cerr<<"8ERROR"<<endl;
            }
            }
           
        }

        //factor
        else if(curr.rules=="factor"){

            //factor → NUM
            if(curr.children[0].rules=="NUM"){
                temp.tokens[0]=": int";
                //cout<<"1"<<temp.children[0].tokens[0]<<endl;
                temp.children[0].tokens[1]=" : int";
            }

           //factor → NULL 
           else if(curr.children[0].rules=="NULL"){
                temp.tokens[0]=": int*";
                temp.children[0].tokens[1]=" : int*";
           }

           //factor → LPAREN expr RPAREN
           else if(curr.children[0].rules=="LPAREN" && curr.children[1].rules=="expr"
                && curr.children[2].rules=="RPAREN"){
                    imm=checkType(curr.children[1]);
                    temp.children[1]=imm;
                    nodeType=temp.children[1].tokens[0];
                    //check for other expr expansions
                    //cout<<"nt: "<<nodeType<<endl;
                    //cout<<"1"<<temp.rules<<endl;
                    temp.tokens[0]=nodeType;
                    //temp.children[2].tokens[1]=nodeType;
                    //cout<<"h: "<<temp.children[1].children[0].rules<<endl;
                   // temp.children[1].children[0].tokens[0]=nodeType;
                }
            
            //factor → ID
            else if(curr.children.size()==1 && curr.children[0].rules=="ID"){
                //cout<<"here?"<<endl;
                //check if it exists in the symbol table 
                vName=curr.children[0].tokens[0];
                //cout<<"var: "<<vName<<endl;
                if(allVar.find(vName)!=allVar.end()){
                    vType=allVar[vName];
                    //cout<<"Vtype: "<<vType<<endl;
                    /*if(vType=="int*"){
                        cerr<<"ERROR"<<endl;
                    }*/
                    //else{
                        temp.children[0].tokens[1]=" : "+vType;
                        //cout<<"token: "<<temp.chi<<endl;
                        temp.tokens[0]=": "+vType;
                    //}
                }
                else{
                    cerr<<"9ERROR"<<endl;
                }
            }

            //factor → STAR factor
            else if(curr.children[0].rules=="STAR" && curr.children[1].rules=="factor"){
                //cout<<"comes in here"<<endl;
                imm=checkType(curr.children[1]);
                temp.children[1]=imm;
                if(temp.children[1].tokens[0]!=": int*" && temp.children[1].children[0].rules=="ID" ){
                    cerr<<"10ERROR"<<endl;
                }
                //nodeType=temp.children[1].tokens[0];
                temp.tokens[0]=": int";
            }

            //factor → NEW INT LBRACK expr RBRACK
            else if(curr.children[0].rules=="NEW" && curr.children[1].rules=="INT"
                    && curr.children[2].rules=="LBRACK" && curr.children[3].rules=="expr"
                    && curr.children[4].rules=="RBRACK"){
                imm=checkType(curr.children[3]);
                temp.children[3]=imm;
                nodeType=temp.children[3].tokens[0];
                if(nodeType!=": int"){
                    cerr<<"11ERROR"<<endl;
                }
                //temp.children
                temp.tokens[0]=": int* ";
            }

            //factor → AMP lvalue
            else if(curr.children[0].rules=="AMP" && curr.children[1].rules=="lvalue"){
                imm=checkType(curr.children[1]); //for lvalue
                temp.children[1]=imm;
                temp.tokens[0]=": int*";
                if(temp.children[1].tokens[0]!=": int" ){
                    cerr<<"12ERROR"<<endl;
                }

            }

            //factor → ID LPAREN RPAREN
            else if(curr.children[0].rules=="ID" && curr.children[1].rules=="LPAREN"
                    && curr.children[2].rules=="RPAREN"){
                    //id refers to procedure name 
                    //check if such a function exists 
                    //cout<<"comes in here2"<<endl;
                    string idName=curr.children[0].tokens[0];
                    if(allProce.find(idName)!= allProce.end()){
                        //procedure exists
                        nodeType=": int"; //cuz all procedures return type int
                        temp.tokens[0]=nodeType; 
                    }
                    else{
                        cerr<<"13ERROR"<<endl;
                    }
            }
            

        }

        //lvalue
        else if(curr.rules=="lvalue"){
            //lvalue → ID
            if(curr.children[0].rules=="ID"){
                vName=curr.children[0].tokens[0];
                //check if it exists in symb table
                if(allVar.find(vName)!= allVar.end()){
                    vType=allVar[vName];
                    temp.children[0].tokens[1]=" : "+vType;
                    temp.tokens[0]=": "+vType;
                }
            }

            //lvalue → STAR factor
            else if(curr.children[0].rules=="STAR" && curr.children[1].rules=="factor"){
                imm=checkType(curr.children[1]); //for factor
                temp.children[1]=imm;
                if(temp.children[1].tokens[0]!=": int*"){
                    cerr<<"14ERROR"<<endl;
                }
                temp.tokens[0]=": int";
            }

            //lvalue → LPAREN lvalue RPAREN
            else if(curr.children[0].rules=="LPAREN" && curr.children[1].rules=="lvalue"
                    && curr.children[2].rules=="RPAREN"){
                    imm=checkType(curr.children[1]); //for lvalue
                    temp.children[1]=imm;
                    nodeType=temp.children[1].tokens[0];
                    temp.tokens[0]=nodeType;
            
            
            
            }
        }

        //statement
        else if(curr.rules=="statement"){
            //statement → lvalue BECOMES expr SEMI
            if(curr.children[0].rules=="lvalue" && curr.children[1].rules=="BECOMES"
                && curr.children[2].rules=="expr" && curr.children[3].rules=="SEMI"){
                imm=checkType(curr.children[0]); //for lvalue;
                temp.children[0]=imm;
                typeLval=imm.tokens[0];
                imm=checkType(curr.children[2]); //for expr
                temp.children[2]=imm;
                typeExpr=imm.tokens[0];
                if(typeExpr!=typeLval){
                    cerr<<"15ERROR"<<endl;
                }
            }

            //statement → PRINTLN LPAREN expr RPAREN SEMI
            else if(curr.children[0].rules=="PRINTLN" && curr.children[1].rules=="LPAREN"
                    && curr.children[2].rules=="expr" && curr.children[3].rules=="RPAREN"
                    && curr.children[4].rules=="SEMI"){
                    imm=checkType(curr.children[2]); //for expr
                    temp.children[2]=imm;
                    typeExpr=imm.tokens[0];
                    if(typeExpr!=": int"){
                        cerr<<"16ERROR"<<endl;
                    }
            }

            //statement → DELETE LBRACK RBRACK expr SEMI
            else if(curr.children[0].rules=="DELETE" && curr.children[1].rules=="LBRACK"
                    && curr.children[2].rules=="RBRACK" && curr.children[3].rules=="expr"
                    && curr.children[4].rules=="SEMI"){
                    imm=checkType(curr.children[3]); //for expr
                    temp.children[3]=imm;
                    if(imm.tokens[0]!=": int*"){
                        cerr<<"17ERROR"<<endl;
                    }
            }
        }

        //test
        else if(curr.rules=="test"){
            /*test → expr EQ expr
             test → expr NE expr
             test → expr LT expr
             test → expr LE expr
             test → expr GE expr
             test → expr GT expr*/
            if(curr.children[0].rules=="expr" && curr.children[2].rules=="expr"){
                imm=checkType(curr.children[0]); //expr #1
                temp.children[0]=imm;
                typeExpr=imm.tokens[0];
                imm=checkType(curr.children[2]); //expr #2
                temp.children[2]=imm;
                if(imm.tokens[0]!=typeExpr){
                    cerr<<"18ERROR"<<endl;
                }
            }
        }

        //procedures
        else if(curr.children.size()==2 && curr.rules=="procedures"){
            cout<<"rule: "<<curr.children[0].children[3].rules<<endl;
            imm=checkType(curr.children[0].children[3]); //for params
            temp.children[0].children[3]=imm;
            imm=checkType(curr.children[0].children[9]); //for expr
            //cout<<"here1"<<endl;
            temp.children[0].children[9]=imm;
            //cout<<"type: "<<imm.tokens[0]<<endl;
            if(imm.tokens[0]!=": int"){
                cerr<<"19ERROR"<<endl;
            }
            buildSignature(temp);
        }
        
    
    return temp;

}

Tree createNode(string root){
    Tree newNode;
    newNode.rules=root;
    newNode.tokens.push_back("");
    newNode.tokens.push_back("");
    return newNode;
}

Tree buildTree(vector<string> &invec,Tree parseTree){
    string line=invec[0];
    //cout<<"line: "<<line<<endl;
    //invec.erase(invec.begin(),invec.begin()+1);
    string root;
    string nodes;
    string indvword;
    istringstream input(line);
    input >> root;
    nodes=line.substr(root.length()+1,line.length());
    //input >> nodes;
    //cout<<"root: "<<root<<endl;
    //cout<<"nodes:"<<nodes<<endl;
    //cout<<"does it come here"<<endl;
    parseTree=createNode(root);
    if(isTerminal(root)){
        //cout<<"here? "<<endl;
        //cout<<"lexeme: "<<nodes<<endl;
        parseTree.tokens[0]=nodes;
        return parseTree;
    }
   else{
        int numloop=countWords(nodes);
        istringstream indvtok(nodes);
        for(int i=0; i<numloop;i++){
            
            indvtok >> indvword;
            //cout<<"node we're processing: "<<indvword<<endl;
            //check if the root is a terminal instead! 
            //empty case
            if(indvword==".EMPTY"){
                //cout<<"comes in here"<<endl;
                parseTree.tokens[0]=".EMPTY";
                //cout<<parseTree.tokens[0]<<endl;
                return parseTree;
            }
            Tree connectNode;
            invec.erase(invec.begin(),invec.begin()+1);
            //if(!invec.empty()){
                connectNode=buildTree(invec,parseTree);
                parseTree.children.push_back(connectNode);
            //}
            
            //cout<<"current root: "<<parseTree.rules<<endl;
            //cout<<"number of children: "<<parseTree.children.size()<<endl;
            //cout<<endl;
        }
}
    
    return parseTree;

}

//functions to print the tree
void printChildren(Tree childtoken){
    //childtoken=checkType(childtoken);
    //cout<<"numchild: "<<childtoken->children.size()<<endl;
    //cout<<childtoken.children[2].rules<<childtoken.tokens[1]<<endl;
    for(int i=0; i<childtoken.children.size();i++){
    //childtoken=checkType(childtoken);
     cout<<childtoken.children[i].rules<<" ";
       // <<childtoken.tokens[0]
       
       

    }
    
    /*if(childtoken.rules == "factor" && childtoken.children[0].rules== "LPAREN" &&
     childtoken.children[2].rules=="RPAREN"){
        cout<<childtoken.children[2].tokens[1];
    }*/
    /*if(childtoken.rules == "factor" && childtoken.children[0].rules== "STAR" &&
     childtoken.children[1].rules=="factor"){
        cout<<": int";
     }*/

     //else{
        cout<<childtoken.tokens[0];
     //}
     /*else if(childtoken.rules == "factor" && childtoken.children[0].rules== "ID"){
        cout<<childtoken.children[0].tokens[1];
       }*/
     
    
    //cout<<endl;
}
vector<string> all_id;
bool error;
void preorderTree(Tree root){
    cout<<root.rules<<" ";
    int numchild=root.children.size();
    //cout<<"num child: "<<numchild<<endl;
    //cout<<root.rules<<" ";
    //cout<<"reach? "<<endl;
    root=checkType(root);
    //cout<<"?? "<<endl;
    error=checkSem(root, all_id);
    
    if(error){
        cerr<<"ERROR"<<endl;
    }
    //cout<<"# ids: "<<all_id.size()<<endl;
    if((root.rules=="dcls" || root.rules=="statements" || root.rules=="params") && root.children.size()==0){
        cout<<".EMPTY"<<endl;
        return;
    }
    if(isTerminal(root.rules)){
        
        cout<<root.tokens[0];
        if(root.rules=="NUM"){
            cout<<root.tokens[1];
        }
        else if(root.rules=="ID"){
            cout<<root.tokens[1];
        }
        else if(root.rules == "NULL"){
            cout<<root.tokens[1];
        }
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
     parseTree=buildTree(infile, parseTree);
     preorderTree(parseTree);
     //printSymbT(allVar);
     //buildSymbT(parseTree);
    //debug the vector
    /*for(auto i: infile){
        cout<<i<<endl;
    }*/
}