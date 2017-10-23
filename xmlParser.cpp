#include "xmlParser.h"
#include <strings.h>
#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>


/* Global */
static const char *debugSplit = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
std::ostream &operator<<(std::ostream &out, const STRCONV_ERROR &error);

enum STRCONV_ERROR str2int(int *i, char const *s, int base)
{
	char *end;
	long  l;
	errno = 0;
	l = strtol(s, &end, base);

	if ((errno == ERANGE && l == LONG_MAX) || l > INT_MAX) {
		return CONV_OVERFLOW;
	}

	if ((errno == ERANGE && l == LONG_MIN) || l < INT_MIN) {
		return CONV_UNDERFLOW;
	}

	if (*s == '\0' || *end != '\0') {
		return CONV_INCONVERTIBLE;
	}

	*i = l;
	return CONV_SUCCESS;
}

enum STRCONV_ERROR str2uint(uint32_t *i, char const *s, int base)
{
	char *end;
	unsigned long  l;
	errno = 0;
	l = strtoul(s, &end, base);

	if ((errno == ERANGE && l == ULONG_MAX) || l > UINT_MAX) {
		return CONV_OVERFLOW;
	}

	if (*s == '\0' || *end != '\0') {
		return CONV_INCONVERTIBLE;
	}

	*i = l;
	return CONV_SUCCESS;
}


enum STRCONV_ERROR str2double(double *i, char const *s)
{
	char *end;
	double d;
	errno = 0;
	d = strtod(s, &end);

	if ((errno == ERANGE && d == HUGE_VAL)) {
		return CONV_OVERFLOW;
	}

	if ((errno == ERANGE && d == 0)) {
		return CONV_UNDERFLOW;
	}

	if (*s == '\0' || *end != '\0') {
		return CONV_INCONVERTIBLE;
	}

	*i = d;
	return CONV_SUCCESS;
}


std::ostream &operator<<(std::ostream &out, const STRCONV_ERROR &error)
{
	switch (error) {

		case	CONV_SUCCESS	:
			out << "success" << endl;
			break;

		case	CONV_OVERFLOW	:
			out << "overflow" << endl;
			break;

		case	CONV_UNDERFLOW	:
			out << "underflow" << endl;
			break;

		case	CONV_INCONVERTIBLE:
			out << "inconvertible" << endl;
			break;

		default					:
			out << "unknow status" << endl;
			break;
	}

	return out;
}

/* struct parse_rule operator== mothed */
bool operator== (const xmlParseRule &lhs, const xmlParseRule &hhs)
{
	return ((lhs.key == hhs.key) && \
	        (lhs.intValue == hhs.intValue) && \
	        (lhs.uintValue == hhs.uintValue && \
	         (lhs.charValue == hhs.charValue && \
	          (lhs.doubleValue == hhs.doubleValue))));
}

/* Overloaded operator<< */
std::ostream &operator<<(std::ostream &out, const xmlParseRule &rule)
{

	out << "key:\t" << rule.key << "\tvalue\t";

	if (rule.charValue) {

		out << rule.charValue << endl;

	}
	else if (rule.intValue) {

		out << *rule.intValue <<  endl;
	}
	else if (rule.uintValue) {

		out << *rule.uintValue << endl;
	}
	else if (rule.doubleValue) {

		out << *rule.doubleValue << endl;
	}

	return out;
}

bool xmlParseRule::check(void)
{
	/* 	Check key */
	if (!key.size()) {

		cerr << "Rule error: key is empty!" << endl;
		return false;
	}

	/*	Check value target */
	if (!intValue &&  !uintValue && !charValue && !doubleValue) {

		cerr << "Rule error: value is invalid!" << endl;
		return false;
	}

	return true;
}

bool xmlParseRule::parse(const char *value)
{
	double dValue = 0;
	int32_t iValue = 0;
	uint32_t uValue = 0;
	enum STRCONV_ERROR result;

	/* Value is a string */
	if (charValue) {

		memcpy(charValue, value, strlen(value));
	}
	/* Value is int value */
	else if (intValue) {

		if ((result = str2int(&iValue, value)) != CONV_SUCCESS) {

			cerr << "Parse[" << key << "]\t value is error: " << result << endl;
			return false;
		}

		*intValue = iValue;
	}
	/* Value is unsigned int value */
	else if (uintValue) {

		if ((result = str2uint(&uValue, value)) != CONV_SUCCESS) {

			cerr << "Parse[" << key << "]\t value is error: " << result << endl;
			return false;
		}

		*uintValue = uValue;
	}
	/* Value is double or flost value */
	else if (doubleValue) {

		if ((result = str2double(&dValue, value)) != CONV_SUCCESS) {

			cerr << "Parse[" << key << "]\t value is error: " << result << endl;
			return false;
		}

		*doubleValue = dValue;
	}

	return true;
}

void xmlParseRule::loadDefaultValue(void)
{
	if (charValue) {

		memcpy(charValue, charDefault.c_str(), charDefault.size());
	}
	else if (intValue) {

		*intValue = intDefault;
	}
	else if (uintValue) {

		*uintValue = uintDefault;
	}
	else if (doubleValue) {

		*doubleValue = doubleDefault;
	}
}


xmlParser::xmlParser(const string &desc)
{
	/* 	Set parser name */
	parserDesc = desc;

	/* 	Clear parser rules */
	attrParserRules.clear();
	valueParserRules.clear();
}

void xmlParser::addRule(const xmlParseRule &rule)
{
	valueParserRules.push_back(rule);
}

void xmlParser::addAttrRule(const xmlParseRule &rule)
{
	attrParserRules.push_back(rule);
}

bool xmlParser::checkRules(const XMLElement *root, const ParserRules &rules)
{
	/*	Check parser rules */
	if (rules.empty()) {

		cerr << "Parser[" << parserDesc << "] parse rules linklist is empty!" << endl;
		return false;
	}

	/*	Check parser root node */
	if (!root) {

		cerr << "Parser[" << parserDesc << "] parse root node unset!" << endl;
		return false;
	}

	return true;
}


std::ostream &operator<<(std::ostream &out, const xmlParser &parser)
{
	ParserRules::const_iterator rule_it;

	out << "=============================================================" << endl;
	out << "Name:\t" << parser.parserDesc << endl;

	if (parser.valueParserRules.size() != 0) {

		out << "Value parser rules num: " << parser.valueParserRules.size() << endl;

		for (rule_it = parser.valueParserRules.begin(); rule_it != parser.valueParserRules.end(); rule_it++) out << *rule_it << endl;
	}

	if (parser.attrParserRules.size()) {

		out << "Attribute parser rules num: " << parser.attrParserRules.size() << endl;

		for (rule_it = parser.attrParserRules.begin(); rule_it != parser.attrParserRules.end(); rule_it++) out << *rule_it << endl;
	}

	return out;
}

/*
**  @brief	:	Parsing xml value according rules
**  #root	:	Parsing start root element
**  #debug	:	Enable or disbale debug mode
**  #output :   Debug output stream
**  $return	:	Success return true, else false
*/
bool xmlParser::parseValue(XMLElement *root, bool debug, ostream &output)
{
	XMLElement *key = NULL;
	const char *value = NULL;
	ParserRules::iterator rule;

	/* Check parser is ready */
	if (!checkRules(root, valueParserRules))
		return false;


	/* Parser each rule form valueParserRules */
	for (rule = valueParserRules.begin(); rule != valueParserRules.end(); rule++) {

		/*	Check rule */
		if (!rule->check())
			return false;

		/* 	Find key */
		if (!(key = root->FirstChildElement(rule->key.c_str()))) {

			rule->loadDefaultValue();
			continue;
		}

		/* 	Find key value ? */
		if (!(value = key->ToElement()->GetText())) {

			rule->loadDefaultValue();
			continue;
		}

		/* Parse rule */
		if (!rule->parse(value))
			return false;

	}

	/* 	Debug output */
	if (debug) {

		output << debugSplit << endl << root->Value() << endl << debugSplit << endl;

		for (rule = valueParserRules.begin(); rule != valueParserRules.end(); rule++) {

			output << *rule << endl;
		}
	}

	return true;
}

/*
**  @brief	:	Parsing xml attribute according rules
**  #root	:	Parsing start root element
**  #debug	:	Enable or disbale debug mode
**  #output :   Debug output stream
**  $return	:	Success return true, else false
*/
bool xmlParser::parseAttr(XMLElement *root, bool debug, ostream &output)
{
	const char *attr = NULL;
	ParserRules::iterator rule;

	/* Check parser is ready */
	if (!checkRules(root, attrParserRules))
		return false;

	/* Parser each rule from attrParserRules */
	for (rule = attrParserRules.begin(); rule != attrParserRules.end(); rule++) {

		/* Check rule */
		if (!rule->check())
			return false;

		/* Find attribute */
		if ((attr = root->Attribute(rule->key.c_str())) == NULL) {

			rule->loadDefaultValue();
			continue;
		}

		/* Parse attrValue */
		if (!rule->parse(attr))
			return false;
	}

	/* Debug output */
	if (debug) {

		output << debugSplit << endl << root->Value() << endl << debugSplit << endl;

		for (rule = attrParserRules.begin(); rule != attrParserRules.end(); rule++) {

			output << *rule << endl;
		}
	}

	return true;
}
