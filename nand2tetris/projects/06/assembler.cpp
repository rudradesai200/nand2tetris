/**************************************
*  Author : Rudra Desai               *
*  Date Created : Aug 16,2019         *
*  Last edit : Aug 16,2019            *
*  Program Name : Assembler           *
***************************************/



#include<iostream>
#include<fstream>
#include<string>

using namespace std;

#define MAX_ADDRESS 100000
#define MAX_SYMBOL_LENGTH 20
#define MAX_SYMBOL_TABLE_ENTRIES 1024

fstream inputFile;
fstream interFile;
fstream outFile;


class SymbolTableEntry{
private:
  string symbolname;
  int address;

public:
//constructor
  SymbolTableEntry(){
    symbolname = "";
    address = -1;
  }

//compares two string
  bool Compare(const string name){
    if(symbolname == name){
      return true;
    }
    return false;
  }

//returns if the address is assigned or not
  bool AddrAssigned(){
    if(address!=-1){
      return true;
    }
    return false;
  }

//updates address of the symbol if it doesn't exist
  void UpdateAddress(const int addr){
    if(addr <= MAX_ADDRESS && AddrAssigned()==false)
      address = addr;
  }

//updates the symbolname
  void UpdateName(const string name){

    if(name.size() <= MAX_SYMBOL_LENGTH)
      symbolname.append(name);
    else
      throw "symbol name too large\n";

  }

//creates a new entry
  void UpdateSymbol(const string name, const int addr){
    // cout<<name<<"\t\t"<<addr<<endl;
    UpdateName(name);
    UpdateAddress(addr);
  }

  void view(){
    cout<<symbolname <<"\t\t"<<address<<endl;
  }
};

class SymbolTable{
private:
    SymbolTableEntry symboltable[MAX_SYMBOL_TABLE_ENTRIES];
    int curr_symbols;
public:
    //loads the default values
    SymbolTable(){
      symboltable[0].UpdateSymbol("SP",0);
      symboltable[1].UpdateSymbol("LCL",1);
      symboltable[2].UpdateSymbol("ARG",2);
      symboltable[3].UpdateSymbol("THIS",3);
      symboltable[4].UpdateSymbol("THAT",4);
      symboltable[5].UpdateSymbol("R0",0);
      symboltable[6].UpdateSymbol("R1",1);
      symboltable[7].UpdateSymbol("R2",2);
      symboltable[8].UpdateSymbol("R3",3);
      symboltable[9].UpdateSymbol("R4",4);
      symboltable[10].UpdateSymbol("R5",5);
      symboltable[11].UpdateSymbol("R6",6);
      symboltable[12].UpdateSymbol("R7",7);
      symboltable[13].UpdateSymbol("R8",8);
      symboltable[14].UpdateSymbol("R9",9);
      symboltable[15].UpdateSymbol("R10",10);
      symboltable[16].UpdateSymbol("R11",11);
      symboltable[17].UpdateSymbol("R12",12);
      symboltable[18].UpdateSymbol("R13",13);
      symboltable[19].UpdateSymbol("R14",14);
      symboltable[20].UpdateSymbol("R15",15);
      symboltable[21].UpdateSymbol("SCREEN",16384);
      symboltable[22].UpdateSymbol("KBD",24576);
      curr_symbols = 23;
    }

  //checks if the symbol is already present else returns -1
    int SymbolPresent(const string name){
      for(int i=0;i<curr_symbols;i++){
        if(symboltable[i].Compare(name)){
          return i;
        }
      }
      return -1;
    }

  //adds a new symbol to the symbol table if a new address is found else does nothing
    void CheckName(const string S){
      int pos = SymbolPresent(S);
      if(pos == -1){
        symboltable[curr_symbols].UpdateSymbol(S,-1);
        // cout<<S<<endl;
        curr_symbols++;
        // cout <<S<<' '<<curr_symbols<<endl;
      }
    }

  //updates the address of the symbol table and will only be called if a label is found
    void AddLabel(const string name,const int addr){
      int pos = SymbolPresent(name);
      if(pos==-1){
        symboltable[curr_symbols].UpdateSymbol(name,16+addr);
        curr_symbols++;
      }
    }


    void ViewTable(){
      for(int i=0 ; i<curr_symbols;i++){
        symboltable[i].view();
      }
    }

    void pass(const string S){
      cout<<S<<endl;
    }
};

SymbolTable S;


class Parser{
public:\
  int f=0;
  string curr_line;
  string curr_command;
  int instruction_count=0;
  int line_count = 0;
//fetches next line and return true if it is not the end of the file

  void RemoveWhiteSpace(){
      f = 0;
        // To keep track of non-space character count
        int count = 0;
        string whitespacefree;
        for (int i = 0; curr_line[i]; i++)
            if (curr_line[i] != ' ')
                whitespacefree.append(1,curr_line[i]); // here count is
                                       // incremented
        curr_line.clear();
        curr_line = whitespacefree;
      }

  void RemoveComments(){
    int i=0;
    for(i=0; curr_line[i];i++){
      if(curr_line[i] == '/'){
        if(curr_line[i+1] == '/'){
          f=1;
          break;
        }
      }
      else{
        curr_command.append(1,curr_line[i]);
      }
    }
    if(curr_command.size()<=1){
      f=1;
    }
  }

  void ResolveSymbol(){
    string addr;
    if(curr_command[0] == '@'){
      // cout<<curr_command<<endl;
      addr.append(curr_command,1,curr_command.size()-1);
      //
      // S.pass(addr);
      if((!(addr[0]>='0' && addr[0]<='9'))){
        // cout<<addr<<endl;
        S.CheckName(addr);
      }
    }
    else{
      if(curr_command[0] == '('){
        addr.append(curr_command,1,curr_command.size()-3);
        // cout<<addr<<endl;
        S.AddLabel(addr,instruction_count);
        instruction_count--;
      }
    }
    instruction_count++;
  }

  void ProcessInputFile(){
    while(getline(inputFile,curr_line)){
      // cout<<curr_line<<endl;
      if(curr_line!=""){
        // cout<<"removing spaces"<<endl;
        RemoveWhiteSpace();
        // cout<<"spaces removed , removing comments"<<endl;
        RemoveComments();

        // cout<<"comments removed"<<endl;
        if(f==0){
          ResolveSymbol();
          interFile << curr_command <<endl;
          // instruction_count++;
        }
      }
      curr_command.clear();
      curr_line.clear();
    }
  }

};

int main(int arc,char *argv[]){
  Parser P;
  // S.Initialize();
  inputFile.open(argv[1]);
  interFile.open("interfile.txt");
  P.ProcessInputFile();
  interFile.close();
  inputFile.close();
  S.ViewTable();
}
