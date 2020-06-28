#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <iostream>
#include <set>
#include "webpage.h"


std::string conv_to_lower(std::string src);


std::string conv_to_upper(std::string src);


void display_hits(const WebPageSet& hits, std::ostream& ostr);


typedef std::vector<std::pair<WebPage*, double> > WebPageScorePairs;
void display_ranked_hits(WebPageScorePairs& hits, std::ostream& ostr);


std::string extract_extension(const std::string& filename);


#endif
