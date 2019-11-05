/**************************************
 *  Author : Rudra Desai               *
 *  Date Created : Aug 30,2019         *
 *  Last edit : Aug 31,2019            *
 *  Program Name : Compiler1.cpp       *
 ***************************************/

#include <iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<map>

using namespace std;

#define addspace " "
#define endl '\n'
enum tokentype {keyword,symbol};

class Parser{
	public:
		string currexpr;
		map<string,tokentype> keymap;
		map<char,tokentype> symbolmap;

		//Initializes parser with values of keywords and symbols
		void Initialize(){
			//Initializing keyword type to map
			keymap.insert(pair<string,tokentype>("class",keyword));
			keymap.insert(pair<string,tokentype>("constructor",keyword));
			keymap.insert(pair<string,tokentype>("function",keyword));
			keymap.insert(pair<string,tokentype>("method",keyword));
			keymap.insert(pair<string,tokentype>("field",keyword));
			keymap.insert(pair<string,tokentype>("static",keyword));
			keymap.insert(pair<string,tokentype>("var",keyword));
			keymap.insert(pair<string,tokentype>("int",keyword));
			keymap.insert(pair<string,tokentype>("char",keyword));
			keymap.insert(pair<string,tokentype>("boolean",keyword));
			keymap.insert(pair<string,tokentype>("void",keyword));
			keymap.insert(pair<string,tokentype>("true",keyword));
			keymap.insert(pair<string,tokentype>("false",keyword));
			keymap.insert(pair<string,tokentype>("null",keyword));
			keymap.insert(pair<string,tokentype>("this",keyword));
			keymap.insert(pair<string,tokentype>("let",keyword));
			keymap.insert(pair<string,tokentype>("do",keyword));
			keymap.insert(pair<string,tokentype>("if",keyword));
			keymap.insert(pair<string,tokentype>("else",keyword));
			keymap.insert(pair<string,tokentype>("while",keyword));
			keymap.insert(pair<string,tokentype>("return",keyword));

			//Initializing symbol type to map
			symbolmap.insert(make_pair('{',symbol));
			symbolmap.insert(make_pair('}',symbol));
			symbolmap.insert(make_pair('(',symbol));
			symbolmap.insert(make_pair(')',symbol));
			symbolmap.insert(make_pair('[',symbol));
			symbolmap.insert(make_pair(']',symbol));
			symbolmap.insert(make_pair('.',symbol));
			symbolmap.insert(make_pair(',',symbol));
			symbolmap.insert(make_pair(';',symbol));
			symbolmap.insert(make_pair('+',symbol));
			symbolmap.insert(make_pair('-',symbol));
			symbolmap.insert(make_pair('*',symbol));
			symbolmap.insert(make_pair('/',symbol));
			symbolmap.insert(make_pair('&',symbol));
			symbolmap.insert(make_pair('|',symbol));
			symbolmap.insert(make_pair('<',symbol));
			symbolmap.insert(make_pair('>',symbol));
			symbolmap.insert(make_pair('=',symbol));
			symbolmap.insert(make_pair('~',symbol));
		}

		//classifies between different identifiers and keywords
		string whichType(string S){
			// cout<<S;
			if(keymap.find(S)!=keymap.end()){
				return "keyword";
			}
			else{
				return "identifier";
			}
		}

		//checks for symbol
		bool isSymbol(char c){
			if(symbolmap.find(c)!=symbolmap.end()){
				return true;
			}
			else{
				return false;
			}
		}

		//tokenizes given strings and writes to the outFile
		void tokenize(string currexpr,fstream &outFile){

			//ignores the initial spaces and tabs
			int i=0;
			while(currexpr[i] == ' '){
				i++;
			}

			while(i<currexpr.size()){
				//ignore spaces and endline
				if(currexpr[i] == ' ' || currexpr[i] =='\n' || currexpr[i] =='\t'){
					i++;
				}

				//captures the string constants and prints it in the file
				else if(currexpr[i] == '\"'){
					string token;
					i++;
					while(currexpr[i] != '\"' && i<currexpr.size()){
						token.append(1,currexpr[i]);
						i++;
					}
					i++;
					outFile<<"<stringConstant>"<<addspace<<token<<addspace<<"</stringConstant>\n";
				}

				//captures integer constants and prints it in the file
				else if(isdigit(currexpr[i])){
					string token;
					while(isdigit(currexpr[i]) && i<currexpr.size()){
						token.append(1,currexpr[i]);
						i++;
					}
					outFile<<"<integerConstant>"<<addspace<<token<<addspace<<"</integerConstant>\n";
				}

				//captures symbol and prints it in the file
				else if(isSymbol(currexpr[i])){
					// cout<<currexpr[i]<<endl;
					if(currexpr[i] == '<'){
						outFile << "<symbol>"<<addspace<<"&lt;"<<addspace<<"</symbol>\n";
					}
					else{
						if(currexpr[i] == '>'){
							outFile << "<symbol>"<<addspace<<"&gt;"<<addspace<<"</symbol>\n";
						}
						else{
							outFile << "<symbol>"<<addspace<<currexpr[i]<<addspace<<"</symbol>\n";
						}
					}
					i++;
				}

				//checks the token if it is a keyword or a identifier
				else{
					string token;
					while(((!isSymbol(currexpr[i])) && (currexpr[i]!=' ') && (currexpr[i]!=';') && (currexpr[i] != '\r') && (i < currexpr.size()))){
						token.append(1,currexpr[i]);
						i++;
					}
					if(token != "\n"){
						string elem = whichType(token);
						outFile<<"<"<<elem<<">"<<addspace<<token<<addspace<<"</"<<elem<<">\n";
					}
				}
			}
		}

		//removes all types of comments (// /* or <code> //)
		bool RemoveComments(string curr_line){
			int f=0;
			string holder;
			for(int i=0; i<curr_line.size();i++){
				if(curr_line[i] == '/'){
					if(curr_line[i+1] == '/' || curr_line[i+1] == '*'){
						f=1;
					}
				}
				else if(f==1 || curr_line[i] == '*'){
					if(curr_line[i+1] == '/'){
						return false;
					}
				}
				else{
					holder.append(1,curr_line[i]);
				}
			}
			if(holder.size()<1){
				return false;
			}
			else{
				currexpr = holder;
			}
			return true;
		}

		//processes inputFile and generates tokens
		void ProcessInputFile(fstream &inputFile,fstream &outFile){
			outFile<<"<tokens>\n";

			string curr_line;

			while(getline(inputFile,curr_line)){

				if(RemoveComments(curr_line)){
					tokenize(currexpr,outFile);
				}
				curr_line.clear();
			}
			outFile<<"</tokens>\n";
		}
};


int main(int argc, char *argv[]) {

	fstream inputFile;
	fstream outFile;
	
  //initiates a parser class object
  Parser P;
	P.Initialize();

  //takes input file and output file as input
	inputFile.open(argv[1],ios::in);
	outFile.open(argv[2],ios::out);

  //starts the processing of input file
	P.ProcessInputFile(inputFile,outFile);

  //closes the opened files
	inputFile.close();
	outFile.close();


	return 0;
}
