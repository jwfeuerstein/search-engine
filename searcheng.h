#ifndef SEARCHENG_H
#define SEARCHENG_H

#include <map>
#include <vector>
#include <string>
#include "webpage.h"
#include "pageparser.h"
#include "wpscombiner.h"



class RankLinks{
public:
    WebPageSet outlinks_;
    WebPageSet inlinks_;
    RankLinks(WebPageSet& incoming, WebPageSet& outgoing);
    int num_outlinks();
    int num_inlinks();
};


class SearchEng {
public:

    SearchEng(PageParser* noExtensionParser);


    ~SearchEng();

    double summation(std::vector<std::pair<WebPage*, double>>& currentRanks, std::map<WebPage*, RankLinks*>& subgraph, WebPage*& page);
    

    void register_parser(const std::string& extension, PageParser* parser);



    void read_pages_from_index(const std::string& index_file);

    WebPage* retrieve_page(const std::string& page_name) const;


    void display_page(std::ostream& ostr, const std::string& page_name) const;

    WebPageSet search(const std::vector<std::string>& terms, WebPageSetCombiner* combiner) /*const */;
    std::string extract_extension(const std::string& filename) const;


    std::vector<std::pair<WebPage*, double> > pageRank(const WebPageSet& in_pages);

private:
    // Helper function to read/parse a single page
    void read_page(const std::string& filename);

    
    std::map<std::string, WebPageSet> pagesWithTerms_;
    std::map<std::string, WebPage*> webpages_;
    std::map<std::string, PageParser*> parsers_;
};

#endif
