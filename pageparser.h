#ifndef ELEM_PARSER_H
#define ELEM_PARSER_H

#include <string>
#include <map>
#include "webpage.h"


class PageParser {
public:
    virtual ~PageParser() { }


    virtual void parse(std::string filename,
                       std::set<std::string>& allSearchableTerms,
                       std::set<std::string>& allOutgoingLinks) = 0;


    virtual std::string display_text(std::string filename) = 0;


    virtual void crawl(
        const std::map<std::string, PageParser*>& parsers,
        std::string filename,
        std::set<std::string>& processed,
        std::ostream& ofile) = 0;

};

#endif
