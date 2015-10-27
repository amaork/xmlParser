#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "xmlParser.h"

using namespace std;
using namespace tinyxml2;


/* Main functions */
int main(int argc, char **argv)
{
	XMLDocument xml;
	XMLElement* root = NULL;
	XMLPrinter printer(stderr);

	char str[32];
	double dvalue = 0.0;
	int number = 0;
	memset(str, 0, sizeof(str));
	xmlParser *parser = NULL;
	const char *xmlDoc =	"<ROOT>"
							"<NUM>20</NUM>"
							"<DOUBLE>1234.5678</DOUBLE>"
							"<STRING>xmlParser</STRING>"
							"<ATTRIBUTE name = \"amaork\" age=\"27\" workage = \"6.7\"></ATTRIBUTE>"
							"</ROOT>";


	/* Parse from string buffer */
	if (xml.Parse(xmlDoc) != XML_NO_ERROR){

		cerr << "Parse from string error: " << xml.GetErrorStr1() << "," << xml.GetErrorStr2() << endl;
		return -1;
	}

	/*	Find root element */
	if ((root = xml.RootElement()) == NULL){

		cerr <<  "Do not find xml root node!" << endl;
		return -1;
	}

	/* Create xmlParser and add parse rules */
	parser = new xmlParser("TEST");

	/* Start parse value */
	parser->addRule(xmlParseRule("NUM", &number));
	parser->addRule(xmlParseRule("STRING", str));
	parser->addRule(xmlParseRule("DOUBLE", &dvalue));

	parser->parseValue(root, true);

	/* Add parse attribute rules */
	memset(str, 0, sizeof(str));
	parser->addAttrRule(xmlParseRule("name", str));
	parser->addAttrRule(xmlParseRule("age", &number));
	parser->addAttrRule(xmlParseRule("workage", &dvalue));

	parser->parseAttr(root->FirstChildElement("ATTRIBUTE"), true);



	delete parser;

	return 0;
}
