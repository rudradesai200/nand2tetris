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

#include<bits/stdc++.h>

#include<map>

using namespace std;


# define addspace ""
# define endl '\n'
# define pss pair < string, string >

enum tokentype {
	keywordConstant,
	keyword,
	symbol
};
enum EntryType {
	Field,
	Static,
	Local,
	Argument,
	NONETYPE
};
enum SubroutineType {
	Constructor,
	Function,
	Method
};

map < EntryType, string > kindmap;

fstream inputFile;
fstream outFile;
fstream errFile;
class Parser {
	public:
		string currexpr;
		map < string, tokentype > keymap;
		map < char, tokentype > symbolmap;
		bool block_Comment = false;

		//Initializes parser with values of keywords and symbols
		void Initialize() {
			//Initializing keyword type to map
			keymap.insert(pair < string, tokentype > ("class", keyword));
			keymap.insert(pair < string, tokentype > ("constructor", keyword));
			keymap.insert(pair < string, tokentype > ("function", keyword));
			keymap.insert(pair < string, tokentype > ("method", keyword));
			keymap.insert(pair < string, tokentype > ("field", keyword));
			keymap.insert(pair < string, tokentype > ("static", keyword));
			keymap.insert(pair < string, tokentype > ("var", keyword));
			keymap.insert(pair < string, tokentype > ("int", keyword));
			keymap.insert(pair < string, tokentype > ("char", keyword));
			keymap.insert(pair < string, tokentype > ("boolean", keyword));
			keymap.insert(pair < string, tokentype > ("void", keyword));
			keymap.insert(pair < string, tokentype > ("true", keywordConstant));
			keymap.insert(pair < string, tokentype > ("false", keywordConstant));
			keymap.insert(pair < string, tokentype > ("null", keywordConstant));
			keymap.insert(pair < string, tokentype > ("this", keywordConstant));
			keymap.insert(pair < string, tokentype > ("let", keyword));
			keymap.insert(pair < string, tokentype > ("do", keyword));
			keymap.insert(pair < string, tokentype > ("if", keyword));
			keymap.insert(pair < string, tokentype > ("else", keyword));
			keymap.insert(pair < string, tokentype > ("while", keyword));
			keymap.insert(pair < string, tokentype > ("return", keyword));

			//Initializing symbol type to map
			symbolmap.insert(make_pair('{', symbol));
			symbolmap.insert(make_pair('}', symbol));
			symbolmap.insert(make_pair('(', symbol));
			symbolmap.insert(make_pair(')', symbol));
			symbolmap.insert(make_pair('[', symbol));
			symbolmap.insert(make_pair(']', symbol));
			symbolmap.insert(make_pair('.', symbol));
			symbolmap.insert(make_pair(',', symbol));
			symbolmap.insert(make_pair(';', symbol));
			symbolmap.insert(make_pair('+', symbol));
			symbolmap.insert(make_pair('-', symbol));
			symbolmap.insert(make_pair('*', symbol));
			symbolmap.insert(make_pair('/', symbol));
			symbolmap.insert(make_pair('&', symbol));
			symbolmap.insert(make_pair('|', symbol));
			symbolmap.insert(make_pair('<', symbol));
			symbolmap.insert(make_pair('>', symbol));
			symbolmap.insert(make_pair('=', symbol));
			symbolmap.insert(make_pair('~', symbol));
		}

		//classifies between different identifiers and keywords
		string whichType(string S) {
			// cout<<S;
			if (keymap.find(S) != keymap.end()) {
				if ((S == "true") || (S == "false") || (S == "null") || (S == "this")) {
					return "keywordConstant";
				}
				return "keyword";
			} else {
				return "identifier";
			}
		}

		//checks for symbol
		bool isSymbol(char c) {
			// cout<<c<<endl;
			if (symbolmap.find(c) != symbolmap.end()) {
				return true;
			} else {
				return false;
			}
		}

		//tokenizes given strings and writes to the outFile
		void tokenize(string currexpr, fstream & outFile) {
			// cout<<currexpr<<endl;
			//ignores the initial spaces and tabs
			int i = 0;
			while (i < currexpr.size() && currexpr[i] == ' ') {
				i++;
			}

			while (i < currexpr.size()) {
				//ignore spaces and endline
				if (currexpr[i] == ' ' || currexpr[i] == '\n' || currexpr[i] == '\t') {
					i++;
				}
				// else if(currexpr[i] == '/'){
				// 	outFile<<"<symbol>"<<addspace<<"/"<<addspace<<"</symbol>"<<endl;
				// }
				//captures the string constants and prints it in the file
				else if (currexpr[i] == '\"') {
					string token;
					i++;
					while (currexpr[i] != '\"' && i < currexpr.size()) {
						token.append(1, currexpr[i]);
						i++;
					}
					i++;
					outFile << "<stringConstant>" << addspace << token << addspace << "</stringConstant>\n";
				}

				//captures symbol and prints it in the file
				else if (isSymbol(currexpr[i])) {
					// cout<<currexpr[i]<<endl;
					if (currexpr[i] == '<') {
						outFile << "<symbol>" << addspace << "<" << addspace << "</symbol>\n";
					} else {
						if (currexpr[i] == '>') {
							outFile << "<symbol>" << addspace << ">" << addspace << "</symbol>\n";
						} else {
							outFile << "<symbol>" << addspace << currexpr[i] << addspace << "</symbol>\n";
						}
					}
					i++;
				}

				//captures integer constants and prints it in the file
				else if (isdigit(currexpr[i])) {
					string token;
					while (isdigit(currexpr[i]) && i < currexpr.size()) {
						token.append(1, currexpr[i]);
						i++;
					}
					outFile << "<integerConstant>" << addspace << token << addspace << "</integerConstant>\n";
				}

				//checks the token if it is a keyword or a identifier
				else {
					string token;
					while (((!isSymbol(currexpr[i])) && (currexpr[i] != ' ') && (currexpr[i] != ';') && (currexpr[i] != '\r') && (i < currexpr.size()))) {
						token.append(1, currexpr[i]);
						i++;
					}
					if (token != "\n") {
						string elem = whichType(token);
						outFile << "<" << elem << ">" << addspace << token << addspace << "</" << elem << ">\n";
					}
				}
			}
		}

		//removes all types of comments (// /* or <code> //)
		bool RemoveComments(string curr_line) {
			int f = 0;
			string holder;
			// cout<<curr_line<<endl;
			int i = 0;
			while ((i < curr_line.size()) && (curr_line[i] == ' ')) {
				i++;
			}

			int si = (curr_line.size() - i);
			if (si == 0) {
				return false;
			} else if (si == 1) {
				holder.append(1, curr_line[i]);
				currexpr = holder;
				return true;
			} else if (si >= 2) {
				if ((curr_line[i] == '/') && (curr_line[i + 1] == '/')) {
					return false;
				} else {
					if ((curr_line[i] == '/') && (curr_line[i + 1] == '*')) {
						block_Comment = true;
						return false;
					} else {
						if ((curr_line[i] == '*') && (curr_line[i + 1] == '/') && (block_Comment)) {
							block_Comment = false;
							return false;
						} else if ((curr_line[i] == '*') && (block_Comment)) {
							return false;
						} else {
							while (i < (curr_line.size() - 1)) {
								if ((curr_line[i] == '/') && (curr_line[i + 1] == '/')) {
									currexpr = holder;
									return true;
								}
								holder.append(1, curr_line[i]);
								i++;
							}
							holder.append(1, curr_line[i]);
							currexpr = holder;
							return true;
						}
					}
				}
			}
			return true;
		}

		//processes inputFile and generates tokens
		void ProcessInputFile(fstream & inputFile, fstream & outFile) {
			// outFile<<"<tokens>\n";
			string curr_line;
			while (getline(inputFile, curr_line)) {
				curr_line.erase(std::remove(curr_line.begin(), curr_line.end(), '\r'), curr_line.end());
				if (curr_line.size() >= 1) {
					if (RemoveComments(curr_line)) {
						tokenize(currexpr, outFile);
					}
				}
				curr_line.clear();
			}
			// outFile<<"</tokens>\n";
		}
};

class symbol_table_entry {
	public:
		string Name;
		EntryType Kind;
		string Type;
		int Index;

		symbol_table_entry(string A, EntryType B, string C, int D): Name(A), Kind(B), Type(C), Index(D) {}

};
class class_symbol_table {
	public:
		vector < symbol_table_entry > CT;
		int Field_count, Static_count, Total_count;
		int labelnum;

		class_symbol_table(int a, int b, int c, int d): Field_count(a), Static_count(b), Total_count(c), labelnum(d) {}
		bool addEntry(string varname, EntryType varkind, string vartype) {
			if (find(varname).Index == -1) {
				int varind;
				if (varkind == Field) {
					varind = Field_count++;
				} else {
					varind = Static_count++;
				}
				Total_count++;
				CT.push_back(symbol_table_entry(varname, varkind, vartype, varind));
				return 1;
			}
			return 0;
		}

		symbol_table_entry find(string varname) {
			for (auto itr: CT) {
				if (itr.Name == varname) {
					return itr;
				}
			}
			return symbol_table_entry("NULL", NONETYPE, "NULL", -1);
		}

};

class subroutine_symbol_table {
	public:
		string currSubName;
		vector < symbol_table_entry > ST;
		SubroutineType currSubType;
		int Arg_count, Lcl_count, Total_count;

		subroutine_symbol_table(string N, SubroutineType T, int a, int b, int c): currSubName(N), currSubType(T), Arg_count(a), Lcl_count(b), Total_count(c) {}
		bool addEntry(string varname, EntryType varkind, string vartype) {
			if (find(varname).Index == -1) {
				int varind;
				if (varkind == Argument) {
					varind = Arg_count++;
				} else {
					varind = Lcl_count++;;
				}
				Total_count++;
				ST.push_back(symbol_table_entry(varname, varkind, vartype, varind));
				return 1;
			} else {
				return 0;
			}
		}

		symbol_table_entry find(string varname) {
			for (auto itr: ST) {
				if (itr.Name == varname) {
					return itr;
				}
			}
			return symbol_table_entry("NULL", NONETYPE, "NULL", -1);
		}
};


class SyntaxCheck {
	public:
		string curr_token;
		string curr_line;
		string classname;
		fstream inputFile;
		fstream outFile;
		fstream errFile;
		fstream vmFile;
		stack < string > opstack;
		class_symbol_table * CT;
		subroutine_symbol_table * ST;
		pss p;

		void clean() {
			inputFile.close();
			outFile.close();
			errFile.close();

		}
		pair < string, string > extracttokens() {
			getline(inputFile, curr_line);
			// cout<<curr_line<<endl;
			string token, tokentype;
			int i = 0;
			while (i < curr_line.size()) {
				if (curr_line[i] == ' ') {
					i++;
				} else {
					if (curr_line[i] == '<' && curr_line[i + 1] == '<') {
						token.append(1, curr_line[i]);
						break;
					}
					if (curr_line[i] == '<') {
						i++;
						while (curr_line[i] != '>') {
							tokentype.append(1, curr_line[i]);
							i++;
						}
						i++;
					} else {
						token.append(1, curr_line[i]);
						i++;
						while (curr_line[i] != '<') {
							token.append(1, curr_line[i]);
							i++;
						}
						break;
					}
				}
			}
			// cout<<tokentype<<endl;
			return make_pair(tokentype, token);
		}

		/**
		 * @brief takes in the symbol
		 * extracts one token and returns true if the token is a symbol
		 * if(flag == true){then it wont extract an extra token}
		 * if(errflag == true){ then it wont print the error in errfile}
		 * if(outflag == true){then it will print curr_line in outFile}
		 * @param symbol 
		 */
		bool checkSymbol(string symbol, bool flag = false, bool errflag = false, bool outflag = false) {
			if (!flag) p = extracttokens();
			if (p.first == "symbol") {
				if (p.second == symbol) {
					if (outflag) outFile << curr_line << endl;
					return true;
				}
				if (!errflag) errFile << "ERROR: " << symbol << endl;
				return false;
			}
			if (!errflag) errFile << "ERROR: Expecting <symbol> but " << symbol << endl;
			return false;
		}

		bool checkToken(string tokentype, string token, bool errflag) {
			p = extracttokens();
			// cout<<curr_line<<endl;
			if (p.first == tokentype) {
				if (p.second == token) {
					return true;
				}
				if (!errflag) errFile << "ERROR: " << token << endl;
				return false;
			}
			if (!errflag) errFile << "ERROR: Expecting <" << tokentype << "> but " << token << endl;
			return false;
		}
		/**
		 * @brief checks symbol
		 * extracts first token itself
		 * @param symbol 
		 * @return true when it is a symbol 
		 * @return false when it is not a symbol
		 */
		void compileClass() {
			p = extracttokens();
			if (p.first == "identifier") {
				classname = p.second;
				outFile << curr_line << endl;
			} else {
				(errFile) << "ERROR: <identifier> but " << p.second << endl;
			}
			if (checkSymbol("{", false, false, true)) {
				CT = new class_symbol_table(0, 0, 0, 0);
				p = extracttokens();
				if ((p.first == "keyword") && ((p.second == "static") || (p.second == "field"))) {
					outFile << "<classVarDec>" << endl;
					outFile << curr_line << endl;
					if (!compileClassVarDec()) {
						return;
					}
					// outFile<<"</classVarDec>"<<endl;
				}
				//cout<<curr_line<<endl;
				if ((p.first == "keyword") && ((p.second == "constructor") || (p.second == "function") || (p.second == "method"))) {
					outFile << "<subroutineDec>" << endl;
					outFile << curr_line << endl;
					if (!compileSubroutineDec()) {
						return;
					};
					// outFile<<"</subroutineDec>"<<endl;
				}
				if (checkSymbol("}", true, false)) {
					outFile << curr_line << endl;
				}
			}
		}

		/**
		 * @brief first token is already checked
		 * it extracts one extra token to check for more compile var dec
		 */
		bool compileClassVarDec() {
			EntryType varkind;
			string varname;
			string vartype;
			if (p.second == "field") {
				varkind = Field;
			} else {
				varkind = Static;
			}
			p = extracttokens();
			if ((p.first == "keyword" && (p.second == "int" || p.second == "boolean" || p.second == "char" || p.second == classname))) {
				outFile << curr_line << endl;
				vartype = p.second;
			} else {
				if ((p.first == "identifier")) {
					outFile << curr_line << endl;
					vartype = p.second;
				} else {
					errFile << "ERROR: " << p.second << endl;
				}
			}
			p = extracttokens();
			if (p.first == "identifier") {
				outFile << curr_line << endl;
				varname = p.second;
				if (!(CT->addEntry(varname, varkind, vartype))) {
					errFile << "Redeclaration error: " << varname << " already declared" << endl;
					return false;
				}
				p = extracttokens();
				while (checkSymbol(",", true, true, true)) {
					p = extracttokens();
					if (p.first != "identifier") {
						(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
						return false;
					}
					varname = p.second;
					if (!(CT->addEntry(varname, varkind, vartype))) {
						errFile << "Redeclaration error: " << varname << " already declared" << endl;
						return false;
					}
					outFile << curr_line << endl;
					p = extracttokens();
				}
				if (!checkSymbol(";", true, false, true)) {
					return false;
				}
			} else {
				(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
				return false;
			}
			outFile << "</classVarDec>" << endl;
			p = extracttokens();
			if ((p.first == "keyword") && ((p.second == "static") || (p.second == "field"))) {
				outFile << "<classVarDec>" << endl;
				outFile << curr_line << endl;
				if (!compileClassVarDec()) {
					return false;
				}
			}
			return true;
		}

		/**
		 * @brief type of method should be checked before hand
		 * it will extract a new line after its declaration
		 * stored symbol will be in p
		 */
		bool compileSubroutineDec() {

			SubroutineType subkind;
			if (p.second == "constructor") {
				subkind = Constructor;
			} else if (p.second == "method") {
				subkind = Method;
			} else if (p.second == "function") {
				subkind = Function;
			}
			p = extracttokens();
			if ((p.first == "keyword" && (p.second == "void" || p.second == "int" || p.second == "boolean" || p.second == "char" || p.second == classname)) || (p.first == "identifier")) {
				outFile << curr_line << endl;
				p = extracttokens();
				if (p.first == "identifier") {
					string subname = p.second;
					outFile << curr_line << endl;
					ST = new subroutine_symbol_table(subname, subkind, 0, 0, 0);
					if (subkind == Method) {
						ST->addEntry("this", Argument, classname);
					}
					if (checkSymbol("(")) {
						outFile << curr_line << endl;
						if (!compileParameterList()) {
							return false;
						}
						if (checkSymbol(")", true)) {
							outFile << curr_line << endl;
							if (!compileSubroutineBody()) {
								return false;
							}
						} else {
							// (errFile)<<"ERROR: ')' expected"<<endl;
							return 0;
						}
					} else {
						// (errFile)<<"ERROR: '(' expected"<<endl;
						return 0;
					}
				} else {
					(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
					return 0;
				}
			} else {
				(errFile) << "ERROR: Expecting <keyword> but " << p.second << endl;
				return 0;
			}
			outFile << "</subroutineDec>" << endl;
			if ((p.first == "keyword") && ((p.second == "constructor") || (p.second == "function") || (p.second == "method"))) {
				outFile << "<subroutineDec>" << endl;
				outFile << curr_line << endl;
				if (!compileSubroutineDec()) {
					return false;
				}
			}
			return 1;
		}

		/**
		 * @brief Checks for parameter list
		 * it extracts its first token itself
		 * it also extracts one last token to check for more params
		 */
		bool compileParameterList() {
			outFile << "<parameterList>" << endl;
			p = extracttokens();

			if ((p.first == "keyword" && (p.second == "int" || p.second == "boolean" || p.second == "char" || p.second == classname)) || (p.first == "identifier")) {
				EntryType varkind = Argument;
				string vartype = p.second;
				outFile << curr_line << endl;
				p = extracttokens();
				if (p.first == "identifier") {
					string varname = p.second;
					if (!(ST->addEntry(varname, varkind, vartype))) {
						errFile << "Redeclaration error: " << varname << " already declared" << endl;
						return false;
					}
					outFile << curr_line << endl;
					p = extracttokens();
					while (checkSymbol(",", true, true, true)) {
						p = extracttokens();
						if (!(p.first == "keyword" && (p.second == "int" || p.second == "boolean" || p.second == "char" || p.second == classname)) || (p.first == "identifier")) {
							errFile << "ERROR: ," << endl;
							return false;
						}
						while ((p.first == "keyword" && (p.second == "int" || p.second == "boolean" || p.second == "char" || p.second == classname)) || (p.first == "identifier")) {
							vartype = p.second;
							outFile << curr_line << endl;
							p = extracttokens();
							if (p.first == "identifier") {
								varname = p.second;
								if (!(ST->addEntry(varname, varkind, vartype))) {
									errFile << "Redeclaration error: " << varname << " already declared" << endl;
									return false;
								}
								outFile << curr_line << endl;
								p = extracttokens();
							} else {
								(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
								return 0;
							}
						}
					}
				} else {
					(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
					return 0;
				}
			}
			outFile << "</parameterList>" << endl;
			return 1;
		}

		/**
		 * @brief it extracts it first line itself
		 * and it extracts any extra token
		 */
		bool compileSubroutineBody() {
			outFile << "<subroutineBody>" << endl;
			if (checkSymbol("{")) {
				outFile << curr_line << endl;
				p = extracttokens();

				if (p.first == "keyword" && p.second == "var") {
					outFile << "<varDec>" << endl;
					outFile << curr_line << endl;
					if (!compileVarDec()) {
						return false;
					}
				}
				vmFile << "function " << classname << "." << ST->currSubName << " " << ST->Lcl_count << endl;
				// outFile<<curr_line<<endl;
				if (ST->currSubType == Constructor) {
					vmFile << "push constant " << CT->Field_count << endl;
					vmFile << "call Memory.alloc 1" << endl;
					vmFile << "pop pointer 0" << endl;
				} else if (ST->currSubType == Method) {
					vmFile << "push argument 0" << endl;
					vmFile << "pop pointer 0" << endl;
				}
				if (!compileStatements()) {
					return false;
				}
				if (!checkSymbol("}", true, false, true)) {
					return 0;
				}

			} else {
				return 0;
			}
			outFile << "</subroutineBody>" << endl;
			p = extracttokens();
			return 1;
		}

		/**
		 * @brief first token needs to be extracted
		 * it extracts one extra token to check for more var decs
		 * it repeats itself for multiple vardecs
		 */
		bool compileVarDec() {
			EntryType varkind = Local;
			p = extracttokens();
			if (p.first == "keyword" && (p.second == "int" || p.second == "boolean" || p.second == "char" || p.second == classname)) {
				outFile << curr_line << endl;
			} else if (p.first == "identifier") {
				outFile << curr_line << endl;
			} else {
				(errFile) << "ERROR: Expecting <keyword> but " << p.second << endl;
				return 0;
			}
			string vartype = p.second;
			p = extracttokens();
			string varname;
			if (p.first == "identifier") {
				varname = p.second;
				outFile << curr_line << endl;
			} else {
				(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
				return 0;
			}
			if (!(ST->addEntry(varname, varkind, vartype))) {
				errFile << "Redeclaration error: " << varname << " already declared" << endl;
				return false;
			}
			while (checkSymbol(",", false, true)) {
				outFile << curr_line << endl;
				p = extracttokens();
				if (p.first == "identifier") {
					outFile << curr_line << endl;
				} else {
					errFile << "ERROR: extra ',' given" << endl;
					return 0;
				}
				varname = p.second;
				if (!(ST->addEntry(varname, varkind, vartype))) {
					errFile << "Redeclaration error: " << varname << " already declared" << endl;
					return false;
				}
			}
			// 
			if (!checkSymbol(";", true, false, true)) {
				// outFile<<curr_line<<endl;
				return 0;
			}
			outFile << "</varDec>" << endl;
			p = extracttokens();
			// cout<<curr_line<<endl;
			if (p.first == "keyword" && p.second == "var") {
				outFile << "<varDec>" << endl;
				outFile << curr_line << endl;
				if (!compileVarDec()) {
					return false;
				}
				// outFile<<"</varDec>"<<endl;
			}
			return 1;
		}

		/**
		 * @brief its first token should be extracted before hand
		 * it will extract an extra token for checking multiple statements
		 */
		bool compileStatements() {
			outFile << "<statements>" << endl;
			while (p.first == "keyword") {
				// cout<<curr_line<<endl;
				if (p.second == "let") {
					outFile << "<letStatement>" << endl;
					outFile << curr_line << endl;
					if (!compileLetStatement()) {
						return false;
					}
					outFile << "</letStatement>" << endl;
					p = extracttokens();
				} else if (p.second == "if") {
					outFile << "<ifStatement>" << endl;
					outFile << curr_line << endl;
					if (!compileIfStatement()) {
						return false;
					}
					outFile << "</ifStatement>" << endl;
				} else if (p.second == "while") {
					outFile << "<whileStatement>" << endl;
					outFile << curr_line << endl;
					if (!compileWhileStatement()) {
						return false;
					}
					outFile << "</whileStatement>" << endl;
					p = extracttokens();
				} else if (p.second == "do") {
					outFile << "<doStatement>" << endl;
					outFile << curr_line << endl;
					if (!compileDoStatement()) {
						return false;
					}
					outFile << "</doStatement>" << endl;
					p = extracttokens();
				} else if (p.second == "return") {
					outFile << "<returnStatement>" << endl;
					outFile << curr_line << endl;
					if (!compileReturnStatement()) {
						return false;
					}
					outFile << "</returnStatement>" << endl;
					p = extracttokens();
				} else {
					errFile << "ERROR: " << p.second << endl;
					return false;
				}

			}
			outFile << "</statements>" << endl;
			return true;
		}

		/**
		 * @brief it needs its first token to be extracted
		 * @return true if there is an op 
		 * @return false 
		 */
		bool compileOp() {
			// cout<<curr_line<<endl;
			// cout<<p.second<<endl;
			// cout<<(p.first == "symbol")<<endl;
			if (p.first != "symbol") {
				return false;
			} else {
				if (p.second == "+") {
					outFile << curr_line << endl;
					opstack.push("add");
				} else if (p.second == "-") {
					outFile << curr_line << endl;
					opstack.push("sub");
				} else if (p.second == "&") {
					outFile << curr_line << endl;
					opstack.push("and");
				} else if (p.second == "|") {
					outFile << curr_line << endl;
					opstack.push("or");
				} else if (p.second == ">") {
					outFile << curr_line << endl;
					opstack.push("gt");
				} else if (p.second == "<") {
					outFile << curr_line << endl;
					opstack.push("lt");
				} else if (p.second == "=") {
					outFile << curr_line << endl;
					opstack.push("eq");
				} else if (p.second == "*") {
					outFile << curr_line << endl;
					opstack.push("call Math.multiply 2");
				} else if (p.second == "/") {
					outFile << curr_line << endl;
					opstack.push("call Math.divide 2");
				} else {
					//   errFile<<"ERROR: incorrect operator given"<<endl;
					return false;
				}
				return true;
			}
		}
		/**
		 * @brief it extracts its first token itself
		 * it returns -1 if there is an error
		 * it returns 0 if no terms are found
		 * TODO : how to take care of one.two.three
		 * @return int 
		 */
		int compileTerm() {
			string op1;
			p = extracttokens();
			// cout<<p.first<<endl;`
			if (p.first == "symbol" && !(p.second == "-" || p.second == "~" || p.second == "(")) {
				return 0;
			}
			if (checkSymbol("-", true, true, false)) {
				outFile << "<term>" << endl;
				outFile << curr_line << endl;
				op1 = "neg";
				int check = compileTerm();
				//add the code generation one here
				if (check > 0) {
					outFile << "</term>" << endl;
					vmFile << op1 << endl;
					return check;
				} else {
					if (check == 0) {
						errFile << "ERROR: Term expected" << endl;
					}
					return -1;
				}
			} else if (checkSymbol("~", true, true, false)) {
				outFile << "<term>" << endl;
				outFile << curr_line << endl;
				op1 = "not";
				int check = compileTerm();
				//add the code generation one here
				if (check > 0) {
					outFile << "</term>" << endl;
					vmFile << op1 << endl;
					return check;
				} else {
					if (check == 0) {
						errFile << "ERROR: No term provided" << endl;
						return -1;
					}
					return -1;
				}
			} else if (p.first == "integerConstant") {
				outFile << "<term>" << endl;
				outFile << curr_line << endl;
				vmFile << "push constant " << p.second << endl;
				outFile << "</term>" << endl;
				p = extracttokens();
				return 1;

			} else if (p.first == "keywordConstant") {
				outFile << "<term>" << endl;
				outFile << curr_line << endl;
				if (p.second == "true") {
					vmFile << "push constant 0" << endl;
					vmFile << "not" << endl;
				} else if (p.second == "false") {
					vmFile << "push constant 0" << endl;
				} else if (p.second == "null") {
					vmFile << "push constant 0" << endl;
				} else if (p.second == "this") {
					vmFile << "push pointer 0" << endl;
				} else {
					errFile << "ERROR: " << p.second << endl;
					return -1;
				}
				outFile << "</term>" << endl;
				p = extracttokens();
				return 1;
			} else if (p.first == "stringConstant") {
				outFile << "<term>" << endl;
				outFile << curr_line << endl;
				vmFile << "push constant " << p.second.size() << endl;
				vmFile << "call String.new 1" << endl;
				for (int i = 0; i < p.second.size(); i++) {
					vmFile << "push constant " << int(p.second[i]) << endl;
					vmFile << "call String.appendChar 2" << endl;
				}
				outFile << "</term>" << endl;
				p = extracttokens();
				return 1;
			} else if (p.first == "identifier") {
				outFile << "<term>" << endl;
				outFile << curr_line << endl;
				string id1 = p.second;
				int f = 1;
				symbol_table_entry
					var = ST->find(p.second);
				// cout<<var.Name<<' '<<kindmap[var.Kind]<<' '<<var.Type<<' '<<var.Index<<endl;
				if (var.Index == -1) {
					var = CT->find(p.second);
					// cout<<var.Name<<endl;
					if (var.Index == -1) {
						// cout<<id1<<endl;
						f = 2;
						// if(!(p.second == "Array" || p.second == "Output" || p.second == "Keyboard" || p.second == "Screen" || p.second == "Memory" || p.second == "Sys"))
						// {
						// 	errFile<<"Declaration error: "<<p.second<<" undeclared"<<endl;
						// 	return false;
						// }
					}
				}
				p = extracttokens();
				// cout<<p.second<<endl;
				if (p.first == "symbol") {
					if (p.second == "[") {
						outFile << curr_line << endl;
						int check = compileExpression();
						if (check == 0) {
							errFile << "ERROR: Expression expected" << endl;
							return -1;
						}
						if (check == -1) {
							return -1;
						}
						vmFile << "push " << kindmap[var.Kind] << " " <<
							var.Index << endl;
						vmFile << "add" << endl;
						vmFile << "pop pointer 1" << endl;
						vmFile << "push that 0" << endl;
						if (!checkSymbol("]", true, false, true)) {
							return -1;
						} else {
							p = extracttokens();
							outFile << "</term>" << endl;
							return 1;
						}
					} else if ((p.second == "(")) {
						vmFile << "push pointer 0" << endl;
						outFile << curr_line << endl;
						int nP = compileExpressionList();
						if (nP == -1) {
							return -1;
						}
						if (!checkSymbol(")", true, false, true)) {
							return -1;
						} else {
							if (!checkSymbol(";", false, false, true)) {
								return -1;
							}
							vmFile << "call " << classname << "." << id1 << " " << (nP + 1) << endl;
							// vmFile<<"pop temp 0"<<endl;
							p = extracttokens();
							outFile << "</term>" << endl;
							return 1;
						}
					} else if (p.second == ".") {
						outFile << curr_line << endl;
						string id2;
						p = extracttokens();
						if (p.first == "identifier") {
							id2 = p.second;
							if (f == 1) {
								vmFile << "push " << kindmap[var.Kind] << " " <<
									var.Index << endl;
							}
							outFile << curr_line << endl;
							if (checkSymbol("(", false, false, true)) {
								int nP = compileExpressionList();
								if ((nP == -1)) {
									return -1;
								}
								if (!checkSymbol(")", true, false, true)) {
									return -1;
								}
								if (f == 1) {
									vmFile << "call " <<
										var.Type << "." << id2 << " " << (nP + 1) << endl;
								} else {
									vmFile << "call " << id1 << "." << id2 << " " << (nP) << endl;
									// vmFile<<"pop temp 0"<<endl;
								}
								p = extracttokens();
							}
							outFile << "</term>" << endl;
							return 1;
						} else {
							(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
							return -1;
						}
					} else {
						vmFile << "push " << kindmap[var.Kind] << " " <<
							var.Index << endl;
						outFile << "</term>" << endl;
						return 1;
					}

				} else {
					vmFile << "push " << kindmap[var.Kind] << " " <<
						var.Index << endl;
					outFile << "</term>" << endl;
					return 1;
				}
			} else if (checkSymbol("(", true, true, false)) {
				outFile << "<term>" << endl;
				outFile << curr_line << endl;
				int check = compileExpression();
				if (check == 0) {
					errFile << "ERROR: Expression expected" << endl;
					return -1;
				}
				if (check == -1) {
					return -1;
				}
				if (checkSymbol(")", true, false, true)) {
					p = extracttokens();
					outFile << "</term>" << endl;
					return 1;
				} else {
					return -1;
				}
			} else {
				// errFile<<"ERROR: invalid term provided"<<endl;
				return 0;
			}
		}
		/**
		 * @brief it should extract its first token itself 
		 * it extracts an extra token too
		 * it should return 0 even if there is no expression given
		 * it should return -1 if there is an error
		 * @return int 
		 */
		int compileExpression() {
			outFile << "<expression>" << endl;
			int check = compileTerm();
			// cout<<check<<endl;
			if (check > 0) {
				//   string one;
				while (compileOp()) {

					if ((check = compileTerm()) > 0) {
						outFile << "</expression>" << endl;
						// cout<<opstack.top()<<endl;
						vmFile << opstack.top() << endl;
						opstack.pop();
						return 1;
					} else {
						if (check == 0) {
							errFile << "ERROR: term expected" << endl;
							outFile << "</expression>" << endl;
						};
						return false;
					}
				}

			} else {
				if (check == -1) {
					return false;
				}
			}
			outFile << "</expression>" << endl;
			return check;
		}

		/**
		 * @brief it extracts its first token itself
		 * it extracts a extra last token
		 * @return int
		 * if(int == -1){implies there is an error} 
		 */
		int compileExpressionList() {
			outFile << "<expressionList>" << endl;
			int nP = 0;
			int check = compileExpression();

			if ((check) > 0) {
				nP++;
				while (checkSymbol(",", true, true, true)) {
					int check;
					if ((check = compileExpression()) > 0) {
						nP++;
					} else {
						if (check == 0) {
							errFile << "ERROR: Expression expected" << endl;
							return -1;
						} else {
							return -1;
						}
					}
				}
				outFile << "</expressionList>" << endl;
				// cout<<check<<" "<<nP<<endl;
				return nP;
			} else {
				if (check == -1) return -1;
				outFile << "</expressionList>" << endl;
				return 0;
			}
		}
		/**
		 * @brief type = "let" should be extracted before hand
		 * 
		 * @return true 
		 * @return false 
		 */
		bool compileLetStatement() {
			bool brackflag = false;
			p = extracttokens();
			if (p.first == "identifier") {
				string varname = p.second;
				symbol_table_entry
					var = ST->find(varname);
				if (var.Index == -1) {
					var = CT->find(varname);
					if (var.Index == -1) {
						if (!(p.second == "Array" || p.second == "Output" || p.second == "Keyboard" || p.second == "Screen" || p.second == "Memory" || p.second == "Sys")) {
							errFile << "Declaration error: " << p.second << " undeclared" << endl;
							return false;
						}
					}
				}

				outFile << curr_line << endl;
				p = extracttokens();
				if (checkSymbol("[", true, true)) {
					brackflag = true;
					outFile << curr_line << endl;
					int check;
					if ((check = compileExpression()) <= 0) {
						if (check == -1) {
							return false;
						} else {
							errFile << "ERROR: Expression expected" << endl;
							return false;
						}
					}
					if (checkSymbol("]", true)) {
						outFile << curr_line << endl;
						vmFile << "push " << kindmap[var.Kind] << " " <<
							var.Index << endl;
						vmFile << "add" << endl;
						p = extracttokens();
						goto label1;
					}
				}
label1:
				if (checkSymbol("=", true)) {
					outFile << curr_line << endl;
					int check;
					if ((check = compileExpression()) > 0) {
						if (brackflag) {
							vmFile << "pop temp 0" << endl;
							vmFile << "pop pointer 1" << endl;
							vmFile << "push temp 0" << endl;
							vmFile << "pop that 0" << endl;
						} else {
							vmFile << "pop " << kindmap[var.Kind] << " " <<
								var.Index << endl;
						}
						if (checkSymbol(";", true)) {
							outFile << curr_line << endl;
						} else {
							return false;
						}
					} else {
						if (check == -1) {
							return false;
						} else {
							errFile << "ERROR: Expression expected" << endl;
							return false;
						}
					}
				}
				else {
					return false;
				}
			} else {
				(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
				return 0;
			}
			return 1;
		}

		/**
		 * @brief In all statements , type of statement token is already extracted 
		 * it extracts one extra token to check else
		 */
		bool compileIfStatement() {
			if (checkSymbol("(", false, false, true)) {
				int check;
				int Tlabelnum = CT->labelnum;
				CT->labelnum += 2;
				if ((check = compileExpression()) > 0) {
					if (checkSymbol(")", true, false, true)) {
						if (checkSymbol("{", false, false, true)) {
							vmFile << "not" << endl;
							vmFile << "if-goto " << classname << "." << Tlabelnum << endl;
							p = extracttokens();
							if (!compileStatements()) {
								return false;
							}
							if (checkSymbol("}", true, false, true)) {
								vmFile << "goto " << classname << '.' << (Tlabelnum + 1) << endl;
								vmFile << "label " << classname << "." << Tlabelnum << endl;
								p = extracttokens();
								if (p.first == "keyword" && p.second == "else") {
									outFile << curr_line << endl;
									if (checkSymbol("{", false, false, true)) {
										p = extracttokens();
										if (!compileStatements()) {
											return false;
										};
										if (checkSymbol("}", true, false, true)) {
											// cout<<"HELLO"<<endl;
											vmFile << "label " << classname << "." << (Tlabelnum + 1) << endl;
											p = extracttokens();
											return true;
										}
										// else{
										// cout<<"ELO"<<endl;
										// }
									}
								} else {
									// cout<<p.second<<endl;
									vmFile << "label " << classname << "." << (Tlabelnum + 1) << endl;
									return true;
								}
							}
							// else{
							// cout<<"hello"<<endl;
							// }
						}
					}
				} else {
					if (check == -1) {
						return false;
					} else {
						errFile << "ERROR: Expression expected" << endl;
						return false;
					}
				}
			}
			return false;
		}

		//TODO : here while(1){} is not taken care of.. check it
		bool compileWhileStatement() {
			if (checkSymbol("(", false, false, true)) {
				int Tlabelnum = CT->labelnum;
				CT->labelnum += 2;
				vmFile << "label " << classname << "." << Tlabelnum << endl;
				int check;
				if ((check = compileExpression()) > 0) {
					if (checkSymbol(")", true, false, true)) {
						vmFile << "not" << endl;
						vmFile << "if-goto " << classname << "." << (Tlabelnum + 1) << endl;
						if (checkSymbol("{", false, false, true)) {
							p = extracttokens();
							if (!compileStatements()) {
								return false;
							}
							if (checkSymbol("}", true, false, true)) {
								vmFile << "goto " << classname << "." << Tlabelnum << endl;
								vmFile << "label " << classname << "." << (Tlabelnum + 1) << endl;
								return true;
							}
						}
					}
				} else {
					if (check == -1) {
						return false;
					} else {
						errFile << "ERROR: Expression expected" << endl;
						return false;
					}
				}
			}
			return false;
		}

		// TODO : check for id1.id2 type statements;
		bool compileDoStatement() {
			p = extracttokens();
			string id1;
			if (p.first == "identifier") {
				id1 = p.second;
				outFile << curr_line << endl;
				p = extracttokens();
				if (p.first == "symbol" && p.second == ".") {
					string id2;
					outFile << curr_line << endl;
					p = extracttokens();
					if (p.first == "identifier") {
						id2 = p.second;
						outFile << curr_line << endl;
						int f = 1;
						symbol_table_entry
							var = ST->find(id1);
						if (var.Index == -1) {
							var = CT->find(id1);
							if (var.Index == -1) {
								f = 2;
							}
						}
						if (f == 1) {
							vmFile << "push " << kindmap[var.Kind] << " " <<
								var.Index << endl;
						}
						if (checkSymbol("(", false, false, true)) {
							int nP = compileExpressionList();
							if ((nP == -1)) {
								return 0;
							}

							if (!checkSymbol(")", true, false, true)) {
								return 0;
							}
							if (!checkSymbol(";", false, false, true)) {
								return 0;
							}
							if (f == 1) {
								vmFile << "call " <<
									var.Type << "." << id2 << " " << (nP + 1) << endl;
								vmFile << "pop temp 0" << endl;
							} else {
								vmFile << "call " << id1 << "." << id2 << " " << (nP) << endl;
								vmFile << "pop temp 0" << endl;
							}
							// p = extracttokens();
						}
						return 1;
					} else {
						(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
						return 0;
					}
				} else {
					vmFile << "push pointer 0" << endl;
					if (checkSymbol("(", true, false, true)) {
						int nP = compileExpressionList();
						if (nP == -1) {
							return false;
						}
						if (checkSymbol(")", true, false, true)) {
							if (checkSymbol(";", false, false, true)) {
								// p = extracttokens();
								vmFile << "call " << classname << "." << id1 << " " << (nP + 1) << endl;
								vmFile << "pop temp 0" << endl;
								return true;
							}
						}
					}
				}
			} else {
				(errFile) << "ERROR: Expecting <identifier> but " << p.second << endl;
			}
			return false;
		}

		//TODO: check the case if expression is not there
		bool compileReturnStatement() {
			int check = compileExpression();
			// cout<<check<<endl;
			if (check > 0) {
				vmFile << "return" << endl;
			} else if (check == 0) {
				vmFile << "push constant 0" << endl;
				vmFile << "return" << endl;
			} else {
				return false;
			}
			if (checkSymbol(";", true, false, true)) {
				return true;
			}
			return true;
		}

	public:

		void StartProcessing(string inp, string out, string err, string vm) {
			// cout<<inp<<' '<<out<<' '<<err<<endl;
			inputFile.open(inp, fstream:: in );
			outFile.open(out, fstream::out);
			errFile.open(err, fstream::out);
			vmFile.open(vm, fstream::out);
			// p = extracttokens();
			if (!checkToken("keyword", "class", true)) {
				// cout<<curr_line<<endl;
				// (errFile)<<"ERROR: Every File should begin with a class"<<endl;
			} else {
				outFile << "<class>" << endl;
				outFile << curr_line << endl;
				compileClass();
				outFile << "</class>" << endl;
			}
			inputFile.close();
			outFile.close();
			errFile.close();
			vmFile.close();
		}
};

string extractname(string filename) {
	string holder;
	for (int i = 0; i < filename.size(); i++) {
		if (filename[i] == '.') {
			break;
		}
		holder.append(1, filename[i]);
	}
	return holder;
}

string appender(string filename, string exten) {
	string holder;
	holder.append(filename);
	holder.append(exten);
	return holder;
}
int main(int argc, char * argv[]) {

	for (int i = 0; i < argc - 2; i++) {
		string filen = extractname(argv[i + 2]);
		string txml = appender(filen, "T.xml");
		string xml = appender(filen, ".xml");
		string vmf = appender(filen, ".vm");
		string errf = appender(filen, ".err");
		string inf = argv[i + 2];

		//initiates a parser class object
		Parser P;
		P.Initialize();

		fstream inputFile;
		fstream outFile;
		inputFile.open(inf, ios:: in );
		outFile.open(txml, ios::out);

		P.ProcessInputFile(inputFile, outFile);
		inputFile.close();
		outFile.close();

		kindmap[Field] = "this";
		kindmap[Static] = "static";
		kindmap[Local] = "local";
		kindmap[Argument] = "argument";
		kindmap[NONETYPE] = "null";

		SyntaxCheck S;
		S.StartProcessing(txml, xml, errf, vmf);

	}

	return 0;
}
