/**
 * @file compiler1.cpp
 * @author Rudra Desai
 * @brief It is a mini compiler implementation for Hack
 * Language in CPP. It is a part of Nand2tetris project
 * @version 0.1
 * @date 2019-11-07
 * 
 */

#include <iostream>
#include<fstream>
#include<string>
#include<algorithm>
#include<map>

using namespace std;

#define addspace ""
#define endl '\n'
#define pss pair<string,string>

enum tokentype {keyword,symbol};

fstream inputFile;
fstream outFile;
fstream errFile;
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
			// outFile<<"<tokens>\n";
			string curr_line;
			while(getline(inputFile,curr_line)){
				curr_line.erase(std::remove(curr_line.begin(), curr_line.end(), '\r'), curr_line.end());
				if(curr_line.size()>=1){
					if(RemoveComments(curr_line)){
						tokenize(currexpr,outFile);
					}
				}
				curr_line.clear();
			}
			// outFile<<"</tokens>\n";
		}
};

class SyntaxCheck{
	public:
		string curr_token;
		string curr_line;
		string classname;
		fstream inputFile;
		fstream outFile;
		fstream errFile;
		pss p;

		void clean(){
			inputFile.close();
			outFile.close();
			errFile.close();
			exit(0);
		}
		pair<string,string> extracttokens(){
			getline(inputFile,curr_line);
			// cout<<curr_line<<endl;
			string token,tokentype;
			int i=0;
			while(i<curr_line.size()){
				if(curr_line[i] == ' '){
					i++;
				}
				else{
					if(curr_line[i] == '<'){
						i++;
						while(curr_line[i] != '>'){
							tokentype.append(1,curr_line[i]);
							i++;
						}
						i++;
					}
					else{
						while(curr_line[i] != '<'){
							token.append(1,curr_line[i]);
							i++;
						}
						break;
					}
				}
			}
			// cout<<tokentype<<endl;
			return make_pair(tokentype,token);
		}

		/**
		 * @brief takes in the symbol
		 * extracts one token and returns true if the token is a symbol
		 * if(flag == true){then it wont extract an extra token}
		 * @param symbol 
		 */
		bool checkSymbol(string symbol,bool flag = false,bool errflag = false){
			if(!flag) p = extracttokens();
			if(p.first == "symbol"){
				if(p.second == symbol){
					return true;
				}
			}
			if(!errflag) errFile<<"ERROR: '"<<symbol<<"' expected"<<endl;
			return false;
		}

		bool checkToken(string tokentype,string token){
			p = extracttokens();
			// cout<<curr_line<<endl;
			if(p.first == tokentype && p.second == token){
				return true;
			}
			else{
				return false;
			}
		}
		/**
		 * @brief checks symbol
		 * extracts first token itself
		 * @param symbol 
		 * @return true when it is a symbol 
		 * @return false when it is not a symbol
		 */
		void compileClass(){
			p = extracttokens();
			if(p.first == "identifier"){
				classname = p.second;
				outFile<<curr_line<<endl;
			}
			else{
				(errFile)<<"ERROR: no class name provided"<<endl;
			}
			if(checkSymbol("{")){
				outFile<<curr_line<<endl;
				p = extracttokens();
				if((p.first == "keyword") && ((p.second == "static") || (p.second == "field"))){
					outFile<<"<classVarDec>"<<endl;
					outFile<<curr_line<<endl;
					// compileClassVarDec();
					outFile<<"</classVarDec>"<<endl;
				}
				if((p.first == "keyword") && ((p.second == "constructor") || (p.second == "function") || (p.second == "method"))){
					outFile<<"<subroutineDec>"<<endl;
					outFile<<curr_line<<endl;
					if(!compileSubroutineDec()){return;};
					outFile<<"</subroutineDec>"<<endl;
				}
				if(checkSymbol("}")){
					outFile<<curr_line<<endl;
				}
			}
		}

		// /**
		//  * @brief it extracts its own first token
		//  * it extracts one extra token to check for more var decs
		//  * it repeats itself for multiple vardecs
		//  */
		// void compileVarDec(){
		// 	p = extracttokens();
		// 	if(p.first == "keyword" && p.second == "var"){
		// 		p = extracttokens();
		// 		if(p.first == "keyword" && (p.second == "void" || p.second =="boolean" || p.second == "char" || p.second == classname)){

		// 		}
		// 		else{ errFile<<"ERROR: Type of variable missing"<<endl;}
		// 		p = extracttokens();
		// 		if(p.first == "identifier"){}
		// 		else{ errFile<<"ERROR: Variable Name missing"<<endl;}
		// 		while(checkSymbol(",",false,true)){
		// 			p = extracttokens();
		// 			if(p.first == "keyword" && (p.second == "void" || p.second =="boolean" || p.second == "char" || p.second == classname)){

		// 			} 
		// 		}
		// 		if(!checkSymbol(";",true)){

		// 		}
		// 		compileVarDec();
		// 	}
		// }

		// /**
		//  * @brief its first token should be extracted before hand
		//  * it will extract an extra token for checking multiple statements
		//  */
		// void compileStatements(){
		// 	while(p.first == "keyword"){
		// 		switch (p.second)
		// 		{
		// 			case "let": compileLetStatement();break;
		// 			case "if": compileIfStatement();break;
		// 			case "while": compileWhileStatement();break;
		// 			case "do": compileDoStatement();break;
		// 			case "return": compileReturnStatement();break;
		// 		}
		// 		p = extracttokens();
		// 	}
		// }

		// /**
		//  * @brief it extracts it first line itself
		//  * and it wont extract any extra token
		//  */
		// void compileSubroutineBody(){
		// 	if(checkSymbol("{")){
		// 		compileVarDec();
		// 		compileStatements();
		// 		if(!checkSymbol("}")){

		// 		}
		// 	}
		// 	else{

		// 	}
		// }

		// /**
		//  * @brief Checks for parameter list
		//  * it extracts its first token itself
		//  * it also extracts one last token to check for more params
		//  */
		// void compileParameterList(){
		// 	p = extracttokens();
		// 	if(p.first == "keyword" && (p.second == "void" || p.second =="boolean" || p.second == "char" || p.second == classname)){
		// 		p = extracttokens();
		// 		if(p.first == "identifier"){
		// 			p = extracttokens();
		// 			while(p.first == "keyword" && (p.second == "void" || p.second =="boolean" || p.second == "char" || p.second == classname)){
		// 				p = extracttokens();
		// 				if(p.first == "identifier"){
		// 					p = extracttokens();
		// 				}
		// 				else{
		// 					errFile<<"ERROR: identifier expected"<<endl;
		// 				}
		// 			}
		// 		}
		// 		else{
		// 			errFile<<"ERROR: identifier expected"<<endl;
		// 		}
		// 	}
		// }

		/**
		 * @brief type of method should be checked before hand
		 * it will extract a new line after its declaration
		 * stored symbol will be in p
		 */
		bool compileSubroutineDec(){
			p = extracttokens();
			if(p.first == "keyword" && (p.second == "void" || p.second =="boolean" || p.second == "char" || p.second == classname)){
				outFile<<curr_line<<endl;
				p = extracttokens();
				if(p.first == "identifier"){
					outFile<<curr_line<<endl;
					if(checkSymbol("(")){
						outFile<<curr_line<<endl;
						// if(!compileParameterList()){return false;};
						if(checkSymbol(")",true)){
							outFile<<curr_line<<endl;
							// if(!compileSubroutineBody()){return false;};
						}
						else{
							return false;
						}
					}
					else{
						return false;
					}
				}
				else{
					(errFile)<<"ERROR: identifier expected"<<endl;
					return false;
				}
			}
			else{
				(errFile) << "ERROR: type expected"<<endl;
				return false;
			}
			if((p.first == "keyword") && ((p.second == "constructor") || (p.second == "function") || (p.second == "method"))){
				outFile<<"<subroutineDec>"<<endl;
				outFile<<curr_line<<endl;
				if(!compileSubroutineDec()){return false;};
				outFile<<"</subroutineDec>"<<endl;
			}
			return true;
		} 

		// /**
		//  * @brief it checks for compile class var dec
		//  * also before exiting it extracts one token and check for more compile var dec
		//  */
		// void compileClassVarDec(){
		// 	p = extracttokens();
		// 	if(p.first == "identifier"){
		// 		p=extracttokens();
		// 		while(checkSymbol(",",true,true)){
		// 			p = extracttokens();
		// 			if(p.first != "identifier"){(errFile)<<"ERROR: identifier expected"<<endl;}
		// 			p = extracttokens();
		// 		}
		// 		if(checkSymbol(";",true)){

		// 		}
		// 		else{}
		// 	}
		// 	else{
		// 		(errFile)<<"ERROR: identifier expected"<<endl;
		// 	}
		// 	p = extracttokens();
		// 	if((p.first == "keyword") && ((p.second == "static") || (p.second == "field"))){
		// 		compileClassVarDec();
		// 	}
		// }

		// /**
		//  * @brief In all statements , type of statement token is already extracted 
		//  * it extracts one extra token to check else
		//  */
		// void compileIfStatement(){
		// 	if(checkSymbol("(")){
		// 		if(compileExpression()){
		// 			if(checkSymbol(")",true)){
		// 				if(checkSymbol("{")){
		// 					compileStatements();
		// 					if(checkSymbol("}")){
		// 						p = extracttokens();
		// 						if(p.first == "keyword" && p.second == "else"){
		// 							if(checkSymbol("{")){
		// 								compileStatements();
		// 								if(checkSymbol("}")){
		// 									p = extracttokens();
		// 								}
		// 							}
		// 						}
		// 					}
		// 				}
		// 			}
		// 		}
		// 	}
		// }
		// void compileReturnStatement(){
		// 	if(compileExpression()){

		// 	}
		// 	else{

		// 	}
		// 	if(checkSymbol(";",true)){

		// 	}
		// 	else{

		// 	}
		// }

		// //TODO : here while(1){} is not taken care of.. check it
		// void compileWhileStatement(){
		// 	if(checkSymbol("(")){
		// 		if(compileExpression()){
		// 			if(checkSymbol(")",true)){
		// 				if(checkSymbol("{")){
		// 					compileStatements();
		// 					if(checkSymbol("}")){

		// 					}
		// 				}
		// 			}
		// 		}
		// 	}
		// }
		// // TODO : check for id1.id2 type statements;
		// void compileDoStatement(){
		// 	p = extracttokens();
		// 	if(p.first == "identifier"){
		// 		p = extracttokens();
		// 		if(p.first == "symbol" && p.second == "."){

		// 		}
		// 		else{
		// 			if(checkSymbol("(")){
		// 				int nP = compileExpressionList();
		// 				if(checkSymbol(")")){
		// 					if(checkSymbol(";")){

		// 					}
		// 				}
		// 			}
		// 		}
		// 	}
		// }
		// void compileLetStatement(){
		// 	p = extracttokens();
		// 	if(p.first == "identifier"){
		// 		p = extracttokens();
		// 		if(checkSymbol("[",true)){
		// 			if(compileExpression()){

		// 			}
		// 			else{

		// 			}
		// 			if(checkSymbol("]")){
		// 				p = extracttokens();
		// 				goto label1;
		// 			}
		// 		}
		// 		else{
		// 			errFile<<"ERROR: expected '['"<<endl;
		// 		}
		//     label1:
		// 		if(checkSymbol("=",true)){
		// 			if(compileExpression()){
		// 				if(checkSymbol(";",true)){

		// 				}
		// 			}
		// 			else{

		// 			}
		// 		}
		// 		else{
		// 		}
		// 	}
		// }
		// /**
		//  * @brief it needs its first token to be extracted
		//  * @return true if there is an op 
		//  * @return false 
		//  */
		// bool compileOp(){
		//   if(p.first != "symbol"){
		//     return false;
		//   }
		//   else{
		//     switch(p.second){
		//       case "+":break;
		//       case "-":break;
		//       case "&":break;
		//       case "|":break;
		//       case ">":break;
		//       case "<":break;
		//       case "=":break;
		//       case "*":break;
		//       case "/":break;
		//       default:
		//       return false;
		//       break;
		//     }
		//     return true;
		//   }
		// }
		// /**
		//  * @brief it extracts it first token itself 
		//  * it also extracts an extra token
		//  */
		// bool compileExpression(){
		//   if(compileTerm()){
		//     while(compileOp()){
		//       if(compileTerm()){

		//       }
		//       else{
		//         errFile<<"ERROR: extra ',' given"<<endl;
		//       }
		//     }
		//   }
		//   else{
		//   }
		// }
		// int compileExpressionList(){
		// 	int nP = 0;
		// 	if(compileExpression()){
		// 		np++;
		// 		while(checkSymbol(",",true,true)){
		// 			if(compileExpression()){
		// 				nP++;
		// 			}
		// 			else{
		// 				errFile<<"ERROR: extra comma given"<<endl;
		// 			}
		// 		}
		// 	}
		//   return nP;
		// }

		// /**
		//  * @brief it takes its first token itself
		//  * and it extracts one more too to check
		//  */
		// bool compileTerm(){
		//   string op;
		//   p = extracttokens();
		//   if(checkSymbol("-",true,true)){
		//     op = p.second;
		//   }
		//   else if(checkSymbol('~',true,true)){
		//     op=p.second;
		//   }
		//   else if(checkSymbol('(',true,true)){
		//     if(compileExpression()){
		//       checkSymbol(')',true,false);
		//     }
		//   }

		// }

	public:

		void StartProcessing(string inp,string out,string err){
			// cout<<inp<<' '<<out<<' '<<err<<endl;
			inputFile.open(inp);
			outFile.open(out);
			errFile.open(err);
			// p = extracttokens();
			if(!checkToken("keyword","class")){
				cout<<curr_line<<endl;
				(errFile)<<"ERROR: Every File should begin with a class"<<endl;
			}
			else{
				outFile<<"<class>"<<endl;
				outFile<<curr_line<<endl;
				compileClass();
				outFile<<"</class>"<<endl;
			}
			inputFile.close();
			outFile.close();
			errFile.close();
		}
};

int main(int argc, char *argv[]) {



	//initiates a parser class object
	Parser P;
	P.Initialize();

	fstream inputFile;
	fstream outFile;
	//takes input file and output file as input
	// cout<<argv[1]<<' '<<argv[2]<<' '<<argv[3]<<' '<<argv[4]<<endl;
	inputFile.open(argv[1],ios::in);
	outFile.open(argv[2],ios::out);

	// cout<<"processing start"<<endl;
	//starts the processing of input file
	P.ProcessInputFile(inputFile,outFile);

	// closes the opened files
	inputFile.close();
	outFile.close();

	// cout<<"MainT.xml created"<<endl;
	//opens inter file and outfile
	// inputFile.open(argv[2],ios::in);
	// outFile.open(argv[3],ios::out);
	// errFile.open(argv[4],ios::out);
	//initializes Syntaxchecker

	// cout<<"syntax check starting"<<endl;
	SyntaxCheck S;

	// cout<<"calling compile class"<<endl;
	S.StartProcessing(argv[2],argv[3],argv[4]);
	// string curr_line;
	// while(getline(inputFile,curr_line)){
	//   cout<<curr_line<<endl;
	// }
	//starts the syntax checking of the xml generated file

	return 0;
}
