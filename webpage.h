#ifndef WEBPAGE_H
#define WEBPAGE_H
#include <string>
#include <iostream>
#include <vector>
#include <set>

class WebPage;

typedef std::set<WebPage*> WebPageSet;
typedef std::set<std::string> StringSet;



class WebPage
{
public:

    WebPage();

    WebPage(std::string filename);


    ~WebPage();


    void filename(std::string fname);


    std::string filename() const;


    void all_terms(const StringSet& words);


    const StringSet& all_terms() const;


    void add_incoming_link(WebPage* wp);

    void remove_incoming_link(WebPage* wp);

    const WebPageSet& incoming_links() const;

  
    void add_outgoing_link(WebPage* wp);

    void remove_outgoing_link(WebPage* wp);

    const WebPageSet& outgoing_links() const;


private:
    std::vector<std::string> strip(std::string word);
    WebPageSet in_links_;
    WebPageSet out_links_;
    StringSet terms_;
    std::string filename_;

};
#endif
