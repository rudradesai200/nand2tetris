/**************************************
*  Author : Rudra Desai               *
*  Date Created : Aug 16,2019         *
*  Last edit : Aug 18,2019            *
*  Program Name : Assembler           *
***************************************/



#include<iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<bitset>
#include<math.h>

using namespace std;

#define MAX_ADDRESS 100000
#define MAX_SYMBOL_LENGTH 25
#define MAX_SYMBOL_TABLE_ENTRIES 1024

fstream inputFile;
fstream interFile;
fstream outFile;


class SymbolTableEntry{
	public:
	  string symbolname;
	  int address;

	
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
		    if(addr==-1){
		    	UpdateName(name);
		    }
		    else{
		    	UpdateName(name);
		    	UpdateAddress(addr);	
		    }  
		 }

		//to view an entry
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

	    void change(int last_ram_addr){
	    	for(int i=22;i<curr_symbols;i++){
	    		if(!symboltable[i].AddrAssigned()){
	    			symboltable[i].UpdateAddress(max(last_ram_addr+1,16));
	    			last_ram_addr++;
	    		}
	    	}
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

	   	//returns value stored corresponding to the name
	    int RamAdrress(const string name){
	    	return symboltable[SymbolPresent(name)].address;
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
	      // ViewTable();
	      // cout<<endl;
	    }

	  //updates the address of the symbol table and will only be called if a label is found
	    void AddLabel(const string name,const int addr){
	      int pos = SymbolPresent(name);
	      if(pos==-1){
	        symboltable[curr_symbols].UpdateSymbol(name,addr);
	        curr_symbols++;
	      }
	      else{
	      	symboltable[pos].UpdateAddress(addr);
	      }
	      // ViewTable();
	      // cout<<endl;
	    }


	    void ViewTable(){
	      for(int i=-0 ; i<curr_symbols;i++){
	        symboltable[i].view();
	      }
	    }
};

SymbolTable S;

class Parser{
	public:\
	  int f=0;
	  string curr_line;
	  string curr_command;
	  int instruction_count=0;
	  int last_ram_addr = 16;
	  bool notlabel;

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

	//resolves symbol and checks for a label or address and sets label for writing in interfile
    void ResolveSymbol(){
	    string addr;
	    if(curr_command[0] == '@'){
	      addr.append(curr_command,1,curr_command.size()-1);
	      
	      if((!(addr[0]>='0' && addr[0]<='9'))){
	      	// cout<<addr<<endl;
	        S.CheckName(addr);
	      }
	    }
	    else{
	      if(curr_command[0] == '('){
	        addr.append(curr_command,1,curr_command.size()-2);
	        // cout<<addr<<endl;
	        S.AddLabel(addr,instruction_count);
	        last_ram_addr = instruction_count;
	        instruction_count--;
	        notlabel = false;
	      }
	    }
	    instruction_count++;
    }

	//processes input file to create inter file
	void ProcessInputFile(){
	  	int i=0;
	    while(getline(inputFile,curr_line)){
	      if(curr_line!=""){
	      	notlabel = true;
	        // cout<<"removing spaces"<<endl;
	        RemoveWhiteSpace();
	        // cout<<"spaces removed , removing comments"<<endl;
	        RemoveComments();

	        // cout<<"comments removed"<<endl;
	        if(f==0){
	        	// cout<<i<<endl;
	        	ResolveSymbol();
	        	if(notlabel)
	        		interFile << curr_command <<endl;
	        	i++;
	          	// instruction_count++;
	        }
	      }
	      curr_command.clear();
	      curr_line.clear();
	    }
	    S.change(last_ram_addr);
  	}
};

Parser P;

//converts decimal string to 15 bit binary
string convertto15bitbinary(string inp){
	// cout<<inp.size()<<endl;
	int no=0;
	int l = inp.size();
	for(int i=0;i<l;i++){
		no += (inp[i]-'0')*int(pow(10,(l-i-1))); 
	}
	// cout<<(no)<<endl;
	return std::bitset< 16 >( no ).to_string();
}


class Converter{

	public:
	string result;
	string curr_command;


	//converts Address string to binary
	void AddressConverter(){
		string check;
	 	check.append(curr_command,1,curr_command.size()-1);
		if(curr_command[1]>='0' && curr_command[1]<='9'){
			result = convertto15bitbinary(check);
		}
		else{
			result = std::bitset< 16 >( S.RamAdrress(check) ).to_string();
		}
		// cout<<result<<endl;
	}

	//converts Dstring to its corresponding binary
	string extractD(string Dstring){
		// cout<<Dstring<<endl;
		if(Dstring == ""){
	      return "000";
	    }
	    else{
	      int d[3];
	      d[0]=d[1]=d[2]=0;
	      if(Dstring.find("A") != std::string::npos){
	        d[0]=1;
	      }
	      if(Dstring.find("D") != std::string::npos){
	        d[1]=1;
	      }
	      if(Dstring.find("M") != std::string::npos){
	        d[2]=1;
	      }
	      string returnstring;
	      for(int i=0;i<3;i++){
	        if(d[i] == 0)
	        	returnstring.append(1,'0');
	        else
	        	returnstring.append(1,'1');
	      }
	      return returnstring;
	    }
	}

	//converts Jstring to its corresponding binary
	string extractJ(string mnemonics){
		// cout<<mnemonics<<endl;
	    if(mnemonics==""){
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

	        if(d[i] == 0)
	        	returnstring.append(1,'0');
	        else
	        	returnstring.append(1,'1');
	      }
	      return returnstring;
	    }
	}

	//converts Cstring to its corresponding binary
	string extractC(string mnemonics){
		// cout<<mnemonics<<endl;
		string returnstring;
	    if(mnemonics=="0"){
	      return "0101010";
	    }
	    if(mnemonics=="1"){
	      return "0111111";
	    }
	    if(mnemonics=="-1"){
	      return "0111010";
	    }
	    else{
	      int d[6];
	      d[0]=d[1]=d[2]=d[3]=d[4]=d[5]=0;
	      if(mnemonics.find('M')!=std::string::npos){
	        returnstring = "1";
	      }
	      else{
	        returnstring = "0";
	      }

	      if(mnemonics.find("|") != std::string::npos){
	        d[1]=d[3]=d[5]=1;
	      }

	      if(mnemonics.size()==2){
	        if(mnemonics[0]=='!'){
	          if(mnemonics[1] == 'D'){
	            d[2]=d[3]=d[5]=1;
	          }
	          else{
	          	d[0]=d[1]=d[5]=1;
	          }
	        }
	        else{
	          if(mnemonics[1] == 'D'){
	            d[2]=d[3]=d[4]=d[5]=1;
	          }
	          else{
	            d[0]=d[1]=d[4]=d[5]=1;
	          }
	        }
	      }
	      else{
	        if(mnemonics.size()==1){
	          if(mnemonics[0] == 'D'){
	            d[2]=d[3]=1;
	          }
	          else{
	          	d[0]=d[1]=1;
	          }
	        }
	        else{
		         	 if(mnemonics == "D+1" || mnemonics == "1+D")
		                d[1]=d[2]=d[3]=d[4]=d[5]=1;
		          if(mnemonics == "D+A" || mnemonics == "A+D" || mnemonics == "D+M" || mnemonics == "M+D" )
		                d[4]=1;
		          if(mnemonics == "A+1" || mnemonics == "1+A" || mnemonics == "M+1" || mnemonics == "1+M")
		                d[0]=d[1]=d[3]=d[4]=d[5]=1;

	            if(mnemonics=="D-1"){
	              d[2]=d[3]=d[4]=1;
	            }
	            if(mnemonics=="A-1" || mnemonics=="M-1"){
	              d[0]=d[1]=d[4]=1;
	            }
	            if(mnemonics=="D-A" || mnemonics=="D-M"){
	              d[1]=d[4]=d[5] = 1;
	            }
	            if(mnemonics=="A-D" || mnemonics=="M-D"){
	              d[3]=d[4]=d[5] = 1;
	            }
	        }
	      }
	      // string returnstring;
	      for(int i=0;i<6;i++){
	      	if(d[i] == 0)
	        	returnstring.append(1,'0');
	        else
	        	returnstring.append(1,'1');
	      }
	      return returnstring;
	    }
	  }

	//combines C , D , J string and outputs binary for C-type string
	string Combine(string Cstring,string Dstring,string Jstring){
		string out;
		out = "111" + extractC(Cstring) + extractD(Dstring) + extractJ(Jstring);
		return out;
	}

	//appends two string from (start,end]
	string appendtwo(string from,int start,int end){
		string x;
		// cout<<start<<' '<<end<<endl;
		for(int i=start;i<end;i++){
			x.append(1,from[i]);
		}
		return x;
	}
	//extracts and stores the output binary string of C-type command
	void CommandConverter(){
		int semicolon_pos = curr_command.find(';');
		int equalto_pos = curr_command.find('=');
		string Dstring;
		string Cstring;
		string Jstring;
		if(semicolon_pos != string::npos ){
			if(equalto_pos != string::npos ){
				Dstring = appendtwo(curr_command,0,equalto_pos);
				Cstring = appendtwo(curr_command,equalto_pos+1,semicolon_pos);
				Jstring = appendtwo(curr_command,semicolon_pos+1,curr_command.size());
				// cout<<Cstring<<'\t'<<Dstring<<'\t'<<Jstring<<endl;
			}
			else{
				Cstring = appendtwo(curr_command,0,semicolon_pos);
				Jstring = appendtwo(curr_command,semicolon_pos+1,curr_command.size());
				// cout<<Cstring<<'\t'<<Dstring<<'\t'<<Jstring<<endl;
			}
		}
		else{
			if(equalto_pos != string::npos ){
				Dstring = appendtwo(curr_command,0,equalto_pos);
				Cstring = appendtwo(curr_command,equalto_pos+1,curr_command.size());
				// cout<<Cstring<<'\t'<<Dstring<<'\t'<<Jstring<<endl;
			}
		}
		result = Combine(Cstring,Dstring,Jstring);
	}

	//processes interfile and distributes the commands according to the type
	void ProcessInterFile(){
		while(getline(interFile,curr_command)){
			// cout<<curr_command<<endl;
			if(curr_command[0]=='@'){
				AddressConverter();
			}
			else{
				if(curr_command[0] != '(')
					CommandConverter();
			}
			outFile <<result<<endl;
		}
	}
};

Converter C;
int main(int arc,char *argv[]){
  
  // S.Initialize();
  inputFile.open(argv[1],ios::in);
  interFile.open("interfile.txt",ios::out);
  cout<<"Pass 1 initiated\n";

  P.ProcessInputFile();

  cout<<"Pass 1 completed\nInterfile made\n";
  inputFile.close();
  interFile.close();

  cout<<"Pass 2 initiated\n";
  outFile.open(argv[2],ios::out);
  interFile.open("interfile.txt",ios::in);

  C.ProcessInterFile();
  
  cout<<"Pass 2 completed\n";
  cout<<"Assembling successfull\n";
  interFile.close();
  outFile.close();
  remove("interfile.txt");
  // S.ViewTable();
}
