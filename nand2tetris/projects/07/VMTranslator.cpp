/**************************************
*  Author : Rudra Desai               *
*  Date Created : Aug 19,2019         *
*  Last edit : Aug 20,2019            *
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
fstream outFile;

enum commandType {C_ARITHMETIC, C_PUSH, C_POP, C_LABEL, C_GOTO, C_IF, C_FUNCTION, C_RETURN, C_CALL};

class Coder{
public:
	int A_LABEL;

	//extracts segment and value from the command
	void extractSegVal(string curr_command,string type,string& segment,string& value,int* value_int){
	  int len = type.size();
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
	}

	//prints conditional commands for arrithmetic type
	void print_sub_arithemetic_conditional(string jump_type,int A_LABEL){
	  outFile<<"@SP"<<endl;
	  outFile<<"A=M-1"<<endl;
	  outFile<<"D=M"<<endl;
	  outFile<<"A=A-1"<<endl;
	  outFile<<"D=M-D"<<endl;
	  outFile<<"@A_LABEL_"<<A_LABEL<<endl;
	  outFile<<"D;"<<jump_type<<endl;
	  outFile<<"D=0"<<endl;
	  outFile<<"@A_LABEL_"<<A_LABEL+1<<endl;
	  outFile<<"0;JMP"<<endl;
	  outFile<<"(A_LABEL_"<<A_LABEL<<')'<<endl;
	  outFile<<"D=-1"<<endl;
	  outFile<<"(A_LABEL_"<<A_LABEL+1<<')'<<endl;
	  outFile<<"@SP"<<endl;
	  outFile<<"M=M-1"<<endl;
	  outFile<<"A=M"<<endl;
	  outFile<<"A=A-1"<<endl;
	  outFile<<"M=D"<<endl;
	}

	//prints arithmetic commands for arrithmetic type
	void print_sub_arithmetic_arith(string arrith_command_type){
	  outFile<<"@SP"<<endl;
	  outFile<<"M=M-1"<<endl;
	  outFile<<"A=M"<<endl;
	  outFile<<"D=M"<<endl;
	  outFile<<"A=A-1"<<endl;
	  outFile<<"M="<<arrith_command_type<<endl;
	}

	//prints push commands for some types
	void print_sub_push_commands(string push_type,int A_LABEL,string value){
	  outFile<<"@"<<push_type<<endl;//value of arg base
	  outFile<<"D=M"<<endl;
	  outFile<<"@"<<value<<endl;//took the value to increment
	  outFile<<"A=A+D"<<endl;
	  outFile<<"D=M"<<endl;
	  outFile<<"@SP"<<endl;
	  outFile<<"A=M"<<endl;
	  outFile<<"M=D"<<endl;
	  outFile<<"@SP"<<endl;
	  outFile<<"M=M+1"<<endl;
	}

	//prints pop commands for some types
	void print_sub_pop_commands(string push_type,int A_LABEL,string value){
	  //stored the address of base in D
	  outFile<<"@"<<value<<endl;
	  outFile<<"D=A"<<endl;
	  outFile<<"@"<<push_type<<endl;
	  outFile<<"D=D+M"<<endl;
	  outFile<<"@13"<<endl;
	  outFile<<"M=D"<<endl;
	  outFile<<"@SP"<<endl;
	  outFile<<"M=M-1"<<endl;
	  outFile<<"A=M"<<endl;
	  outFile<<"D=M"<<endl;
	  outFile<<"@13"<<endl;
	  outFile<<"A=M"<<endl;
	  outFile<<"M=D"<<endl;
	}

	//writes arithmetic commands in the out file
	void WriteArithmetic(string curr_command){
		if(curr_command == "add"){
		  print_sub_arithmetic_arith("D+M");
		}

		if(curr_command == "sub"){
		print_sub_arithmetic_arith("M-D");
		}

		if(curr_command == "neg"){
		outFile<<"@SP"<<endl;
		outFile<<"A=M-1"<<endl;
		outFile<<"M=-M"<<endl;
		}

		if(curr_command == "eq"){
		print_sub_arithemetic_conditional("JEQ",A_LABEL);
		A_LABEL+=2;
		}

		if(curr_command == "gt"){
		print_sub_arithemetic_conditional("JGT",A_LABEL);
		A_LABEL+=2;
		}

		if(curr_command == "lt"){
		print_sub_arithemetic_conditional("JLT",A_LABEL);
		A_LABEL+=2;
		}

		if(curr_command == "and"){
		print_sub_arithmetic_arith("D&M");
		}

		if(curr_command == "or"){
		print_sub_arithmetic_arith("D|M");
		}

		if(curr_command == "not"){
		outFile<<"@SP"<<endl;
		outFile<<"A=M-1"<<endl;
		outFile<<"M=!M"<<endl;
		}
	    A_LABEL++;
  	}

  	//writes push commands in the out file
	void WritePush(string curr_command){
	    string segment,value;
	    int value_int;
	    extractSegVal(curr_command,"push",segment,value,&value_int);
		if(segment == "argument"){
		print_sub_push_commands("ARG",A_LABEL,value);
		A_LABEL++;
		}

		if(segment == "local"){
		// if(value )
		print_sub_push_commands("LCL",A_LABEL,value);
		A_LABEL++;
		}
		if(segment == "static"){
			outFile<<"@16"<<endl;//value of arg base
			outFile<<"D=A"<<endl;
			outFile<<"@"<<value<<endl;//took the value to increment
			outFile<<"A=A+D"<<endl;
			outFile<<"D=M"<<endl;
			outFile<<"@SP"<<endl;
			outFile<<"A=M"<<endl;
			outFile<<"M=D"<<endl;
			outFile<<"@SP"<<endl;
			outFile<<"M=M+1"<<endl;
			A_LABEL++;
		}
		if(segment == "constant"){
			outFile<<"@"<<value<<endl;//value of arg base
			outFile<<"D=A"<<endl;
			outFile<<"@SP"<<endl;//stored it in R13
			outFile<<"A=M"<<endl;
			outFile<<"M=D"<<endl;//took the value to increment
			outFile<<"@SP"<<endl;
			outFile<<"M=M+1"<<endl;
		}
		if(segment == "this"){
			print_sub_push_commands("THIS",A_LABEL,value);
			A_LABEL++;
		}
		if(segment == "that"){
			print_sub_push_commands("THAT",A_LABEL,value);
			A_LABEL++;
		}
		if(segment == "pointer"){
			if(value_int == 0){
				outFile<<"@THIS"<<endl;
				outFile<<"D=M"<<endl;
				outFile<<"@SP"<<endl;
				outFile<<"A=M"<<endl;
				outFile<<"M=D"<<endl;
				outFile<<"@SP"<<endl;
				outFile<<"M=M+1"<<endl;
			}
		else{
		  if(value_int == 1){
			outFile<<"@THAT"<<endl;
			outFile<<"D=M"<<endl;
			outFile<<"@SP"<<endl;
			outFile<<"A=M"<<endl;
			outFile<<"M=D"<<endl;
			outFile<<"@SP"<<endl;
			outFile<<"M=M+1"<<endl;
		  }
		  else{
		    throw "illegal use of pointer\n";
		  }
		}
	  }
	  if(segment == "temp"){
	    if(value_int<=7){
	      outFile<<"@R"<<to_string(5+value_int)<<endl;
	      outFile<<"D=M"<<endl;
	      outFile<<"@SP"<<endl;
	      outFile<<"A=M"<<endl;
	      outFile<<"M=D"<<endl;
	      outFile<<"@SP"<<endl;
	      outFile<<"M=M+1"<<endl;
	    }
	    else{
	      throw "illegal use of temp\n";
	    }
	  }
    }

    //writes pop commands in the out file
  	void WritePop(string curr_command){
		string segment,value;
		int value_int;

		extractSegVal(curr_command,"pop",segment,value,&value_int);//extract segment and values

		if(segment == "argument"){
			print_sub_pop_commands("ARG",A_LABEL,value);
			A_LABEL++;
		}

		if(segment == "local"){
			print_sub_pop_commands("LCL",A_LABEL,value);
			A_LABEL++;
		}
		if(segment == "static"){
			outFile<<"@"<<value<<endl;
			outFile<<"D=A"<<endl;
			outFile<<"@16"<<endl;
			outFile<<"D=D+A"<<endl;
			outFile<<"@13"<<endl;
			outFile<<"M=D"<<endl;
			outFile<<"@SP"<<endl;
			outFile<<"M=M-1"<<endl;
			outFile<<"A=M"<<endl;
			outFile<<"D=M"<<endl;
			outFile<<"@13"<<endl;
			outFile<<"A=M"<<endl;
			outFile<<"M=D"<<endl;
			A_LABEL++;
		}
		if(segment == "constant"){
			throw "pop has no 'constant' segment\n";
		}
		if(segment == "this"){
			print_sub_pop_commands("THIS",A_LABEL,value);
			A_LABEL++;
		}
		if(segment == "that"){
			print_sub_pop_commands("THAT",A_LABEL,value);
			A_LABEL++;
		}
		if(segment == "pointer"){
			if(value_int == 0){
				outFile<<"@SP"<<endl;
				outFile<<"M=M-1"<<endl;
				outFile<<"A=M"<<endl;
				outFile<<"D=M"<<endl;
				outFile<<"@THIS"<<endl;
				outFile<<"M=D"<<endl;
			}
			else{
			  if(value_int == 1){
				outFile<<"@SP"<<endl;
				outFile<<"M=M-1"<<endl;
				outFile<<"A=M"<<endl;
				outFile<<"D=M"<<endl;
				outFile<<"@THAT"<<endl;
				outFile<<"M=D"<<endl;
			  }
			  else{
			    throw "illegal use of pointer\n";
			  }
			}
		}
		if(segment == "temp"){
			if(value_int<=7){
				outFile<<"@SP"<<endl;
				outFile<<"M=M-1"<<endl;
				outFile<<"A=M"<<endl;
				outFile<<"D=M"<<endl;
				outFile<<"@R"<<to_string(5+value_int)<<endl;
				outFile<<"M=D"<<endl;
			}
			else{
			  throw "temp limit out of range\n";
			}
		}
    }
    
    //writes label command to outfile
    void WriteLabel(string curr_command){
    	string label;
    	label.append(curr_command,5,curr_command.size()-5);
    	outFile << "("<<label<<")"<<endl;
    }
    
    void WriteGoto(string curr_command){
    	string label;
    	label.append(curr_command,4,curr_command.size()-4);
    	outFile <<"@"<<label<<endl;
    	outFile<<"0;JMP"<<endl;
    }
    
    void WriteIfGoto(string curr_command){
    	string label;
    	label.append(curr_command,7,curr_command.size()-7);
    	outFile<<"@SP"<<endl;
    	outFile<<"M=M-1"<<endl;
    	outFile<<"A=M"<<endl;
    	outFile<<"D=M"<<endl;
    	outFile<<"@"<<label<<endl;
    	outFile<<"D;JGT"<<endl;
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

		//sees if needl is present in haystack at specified position
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

		//sets command type and process thereby
		void SetCommandType(){
			if(curr_command.size() <=3){
				type = C_ARITHMETIC;
			}
			if(present(curr_command,"push",0,3)){
			  type = C_PUSH;
			}
			if(present(curr_command,"pop",0,2)){
			  type = C_POP;
			}
			if(present(curr_command,"label",0,4)){
				type = C_LABEL;
			}
			if(present(curr_command,"goto",0,3)){
				type = C_GOTO;
			}
			if(present(curr_command,"if-goto",0,6)){
				type = C_IF;
			}
		}

		//takes line by line commands and processes it
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
			      case C_LABEL:
			      C.WriteLabel(curr_command);
			      break;
			      case C_GOTO:
			      C.WriteGoto(curr_command);
			      break;
			      case C_IF:
			      C.WriteIfGoto(curr_command);
			      break;
			    }
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
  cout<<"inputFile opened\n";

  strcpy(inputname,argv[1]);
  strcat(inputname,".asm");
  cout<<"outFile opened\n";
  
  outFile.open(inputname,ios::out);
  cout<<"processing started"<<endl;

  P.ProcessInputFile();
  cout<<"conversion successful\n";
  outFile.close();
  inputFile.close();

}
