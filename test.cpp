#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "xml_parser.h"

using namespace std;
using namespace tinyxml2;


/* Main functions */
int main(int argc, char **argv)
{
	XMLDocument xml;
	XMLElement* root = NULL;
	XMLPrinter printer(stderr);

	unsigned int number = 0;
	unsigned char str[32];
	memset(str, 0, sizeof(str));
	XML_PARSER	*parser = NULL;
	const char *xmlDoc =	"<ROOT>"
							"<NUM>15</NUM>"
							"<STRING>xmlParser</STRING>"
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
	parser = new XML_PARSER("TEST", root);
	
	parser->add_rule("NUM", &number);
	parser->add_rule("STRING", str);

	cout << *parser << endl;

	/* Start parse */
	if (parser->parse() == true){

		cout << "Parse success:" << number << "," << str << endl;
	}
	else{

		cerr << "Parse error!!!!" << endl;
	}

	delete parser;

	return 0;
}

