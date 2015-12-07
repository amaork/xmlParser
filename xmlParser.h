#ifndef	_xmlParser_H_
#define _xmlParser_H_

#include <vector>
#include <string>
#include <iostream>
#include <stdint.h>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

/* XML parse rule */
class xmlParseRule{

	public:

		friend class xmlParser;

		/* Constructor */
		xmlParseRule(){init();}
		xmlParseRule(const char *k, char *v){init(); key = k, charValue = v;}
		xmlParseRule(const char *k, double *v){init(); key = k, doubleValue = v;}
		xmlParseRule(const char *k, int32_t *v, int base = 0) {init(); key = k, intValue = v, intBase = base;}
		xmlParseRule(const char *k, uint32_t *v, int base = 0) {init(); key = k, uintValue = v, intBase = base;}
		
		/* Check this rule is valid */
		bool check(void);

		/* Parse this rule */
		bool parse(const char* value);

		/* Reload operator */
		friend ostream& operator<<(ostream& os, const xmlParseRule &rule);
		friend bool operator==(const xmlParseRule &lhs, const xmlParseRule &hhs); 
		
	private:
		/* Data */
		string key;
		int intBase;
		int32_t *intValue;
		uint32_t *uintValue;

		char *charValue;
		double *doubleValue;
		void init(){doubleValue = NULL, intValue = NULL, uintValue = NULL, charValue = NULL, intBase = 0;};
};



/* Typedef */
typedef vector< xmlParseRule> ParserRules;


/* XML parser */
class xmlParser{

	public:

		/* Constructor */
		xmlParser(const string &desc);

		/* Parse xml */
		bool parseAttr(XMLElement *root, bool debug = false, ostream &debugOS = cerr);
		bool parseValue(XMLElement *root, bool debug = false, ostream &debug_os = cerr);

		/* Add  xml parsing rules */
		void addRule(const xmlParseRule &rule);
		void addAttrRule(const xmlParseRule &rule);

		/* Clear parsing rules */
		void clearRules(void){valueParserRules.clear();}
		void clearAttrRules(void) {attrParserRules.clear();}

		/* Reload output operator */
		friend ostream& operator<<(ostream &out, const xmlParser &parser);

	private:
		/* parser desc */
		string parserDesc;

		/* xml parse rules */
		ParserRules  attrParserRules;
		ParserRules  valueParserRules;

		/* Check parser rules*/
		bool checkRules(const XMLElement *root, const ParserRules &rules);
};


#endif
