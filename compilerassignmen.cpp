//============================================================================
// Name        : compilerassignmen.cpp
// Author      :
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <exception>
using namespace std;

class TokenLexeme{

public:
	string token;
	string lexem;
	int line_number;

};
bool isoperator(char ch){
	return (ch =='<'|| ch =='>'|| ch =='='|| ch ==';'|| ch ==','||  ch =='+'
	|| ch =='-'|| ch =='*'|| ch =='/'|| ch =='!'|| ch ==':');
}


bool ismisc(char ch){
	return (ch==' ' ||ch ==';'|| ch ==','|| ch =='{'|| ch =='}'|| ch =='('|| ch ==')'|| ch =='#');
}

void token_lexeme_generator(string* arr,int arr_size){
	ofstream file;
	file.open("words.txt");
	int symbol_table_start = 128;
	ofstream file1;
	file1.open("symbolTable.txt");
	string symbolTable[500];
	int unique_variable_count=0;
	for (int i=0;i<arr_size;i++){
		if(arr[i]=="int" ||arr[i]=="char"||arr[i]=="if"||arr[i]=="while"||arr[i]=="else"||arr[i]=="ret"||arr[i]=="in"||arr[i]=="out"){//checking keywords
			for(int j=0;arr[i][j]!='\0';j++)
					arr[i][j]=arr[i][j]-32;
			file<<"("<<arr[i]<<",^)\n";
		}
		else if(arr[i]=="<"||arr[i]==">"||arr[i]==":"||arr[i]=="+"||arr[i]=="-"||arr[i]=="*"||arr[i]=="/"||arr[i]=="="||arr[i]=="!"){//checking operators
			if(arr[i]=="<"){
				if(arr[i+1]=="="){//checking combined operators like <= , >=, ==
					file<<"(RO,LE)\n";
					i++;
				}
				else{
					file<<"(RO,LT)\n";
				}
			}
			else if(arr[i]==">"){
				if(arr[i+1]=="="){
					file<<"(RO,GE)\n";
					i++;
				}
				else
					file<<"(RO,GT)\n";
			}
			else if(arr[i]==":"){
				if(arr[i+1]=="="){
					file<<"(RO,ASSIGN)\n";
					i++;
				}
				else{// checking for single :
					cout<<"error for : there must be a = sign following this\n";
					exit(1);
				}
			}
			else if(arr[i]=="="){
				if(arr[i+1]=="="){
					file<<"(RO,ET)\n";
					i++;
				}
				else{//checking for single =
					cout<<"error  operator is not found";
					exit(1);
				}
			}
			else if(arr[i]=="!"){
				if(arr[i+1]=="="){
							file<<"(RO,NE)\n";
							i++;
					}
				else{//checking for single !
					cout<<"error  operator is not found";
					exit(1);
				}}}
				else{
					if(arr[i][0]=='\"'){//checking for strings
						file<<"(STR,"<<arr[i]<<")\n";
					}
					else if(arr[i][0]>='0'&&arr[i][0]<='9')//checking for numbers
						file<<"(NUM,"<<arr[i]<<")\n";
					else if((arr[i][0]>='a'&&arr[i][0]<='z')||(arr[i][0]>='A'&&arr[i][0]<='Z')){//checking for variables
						file<<"(ID,\""<<arr[i]<<"\")\n";
						bool unique_flag=false; //checks whethers a variable already exists in the symbol table or not
						for (int zz=0;zz<unique_variable_count;zz++){
							if(symbolTable[zz]==arr[i])
								unique_flag=true;

						}
						if (!unique_flag){
							symbolTable[unique_variable_count++]=arr[i];
							file1<<arr[i]<<"   "<<symbol_table_start++<<"\n";
							unique_flag=false;
						}
					}
					else{
						file<<"("<<arr[i]<<",^)\n";

					}
				}


	}
}

string* parseFileLines(char *filename){
	ifstream file;
		int null_count=0;
	const int arr_size = 5000;
	file.open(filename);
	string * line_arr;
	int comment_flag=0;
	string * token_arr = new string [10000];
	if (file){
		line_arr = new string [arr_size];
		int i=0;
		while(getline(file,line_arr[i++]));
		file.close();
		int last_character=0;
		string temp = "";
		int state=0;


		int token_count=0;
		for(int j=0;j<i;j++){

			for(int k=0;line_arr[j][k]!='\0';k++){

				if(line_arr[j][k]>='0' && line_arr[j][k]<='9' && last_character!=2){//dealing with number

					if (last_character==1){// 1=numbers 2=alphabets 3=space 4=everything else other than alphanumeric values
						temp+=line_arr[j][k];
					}
					else{
						if (temp!="")
							{
								token_arr[token_count++]=temp;
								temp = "";
							}

						last_character=1;

						temp+=line_arr[j][k];
					}
				}
				else if((line_arr[j][k]>='A' && line_arr[j][k]<='Z')||(line_arr[j][k]>='a' && line_arr[j][k]<='z') ||(line_arr[j][k]>='0' && line_arr[j][k]<='9') ||
				(line_arr[j][k]=='_')){ //dealing with variables or aplhanumeric stuff

					if (last_character==2){

						temp+=line_arr[j][k];

					}
					else{
						if (temp!="")
						{
							token_arr[token_count++]=temp;
							temp = "";
						}
						last_character=2;
						if((line_arr[j][k]>='0' && line_arr[j][k]<='9')){
							cout<<"Error on line number "<<j+1<<" \n"<<line_arr[j]<<"\nvariable can not start with a number";
							exit(1);
						}

						temp+=line_arr[j][k];
					}
				}
				else{

					if(line_arr[j][k]=='#'){
						comment_flag=1;
						if(line_arr[j][k+1]=='#')
							std::cout << "we are not handling multiline comments in this language now" << '\n';
					}
					else if(line_arr[j][k]==' ')
						{
							if(last_character!=3)
							{
								if(temp!="")
								{
									token_arr[token_count++]=temp;
									temp = "";
									last_character=3;
								}
							}

						}
						else if(line_arr[j][k]=='\"'){
							if(temp!="")
								token_arr[token_count++]=temp;
								temp="";
							k++;
							temp+='\"';
							while(line_arr[j][k]!='\"'){
								if(line_arr[j][k]=='\0'){
									std::cout << "String is not closed inverted commas must be in pairs" << '\n';
									exit(1);
								}
								temp+=line_arr[j][k];
								k++;
							}

						}
						else{//handling operators and brackerts,delimeters,colon and stuff
							if(temp!="")
								token_arr[token_count++]=temp;
							token_arr[token_count]=line_arr[j][k];
							token_count++;
							last_character=4;
							temp="";
						}

				}
				if(comment_flag==1){
					line_arr[k+1]='\0';
				}
			}
		}

token_lexeme_generator(token_arr,token_count);
	}

	return token_arr;
}


int main(int argc, char **argv) {
string * arr_token;
	if (argc==1){
		cout<<"file name is not given";
		exit(1);
	}

	else if(argc==2){
		ifstream file;
		file.open(argv[1]);
		if (!file){
			cout<<"file does not open properly or does not exist";
			exit(1);
		}
		else{
			file.close();
arr_token = parseFileLines(argv[1]);
		}
	}

	return 0;
}
