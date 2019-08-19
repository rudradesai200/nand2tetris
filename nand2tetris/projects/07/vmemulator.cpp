/**************************************
*  Author : Rudra Desai               *
*  Date Created : Aug 16,2019         *
*  Last edit : Aug 18,2019            *
*  Program Name : VMTranslator        *
***************************************/



#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<math.h>
#include<string.h>

using namespace std;

#define MAX_MEMORY 24576

fstream inputFile;
fstream interFile;
fstream outFile;

enum commandType {C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL};

/*
void split(const string& str, const string& delim, vector<string>& parts) {
  size_t start, end = 0;
  while (end < str.size()) {
    start = end;
    while (start < str.size() && (delim.find(str[start]) != string::npos)) {
      start++;  // skip initial whitespace
    }
    end = start;
    while (end < str.size() && (delim.find(str[end]) == string::npos)) {
      end++; // skip to end of word
    }
    if (end-start != 0) {  // just ignore zero-length strings.
      parts.push_back(string(str, start, end-start));
    }
  }
}
*/

bool present(string haystack, string needl, int start,int end){
  if(needl.size() != (end-start+1)){
    throw "illegal use of function present";
  }
  int j=0;
  for(int i=start;i<=end;i++){
    if(haystack[i]!=needl[j]){
      return false;
    }
    j++;
  }
  return true;
}

void extractSegVal(string curr_command,string type,string& segment,string& value,int* value_int){
  int len = type.size();
  // cout<<curr_command<<endl;
  for(int i=len;i<curr_command.size();i++){
    if((curr_command[i]>='0' && curr_command[i]<='9')){
      value.append(1,curr_command[i]);
    }
    else{
      segment.append(1,curr_command[i]);
    }
  }
  int l = value.size();
  if(l == 1){
    *value_int = value[0]-'0';
    }
  else{
        for(int i=0;i<l;i++){
          *value_int = (value[i]-'0')*pow(10,(l-i-1));
        }
  }
  // cout<<segment<<'\t'<<value<<endl;
}

void printifconditionalsblock(string jump_type,int A_LABEL){
  interFile<<"@SP"<<endl;
  interFile<<"A=M"<<endl;
  interFile<<"A=A-1"<<endl;
  interFile<<"D=M"<<endl;
  interFile<<"A=A-1"<<endl;
  interFile<<"D=M-D"<<endl;
  interFile<<"@A_LABEL_"<<A_LABEL<<endl;
  interFile<<"D;"<<jump_type<<endl;
  interFile<<"D=0"<<endl;
  interFile<<"@A_LABEL_"<<A_LABEL+1<<endl;
  interFile<<"0;JMP"<<endl;
  interFile<<"(A_LABEL_"<<A_LABEL<<')'<<endl;
  interFile<<"D=-1"<<endl;
  interFile<<"(A_LABEL_"<<A_LABEL+1<<')'<<endl;
  interFile<<"@SP"<<endl;
  interFile<<"M=M-1"<<endl;
  interFile<<"A=M"<<endl;
  interFile<<"A=A-1"<<endl;
  interFile<<"M=D"<<endl;
}

void printarithsubcommands(string arrith_command_type){
  interFile<<"@SP"<<endl;
  interFile<<"M=M-1"<<endl;
  interFile<<"A=M"<<endl;
  interFile<<"D=M"<<endl;
  interFile<<"A=A-1"<<endl;
  interFile<<"M="<<arrith_command_type<<endl;
}

void printpushsubcommands(string push_type,int A_LABEL,string value){
  interFile<<"@"<<push_type<<endl;//value of arg base
  interFile<<"D=M"<<endl;
  interFile<<"@R13"<<endl;//stored it in R13
  interFile<<"M=D-1"<<endl;
  interFile<<"@"<<value<<endl;//took the value to increment
  interFile<<"D=A+1"<<endl;
  interFile<<"(loop."<<A_LABEL<<')'<<endl;//adding that value to A
  interFile<<"D=D-1"<<endl;
  interFile<<"@R13"<<endl;
  interFile<<"M=M+1"<<endl;
  interFile<<"@loop."<<A_LABEL<<endl;
  interFile<<"D;JNE"<<endl;
  interFile<<"@R13"<<endl;
  interFile<<"A=M"<<endl;
  interFile<<"D=M"<<endl;
  interFile<<"@SP"<<endl;
  interFile<<"A=M"<<endl;
  interFile<<"M=D"<<endl;
  interFile<<"@SP"<<endl;
  interFile<<"M=M+1"<<endl;
}

void printpopsubcommands(string push_type,int A_LABEL,string value){
  interFile<<"@"<<push_type<<endl;//value of command base
  interFile<<"D=M"<<endl;//stored the address of base in D
  interFile<<"@R13"<<endl;//storing it in R13
  interFile<<"M=D-1"<<endl;
  interFile<<"@"<<value<<endl;//took the value to increment
  interFile<<"D=A+1"<<endl;
  interFile<<"(loop."<<A_LABEL<<')'<<endl;//adding that value to A
  interFile<<"D=D-1"<<endl;
  interFile<<"@R13"<<endl;
  interFile<<"M=M+1"<<endl;
  interFile<<"@loop."<<A_LABEL<<endl;
  interFile<<"D;JNE"<<endl;
  interFile<<"@SP"<<endl;
  interFile<<"M=M-1"<<endl;
  interFile<<"A=M"<<endl;
  interFile<<"D=M"<<endl;
  interFile<<"@R13"<<endl;
  interFile<<"A=M"<<endl;
  interFile<<"M=D"<<endl;
}
class Coder{
public:
  int A_LABEL;
  void WriteArithmetic(string curr_command){
    if(curr_command == "add"){
      printarithsubcommands("D+M");
    }

      if(curr_command == "sub"){
      printarithsubcommands("M-D");
      }

      if(curr_command == "neg"){
      interFile<<"@SP"<<endl;
      interFile<<"A=M-1"<<endl;
      interFile<<"M=-M"<<endl;
      }

      if(curr_command == "eq"){
      printifconditionalsblock("JEQ",A_LABEL);
      A_LABEL+=2;
      }

      if(curr_command == "gt"){
      printifconditionalsblock("JGT",A_LABEL);
      A_LABEL+=2;
      }

      if(curr_command == "lt"){
      printifconditionalsblock("JLT",A_LABEL);
      A_LABEL+=2;
      }

      if(curr_command == "and"){
      printarithsubcommands("D&M");
      }

      if(curr_command == "or"){
      printarithsubcommands("D|M");
      }

      if(curr_command == "not"){
      interFile<<"@SP"<<endl;
      interFile<<"A=M-1"<<endl;
      interFile<<"M=!M"<<endl;
      }
    A_LABEL++;
  }
  void WritePush(string curr_command){
    string segment,value;
    int value_int;
    extractSegVal(curr_command,"push",segment,value,&value_int);
    cout<<segment<<'\t'<<value<<endl;
      if(segment == "argument"){
        printpushsubcommands("ARG",A_LABEL,value);
        A_LABEL++;
      }

      if(segment == "local"){
      // if(value )
      printpushsubcommands("LCL",A_LABEL,value);
      A_LABEL++;
      }
      if(segment == "static"){
        interFile<<"@16"<<endl;//value of arg base
        interFile<<"D=A"<<endl;
        interFile<<"@R13"<<endl;//stored it in R13
        interFile<<"M=D-1"<<endl;
        interFile<<"@"<<value<<endl;//took the value to increment
        interFile<<"D=A+1"<<endl;
        interFile<<"(loop."<<A_LABEL<<')'<<endl;//adding that value to A
        interFile<<"D=D-1"<<endl;
        interFile<<"@R13"<<endl;
        interFile<<"M=M+1"<<endl;
        interFile<<"@loop."<<A_LABEL<<endl;
        interFile<<"D;JNE"<<endl;
        interFile<<"@R13"<<endl;
        interFile<<"A=M"<<endl;
        interFile<<"D=M"<<endl;
        interFile<<"@SP"<<endl;
        interFile<<"A=M"<<endl;
        interFile<<"M=D"<<endl;
        interFile<<"@SP"<<endl;
        interFile<<"M=M+1"<<endl;
        A_LABEL++;
      }
      if(segment == "constant"){
        interFile<<"@"<<value<<endl;//value of arg base
        interFile<<"D=A"<<endl;
        interFile<<"@SP"<<endl;//stored it in R13
        interFile<<"A=M"<<endl;
        interFile<<"M=D"<<endl;//took the value to increment
        interFile<<"@SP"<<endl;
        interFile<<"M=M+1"<<endl;
      }
      if(segment == "this"){
        printpushsubcommands("THIS",A_LABEL,value);
        A_LABEL++;
      }
      if(segment == "that"){
        printpushsubcommands("THAT",A_LABEL,value);
        A_LABEL++;
      }
      if(segment == "pointer"){
        if(value_int == 0){
          interFile<<"@THIS"<<endl;
          interFile<<"D=M"<<endl;
          interFile<<"@SP"<<endl;
          interFile<<"A=M"<<endl;
          interFile<<"M=D"<<endl;
          interFile<<"@SP"<<endl;
          interFile<<"M=M+1"<<endl;
        }
        else{
          if(value_int == 1){
            interFile<<"@THAT"<<endl;
            interFile<<"D=M"<<endl;
            interFile<<"@SP"<<endl;
            interFile<<"A=M"<<endl;
            interFile<<"M=D"<<endl;
            interFile<<"@SP"<<endl;
            interFile<<"M=M+1"<<endl;
          }
          else{
            throw "illegal use of pointer\n";
          }
        }
      }
      if(segment == "temp"){
        if(value_int<=7){
          interFile<<"@R"<<to_string(5+value_int)<<endl;
          interFile<<"D=M"<<endl;
          interFile<<"@SP"<<endl;
          interFile<<"A=M"<<endl;
          interFile<<"M=D"<<endl;
          interFile<<"@SP"<<endl;
          interFile<<"M=M+1"<<endl;
        }
        else{
          throw "illegal use of temp\n";
        }
      }
    }
  void WritePop(string curr_command){
      string segment,value;
      int value_int;
      extractSegVal(curr_command,"pop",segment,value,&value_int);
      cout<<segment<<'\t'<<value<<endl;
      if(segment == "argument"){
        printpopsubcommands("ARG",A_LABEL,value);
        A_LABEL++;
      }

      if(segment == "local"){
      // if(value )
      printpopsubcommands("LCL",A_LABEL,value);
      A_LABEL++;
      }
      if(segment == "static"){
        interFile<<"@16"<<endl;//value of command base
        interFile<<"D=A"<<endl;//stored the address of base in D
        interFile<<"@R13"<<endl;//storing it in R13
        interFile<<"M=D-1"<<endl;
        interFile<<"@"<<value<<endl;//took the value to increment
        interFile<<"D=A+1"<<endl;
        interFile<<"(loop."<<A_LABEL<<')'<<endl;//adding that value to A
        interFile<<"D=D-1"<<endl;
        interFile<<"@R13"<<endl;
        interFile<<"M=M+1"<<endl;
        interFile<<"@loop."<<A_LABEL<<endl;
        interFile<<"D;JNE"<<endl;
        interFile<<"@SP"<<endl;
        interFile<<"M=M-1"<<endl;
        interFile<<"A=M"<<endl;
        interFile<<"D=M"<<endl;
        interFile<<"@R13"<<endl;
        interFile<<"A=M"<<endl;
        interFile<<"M=D"<<endl;
        A_LABEL++;
      }
      if(segment == "constant"){
        throw "pop has no 'constant' segment\n";
      }
      if(segment == "this"){
        printpopsubcommands("THIS",A_LABEL,value);
        A_LABEL++;
      }
      if(segment == "that"){
        printpopsubcommands("THAT",A_LABEL,value);
        A_LABEL++;
      }
      if(segment == "pointer"){
        if(value_int == 0){
          interFile<<"@SP"<<endl;
          interFile<<"M=M-1"<<endl;
          interFile<<"A=M"<<endl;
          interFile<<"D=M"<<endl;
          interFile<<"@THIS"<<endl;
          interFile<<"M=D"<<endl;
        }
        else{
          if(value_int == 1){
            interFile<<"@SP"<<endl;
            interFile<<"M=M-1"<<endl;
            interFile<<"A=M"<<endl;
            interFile<<"D=M"<<endl;
            interFile<<"@THAT"<<endl;
            interFile<<"M=D"<<endl;
          }
          else{
            throw "illegal use of pointer\n";
          }
        }
      }
      if(segment == "temp"){
        if(value_int<=7){
          interFile<<"@SP"<<endl;
          interFile<<"M=M-1"<<endl;
          interFile<<"A=M"<<endl;
          interFile<<"D=M"<<endl;
          interFile<<"@R"<<to_string(5+value_int)<<endl;
          interFile<<"M=D"<<endl;
        }
        else{
          throw "temp limit out of range\n";
        }
      }
    }
};
Coder C;
class Parser{
  public:
	  int f=0;
	  string curr_line;
	  string curr_command;
	  int instruction_count=0;
	  int last_ram_addr = 16;
	  bool notlabel;
    commandType type;

  	//removes spaces
  	void RemoveWhiteSpace(){
    	f = 0;
  		curr_line.erase(std::remove_if(curr_line.begin(), curr_line.end(), ::isspace), curr_line.end());
  	}

  	//removes comments from curr_command and sets label for resolving symbol
  	void RemoveComments(){
  	    int i=0;
  	    for(i=0; curr_line[i];i++){
  	      if(curr_line[i] == '/'){
  	        if(curr_line[i+1] == '/'){
  	          break;
  	        }
  	      }
  	      else{
  	        curr_command.append(1,curr_line[i]);
  	      }
  	    }
  	    if(curr_command.size()<1){
  	      f=1;
  	    }
  	}
    void SetCommandType(){
      if(curr_command.size() <=3){
        type = C_ARITHMETIC;
        // C.WriteArithmetic(curr_command);
      }
      else{
        if(present(curr_command,"push",0,3)){
          type = C_PUSH;
          // C.WritePush(curr_command);
        }
        if(present(curr_command,"pop",0,2)){
          type = C_POP;
          // C.WritePop(curr_command);
        }
      }
    }

    void ProcessInputFile(){
        while (getline(inputFile,curr_line)) {
          RemoveWhiteSpace();
          RemoveComments();
          if(f==0){
            // cout<<curr_command<<endl;
            SetCommandType();
            switch (type) {
              case C_ARITHMETIC:
              C.WriteArithmetic(curr_command);
              break;
              case C_PUSH:
              C.WritePush(curr_command);
              break;
              case C_POP:
              C.WritePop(curr_command);
              break;
            }

            // interFile << curr_command<<endl;
          }
          curr_line.clear();
          curr_command.clear();
        }

    }
};
Parser P;

int main(int argc,char* argv[]){
  char inputname[100];
  strcpy(inputname,argv[1]);
  strcat(inputname,".vm");
  inputFile.open(inputname,ios::in);
  cout<<inputname<<endl;
  strcpy(inputname,argv[1]);
  strcat(inputname,".asm");
  interFile.open(inputname,ios::out);
  cout<<inputname<<endl;
  P.ProcessInputFile();
  interFile.close();
  inputFile.close();

}
