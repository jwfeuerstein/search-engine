#include <iostream>
#include <fstream>
#include "searcheng.h"
#include "searchui.h"
#include "md_parser.h"
#include "txt_parser.h"
#include "cmdhandler.h"

using namespace std;

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cout << "Must provide an index file" << endl;
        return 1;
    }

    // Declare the search engine
    SearchEng seng(new TXTParser);

    // Create and register appropriate parsers for .md and .txt files
    
    PageParser* mdparser = new MDParser;
    PageParser* txtparser = new TXTParser;
    seng.register_parser("md", mdparser);
    seng.register_parser("txt", txtparser);

    // Create the SearchUI object
    SearchUI ui(&seng);

    // Create and add appropriate Command Handlers to the SearchUI

    Handler* quithandler = new QuitHandler(NULL);
    Handler* printhandler = new PrintHandler(quithandler);
    Handler* incominghandler = new IncomingHandler(printhandler);
    Handler* outgoinghandler = new OutgoingHandler(incominghandler);
    Handler* andhandler = new AndHandler(outgoinghandler);
    Handler* orhandler = new OrHandler(andhandler);
    Handler* diffhandler = new DiffHandler(orhandler);
    Handler* prorhandler = new ProrHandler(diffhandler);
    Handler* prandhandler = new PrandHandler(prorhandler);
    ui.add_handler(quithandler);
    ui.add_handler(printhandler);
    ui.add_handler(incominghandler);
    ui.add_handler(outgoinghandler);
    ui.add_handler(andhandler);
    ui.add_handler(orhandler);
    ui.add_handler(diffhandler);
    ui.add_handler(prorhandler);
    ui.add_handler(prandhandler);


    // ------------------------------------------------------------------------
    try {
        seng.read_pages_from_index(argv[1]);
    }
    catch (std::exception& e) {
        cout << e.what() << endl;
        return 1;
    }
    bool result;
    if (argc == 2)
    {
        result = ui.run(cin, cout);

    }
    else if (argc == 3) {
        ifstream in_file(argv[2]);
        result = ui.run(in_file, cout);
    }
    else  {
        ifstream in_file(argv[2]);
        ofstream out_file(argv[3]);
        result = ui.run(in_file, out_file);
    }


    return result;
}
