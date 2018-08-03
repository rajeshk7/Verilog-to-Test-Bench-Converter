#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

string holder,finale,outer,gateI,portI,variableI;

void converter();

ifstream infile; //Bench file
ofstream gates;
ofstream port_declaration;
ofstream variable_instantiation;
ofstream outfile;

int main()
{
    DIR *d;
    char *p1,*p2;
    int ret;
    struct dirent *dir;
    //std::string holder,finale,outer,gateI,portI,variableI;
    d=opendir(".");
    if(d)
    {
        while((dir=readdir(d))!=NULL)
        {
            p1=strtok(dir->d_name,".");
            p2=strtok(NULL,".");
            if(p2!=NULL)
            {
                ret=strcmp(p2,"bench");
                if(ret==0)
                {
                    holder = p1;
                    finale = holder + "." + p2;
                    outer = holder + ".v";
                    gateI = holder + "_Gate.txt";
                    portI = holder + "_Port.txt";
                    variableI = holder + "_Var.txt";
                    std::cout<<"Parsing the file : "<<finale<<endl;
                    outfile.open(outer.c_str());
                    gates.open(gateI.c_str());
                    port_declaration.open(portI.c_str());
                    variable_instantiation.open(variableI.c_str());
                    infile.open(finale.c_str());
                    converter();
                }
            }
        }
        closedir(d);
    }
    return 0;
}

void converter()
{

  if(!infile	)
  {
    cout << "Cannot open input file.\n";
    exit(EXIT_FAILURE);
  }

  string none(""); //Bunch of strings that are used in Verilog as syntax or keywords
  string suffix("_v"); //To avoid the problem of variables mapping to keywords after conversion we pad the converted variables with prefixes and suffixes to avoid conflicts
  string prefix("v_");
  string outputfilename("s9234");
  string str;
  string str_temp1;
  string str_temp2;
  string input_scan("INPUT");
  string input_print("input");
  string output_scan("OUTPUT");
  string output_print("output");
  string dff("DFF");
  string reg("reg");
  string wire("wire");
  string always("always");
  string posedge("posedge");
  string nonblocking_assignment("<=");
  string begin("begin");
  string end("end");
  string andgate_scan("AND");
  string andgate_print("and");
  string nandgate_scan("NAND");
  string nandgate_print("nand");
  string orgate_scan("OR");
  string orgate_print("or");
  string norgate_scan("NOR");
  string norgate_print("nor");
  string xorgate_scan("XOR");
  string xorgate_print("xor");
  string notgate_scan("NOT");
  string notgate_print("not");
  string underscore("_");
  string opening_bracket("(");
  string closing_bracket(")");
  string comma(",");
  string equal("=");
  string semicolon(";");
  string endline("\n");
  string space(" ");
  string comment("#");
  string dot(".");
  string at_the_rate("@");
  string clock("clock");

  string module("module");
  string endmodule("endmodule");

  int andgate_num = 1;//Initialization of primitive-name iterators
  int nandgate_num = 1;
  int orgate_num = 1;
  int norgate_num = 1;
  int xorgate_num = 1;
  int notgate_num = 1;

  int i = 0; //Loop variables and other control flow signals
  int j = 0;
  int k = 1;
  int i1;
  int length1; //Length of substrings inside a loop

  port_declaration << module; //file for the module part of the verilog code
  port_declaration << space; //contains module name and port declarations
  port_declaration << outputfilename;
  port_declaration << opening_bracket;
  port_declaration << clock;

  variable_instantiation << input_print << space << wire << space << clock << semicolon << endline; //file containing all the variable declarations

  while(getline(infile, str)) //line by line parsing of the file
  {
    int length = str.size();
    //cout << str << endl;
    while(str.find(dot) != string::npos) //'.' is not allowed in any verilog variable name
    {
    	i = str.find(dot);
    	str_temp1 = str.substr(0,i);
    	str_temp2 = str.substr(i+1);
    	str = str_temp1 + underscore + str_temp2;
    }

    if(str.substr(0,1).compare(comment) == 0 ) //We ignore anything that starts with #
    {

    }
    else if(str.size() == 0 ) //We ignore blank lines
    {

    }
    else if(str.substr(0,5).compare(input_scan) == 0)
    {
    	//An 'INPUT' statement contributes to port declarations and variable declarations
    	variable_instantiation << input_print;
    	variable_instantiation << space;
    	variable_instantiation << prefix << str.substr(6, length - 7) << suffix;
    	variable_instantiation << semicolon;
    	variable_instantiation << endline;
    	if(k == 1)
    	{
    		port_declaration << comma;
    		port_declaration << space;
    	}
    	port_declaration << prefix << str.substr(6, length - 7) << suffix;
    }
    else if(str.substr(0,6).compare(output_scan) == 0)
    {
    	//An 'OUTPUT' statement contributes to port declarations and variable declarations
    	variable_instantiation << output_print;
    	variable_instantiation << space;
    	variable_instantiation << prefix << str.substr(7,length-8) << suffix;
    	variable_instantiation << semicolon;
    	variable_instantiation << endline;
    	if(k == 1)
    	{
    		port_declaration << comma;
    		port_declaration << space;
    	}
    	port_declaration << prefix << str.substr(7, length - 8) << suffix;
    }
    else if(str.find(equal) != string::npos )
    {
    	i = str.find(equal);
    	if(str.substr(i+2,3).compare(andgate_scan) == 0)
    	{
    		gates << andgate_print;
    		gates << space;
    		gates << andgate_print;
    		gates << underscore;
    		gates << andgate_num;
    		andgate_num += 1;
    		gates << opening_bracket;
    		gates << prefix << str.substr(0,i-1) << suffix;
    		gates << comma;
    		gates << space;
    		str_temp2 = str.substr(i+2+4,length-5-i-2);
		str_temp1 = none;
    		while(str_temp2.find(comma) != string::npos)
    		{
    			i1 = str_temp2.find(comma);
    			length1 = str_temp2.size();
    			str_temp1 = str_temp1+prefix+str_temp2.substr(0,i1)+suffix+str_temp2.substr(i1,2);
    			str_temp2 = str_temp2.substr(i1+2,length-i1-2);
    		}
    		gates << str_temp1 << prefix << str_temp2 << suffix;
    		gates << closing_bracket;
    		gates << semicolon;
    		gates << endline;
    		variable_instantiation << wire << space << prefix << str.substr(0,i-1) << suffix << semicolon << endline;
    	}
    	else if(str.substr(i+2,4).compare(nandgate_scan) == 0)
    	{
    		gates << nandgate_print;
    		gates << space;
    		gates << nandgate_print;
    		gates << underscore;
    		gates << nandgate_num;
    		nandgate_num += 1;
    		gates << opening_bracket;
    		gates << prefix << str.substr(0,i-1) << suffix;
    		gates << comma;
    		gates << space;
		str_temp1 = none;
    		str_temp2 = str.substr(i+2+5,length-6-i-2);
    		while(str_temp2.find(comma) != string::npos)
    		{
    			i1 = str_temp2.find(comma);
    			length1 = str_temp2.size();
    			str_temp1 = str_temp1+prefix+str_temp2.substr(0,i1)+suffix+str_temp2.substr(i1,2);
    			str_temp2 = str_temp2.substr(i1+2,length-i1-2);
    		}
    		gates << str_temp1 << prefix << str_temp2 << suffix;
    		gates << closing_bracket;
    		gates << semicolon;
    		gates << endline;
    		variable_instantiation << wire << space << prefix << str.substr(0,i-1) << suffix << semicolon << endline;
    	}
    	else if(str.substr(i+2,2).compare(orgate_scan) == 0)
    	{
    		gates << orgate_print;
    		gates << space;
    		gates << orgate_print;
    		gates << underscore;
    		gates << orgate_num;
    		orgate_num += 1;
    		gates << opening_bracket;
    		gates << prefix << str.substr(0,i-1) << suffix;
    		gates << comma;
    		gates << space;
		str_temp1 = none;
    		str_temp2 = str.substr(i+2+3,length-4-i-2);
    		while(str_temp2.find(comma) != string::npos)
    		{
    			i1 = str_temp2.find(comma);
    			length1 = str_temp2.size();
    			str_temp1 = str_temp1+prefix+str_temp2.substr(0,i1)+suffix+str_temp2.substr(i1,2);
    			str_temp2 = str_temp2.substr(i1+2,length-i1-2);
    		}
    		gates << str_temp1 << prefix << str_temp2 << suffix;
    		gates << closing_bracket;
    		gates << semicolon;
    		gates << endline;
    		variable_instantiation << wire << space << prefix << str.substr(0,i-1) << suffix << semicolon << endline;
    	}
    	else if(str.substr(i+2,3).compare(norgate_scan) == 0)
    	{
    		gates << norgate_print;
    		gates << space;
    		gates << norgate_print;
    		gates << underscore;
    		gates << norgate_num;
    		norgate_num += 1;
    		gates << opening_bracket;
    		gates << prefix << str.substr(0,i-1) << suffix;
    		gates << comma;
    		gates << space;
		str_temp1 = none;
    		str_temp2 = str.substr(i+2+4,length-5-i-2);
    		while(str_temp2.find(comma) != string::npos)
    		{
    			i1 = str_temp2.find(comma);
    			length1 = str_temp2.size();
    			str_temp1 = str_temp1+prefix+str_temp2.substr(0,i1)+suffix+str_temp2.substr(i1,2);
    			str_temp2 = str_temp2.substr(i1+2,length-i1-2);
       		}
    		gates << str_temp1 << prefix << str_temp2 << suffix;
    		gates << closing_bracket;
    		gates << semicolon;
    		gates << endline;
    		variable_instantiation << wire << space << prefix << str.substr(0,i-1) << suffix << semicolon << endline;
    	}
    	else if(str.substr(i+2,3).compare(xorgate_scan) == 0)
    	{
    		gates << xorgate_print;
    		gates << space;
    		gates << xorgate_print;
    		gates << underscore;
    		gates << xorgate_num;
    		xorgate_num += 1;
    		gates << opening_bracket;
    		gates << prefix << str.substr(0,i-1) << suffix;
    		gates << comma;
    		gates << space;
		str_temp1 = none;
    		str_temp2 = str.substr(i+2+4,length-5-i-2);
    		while(str_temp2.find(comma) != string::npos)
    		{
    			i1 = str_temp2.find(comma);
    			length1 = str_temp2.size();
    			str_temp1 = str_temp1+prefix+str_temp2.substr(0,i1)+suffix+str_temp2.substr(i1,2);
    			str_temp2 = str_temp2.substr(i1+2,length-i1-2);
    		}
    		gates << str_temp1 << prefix << str_temp2 << suffix;
    		gates << closing_bracket;
    		gates << semicolon;
    		gates << endline;
    		variable_instantiation << wire << space << prefix << str.substr(0,i-1) << suffix << semicolon << endline;
    	}
    	else if(str.substr(i+2,3).compare(notgate_scan) == 0)
    	{
    		gates << notgate_print;
    		gates << space;
    		gates << notgate_print;
    		gates << underscore;
    		gates << notgate_num;
    		notgate_num += 1;
    		gates << opening_bracket;
    		gates << prefix << str.substr(0,i-1) << suffix;
    		gates << comma;
    		gates << space;
    		gates << prefix << str.substr(i+2+4,length - 5 - i - 2) << suffix;
    		gates << closing_bracket;
    		gates << semicolon;
    		gates << endline;
    		variable_instantiation << wire << space << prefix << str.substr(0,i-1) << suffix << semicolon << endline;
    	}
    	else if(str.substr(i+2,3).compare(dff) == 0)
    	{
    		gates << always << at_the_rate << opening_bracket << posedge << space << clock << space << closing_bracket << endline;
    		gates << begin;
    		gates << endline;
    		gates << prefix << str.substr(0,i-1) << suffix;
    		gates << nonblocking_assignment;
    		gates << prefix << str.substr(i+2+4,length - 5 - i - 2) << suffix;
    		gates << semicolon;
    		gates << endline;
    		gates << end;
    		gates << endline;
    		variable_instantiation << reg << space << prefix << str.substr(0,i-1) << suffix << semicolon << endline;
    	}
    }
    else
    {
    	//error();
    	break;
    }
  }

  port_declaration << closing_bracket;
  port_declaration << semicolon;
  port_declaration << endline;

  gates << endmodule;

  gates.close();
  port_declaration.close();
  variable_instantiation.close();
  infile.close();

  ifstream gates1(gateI.c_str());
  ifstream port_declaration1(portI.c_str());
  ifstream variable_instantiation1(variableI.c_str());

  while(getline(port_declaration1, str))
  {
    outfile << str;
    outfile << endline;
  }
  outfile << endline;
  while(getline(variable_instantiation1, str))
  {
    outfile << str;
    outfile << endline;
  }
  outfile << endline;
  while(getline(gates1,str))
  {
  	outfile << str;
  	outfile << endline;
  }
  outfile << endline;

  gates1.close();
  port_declaration1.close();
  variable_instantiation1.close();
  outfile.close();
  //cout << k;
}
