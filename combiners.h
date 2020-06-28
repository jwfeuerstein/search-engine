#ifndef COMBINERS_H
#define COMBINERS_H

#include "wpscombiner.h"

class ANDWebPageSetCombiner : public WebPageSetCombiner
{
public:
    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
};

class ORWebPageSetCombiner : public WebPageSetCombiner
{
public:
    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
};

class DiffWebPageSetCombiner : public WebPageSetCombiner
{
public:
    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB);
};

#endif
