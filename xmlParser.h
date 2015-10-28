#ifndef	_xmlParser_H_
#define _xmlParser_H_

#include <vector>
#include <string>
#include <iostream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

/* XML parse rule */
class xmlParseRule{

	public:

		/* Constructor */
		xmlParseRule(){init();}
		xmlParseRule(const char *k, char *v){init(); key = k, charValue = v;}
		xmlParseRule(const char *k, double *v){init(); key = k, doubleValue = v;}
		xmlParseRule(const char *k, int *v, int base = 0) {init(); key = k, intValue = v, intBase = base;}
		
		/* Check this rule is valid */
		bool check(void);

		/* Parse this rule */
		bool parse(const char* value);

		/* Data */
		string key;
		int *intValue;
		int intBase;
		char *charValue;
		double *doubleValue;

	private:
		void init(){doubleValue = NULL, intValue = NULL, charValue = NULL, intBase = 0;};
};

std::ostream& operator<<(std::ostream &out, const xmlParseRule &rule);

/* Typedef */
typedef vector< xmlParseRule> ParserRules;


/* XML parser */
class xmlParser{

	public:

		/*	Constructor */
		xmlParser(const string &desc);

		/*	Parse xml */
		bool parseAttr(XMLElement *root, bool debug = false, ostream &debugOS = cerr);
		bool parseValue(XMLElement *root, bool debug = false, ostream &debug_os = cerr);

		/*	Add  xml parsing rules */
		void addRule(const xmlParseRule &rule);
		void addAttrRule(const xmlParseRule &rule);

		/*	xml parser desc */
		string parserDesc;

		/*	xml parse rules */
		ParserRules  attrParserRules;
		ParserRules  valueParserRules;

	private:
		/*	Check parser rules*/
		bool checkRules(const XMLElement *root, const ParserRules &rules);
};

std::ostream& operator<<(std::ostream &out, const xmlParser &parser);

#endif
