#ifndef MD_PARSER_H
#define MD_PARSER_H

#include <string>
#include <set>
#include <stack>

#include "pageparser.h"

class MDParser : public PageParser {
public:
    /**
     * See documentation in PageParser class
     * [TO BE COMPLETED]
     */
    void parse(std::string filename,
               std::set<std::string>& allSearchableTerms,
               std::set<std::string>& allOutgoingLinks);

    void crawl(
        const std::map<std::string, PageParser*>& parsers,
        std::string filename,
        std::set<std::string>& processed,
        std::ostream& ofile);

    bool getLinks(std::string filename, std::vector<std::string>& linkOrder);

    /**
     * See documentation in PageParser class
     * [COMPLETED]
     */
    std::string display_text(std::string filename);

private:
    // You may add additional private helpers or data members below

};

#endif
