all: search-shell crawl

crawl: crawl.cpp util.cpp md_parser.cpp txt_parser.cpp webpage.cpp
	g++ -g -Wall -std=c++11 crawl.cpp util.cpp md_parser.cpp webpage.cpp txt_parser.cpp -o crawl

search-shell: search-shell.cpp cmdhandler.cpp combiners.cpp handler.cpp md_parser.cpp searcheng.cpp searchui.cpp txt_parser.cpp webpage.cpp util.cpp msort.h
	g++ -g -Wall -std=c++11 -o search-shell search-shell.cpp cmdhandler.cpp combiners.cpp handler.cpp md_parser.cpp searcheng.cpp searchui.cpp txt_parser.cpp webpage.cpp util.cpp msort.h
