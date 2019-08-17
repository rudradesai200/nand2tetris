#include<iostream>
#include<fstream>
#include<string>


using namespace std;

class Parser{
public:
  ifstream file;
  string curr_command;

  Parser(char argv[]){
    file.open(argv,ios::in);
  }


  void advance(){
    string S;
    if(getline(file,S)){
      if(S[0]!='/' && S[1]!='/'){
        curr_command.clear();
        for(int i=0;i<S.size();i++){
          if(S[i]!=' '){
            curr_command.append(1,S[i]);
          }
        }
      }
    }
  }

  int commandType(){
    if(curr_command[0]=='@'){
      return 0;
    }
    else{
      if(curr_command[0]=='('){
        return 2;
      }
      else{
        return 1;
      }
    }
  }

  string symbol(){
    if(commandType()==0){
      string returnstring;
      for(int i=0;i<curr_command.size();i++){
        returnstring.append(1,curr_command[i+1]);
      }
      return returnstring;
    }
    else{
      if(commandType()==2){
        string returnstring;
        for(int i=0;i<(curr_command.size()-1);i++){
          returnstring.append(1,curr_command[i+1]);
        }
        return returnstring;
      }
      else{
        throw "commandType == 1";
      }
    }
  }

  string dest(){
    if(commandType() == 1){
      string returnstring;
      std::size_t found = curr_command.find('=');
      if(found!=std::string::npos){
        for(int i=0;i<found;i++){
          returnstring.append(1,curr_command[i]);
        }
        return returnstring;
      }
      else{
        return "null";
      }
    }
    else{
      throw "not a C-command";
    }
  }

  string comp(){
    if(commandType() == 1){
      string returnstring;
      std::size_t foundeq = curr_command.find('=');
      std::size_t foundsem = curr_command.find(';');
      if(foundeq!=std::string::npos){
        if(foundsem!=std::string::npos){
          for(int i=foundeq+1;i<foundsem;i++){
            returnstring.append(1,curr_command[i]);
          }
        }
        else{
          for(int i=foundeq+1;i<curr_command.size();i++){
            returnstring.append(1,curr_command[i]);
          }
        }
        return returnstring;
      }
      else{
        if(foundsem!=std::string::npos){
          for(int i=0;i<foundsem;i++){
            returnstring.append(1,curr_command[i]);
          }
        }
        else{
          throw "invalid command";
        }
        return returnstring;
      }
    }
    else{
      throw "not a C-command";
    }
  }

  string jump(){
    if(commandType() == 1){
      string returnstring;
      std::size_t found = curr_command.find(';');
      if(found!=std::string::npos){
        for(int i=found+1;i<curr_command.size();i++){
          returnstring.append(1,curr_command[i]);
        }
        return returnstring;
      }
      else{
        return "null";
      }
    }
    else{
      throw "not a C-command";
    }
  }

};


class Code{
  string dest(string mnemonics){
    if(mnemonics=="null"){
      return "000";
    }
    else{
      int d[3];
      d[0]=d[1]=d[2]=0;
      if(mnemonics.find("A") != std::string::npos){
        d[0]=1;
      }
      if(mnemonics.find("D") != std::string::npos){
        d[1]=1;
      }
      if(mnemonics.find("M") != std::string::npos){
        d[2]=1;
      }
      string returnstring;
      for(int i=0;i<3;i++){
        returnstring.append(char(d[i]));
      }
      return returnstring;
    }
  }

  string dest(string mnemonics){
    if(mnemonics=="0"){
      return "101010";
    }
    if(mnemonics=="1"){
      return "111111";
    }
    if(mnemonics=="-1"){
      return "111010";
    }
    else{
      int d[6];
      d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=0;
      if(mnemonics.find("&") != std::string::npos){
        return "000000";
      }
      if(mnemonics.find("|") != std::string::npos){
        return "010101";
      }
      if(mnemonics.size()==2){
        if(mnemonics[0]=='!'){
          if(mnemonics[1] == 'D'){
            return "001101"
          }
          else{
            return "110001";
          }
        }
        else{
          if(mnemonics[1] == 'D'){
            return "001111"
          }
          else{
            return "110011";
          }
        }
      }
      else{
        if(mnemonics.size()==1){
          if(mnemonics[1] == 'D'){
            return "001100"
          }
          else{
            return "110000";
          }
        }
        else{
          if(mnemonics[1]=='+'){
            if(mnemonics[0] == 'D' || mnemonics[2]=='D'){
              if(mnemonics[2] == '1' || mnemonics[2]=='1'){
                return "011111"
              }
              else{
                return "000010";
              }
            }
            else{
              if(mnemonics[2] == '1' || mnemonics[2]=='1'){
                return "110111"
              }
              else{
                throw "invalid mnemonic"
              }
            }
          }
          if(mnemonics[1]=='-'){
            if(mnemonics=="D-A"){
              return "010011";
            }
            if(mnemonics=="A-A"){
              return "000111";
            }
            if(mnemonics=="D-1"){
              return "001110";
            }
            if(mnemonics=="D-A"){
              return "110010";
            }
          }
        }
      }
      string returnstring;
      for(int i=0;i<3;i++){
        returnstring.append(char(d[i]));
      }
      return returnstring;
    }
  }

  string jump(string mnemonics){
    if(mnemonics=="null"){
      return "000";
    }
    else{
      int d[3];
      d[0]=d[1]=d[2]=0;
      if(mnemonics.find("G") != std::string::npos){
        d[2]=1;
      }
      if(mnemonics.find("L") != std::string::npos){
        d[0]=1;
      }
      if(mnemonics.find("E") != std::string::npos){
        d[1]=1;
      }
      if(mnemonics=="JMP"){
        return "111";
      }
      if(mnemonics=="JNQ"){
        return "101";
      }

      string returnstring;
      for(int i=0;i<3;i++){
        returnstring.append(char(d[i]));
      }
      return returnstring;
    }
  }
}
int main(int argc, char* argv[]){
  Parser p("trial.txt");
}
