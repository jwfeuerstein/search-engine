#include <iostream>
#include <fstream>
#include <string>
#include <map> 
#include <set>
#include "md_parser.h"
#include "txt_parser.h"
#include "util.h" 

using namespace std; 

int main(int argc, char* argv[])
{
    if (argc < 3) {
        cout << "Must provide an index file and output file" << endl;
        return 1;
    }


    map<string, PageParser*> parsers;
    PageParser* MDparser = new MDParser;
    PageParser* TXTparser = new TXTParser;
    PageParser* NEparser = new TXTParser;
    parsers.insert(make_pair("", NEparser));
    parsers.insert(make_pair("txt", TXTparser));
    parsers.insert(make_pair("md", MDparser));

    ifstream ifile(argv[1]);
    if(ifile.fail()){
    	cout << "Failed to open index file" << endl;
    	return 0;
    }

    ofstream ofile(argv[2]);
    if(ofile.fail()){
    	cout << "Failed to open output file" << endl;
    	return 0;
    }

    // use appropriate parser to crawl files
    set<string> processed;
    string filename;
    while(ifile >> filename){
    	if(parsers.find(extract_extension(filename)) != parsers.end() && processed.find(filename) == processed.end()){
    		parsers.find(extract_extension(filename)) -> second -> crawl(parsers, filename, processed, ofile);
    	}
    }

    ifile.close();
    ofile.close();
    
    // delete parsers
    map<string, PageParser*>::iterator it;
    for(it = parsers.begin(); it != parsers.end(); it++){
    	delete it -> second;
    }

    return 0;
}
