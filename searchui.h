#ifndef SEARCHUI_H
#define SEARCHUI_H
#include <string>
#include <istream>
#include "searcheng.h"
#include "handler.h"


class SearchUI
{
public:

    SearchUI(SearchEng* eng);


    ~SearchUI();

    void add_handler(Handler* handler);

    virtual bool run(std::istream& istr, std::ostream& ostr);


protected:
    SearchEng* eng_;

private:
    Handler* handlers_;
};

#endif
