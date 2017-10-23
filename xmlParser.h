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
class xmlParseRule {

public:

	friend class xmlParser;

	/* Constructor */
	xmlParseRule() {init();}
	xmlParseRule(const char *k, char *v, const string &def = "") {init(); key = k, charValue = v, charDefault = def;}
	xmlParseRule(const char *k, double *v, double def = 0.0) {init(); key = k, doubleValue = v, doubleDefault = def;}
	xmlParseRule(const char *k, int32_t *v, int32_t def = 0) {init(); key = k, intValue = v, intDefault = def;}
	xmlParseRule(const char *k, uint32_t *v, uint32_t def = 0) {init(); key = k, uintValue = v, uintDefault = def;}

	/* Check this rule is valid */
	bool check(void);

	/* Load default value */
	void loadDefaultValue(void);

	/* Parse this rule */
	bool parse(const char *value);

	/* Reload operator */
	friend ostream &operator<<(ostream &os, const xmlParseRule &rule);
	friend bool operator==(const xmlParseRule &lhs, const xmlParseRule &hhs);

private:

	/* Data */
	string key;
	char *charValue;
	string charDefault;
	int32_t *intValue, intDefault;
	uint32_t *uintValue, uintDefault;
	double *doubleValue, doubleDefault;

	void init() {doubleValue = NULL, intValue = NULL, uintValue = NULL, charValue = NULL, intDefault = uintDefault = doubleDefault = 0;}
};



/* Typedef */
typedef vector< xmlParseRule> ParserRules;


/* XML parser */
class xmlParser {

public:

	/* Constructor */
	xmlParser(const string &desc);

	/* Parse xml */
	bool parseAttr(XMLElement *root, bool debug = false, ostream &output = cerr);
	bool parseValue(XMLElement *root, bool debug = false, ostream &output = cerr);

	/* Add  xml parsing rules */
	void addRule(const xmlParseRule &rule);
	void addAttrRule(const xmlParseRule &rule);

	/* Clear parsing rules */
	void clearRules(void) {valueParserRules.clear();}
	void clearAttrRules(void) {attrParserRules.clear();}

	/* Reload output operator */
	friend ostream &operator<<(ostream &out, const xmlParser &parser);

private:
	/* parser desc */
	string parserDesc;

	/* xml parse rules */
	ParserRules  attrParserRules;
	ParserRules  valueParserRules;

	/* Check parser rules*/
	bool checkRules(const XMLElement *root, const ParserRules &rules);
};

/* String convert functions group */
enum STRCONV_ERROR {CONV_SUCCESS, CONV_OVERFLOW, CONV_UNDERFLOW, CONV_INCONVERTIBLE};
enum STRCONV_ERROR str2double(double *i, char const *s);
enum STRCONV_ERROR str2int (int *i, char const *s, int base = 0);
enum STRCONV_ERROR str2uint (uint32_t *i, char const *s, int base = 0);

#endif
