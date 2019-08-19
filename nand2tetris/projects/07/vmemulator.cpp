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

using namespace std;

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
class Coder{
public:
  void WriteArithmetic(string curr_command){

  }
  void WritePush(string curr_command){

  }
  void WritePop(string curr_command){

  }

};
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
  inputFile.open(argv[1],ios::in);
  interFile.open("interfile.txt",ios::out);
  P.ProcessInputFile();
  interFile.close();
  inputFile.close();

}
