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

		/*	Constructor */
		xmlParseRule(){uintValue = NULL, ucharValue = NULL, isStr = false;}
		xmlParseRule(const char *k, unsigned int *v) {key = k, uintValue = v, isStr = false;}
		xmlParseRule(const char *k, unsigned char *v){key = k, ucharValue = v, isStr = true;}		

		/*	Check this rule is valid */
		bool check(void);

		/* Data */
		string key;
		bool isStr;
		unsigned int *uintValue;
		unsigned char *ucharValue;	
};

std::ostream& operator<<(std::ostream &out, const xmlParseRule &rule);

/* Typedef */
typedef vector< xmlParseRule> ParserRules;


/* XML parser */
class xmlParser{

	public:

		/*	Constructor */
		xmlParser(const string &desc);
		xmlParser(const string &desc, XMLElement *node);

		/*	Parse xml */
		bool parse(bool debug = false, ostream &debug_os = cerr);

		/* 	Set xml root node */	
		void setRoot(XMLElement *node){ root = node;}

		/*	Set parser desc */
		void setDesc(const string &desc) {parserDesc = desc;}

		/*	Add  xml parsing rules */
		void addRule(const xmlParseRule &rule);
		void addRule(const char *key, unsigned int *value);
		void addRule(const char *key, unsigned char *value);

		/*	xml parser root node */
		XMLElement *root;

		/*	xml parser desc */
		string parserDesc;

		/*	xml parse rules */
		ParserRules  parserRules;

	private:

		/*	Check parser */
		bool check(void);

};

std::ostream& operator<<(std::ostream &out, const xmlParser &parser);

#endif
