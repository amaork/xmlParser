#include "xmlParser.h"
#include <strings.h>
#include <algorithm>

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
	out << "isStr\t" << rule.isStr << "\tkey:\t" << rule.key << endl; 
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
	if ((isStr && !ucharValue) || (!isStr && !uintValue)){

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
	parserRules.clear();

	/*	Clear xml parser root */
	root = NULL;
}


xmlParser::xmlParser(const string &desc, XMLElement *node)
{
	/* 	Set parser name */
	parserDesc = desc;
	
	/* 	Clear parser rules */
	parserRules.clear();

	/*	Clear xml parser root */
	root = node;
}

/**************************************************************************************
	@brief	Add xml parse rule, value is an integer type 
**************************************************************************************/
void xmlParser::addRule(const char *key, unsigned int *value)
{
	/*	Add new parse rule to parser rule linklist */	
	parserRules.push_back(xmlParseRule(key, value));	
}

void xmlParser::addRule(const char *key, unsigned char *value)
{
	/*	Add new parse rule to parser rule linklist */	
	parserRules.push_back(xmlParseRule(key, value));	
}

void xmlParser::addRule(const xmlParseRule &rule)
{
	parserRules.push_back(rule);
}


/**************************************************************************************
	@brief	:	Parser self check 
**************************************************************************************/
bool xmlParser::check(void)
{
	/*	Check parser rules */
	if (parserRules.empty()){

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
	out << "Parser info" << endl;
	out << "Name:\t\t" << parser.parserDesc << endl;
	out << "Rules num:\t" << parser.parserRules.size() << endl;
	out << "Root node name:\t" << parser.root->Value() << endl;

	/*	Print every rules */
	for (rule_it = parser.parserRules.begin(); rule_it != parser.parserRules.end(); rule_it++){
			
			out << *rule_it << endl;
	}

	return out;
}

/**************************************************************************************
	@brief	:	Parsing xml according rules, after parsing delete the rule  
**************************************************************************************/
bool xmlParser::parse(bool debug, ostream &debug_os)
{
	XMLElement *key = NULL;
	const char *value = NULL;
	ParserRules::iterator rule;

	/*	Check parser is ready */
	if (!check()) return false;

	/* 	Debug output */
	if (debug)
		debug_os << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl; 		

	/* 	Parser each rule form parserRules */
	for (rule = parserRules.begin(); rule != parserRules.end(); rule++){

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
	
		/*	Read key value */
		if (rule->isStr && rule->ucharValue){

			/* Value is a string */
			bcopy(value, rule->ucharValue, strlen(value));
		}
		else if (rule->uintValue){

			/* Value is a integer */
			*rule->uintValue = atoi(value);
		}

		/* Debug output */	
		if (debug){

			if (rule->isStr){

				debug_os << rule->key << ":" << rule->ucharValue << endl;
			}
			else{

				debug_os << rule->key << ":" << *rule->uintValue << endl;
			}
		}	

	} /* end of for */

	return true;
}

