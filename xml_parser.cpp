#include "xml_parser.h"
#include <strings.h>
#include <algorithm>

/**************************************************************************************
	@brief	:	struct parse_rule operator== mothed	
**************************************************************************************/
bool operator== (const XML_PARSE_RULE &lhs, const XML_PARSE_RULE &hhs)
{
	return ((lhs.key == hhs.key) && (lhs.is_str == hhs.is_str));
}

/**************************************************************************************
	@brief	:	Overloaded operator << 
**************************************************************************************/
std::ostream& operator<<(std::ostream &out, const XML_PARSE_RULE &rule) 
{
	out << "is_str\t" << rule.is_str << "\tkey:\t" << rule.key << endl; 
	return out;
}

/**************************************************************************************
	@breif	:	Parse rule check
***************************************************************************************/
bool XML_PARSE_RULE::check(void)
{
	/* 	Check key */
	if (!key.size()){

		cerr << "Rule error: key is empty!" << endl;
		return false;
	}
	
	/*	Check value target */	
	if ((is_str && !c_value) || (!is_str && !i_value)){

		cerr << "Rule error: value is invalid!" << endl;
		return false;
	}

	return true;
}


/**************************************************************************************
	@brief	:	Constructor
**************************************************************************************/
XML_PARSER::XML_PARSER(const string &desc)
{
	/* 	Set parser name */
	parser_desc = desc;
	
	/* 	Clear parser rules */
	parser_rules.clear();

	/*	Clear xml parser root */
	root = NULL;
}


XML_PARSER::XML_PARSER(const string &desc, XMLElement *node)
{
	/* 	Set parser name */
	parser_desc = desc;
	
	/* 	Clear parser rules */
	parser_rules.clear();

	/*	Clear xml parser root */
	root = node;
}

/**************************************************************************************
	@brief	Add xml parse rule, value is an integer type 
**************************************************************************************/
void XML_PARSER::add_rule(const char *key, unsigned int *value)
{
	/*	Add new parse rule to parser rule linklist */	
	parser_rules.push_back(XML_PARSE_RULE(key, value));	
}

void XML_PARSER::add_rule(const char *key, unsigned char *value)
{
	/*	Add new parse rule to parser rule linklist */	
	parser_rules.push_back(XML_PARSE_RULE(key, value));	
}

void XML_PARSER::add_rule(const XML_PARSE_RULE &rule)
{
	parser_rules.push_back(rule);
}


/**************************************************************************************
	@brief	:	Parser self check 
**************************************************************************************/
bool XML_PARSER::check(void)
{
	/*	Check parser rules */
	if (parser_rules.empty()){

		cerr << "Parser[" << parser_desc << "] parse rules linklist is empty!" << endl;
		return false; 
	}

	/*	Check parser root node */
	if (!root){

		cerr << "Parser[" << parser_desc << "] parse root node unset!" << endl;
		return false;
	}

	return true;
}


/**************************************************************************************
	@brief:	overloaded operator <<  
**************************************************************************************/
std::ostream& operator<<(std::ostream &out, const XML_PARSER &parser) 
{
	Parser_rule::const_iterator rule_it;

	out << "=============================================================" << endl; 
	out << "Parser info" << endl;
	out << "Name:\t\t" << parser.parser_desc << endl;
	out << "Rules num:\t" << parser.parser_rules.size() << endl;
	out << "Root node name:\t" << parser.root->Value() << endl;

	/*	Print every rules */
	for (rule_it = parser.parser_rules.begin(); rule_it != parser.parser_rules.end(); rule_it++){
			
			out << *rule_it << endl;
	}

	return out;
}

/**************************************************************************************
	@brief	:	Parsing xml according rules, after parsing delete the rule  
**************************************************************************************/
bool XML_PARSER::parse(bool debug, ostream &debug_os)
{
	XMLElement *key = NULL;
	const char *value = NULL;
	Parser_rule::iterator rule;

	/*	Check parser is ready */
	if (!check()) return false;

	/* 	Debug output */
	if (debug)
		debug_os << "+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++" << endl; 		

	/* 	Parser each rule form parser_rules */
	for (rule = parser_rules.begin(); rule != parser_rules.end(); rule++){

		/*	Check rule */
		if (!rule->check())continue;
	
		/* 	Find key */	   
		key = root->FirstChildElement(rule->key.c_str());	

		/* 	Key is find ? */
		if (!key){

			cerr << "Parser[" << parser_desc << "]\tUndefined key:" << rule->key << endl;
			continue;
		}

		/* 	Get key value */
		value = key->ToElement()->GetText();

		/* 	Find key value ? */
		if (!value){

			cerr << "Parser[" << parser_desc << "]\tKey[" << rule->key << "] value is empty!" << endl;
			continue;
		}
	
		/*	Read key value */
		if (rule->is_str && rule->c_value){

			/* Value is a string */
			bcopy(value, rule->c_value, strlen(value));
		}
		else if (rule->i_value){

			/* Value is a integer */
			*rule->i_value = atoi(value);
		}

		/* Debug output */	
		if (debug){

			if (rule->is_str){

				debug_os << rule->key << ":" << rule->c_value << endl;
			}
			else{

				debug_os << rule->key << ":" << *rule->i_value << endl;
			}
		}	

	} /* end of for */

	return true;
}

