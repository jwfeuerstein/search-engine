#include "cmdhandler.h"
#include "util.h"
using namespace std;
 
QuitHandler::QuitHandler()
{

}

QuitHandler::QuitHandler(Handler* next)
    : Handler(next)
{

}

bool QuitHandler::canHandle(const std::string& cmd) const
{
    return cmd == "QUIT";

}

Handler::HANDLER_STATUS_T QuitHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    return HANDLER_QUIT;
}

PrintHandler::PrintHandler()
{

}

PrintHandler::PrintHandler(Handler* next)
    : Handler(next)
{

}

bool PrintHandler::canHandle(const std::string& cmd) const 
{
    return cmd == "PRINT";

}

Handler::HANDLER_STATUS_T PrintHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    string name;

    if (!(instr >> name)) {
        return HANDLER_ERROR;
    }
    try {
        eng->display_page(ostr, name);
    }
    catch (std::exception& e) {
        return HANDLER_ERROR;
    }
    return HANDLER_OK;
}

IncomingHandler::IncomingHandler()
{

}

IncomingHandler::IncomingHandler(Handler* next)
    : Handler(next)
{

}

bool IncomingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "INCOMING";
}

Handler::HANDLER_STATUS_T IncomingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //list incoming links to page
    string name;
    if(!(instr >> name)){
        return HANDLER_ERROR;
    }
    WebPage* page = eng -> retrieve_page(name);
    if(page == NULL){
        return HANDLER_ERROR;
    }
    WebPageSet incomingLinks = page -> incoming_links();
    ostr << incomingLinks.size() << endl;
    WebPageSet::iterator it;
    for(it = incomingLinks.begin(); it != incomingLinks.end(); ++it){
        ostr << (*it) -> filename() << endl;
    }
    return HANDLER_OK;
}

OutgoingHandler::OutgoingHandler()
{

}

OutgoingHandler::OutgoingHandler(Handler* next)
    : Handler(next)
{

}

bool OutgoingHandler::canHandle(const std::string& cmd) const
{
    return cmd == "OUTGOING";
}

Handler::HANDLER_STATUS_T OutgoingHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //list outgoing links from page
    string name;
    if(!(instr >> name)){
        return HANDLER_ERROR;
    }
    WebPage* page = eng -> retrieve_page(name);
    if(page == NULL){
        return HANDLER_ERROR;
    }
    WebPageSet outgoingLinks = page -> outgoing_links();
    ostr << outgoingLinks.size() << endl;
    WebPageSet::iterator it;
    for(it = outgoingLinks.begin(); it != outgoingLinks.end(); ++it){
        ostr << (*it) -> filename() << endl;
    }
    return HANDLER_OK;
}

AndHandler::AndHandler()
{ 

}

AndHandler::AndHandler(Handler* next)
    : Handler(next)
{

}

bool AndHandler::canHandle(const std::string& cmd) const
{
    return cmd == "AND";
}

Handler::HANDLER_STATUS_T AndHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //display results from and search
    vector<string> terms;
    string term;
    while(instr >> term){
        term = conv_to_lower(term);
        terms.push_back(term);
    }
    ANDWebPageSetCombiner andcombiner; 
    WebPageSet results = eng -> search(terms, &andcombiner);

    display_hits(results, ostr);

    return HANDLER_OK;
}

OrHandler::OrHandler()
{

}

OrHandler::OrHandler(Handler* next)
    : Handler(next)
{

}

bool OrHandler::canHandle(const std::string& cmd) const
{
 return cmd == "OR";
}

Handler::HANDLER_STATUS_T OrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //display results from or search
    vector<string> terms;
    string term;
    while(instr >> term){
        term = conv_to_lower(term);
        terms.push_back(term);
    }
    ORWebPageSetCombiner orcombiner; 
    WebPageSet results = eng -> search(terms, &orcombiner);

    display_hits(results, ostr);
    return HANDLER_OK;
}

DiffHandler::DiffHandler()
{

}

DiffHandler::DiffHandler(Handler* next)
    : Handler(next)
{

}

bool DiffHandler::canHandle(const std::string& cmd) const
{
    return cmd == "DIFF";
}

Handler::HANDLER_STATUS_T DiffHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    //display results from diff search
    vector<string> terms;
    string term;
    while(instr >> term){
        term = conv_to_lower(term);
        terms.push_back(term);
    }
    DiffWebPageSetCombiner diffcombiner; 

    WebPageSet results = eng -> search(terms, &diffcombiner);

    display_hits(results, ostr);

    return HANDLER_OK;
}


ProrHandler::ProrHandler()
{

}

ProrHandler::ProrHandler(Handler* next)
    : Handler(next)
{

}

bool ProrHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PROR";
}

Handler::HANDLER_STATUS_T ProrHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    char sort;
    instr >> sort;
    if(instr.fail()){
        return HANDLER_OK; 
    }
    // get results from or search
    vector<string> terms;
    string term;
    while(instr >> term){
        term = conv_to_lower(term);
        terms.push_back(term);
    }
    ORWebPageSetCombiner orcombiner; 
    WebPageSet results = eng -> search(terms, &orcombiner);

    // apply pageRank to results
    std::vector<std::pair<WebPage*, double> > pageRanking;
    pageRanking = eng -> pageRank(results); 

    RankByRank r;
    RankByName n;
    if(sort == 'r'){
        mergeSort(pageRanking, r);
        //std::sort(pageRanking.begin(), pageRanking.end(), r);
    }
    else if(sort == 'n'){
        mergeSort(pageRanking, n);
        //std::sort(pageRanking.begin(), pageRanking.end(), n);
    }
    else{
        return HANDLER_ERROR;
    }


    display_ranked_hits(pageRanking, ostr);

    return HANDLER_OK;
}

PrandHandler::PrandHandler()
{

}

PrandHandler::PrandHandler(Handler* next)
    : Handler(next)
{

}

bool PrandHandler::canHandle(const std::string& cmd) const
{
    return cmd == "PRAND";
}

Handler::HANDLER_STATUS_T PrandHandler::process(SearchEng* eng, std::istream& instr, std::ostream& ostr)
{
    char sort;
    instr >> sort;
    if(instr.fail()){
        return HANDLER_OK;
    }
    // get results from and search
    vector<string> terms;
    string term;
    while(instr >> term){
        term = conv_to_lower(term);
        terms.push_back(term);
    }
    ANDWebPageSetCombiner andcombiner; 
    WebPageSet results = eng -> search(terms, &andcombiner);

    // apply pageRank to results
    std::vector<std::pair<WebPage*, double> > pageRanking;
    pageRanking = eng -> pageRank(results);

    RankByRank r;
    RankByName n;
    if(sort == 'r'){
        mergeSort(pageRanking, r);
        //std::sort(pageRanking.begin(), pageRanking.end(), r);
    }
    else if(sort == 'n'){
        mergeSort(pageRanking, n);
        //std::sort(pageRanking.begin(), pageRanking.end(), n);
    }
    else{
        return HANDLER_ERROR;
    }

    
    display_ranked_hits(pageRanking, ostr);

    return HANDLER_OK;

}

bool RankByRank::operator()(std::pair<WebPage*, double> page1, std::pair<WebPage*, double> page2){
    return page1.second > page2.second;
}

bool RankByName::operator()(std::pair<WebPage*, double> page1, std::pair<WebPage*, double> page2){
    return page1.first -> filename() < page2.first -> filename();
}