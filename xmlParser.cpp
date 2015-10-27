#include "xmlParser.h"
#include <strings.h>
#include <algorithm>

const char* debugSplit = "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";

/**************************************************************************************
	@brief	:	struct parse_rule operator== mothed
**************************************************************************************/
bool operator== (const xmlParseRule &lhs, const xmlParseRule &hhs)
{
	return ((lhs.key == hhs.key) && (lhs.isStr == hhs.isStr));
}

/**************************************************************************************
	@brief	:	Overloaded operator <<
**************************************************************************************/
std::ostream& operator<<(std::ostream &out, const xmlParseRule &rule)
{

	out << "isStr\t" << rule.isStr << "\tkey:\t" << rule.key << "\tvalue\t";

	if (rule.isStr){

		out << rule.charValue << endl;

	}
	else if (rule.intValue){

		out << *rule.intValue << endl;
	}
	else if (rule.doubleValue){

		out << *rule.doubleValue << endl;
	}

	return out;
}

/**************************************************************************************
	@breif	:	Parse rule check
***************************************************************************************/
bool xmlParseRule::check(void)
{
	/* 	Check key */
	if (!key.size()){

		cerr << "Rule error: key is empty!" << endl;
		return false;
	}

	/*	Check value target */
	if ((isStr && !charValue) ||( (!isStr && !intValue) && (!isStr && !doubleValue))){

		cerr << "Rule error: value is invalid!" << endl;
		return false;
	}

	return true;
}


/**************************************************************************************
	@brief	:	Constructor
**************************************************************************************/
xmlParser::xmlParser(const string &desc)
{
	/* 	Set parser name */
	parserDesc = desc;

	/* 	Clear parser rules */
	attrParserRules.clear();
	valueParserRules.clear();
}

/**************************************************************************************
	@brief	Add xml parse rule, value is an integer type
**************************************************************************************/
void xmlParser::addRule(const xmlParseRule &rule)
{
	valueParserRules.push_back(rule);
}

void xmlParser::addAttrRule(const xmlParseRule &rule)
{
	attrParserRules.push_back(rule);
}

/**************************************************************************************
	@brief	:	Parser self check
**************************************************************************************/
bool xmlParser::checkRules(const XMLElement *root, const ParserRules &rules)
{
	/*	Check parser rules */
	if (rules.empty()){

		cerr << "Parser[" << parserDesc << "] parse rules linklist is empty!" << endl;
		return false;
	}

	/*	Check parser root node */
	if (!root){

		cerr << "Parser[" << parserDesc << "] parse root node unset!" << endl;
		return false;
	}

	return true;
}


/**************************************************************************************
	@brief:	overloaded operator <<
**************************************************************************************/
std::ostream& operator<<(std::ostream &out, const xmlParser &parser)
{
	ParserRules::const_iterator rule_it;

	out << "=============================================================" << endl;
	out << "Name:\t" << parser.parserDesc << endl;

	if (parser.valueParserRules.size() != 0){

		out << "Value parser rules num: " << parser.valueParserRules.size() << endl;
		for (rule_it = parser.valueParserRules.begin(); rule_it != parser.valueParserRules.end(); rule_it++) out << *rule_it << endl;
	}

	if (parser.attrParserRules.size()){

		out << "Attribute parser rules num: " << parser.attrParserRules.size() << endl;
		for (rule_it = parser.attrParserRules.begin(); rule_it != parser.attrParserRules.end(); rule_it++) out << *rule_it << endl;
	}

	return out;
}

/**************************************************************************************
	@brief	:	Parsing xml value according rules
	#root		:	Parsing start root element
	#debug	:	Enable or disbale debug mode
	#debugOS: Debug output stream
	$return	:	Success return true, else false
**************************************************************************************/
bool xmlParser::parseValue(XMLElement *root, bool debug, ostream &debugOS)
{
	XMLElement *key = NULL;
	const char *value = NULL;
	ParserRules::iterator rule;

	/*	Check parser is ready */
	if (!checkRules(root, valueParserRules)) return false;

	/* 	Debug output */
	if (debug)	debugOS << debugSplit << endl << root->Value() << endl << debugSplit << endl;

	/* 	Parser each rule form valueParserRules */
	for (rule = valueParserRules.begin(); rule != valueParserRules.end(); rule++){

		/*	Check rule */
		if (!rule->check())continue;

		/* 	Find key */
		key = root->FirstChildElement(rule->key.c_str());

		/* 	Key is find ? */
		if (!key){

			cerr << "Parser[" << parserDesc << "]\tUndefined key:" << rule->key << endl;
			continue;
		}

		/* 	Get key value */
		value = key->ToElement()->GetText();

		/* 	Find key value ? */
		if (!value){

			cerr << "Parser[" << parserDesc << "]\tKey[" << rule->key << "] value is empty!" << endl;
			continue;
		}

		/* Value is a string */
		if (rule->isStr && rule->charValue){

			bcopy(value, rule->charValue, strlen(value));
		}
		/* Value is a integer */
		else if (rule->intValue){

			*rule->intValue = atoi(value);
		}
		/* Value is a double */
		else if (rule->doubleValue){

			*rule->doubleValue = atof(value);
		}

		/* Debug output */
		if (debug) debugOS << *rule << endl;

	} /* end of for */

	return true;
}

/**************************************************************************************
	@brief	:	Parsing xml attribute according rules
	#root		:	Parsing start root element
	#debug	:	Enable or disbale debug mode
	#debugOS: Debug output stream
	$return	:	Success return true, else false
**************************************************************************************/
bool xmlParser::parseAttr(XMLElement *root, bool debug, ostream &debugOS)
{
	const char* value = NULL;
	ParserRules::iterator rule;

	/* Check parser is ready */
	if (!checkRules(root, attrParserRules)) return false;

	/* Debug output */
	if (debug)	debugOS << debugSplit << endl << root->Value() << endl << debugSplit << endl;

	/* Parser each rule from attrParserRules */
	for (rule = attrParserRules.begin(); rule != attrParserRules.end(); rule++){

		/* Check rule */
		if (!rule->check())continue;

		/* Find attribute, attribute is a string */
		if (rule->isStr && rule->charValue){

			if ((value = root->Attribute(rule->key.c_str())) == NULL){

					cerr << "Parser[" << parserDesc << "]\tKey[" << rule->key << "] value is empty!" << endl;
					continue;
			}
			else{

					memcpy(rule->charValue, value, strlen(value));
			}
		}
		else if (rule->intValue){

				if (root->QueryIntAttribute(rule->key.c_str(), rule->intValue) != XML_NO_ERROR){

					cerr << "Parser[" << parserDesc << "]\tKey[" << rule->key << "] value is empty!" << endl;
					continue;
				}
		}
		else if (rule->doubleValue){

				if (root->QueryDoubleAttribute(rule->key.c_str(), rule->doubleValue) != XML_NO_ERROR){

					cerr << "Parser[" << parserDesc << "]\tKey[" << rule->key << "] value is empty!" << endl;
					continue;
				}
		}

		/* Debug output */
		if (debug) debugOS << *rule << endl;

	} /* end of for */

	return true;
}
