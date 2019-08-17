class Parser{

  ofstream file;
  string curr_command;

  Parser(char *argv){
    file.open(argv,ios::in);
  }


  void advance(){
    string S;
    if(getline(file,S){
      if(S[0]!='/' && S[1]!='/'){
        curr_command.clear();
        for(int i=0;i<S.size()){
          if(S[i]!=' '){
            curr_command.append(S[i]);
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
        returnstring.append(curr_command[i+1]);
      }
      return returnstring;
    }
    else{
      if(commandType()==2){
        string returnstring;
        for(int i=0;i<(curr_command.size()-1);i++){
          returnstring.append(curr_command[i+1]);
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
      std::size_t found = str.find('=');
      if(found!=std::string::npos){
        for(int i=0;i<found;i++){
          returnstring.append(curr_command[i]);
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
      std::size_t foundeq = str.find('=');
      std::size_t foundsem = str.find(';');
      if(foundeq!=std::string::npos){
        if(foundsem!=std::string::npos){
          for(int i=foundeq+1;i<foundsem;i++){
            returnstring.append(curr_command[i]);
          }
        }
        else{
          for(int i=foundeq+1;i<curr_command.size();i++){
            returnstring.append(curr_command[i]);
          }
        }
        return returnstring;
      }
      else{
        if(foundsem!=std::string::npos){
          for(int i=0;i<foundsem;i++){
            returnstring.append(curr_command[i]);
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
      std::size_t found = str.find(';');
      if(found!=std::string::npos){
        for(int i=found+1;i<curr_command.size();i++){
          returnstring.append(curr_command[i]);
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

}
