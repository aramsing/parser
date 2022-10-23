#include <list>
#include <bits/stdc++.h>
#include <iostream>
#include <iterator>
#include <fstream>
#include <istream>
#include <malloc.h>
#include <stdio.h>
#include <string>


using namespace std;

struct ID{
  string tokenType;
  string content;
};

// Global Variables
int indent; // current indentation
int comment = 0;
list<ID> IDlist;
int stTracker = 1;
bool factor = 0;

//indents inside
void Lindt(){
  int temp = 0;
  temp = indent;
  while(temp!=0){
    cout<<"  ";
    temp--;
  }
}

// Parser Error Notifier
/*void parserError() {
  system("clear");
  cout << "Error." << endl;
  indent = -1;
} */

void printID() {
  Lindt();
  cout << "<" << IDlist.front().tokenType << ">" << endl;

  indent++;
  Lindt();
  cout << IDlist.front().content << endl;
  indent--;

  Lindt();      
  cout << "<" << "/" << IDlist.front().tokenType << ">" << endl;
}

// Character Scanner
string charScanner(int ascii){
  switch(ascii){
    case 65 ... 90:
    case 97 ... 122:
      return "id";
      break;
    case 48 ... 57:
      return "number";
      break;
    case 40:
      return "lpara";
      break;
    case 41:
      return "rpara";
      break;
    case 42:
      return "times";
      break;
    case 43:
      return "plus";
      break;
    case 45:
      return "minus";
      break;
    case 47:
      return "div";
      break;
    case 32:
      break; //space
    case 58:
      break;
    case 61:
      break;
    case 46:
      return "decimal";
      break; 
    default:
      return "error";
  }
}

// Scanner of File
string scanner(string word) { 
  int acele;
  int acele2;
  char ele;
  char ele2;
  string tokenType;
  int decimal = 0;
  for(int j = 0; j<word.length(); j++){
    ele = word.at(j);
    acele = (int)ele;
    if (word[j+1] != NULL) {
      ele2 = word[j+1];
      acele2 = (int)ele2;
    }
    else {
      
    }
    if (comment == 1) {
      string s1(1, ele);
      string s2(1, ele2);
      string type1 = s1 + s2;
      if (!type1.compare("*/")){
        comment = 0;
        j++;
        tokenType = "";
      }
    }
    else if(!word.compare("read")){
      return "read";
    }
    else if(!word.compare("write")){
      return "write";
    }
    else if(!word.compare("assign")){
      return "assign";
    }
    else if (tokenType.empty()) {
      tokenType = charScanner(acele);
      if (acele == 42) {
        return tokenType;
      }
      else {}
    }
    else if (!tokenType.compare("id")) {
      string type2 = charScanner(acele);
      if (type2.compare("id") && type2.compare("number")) {
        return "error";
      }
    }
    else if (!tokenType.compare("number")) {
      string type2 = charScanner(acele);
      if (type2.compare("number") || (!type2.compare("decimal") && (decimal == 1)) || type2.compare("decimal")) {
        return "error";
      }
      else if (!type2.compare("decimal")) {
        decimal++;
      }
    }
    else if (!tokenType.compare("lpara") || !tokenType.compare("rpara") || !tokenType.compare("minus") || !tokenType.compare("plus") || !tokenType.compare("times")) {
      if (acele != NULL) {
        return "error";
      }
      else {}
    }
    else if (!tokenType.compare("div")) {
      if (acele2 == 42) {
        comment = 1;
        tokenType = "";
      }
      else if (acele != NULL) {
        return "error";
      }
      else {}
    }
  }
  if (!tokenType.compare("div")) {
    if(comment == 1){
      return "";
    }
    else{
      return "div";
    }
  }
  else if (comment == 1) {
    return "";
  }
  else {
    return tokenType;
  }
}

// Parser through Context Free Grammar
bool parser() {//system("clear");
  int test = 0;

  // Program
  if (indent == 0) {
    cout << "<" << "Program" << ">" << endl;
    indent++;
    if (parser()) {
      test = -1;
      return 1;
    }
    indent--;
    cout << "<" << "/" << "Program" << ">" << endl;
  }
    
  // Statement list
  else if (stTracker == 1) {
    Lindt();
    cout << "<" << "stmt_list" << ">" << endl;
    if (!IDlist.empty()) {
      indent++;
      stTracker++;
      if (parser()) {
        test = -1;
        return 1;
      }
      indent--;
    }
    stTracker--;
    Lindt();
    cout << "<" << "/" << "stmt_list" << ">" << endl;
  }
    
  // Statement
  else if (stTracker == 2)  {
    Lindt();
    cout << "<" << "stmt" << ">" << endl;
    indent++;
    stTracker++;
    if (!IDlist.front().tokenType.compare("id")) { // If (id assign <expr>)
      printID();
      IDlist.pop_front();
      if (!IDlist.front().tokenType.compare("assign")) {
        printID();
        IDlist.pop_front();
        if (!IDlist.empty()) { // go to <expr>
          indent++;
          if (parser()) {
            test = -1;
            return 1;
          }
          indent--;
        }
        else {
          test = -1;
          return 1;
        }
      }
      else {
        return 1;
      }
    }
    else if (!IDlist.front().tokenType.compare("read")) { // If (read id)
      printID();
      IDlist.pop_front();
      if (!IDlist.front().tokenType.compare("id")) {
        printID();
        IDlist.pop_front();
      }
      else {
        test = -1;
        return 1;
      }
    }
    else if (!IDlist.front().tokenType.compare("write")) { // If (write <expr>) 
      printID();
      IDlist.pop_front();
      if (!IDlist.empty()) { // go to <expr>
        if (parser()) {
          test = -1;
          return 1;
        }
      }
      else {
        test = -1;
        return 1;
      }
    }
    else { // Unexpected Token. Return Error
      test = -1;
      return 1;
    }
    stTracker = 1;
    indent--;
    Lindt();
    cout << "<" << "/" << "stmt" << ">" << endl;
    if (parser()) {
      test = -1;
      return 1;
    }
  }

  // Expression
  else if (stTracker == 3) {
    Lindt();
    cout << "<" << "expr" << ">" << endl;
    if (!IDlist.empty()) { //check term
      indent++;
      stTracker = 5;
      if (parser()) {
        test = -1;
        return 1;
      }
    }
    if (parser()) {
      test = -1;
      return 1;
    }
    indent--;
    stTracker--;
    Lindt();
    cout << "<" << "/" << "expr" << ">" << endl;
  }

  // Term Tail
  else if (stTracker == 4) {
    Lindt();
    cout << "<" << "term_tail" << ">" << endl;
    if (!IDlist.empty()) { //check add_op
      indent++;
      stTracker = 8;
      if (parser()) {
        test = -1;
        return 1;
      }
      indent--;
      stTracker = 4;
      
      //check term
      indent++;
      stTracker++;
      if (parser()) {
        test = -1;
        return 1;
      }
      indent--;
    }
    stTracker--;
    Lindt();
    cout << "<" << "/" << "term_tail" << ">" << endl;
  }

  // Term
  else if (stTracker == 5) {
    Lindt();
    cout << "<" << "term" << ">" << endl;
    if (!IDlist.empty()) {
      indent++;
      stTracker = stTracker + 2;
      if (parser()) {
        test = -1;
        return 1;
      }
      indent--;
      stTracker--;
    }
    Lindt();
    cout << "<" << "/" << "term" << ">" << endl;
  }
  
  // Factor Tail
  else if (stTracker == 6) {
    Lindt();
    cout << "<" << "fact_tail" << ">" << endl;
    if (!IDlist.empty()) { //check mult_op
      indent++;
      stTracker = 9;
      if (parser()) {
        test = -1;
        return 1;
      }
      indent--;
      stTracker = 6;
      if (!IDlist.empty()) { //check factor
        indent++;
        stTracker++;
        if (parser()) {
          test = -1;
          return 1;
        }
        indent--;
      }
    }
    stTracker--;
    Lindt();
    cout << "<" << "/" << "fact_tail" << ">" << endl;
  }

  // Factor
  else if (stTracker == 7) {
    Lindt();
    cout << "<" << "factor" << ">" << endl;
    indent++;
    // case for (lparen and rparen), (id), (number) go here
    if (!IDlist.front().tokenType.compare("lpara")) { // if left parenthesis
      printID();
      IDlist.pop_front();
      factor = 1;
      stTracker = 3;
      if (!IDlist.empty()) {
        if (parser()) {
          test = -1;
          return 1;
        }
      }
      else {
        return 1;
      }
      stTracker = 7;
      factor = 0;

      if (!IDlist.front().tokenType.compare("rpara")) { // check for right parenthesis
        printID();
        IDlist.pop_front();
      }
      else {
        return 1;
      }
    }
    else if ((!IDlist.front().tokenType.compare("id")) || (!IDlist.front().tokenType.compare("number"))) { // if id or number
      printID();
      IDlist.pop_front();
    }
    indent--;
    Lindt();
    cout << "<" << "/" << "factor" << ">" << endl;
    stTracker--;
    if (!IDlist.front().tokenType.compare("rpara")) {
      return 0;
    }
    if (parser()) {
      test = -1;
      return 1;
    }
  }

  // Addition operator
  else if (stTracker == 8) {
    Lindt();
    cout << "<" << "add_op" << ">" << endl;
    indent++;
    // case for * and / go here
    if ((!IDlist.front().tokenType.compare("plus")) || (!IDlist.front().tokenType.compare("minus"))) {
      printID();
      IDlist.pop_front();
    }
    else {
      return 1;
    }
    indent--;
    Lindt();
    cout << "<" << "/" << "add_op" << ">" << endl;
  }
    
  // Multiplication operator
  else if (stTracker == 9) {
    Lindt();
    cout << "<" << "mult_op" << ">" << endl;
    indent++;
    // case for * and / go here
    if ((!IDlist.front().tokenType.compare("times")) || (!IDlist.front().tokenType.compare("div"))) {
      printID();
      IDlist.pop_front();
    }
    indent--;
    Lindt();
    cout << "<" << "/" << "mult_op" << ">" << endl;
  }
  
  else {
    return 1;
  }

  if (test == -1) { //causing to print error regardless
    return 1;
  }
  
  return 0;
}

int main(void) {
  // Read file input into queued structures
  ifstream file;
  file.open("input.txt");
  if (!file) {
    cout << "File cannot be opened." << endl;
    return 0;
  }

  // Put IDs into global list
  while (!file.eof()) { // while file not end of file
    ID currentID;
    file >> currentID.content;
    currentID.tokenType = scanner(currentID.content);
    IDlist.push_back(currentID);
    if (!currentID.tokenType.compare("error")) { // Check if there is an invalid token
      cout << "Error." << endl;
      file.close();
      return 0;
    }
  }

  // Run Parser that recursively parses through the entire file
  if (parser()) {
    //system("clear");
    cout << "Error." << endl;
  }

  file.close();
  return 0;
}