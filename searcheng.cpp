#include <map>
#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include "searcheng.h"

using namespace std;

//std::string extract_extension(const std::string& filename);



SearchEng::SearchEng(PageParser* noExtensionParser)
{
    if (NULL == noExtensionParser)
    {
        throw std::invalid_argument("default parser cannot be NULL");
    }
    //this->noExtensionParser_ = noExtensionParser;
    parsers_.insert(make_pair("", noExtensionParser));
    // Add additional code if necessary

}

SearchEng::~SearchEng(){
    //delete webpage pointers
    std::map<string, WebPage*>::iterator it;
    for(it = webpages_.begin(); it != webpages_.end(); it++){
        if(it -> second != NULL){
            delete it -> second;
        }
    }
    //delete parsers
    /*
    if(textparser_ != NULL){
        delete textparser_;
    }
    if(mdparser_ != NULL){
        delete mdparser_;
    }
    if(noExtensionParser_ != NULL){
        delete noExtensionParser_;
    }
*/
    std::map<string, PageParser*>::iterator i;
    for(i = parsers_.begin(); i != parsers_.end(); i++){
        delete i -> second;
    }
}


std::vector<std::pair<WebPage*, double> > SearchEng::pageRank(const WebPageSet& in_pages){
    WebPageSet candidates(in_pages);
    WebPageSet::iterator it;
    WebPageSet::iterator it2;
    WebPageSet::iterator it3;
    WebPageSet outlinks;
    WebPageSet inlinks;
    // expand candidate set to include inlnks and outlinks
    for(it = in_pages.begin(); it != in_pages.end(); ++it){
        WebPage* curr = *it;
        outlinks = curr -> outgoing_links();
        inlinks = curr -> incoming_links();
        for(it2 = outlinks.begin(); it2 != outlinks.end(); ++it2){
            candidates.insert(*it2);
        }
        for(it3 = inlinks.begin(); it3 != inlinks.end(); ++it3){
            candidates.insert(*it3);
        }
    }

    // generate subgraph with candidate pages
    std::map<WebPage*, RankLinks*> subgraph;
    for(it = candidates.begin(); it != candidates.end(); ++it){
        WebPageSet subIncoming;
        WebPageSet subOutgoing;
        WebPage* curr = *it;
        WebPageSet incoming = curr -> incoming_links();
        WebPageSet outgoing = curr -> outgoing_links();
        for(it2 = incoming.begin(); it2 != incoming.end(); ++it2){
            if(candidates.find(*it2) != candidates.end()){
                subIncoming.insert(*it2);
            }
        }
        for(it3 = outgoing.begin(); it3 != outgoing.end(); ++it3){
            if(candidates.find(*it3) != candidates.end()){
                subOutgoing.insert(*it3);
            }
        }
        // insert outgoing and incoming link to self
        subIncoming.insert(*it);
        subOutgoing.insert(*it);
        // insert page into subgraph
        RankLinks* links = new RankLinks(subIncoming, subOutgoing); // dynamically allocate?
        subgraph.insert(make_pair(*it, links));
    }

    vector<pair<WebPage*, double>> pageRanks;
    int t = 20;
    double damp = 0.15;
    double N = (double) subgraph.size();

    // fill in starting rank values
    std::map<WebPage*, RankLinks*>::iterator itx;
    for(itx = subgraph.begin(); itx != subgraph.end(); ++itx){
        pageRanks.push_back(make_pair(itx -> first, 1.0 / N));
    } 

    vector<pair<WebPage*, double>> currRanks(pageRanks);
    double currentRank;
    for(int i = 0; i < t; i++){
        for(unsigned int j = 0; j < pageRanks.size(); j++){
            currentRank = (damp / N) + (1.0 - damp) * summation(currRanks, subgraph, pageRanks[j].first);
            pageRanks[j].second = currentRank;
        }
        currRanks = pageRanks;
    }

    return pageRanks;
}

double SearchEng::summation(vector<pair<WebPage*, double>>& currentRanks, map<WebPage*, RankLinks*>& subgraph, WebPage*& page){
    double sum = 0.0;

    WebPageSet::iterator it;
    for(it = subgraph.find(page) -> second -> inlinks_.begin(); it != subgraph.find(page) -> second -> inlinks_.end(); ++it){
        WebPage* curr; int currIdx;
        for(unsigned int i = 0; i < currentRanks.size(); i++){
            if(currentRanks[i].first == *it){
                curr = currentRanks[i].first;
                currIdx = i;
            }
        }
        sum += (currentRanks[currIdx].second) / (subgraph.find(curr) -> second -> num_outlinks());
    }

    return sum;
}

RankLinks::RankLinks(WebPageSet& incoming, WebPageSet& outgoing){
    inlinks_ = incoming;
    outlinks_ = outgoing;
}

int RankLinks::num_outlinks(){
    return outlinks_.size();
}

int RankLinks::num_inlinks(){
    return inlinks_.size();
}

WebPageSet SearchEng::search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) /*const*/{
    //if no search terms provided
    if(terms.size() == 0){
        WebPageSet empty;
        return empty;
    }

    //use combiner to generate set of results
    WebPageSet combined;
    if(terms.size() == 1 && pagesWithTerms_.find(terms[0]) != pagesWithTerms_.end()){
        combined = pagesWithTerms_.find(terms[0]) -> second;
    } 
    else if((pagesWithTerms_.find(terms[0]) != pagesWithTerms_.end()) && terms.size() > 1){
        combined = pagesWithTerms_.find(terms[0]) -> second;
        for(unsigned int i = 1; i < terms.size(); i++){
            if(pagesWithTerms_.find(terms[i]) != pagesWithTerms_.end()){
                combined = combiner -> combine(combined, pagesWithTerms_.find(terms[i]) -> second);
                // causing segmentation fault
            }
            else{
                WebPageSet blank;
                combined = combiner -> combine(combined, blank);
            }
        }
        for(WebPage* p : combined){
            if(p == NULL){
                WebPageSet empty;
                combined = empty;
            }
        }
    }
    else if(terms.size() > 1){
        WebPageSet empty;
        combined = empty;
        for(unsigned int i = 1; i < terms.size(); i++){
            if(pagesWithTerms_.find(terms[i]) != pagesWithTerms_.end()){
                combined = combiner -> combine(combined, pagesWithTerms_.find(terms[i]) -> second);
                // causing segmentation fault    
            }
            else{
                WebPageSet blank;
                combined = combiner -> combine(combined, blank);
                // causing segmentation fault
            }
        }
        for(WebPage* p : combined){
            if(p == NULL){
                WebPageSet empty;
                combined = empty;
            }
        }
    }

    
    
    return combined;
    
}


void SearchEng::register_parser(const std::string& extension, PageParser* parser){
    /*
    if(extension == "txt"){
        textparser_ = parser;
    }
    else if(extension == "md"){
        mdparser_ = parser;
    }
*/
    if(parsers_.find(extension) == parsers_.end()){
        parsers_.insert(make_pair(extension, parser));
    }
}

void SearchEng::read_pages_from_index(const std::string& index_file)
{
    ifstream ifile(index_file.c_str());
    if(ifile.fail()) {
        cerr << "Unable to open index file: " << index_file << endl;
    }

    // Parse all the files
    string filename;
    while(ifile >> filename) {
#ifdef DEBUG
        cout << "Reading " << filename << endl;
#endif
        read_page(filename);
    }
    ifile.close();
}

WebPage* SearchEng::retrieve_page(const std::string& page_name) const {
    if(webpages_.find(page_name) == webpages_.end()){
        return NULL;
    }
    else{
        return webpages_.find(page_name) -> second;
    }
}

void SearchEng::display_page(std::ostream& ostr, const std::string& page_name) const {
    //check for exceptions
    /*
    if(extract_extension(page_name) != "txt" && extract_extension(page_name) != "md" && extract_extension(page_name) != ""){
        throw std::logic_error("No parser registered to file extension.1");
    }
    */
    if(parsers_.find(extract_extension(page_name)) == parsers_.end()){
        throw std::logic_error("No parser registered to file extension.1");
    }
    if(webpages_.find(page_name) == webpages_.end()){
        throw std::invalid_argument("File name doesn't exist (display_page).");
    }
    /*
    //use appropriate parser to display text
    if(extract_extension(page_name) == "txt"){
        ostr << textparser_ -> display_text(page_name);
    }
    else if(extract_extension(page_name) == "md"){
        ostr << mdparser_ -> display_text(page_name);
    }
    else{
        ostr << noExtensionParser_ -> display_text(page_name);
    }
    */
    ostr << parsers_.find(extract_extension(page_name)) -> second -> display_text(page_name);
}

void SearchEng::read_page(const std::string& filename){
    if(extract_extension(filename) != "txt" && extract_extension(filename) != "md" && extract_extension(filename) != ""){
        throw std::logic_error("No parser registered to file extension.2");
    }
    StringSet allTerms;
    WebPage* currentPage;
    //if webpage does not already exist
    if(webpages_.find(filename) == webpages_.end()){
        WebPage* page = new WebPage(filename);
        currentPage = page;
        std::set<string> allSearchableTerms;
        std::set<string> allOutgoingLinks;
        //parse with appropriate parser
        /*
        if(extract_extension(filename) == "md"){
            mdparser_ -> parse(filename, allSearchableTerms, allOutgoingLinks);
        }
        else if(extract_extension(filename) == "txt"){
            textparser_ -> parse(filename, allSearchableTerms, allOutgoingLinks);
        }
        else{
            noExtensionParser_ -> parse(filename, allSearchableTerms, allOutgoingLinks);
        }
        */
        parsers_.find(extract_extension(filename)) -> second -> parse(filename, allSearchableTerms, allOutgoingLinks);
        allTerms = allSearchableTerms;
        page -> all_terms(allSearchableTerms);
        //designate incoming and outgoing links
        std::set<string>::iterator it;
        for(it = allOutgoingLinks.begin(); it != allOutgoingLinks.end(); ++it){
            if(webpages_.find(*it) == webpages_.end() && *it != page -> filename()){
                WebPage* newPage = new WebPage(*it);  
                page -> add_outgoing_link(newPage);
                newPage -> add_incoming_link(page);
                webpages_.insert(make_pair(*it, newPage));
            }
            else if(*it == page -> filename()){
                page -> add_outgoing_link(page);
                page -> add_incoming_link(page);
            }
            else{
                page -> add_outgoing_link(webpages_.find(*it) -> second);
                webpages_.find(*it) -> second -> add_incoming_link(page);
            }
        }
        webpages_.insert(make_pair(filename, page));
    }
    //else webpage already exists
    else{
        WebPage* existingPage = webpages_.find(filename) -> second;
        currentPage = existingPage;
        std::set<string> allSearchableTerms;
        std::set<string> allOutgoingLinks;
        //parse with appropriate parser
        /*
        if(extract_extension(filename) == "md"){
            mdparser_ -> parse(filename, allSearchableTerms, allOutgoingLinks);
        }
        else if(extract_extension(filename) == "txt"){
            textparser_ -> parse(filename, allSearchableTerms, allOutgoingLinks);
        }
        else{
            noExtensionParser_ -> parse(filename, allSearchableTerms, allOutgoingLinks);
        }
        */
        parsers_.find(extract_extension(filename)) -> second -> parse(filename, allSearchableTerms, allOutgoingLinks);
        existingPage -> all_terms(allSearchableTerms);
        allTerms = allSearchableTerms;
        std::set<string>::iterator it;
        //designate incoming and outgoing links
        for(it = allOutgoingLinks.begin(); it != allOutgoingLinks.end(); ++it){
            if(webpages_.find(*it) == webpages_.end()){
                WebPage* newPage = new WebPage(*it);  
                existingPage -> add_outgoing_link(newPage);
                newPage -> add_incoming_link(existingPage);
                webpages_.insert(make_pair(*it, newPage));
            }
            else{
                existingPage -> add_outgoing_link(webpages_.find(*it) -> second);
                webpages_.find(*it) -> second -> add_incoming_link(existingPage);
            }
        }
    }

    //update pagesWithTerms_ data member with info from parsed page
    StringSet::iterator it;
    for(it = allTerms.begin(); it != allTerms.end(); it++){
        if(pagesWithTerms_.find(*it) == pagesWithTerms_.end()){
            WebPageSet set;
            set.insert(currentPage);
            pagesWithTerms_.insert(make_pair(*it, set));
        }
        else{
            pagesWithTerms_.find(*it) -> second.insert(currentPage);
        }
    }
}

std::string SearchEng::extract_extension(const std::string& filename) const   //Added const
{
    size_t idx = filename.rfind(".");
    if (idx == std::string::npos) {
        return std::string();
    }
    return filename.substr(idx + 1);
}


