#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "xmlParser.h"

using namespace std;
using namespace tinyxml2;

/* Main functions */
int main(int argc, char **argv) {
    XMLDocument xml;
    XMLElement *root = NULL;

    char str[32];
    int number = 0;
    int decNum = 0;
    int octNum = 0;
    uint32_t hexNum = 0;

    int attrNum = 0;
    int64_t i64 = 0;
    uint64_t u64 = 0;
    float fvalue = 0.0;
    double dvalue = 0.0;
    memset(str, 0, sizeof(str));
    xmlParser parser = xmlParser("TEST");

    const char *xmlDoc =	"<ROOT>"
                            "<DEC>255</DEC>"
                            "<OCT>0377</OCT>"
                            "<HEX>0xff</HEX>"
                            "<NUM>1234</NUM>"
                            "<I64>-1234567891233</I64>"
                            "<U64>19008000000</U64>"
//                            "<FLOAT>1.234567</DOUBLE>"
//                            "<DOUBLE>1.23456789</DOUBLE>"
                            "<STRING>xmlParser</STRING>"
                            "<ATTRIBUTE dec = \"255\" oct=\"0377\" hex = \"0xF9007703\" num = \"1234\" "
                            "i64 = \"-1234589343434\" u64 = \"0x123456789ABC\" "
                            "float = \"3.141592\" double = \"3.1415926535\" str = \"xmlParser\">0x100</ATTRIBUTE>"
                            "</ROOT>";

    /* Parse from string buffer */
    if (xml.Parse(xmlDoc) != XML_NO_ERROR) {

        cerr << "Parse from string error: " << xml.GetErrorStr1() << "," << xml.GetErrorStr2() << endl;
        return -1;
    }

    /*	Find root element */
    if ((root = xml.RootElement()) == NULL) {

        cerr <<  "Do not find xml root node!" << endl;
        return -1;
    }

    /* Create xmlParser and add parse rules */
    parser.addRule(xmlParseRule("I64", &i64));
    parser.addRule(xmlParseRule("U64", &u64));
    parser.addRule(xmlParseRule("DEC", &decNum));
    parser.addRule(xmlParseRule("OCT", &octNum));
    parser.addRule(xmlParseRule("HEX", &hexNum));
    parser.addRule(xmlParseRule("NUM", &number));
    parser.addRule(xmlParseRule("STRING", str));
    parser.addRule(xmlParseRule("DOUBLE", &dvalue, 1.23456789));
    parser.addRule(xmlParseRule("FLOAT", &fvalue, 1.234567));
    parser.addRule(xmlParseRule("ATTRIBUTE", &attrNum));
    parser.parseValue(root, true);

    /* Add parse attribute rules */
    memset(str, 0, sizeof(str));
    decNum = octNum = hexNum = number = attrNum = dvalue = 0;

    parser.addAttrRule(xmlParseRule("str", str));
    parser.addAttrRule(xmlParseRule("i64", &i64));
    parser.addAttrRule(xmlParseRule("u64", &u64));
    parser.addAttrRule(xmlParseRule("dec", &decNum));
    parser.addAttrRule(xmlParseRule("oct", &octNum));
    parser.addAttrRule(xmlParseRule("hex", &hexNum));
    parser.addAttrRule(xmlParseRule("num", &number));

    parser.addAttrRule(xmlParseRule("float", &fvalue));
    parser.addAttrRule(xmlParseRule("double", &dvalue));

    parser.parseAttr(root->FirstChildElement("ATTRIBUTE"), true);

    return 0;
}
