#ifndef TXT_PARSER_H
#define TXT_PARSER_H

#include <string>
#include <set>

#include "pageparser.h"

class TXTParser : public PageParser {
public:

    void parse(std::string filename,
               std::set<std::string>& allSearchableTerms,
               std::set<std::string>& allOutgoingLinks);

    void crawl(
        const std::map<std::string, PageParser*>& parsers,
        std::string filename,
        std::set<std::string>& processed,
        std::ostream& ofile);

    std::string display_text(std::string filename);

private:


};

#endif
