#ifndef WPSCOMBINER_H
#define WPSCOMBINER_H

#include "webpage.h"

class WebPageSetCombiner
{
public:

    virtual ~WebPageSetCombiner() {}


    virtual
    WebPageSet combine(const WebPageSet& setA, const WebPageSet& setB) = 0;
};


#endif
