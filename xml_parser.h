#ifndef	_XML_PARSER_H_
#define _XML_PARSER_H_

#include <vector>
#include <string>
#include <iostream>
#include "tinyxml2.h"

using namespace std;
using namespace tinyxml2;

/* XML parse rule */
class XML_PARSE_RULE{

	public:

		/*	Constructor */
		XML_PARSE_RULE(){i_value = NULL, c_value = NULL, is_str = false;}
		XML_PARSE_RULE(const char *k, unsigned int *v) {key = k, i_value = v, is_str = false;}
		XML_PARSE_RULE(const char *k, unsigned char *v){key = k, c_value = v, is_str = true;}		

		/*	Check this rule is valid */
		bool check(void);

		/* Data */
		string key;
		bool is_str;
		unsigned int *i_value;
		unsigned char *c_value;	
};

std::ostream& operator<<(std::ostream &out, const XML_PARSE_RULE &rule);

/* Typedef */
typedef vector< XML_PARSE_RULE> Parser_rule;


/* XML parser */
class XML_PARSER{

	public:

		/*	Constructor */
		XML_PARSER(const string &desc);
		XML_PARSER(const string &desc, XMLElement *node);

		/*	Parse xml */
		bool parse(bool debug = false, ostream &debug_os = cerr);

		/* 	Set xml root node */	
		void set_root(XMLElement *node){ root = node;}

		/*	Set parser desc */
		void set_desc(const string &desc) {parser_desc = desc;}

		/*	Add  xml parsing rules */
		void add_rule(const XML_PARSE_RULE &rule);
		void add_rule(const char *key, unsigned int *value);
		void add_rule(const char *key, unsigned char *value);

		/*	xml parser root node */
		XMLElement *root;

		/*	xml parser desc */
		string parser_desc;

		/*	xml parse rules */
		Parser_rule  parser_rules;

	private:

		/*	Check parser */
		bool check(void);

};

std::ostream& operator<<(std::ostream &out, const XML_PARSER &parser);

#endif
