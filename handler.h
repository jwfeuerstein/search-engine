#ifndef HANDLER_H
#define HANDLER_H
#include <string>
#include <istream>
#include <algorithm>
#include "searcheng.h"


class Handler
{
public:

    enum HANDLER_STATUS_T {HANDLER_OK = 0, HANDLER_QUIT, HANDLER_ERROR, HANDLER_KILL};


    Handler();


    Handler(Handler* next);


    virtual ~Handler();


    HANDLER_STATUS_T handle(SearchEng* eng, const std::string& cmd, std::istream& instr, std::ostream& ostr);


    void set_next(Handler* next);

protected:



    virtual bool canHandle(const std::string& cmd) const = 0;

    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr)  = 0;

private:
    Handler* next_;
};

#endif
