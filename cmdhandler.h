#ifndef CMDHANDLER_H
#define CMDHANDLER_H
#include "handler.h"
#include "searcheng.h"
#include "combiners.h"
#include "../mergesort/msort.h"


class QuitHandler : public Handler
{
public:
    QuitHandler();
    QuitHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class PrintHandler : public Handler
{
public:
    PrintHandler();
    PrintHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class IncomingHandler : public Handler
{
public:
    IncomingHandler();
    IncomingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class OutgoingHandler : public Handler
{
public:
    OutgoingHandler();
    OutgoingHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class AndHandler : public Handler
{
public:
    AndHandler();
    AndHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class OrHandler : public Handler
{
public:
    OrHandler();
    OrHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class DiffHandler : public Handler
{
public:
    DiffHandler();
    DiffHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};


class ProrHandler : public Handler
{
public:
    ProrHandler();
    ProrHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

class PrandHandler : public Handler
{
public:
    PrandHandler();
    PrandHandler(Handler* next);

protected:

    virtual bool canHandle(const std::string& cmd) const;
    virtual HANDLER_STATUS_T process(SearchEng* eng, std::istream& instr, std::ostream& ostr);
};

struct RankByRank{
    bool operator()(std::pair<WebPage*, double> page1, std::pair<WebPage*, double> page2);
};

struct RankByName{
    bool operator()(std::pair<WebPage*, double> page1, std::pair<WebPage*, double> page2);
};

#endif
