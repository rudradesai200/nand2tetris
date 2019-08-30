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
  string classifier(string S){
    // cout<<S;
    if(keymap.find(S)!=keymap.end()){
        return "keyword";
    }
    else{
      return "identifier";
    }
  }

  //checks for symbol
  bool classifier(char c){
    if(symbolmap.find(c)!=symbolmap.end()){
      return true;
    }
    else{
      return false;
    }
  }

  //tokenizes given strings and writes to the outFile
  void tokenize(string currexpr,fstream &outFile){
    //ignore the initial spaces and tabs
    // cout<<currexpr;
    int i=0;
    while(currexpr[i] == ' '){
      i++;
    }

    //checks if the char is a space if not then checks if it is a stringConstant
    //else checks for int constant and then for keyword and symbol
    while(i<currexpr.size()){
      if(currexpr[i] == ' ' || currexpr[i] =='\n' || currexpr[i] =='\t'){
        i++;
      }
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
      else if(isdigit(currexpr[i])){
        string token;
        while(isdigit(currexpr[i]) && i<currexpr.size()){
          token.append(1,currexpr[i]);
          i++;
        }
        outFile<<"<integerConstant>"<<addspace<<token<<addspace<<"</integerConstant>\n";
      }
      else if(classifier(currexpr[i])){
        if(currexpr[i] == '<'){
          outFile << "<symbol>"<<addspace<<"&lt;"<<addspace<<"</symbol>\n";
        }
        else{
        outFile << "<symbol>"<<addspace<<currexpr[i]<<addspace<<"</symbol>\n";
        }
        i++;
      }
      else{
        string token;
        while(!classifier(currexpr[i]) && currexpr[i]!=' ' && i<currexpr.size()){
          token.append(1,currexpr[i]);
          i++;
        }
        if(token != "\n"){
          string elem = classifier(token);
          outFile<<"<"<<elem<<">"<<addspace<<token<<addspace<<"</"<<elem<<">\n";
        }
      }
    }
  }

  //removes all types of comments
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

  //process inputFile and calls other functions accordingly
  void ProcessInputFile(fstream &inputFile,fstream &outFile){
    outFile<<"<tokens>\n";
    string curr_line;
    while(getline(inputFile,curr_line)){
      curr_line.erase(std::remove(curr_line.begin(), curr_line.end(), '\r'), curr_line.end());
      // cout<<curr_line;
      if(curr_line.size()>=1){
        if(RemoveComments(curr_line)){
            tokenize(currexpr,outFile);
        }
      }
      curr_line.clear();
    }
    outFile<<"</tokens>\n";
  }
};


int main(int argc, char *argv[]) {
  // string curr_line;
  fstream inputFile;
  fstream outFile;
  Parser P;

  P.Initialize();
  inputFile.open(argv[1],ios::in);
  outFile.open(argv[2],ios::out);
  
  P.ProcessInputFile(inputFile,outFile);
  inputFile.close();
  outFile.close();


  return 0;
}
