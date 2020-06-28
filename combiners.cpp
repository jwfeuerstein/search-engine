#include "searcheng.h"
#include "combiners.h"

// Place your function implementations for your derived
// WebPageSetCombiner classes here

WebPageSet ANDWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	 
	WebPageSet combined;
	if(setA.empty() || setB.empty()){
		return combined;
	}
	WebPageSet::iterator it;
	//find elements in both setA and setB
	
	for(it = setA.begin(); it != setA.end(); ++it){
		WebPage* temp = *it;
		if(setB.find(temp) != setB.end()){
			combined.insert(temp);
		}
	}
	
	/*
	for(it = setB.begin(); it != setB.end(); ++it){
		if(setA.find(*it) != setA.end()){
			combined.insert(*it);
		}
	}
	*/
	return combined;
}



WebPageSet ORWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet combined = setA;
	WebPageSet::iterator it;
	//add elements of setB to setA
	if(setB.empty()){
		return setA;
	}
	if(setA.empty())
	{
		return setB;
	}	
	for(it = setB.begin(); it != setB.end(); ++it){  // causes segmentation fault, conditional jump or move depends on uninit
		if(combined.find(*it) == combined.end()){
			combined.insert(*it);  // use of uninitialized value
		}
	}
	return combined;
}


WebPageSet DiffWebPageSetCombiner::combine(const WebPageSet& setA, const WebPageSet& setB){
	WebPageSet combined = setA;
	WebPageSet erased;
	WebPageSet::iterator it;
	//find items to erase
	for(it = setA.begin(); it != setA.end(); ++it){
		if(setB.find(*it) != setB.end()){
			erased.insert(*it);
		}
	}
	//erase items
	WebPageSet::iterator wit;
	for(wit = erased.begin(); wit != erased.end(); wit++){
		combined.erase(*wit);
	}
	return combined;
}

