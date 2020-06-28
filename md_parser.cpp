#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cctype>
#include "md_parser.h"
#include "util.h"

using namespace std;

typedef enum { NORMALTEXT, LINKTEXT, ISLINK, LINKURL } PARSE_STATE_T;

void MDParser::crawl(
        const std::map<std::string, PageParser*>& parsers,
        std::string filename,
        std::set<std::string>& processed,
        std::ostream& ofile){
    
    ifstream wfile(filename.c_str());
    if(!wfile) {
        throw invalid_argument("Bad webpage filename in MDParser::crawl()");
    }
    if(processed.find(filename) != processed.end()){
        return;
    }
    // create stack and insert first file
    std::stack<std::string> DFSstack;
    DFSstack.push(filename);
    processed.insert(filename);
    //ofile << filename << endl;
    string curr;
    vector<string> links;

    // perform depth first search to explore all reachable pages
    while(!DFSstack.empty()){
        curr = DFSstack.top();
        DFSstack.pop();
        if(getLinks(curr, links) && parsers.find(extract_extension(curr)) != parsers.end()){
            processed.insert(curr);
            ofile << curr << endl;

            vector<string>::reverse_iterator i;
            for(i = links.rbegin(); i != links.rend(); ++i){
                 if(processed.find(*i) == processed.end()){
                    DFSstack.push(*i);
                }
            }
            
        }
        links.clear();
    }
}

bool MDParser::getLinks(std::string filename, std::vector<std::string>& linkOrder){

    ifstream wfile(filename.c_str());
    if(!wfile) {
        return false;
    }

    PARSE_STATE_T state = NORMALTEXT; 

    string term = "";
    string link = "";

    // Get the first character from the file.
    char c = wfile.get();

    // Continue reading from the file until input fails.
    while(!wfile.fail())
    {
        // Logic for parsing a normal term.
        if(state == NORMALTEXT)
        {
            // ADD YOUR CODE HERE
            if(isalnum(c)){
                term += c;
            }
            else{
                if(term != ""){
                    term = conv_to_lower(term);
                    
                    term.clear();
                    
                }
                if(c == '['){
                    state = LINKTEXT;
                }
            }

        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            // ADD YOUR CODE HERE
            if(isalnum(c)){
                term += c;
            }
            else{
                if(term != ""){
                    term = conv_to_lower(term);
                    
                    term.clear();
                }
                if(c == ']'){
                    state = ISLINK;
                }
            }


        }
        else if( state == ISLINK )
        {
            // ADD YOUR CODE HERE
            if(isalnum(c)){
                term += c;
            }

            else{
                link.clear();
                if(c == '('){

                    state = LINKURL;
                }
                else if(c != '('){

                    state = NORMALTEXT;
                }
            }

        }
        // Else we are in the LINKURL state.
        else
        {
            // ADD YOUR CODE HERE
            if(c != ')'){
                link += c;
                if(c == ' ' || (int) wfile.peek() == (int) std::char_traits<wchar_t>::eof()){
                    
                    linkOrder.push_back(link);
                    link.clear();
                    state = NORMALTEXT;
                }
            }
            else{
                
                linkOrder.push_back(link);
                link.clear();
                state = NORMALTEXT;
                
            }

        }
        // Attempt to get another character from the file.
        c = wfile.get();
    }

    wfile.close();
    return true;
}

void MDParser::parse(std::string filename, std::set<std::string>& allSearchableTerms, std::set<std::string>& allOutgoingLinks)
{
    // Attempts to open the file.
    ifstream wfile(filename.c_str());
    if(!wfile) {
        throw invalid_argument("Bad webpage filename in MDParser::parse()");
    }

    // Remove any contents of the sets before starting to parse.
    allSearchableTerms.clear();
    allOutgoingLinks.clear();

    // The initial state is parsing a normal term.
    PARSE_STATE_T state = NORMALTEXT;

    // Initialize the current term and link as empty strings.
    string term = "";
    string link = "";

    // Get the first character from the file.
    char c = wfile.get();

    // Continue reading from the file until input fails.
    while(!wfile.fail())
    {
        // Logic for parsing a normal term.
        if(state == NORMALTEXT)
        {
            // ADD YOUR CODE HERE
            if(isalnum(c)){
                term += c;
            }
            else{
                if(term != ""){
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                    term.clear();
                }
                if(c == '['){
                    state = LINKTEXT;
                }
            }

        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            // ADD YOUR CODE HERE
            if(isalnum(c)){
                term += c;
            }
            else{
                if(term != ""){
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                    term.clear();
                }
                if(c == ']'){
                    state = ISLINK;
                }
            }


        }
        else if( state == ISLINK )
        {
            // ADD YOUR CODE HERE
            if(isalnum(c)){
                term += c;
            }

            else{
                link.clear();
                if(c == '('){

                    state = LINKURL;
                }
                else if(c != '('){

                    state = NORMALTEXT;
                }
            }

        }
        // Else we are in the LINKURL state.
        else
        {
            // ADD YOUR CODE HERE
            if(c != ')'){
                link += c;
                if(c == ' ' || (int) wfile.peek() == (int) std::char_traits<wchar_t>::eof()){
                    allOutgoingLinks.insert(link);
                    link.clear();
                    state = NORMALTEXT;
                }
            }
            else{
                allOutgoingLinks.insert(link);
                link.clear();
                state = NORMALTEXT;
                
            }

        }
        // Attempt to get another character from the file.
        c = wfile.get();
        if(wfile.eof()){
            if(term != ""){
                    term = conv_to_lower(term);
                    allSearchableTerms.insert(term);
                    term.clear();
            }
        }
    }
    // ADD ANY REMAINING CODE HERE




    // Close the file.
    wfile.close();
}

std::string MDParser::display_text(std::string filename)
{
    // Attempts to open the file.
    ifstream wfile(filename.c_str());
    if (!wfile) {
        throw std::invalid_argument("Bad webpage filename in TXTParser::parse()");
    }
    std::string retval;

    // The initial state is parsing a normal term.
    PARSE_STATE_T state = NORMALTEXT;

    char c = wfile.get();

    // Continue reading from the file until input fails.
    while (!wfile.fail()) {
        // Logic for parsing a normal term.
        if (state == NORMALTEXT)
        {
            // The moment we hit a bracket, we input our current working term
            // into the allSearchableTerms set, reset the current term, and move into
            // parsing a link.
            if (c == '[')
            {
                state = LINKTEXT;
            }
            retval += c;
        }
        // Logic for parsing a link.
        else if (state == LINKTEXT)
        {
            // When we hit the closing bracket, then we must be finished getting the link.
            if (c == ']')
            {
                state = ISLINK;
            }
            retval += c;
        }
        else if (state == ISLINK) {
            if (c == '(') {
                state = LINKURL;
            }
            else
            {
                state = NORMALTEXT;
                retval += c;
            }
        }
        // Else we are in the LINKURL state.
        else
        {
            // When we hit a closing parenthese then we are done, and the link can be inserted.
            if (c == ')')
            {
                state = NORMALTEXT;
            }
        }
        c = wfile.get();
    }
    return retval;
}


