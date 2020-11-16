#include <stdio.h>
#include "unittest.h"
#include "xmlParser.h"

void test_decode_char(XMLElement *root, XMLElement *attribute) {
    xmlParser parser(__func__);
    char value[32], attr[32], def[32], buffer[4] = {0};

    parser.addRule(xmlParseRule("STRING", value, sizeof(value)));
    parser.addAttrRule(xmlParseRule("string", attr, sizeof(attr)));
    parser.addAttrRule(xmlParseRule("default", def, sizeof(def), "default"));
    parser.addAttrRule(xmlParseRule("default", buffer, sizeof(buffer), "default"));

    UNITTEST_ASSERT_EQ(parser.parseValue(root), true);
    UNITTEST_ASSERT_EQ(parser.parseAttr(attribute), true);
    UNITTEST_ASSERT_STR_EQ(buffer, "def", strlen("def"));
    UNITTEST_ASSERT_STR_EQ(def, "default", strlen("default"));
    UNITTEST_ASSERT_STR_EQ(attr, "xmlParser", strlen("xmlParser"));
    UNITTEST_ASSERT_STR_EQ(value, "xmlParser", strlen("xmlParser"));

    UNITTEST_AUTO_PASS();
}

void test_decode_float(XMLElement *root, XMLElement *attribute) {
    float attr, value, def;
    xmlParser parser(__func__);

    parser.addRule(xmlParseRule("FLOAT", &value));
    parser.addAttrRule(xmlParseRule("float", &attr));
    parser.addAttrRule(xmlParseRule("default", &def, 3.14));

    UNITTEST_ASSERT_EQ(parser.parseValue(root), true);
    UNITTEST_ASSERT_EQ(parser.parseAttr(attribute), true);
    UNITTEST_ASSERT_F32_EQ(def, 3.14);
    UNITTEST_ASSERT_F32_EQ(attr, 3.141592);
    UNITTEST_ASSERT_F32_EQ(value, 1.234567);

    UNITTEST_AUTO_PASS();
}

void test_decode_double(XMLElement *root, XMLElement *attribute) {
    double attr, value, def;
    xmlParser parser(__func__);

    parser.addRule(xmlParseRule("DOUBLE", &value));
    parser.addAttrRule(xmlParseRule("double", &attr));
    parser.addAttrRule(xmlParseRule("default", &def, 1.456789321));

    UNITTEST_ASSERT_EQ(parser.parseValue(root), true);
    UNITTEST_ASSERT_EQ(parser.parseAttr(attribute), true);
    UNITTEST_ASSERT_D64_EQ(def, 1.456789321);
    UNITTEST_ASSERT_D64_EQ(value, 1.23456789);
    UNITTEST_ASSERT_D64_EQ(attr, 3.1415926535);

    UNITTEST_AUTO_PASS();
}

void test_decode_int32_t(XMLElement *root, XMLElement *attribute) {
    xmlParser parser(__func__);
    int32_t dec_attr, oct_attr, hex_attr, num_attr;
    int32_t dec_value, oct_value, hex_value, num_value, def;

    parser.addRule(xmlParseRule("DEC", &dec_value));
    parser.addRule(xmlParseRule("OCT", &oct_value));
    parser.addRule(xmlParseRule("HEX", &hex_value));
    parser.addRule(xmlParseRule("NUM", &num_value));

    parser.addAttrRule(xmlParseRule("dec", &dec_attr));
    parser.addAttrRule(xmlParseRule("oct", &oct_attr));
    parser.addAttrRule(xmlParseRule("hex", &hex_attr));
    parser.addAttrRule(xmlParseRule("num", &num_attr));
    parser.addAttrRule(xmlParseRule("default", &def, -12345));

    UNITTEST_ASSERT_EQ(parser.parseValue(root), true);
    UNITTEST_ASSERT_EQ(parser.parseAttr(attribute), true);

    UNITTEST_ASSERT_EQ(def, -12345);
    UNITTEST_ASSERT_EQ(dec_value, 255);
    UNITTEST_ASSERT_EQ(oct_value, 255);
    UNITTEST_ASSERT_EQ(hex_value, 255);
    UNITTEST_ASSERT_EQ(num_value, 1234);

    UNITTEST_ASSERT_EQ(dec_attr, 255);
    UNITTEST_ASSERT_EQ(oct_attr, 255);
    UNITTEST_ASSERT_EQ(num_attr, 1234);
    UNITTEST_ASSERT_EQ(hex_attr, 151025411);

    UNITTEST_AUTO_PASS();
}

void test_decode_uint32_t(XMLElement *root, XMLElement *attribute) {
    uint32_t attr, value, def;
    xmlParser parser(__func__);

    parser.addRule(xmlParseRule("U32", &value));
    parser.addAttrRule(xmlParseRule("u32", &attr));
    parser.addAttrRule(xmlParseRule("default", &def, 1456789321));

    UNITTEST_ASSERT_EQ(parser.parseValue(root), true);
    UNITTEST_ASSERT_EQ(parser.parseAttr(attribute), true);
    UNITTEST_ASSERT_EQ(def, 1456789321);
    UNITTEST_ASSERT_EQ(value, 0xffffffff);
    UNITTEST_ASSERT_EQ(attr, 353455454);

    UNITTEST_AUTO_PASS();
}

void test_decode_int64_t(XMLElement *root, XMLElement *attribute) {
    int64_t attr, value, def;
    xmlParser parser(__func__);

    parser.addRule(xmlParseRule("I64", &value));
    parser.addAttrRule(xmlParseRule("i64", &attr));
    parser.addAttrRule(xmlParseRule("default", &def, 1456789321243));

    UNITTEST_ASSERT_EQ(parser.parseValue(root), true);
    UNITTEST_ASSERT_EQ(parser.parseAttr(attribute), true);
    UNITTEST_ASSERT_EQ(def, 1456789321243);
    UNITTEST_ASSERT_EQ(value, -1234567891233);
    UNITTEST_ASSERT_EQ(attr, -1234589343434);

    UNITTEST_AUTO_PASS();
}

void test_decode_uint64_t(XMLElement *root, XMLElement *attribute) {
    uint64_t attr, value, def;
    xmlParser parser(__func__);

    parser.addRule(xmlParseRule("U64", &value));
    parser.addAttrRule(xmlParseRule("u64", &attr));
    parser.addAttrRule(xmlParseRule("default", &def, 134456789321));

    UNITTEST_ASSERT_EQ(parser.parseValue(root), true);
    UNITTEST_ASSERT_EQ(parser.parseAttr(attribute), true);
    UNITTEST_ASSERT_EQ(def, 134456789321);
    UNITTEST_ASSERT_EQ(value, 19008000000);
    UNITTEST_ASSERT_EQ(attr, 0x123456789ABC);

    UNITTEST_AUTO_PASS();
}

int main(int argc, char **argv) {
    const char *xmlDoc =
        "<ROOT>"
        "<DEC>255</DEC>"
        "<OCT>0377</OCT>"
        "<HEX>0xff</HEX>"
        "<NUM>1234</NUM>"
        "<U32>0xffffffff</U32>"
        "<I64>-1234567891233</I64>"
        "<U64>19008000000</U64>"
        "<FLOAT>1.234567</FLOAT>"
        "<DOUBLE>1.23456789</DOUBLE>"
        "<STRING>xmlParser</STRING>"
        "<ATTRIBUTE dec = \"255\" oct= \"0377\" hex = \"0x9007703\" num = \"1234\" "
        "u32 = \"353455454\" i64 = \"-1234589343434\" u64 = \"0x123456789ABC\" "
        "float = \"3.141592\" double = \"3.1415926535\" string = \"xmlParser\">0x100</ATTRIBUTE>"
        "</ROOT>";

    XMLDocument xml;
    XMLElement *root = NULL;
    XMLElement *attr = NULL;

    /* Parse from string buffer */
    if (xml.Parse(xmlDoc) != XML_NO_ERROR) {
        fprintf(stderr, "Parse from string error: %s, %s\n", xml.GetErrorStr1(), xml.GetErrorStr2());
        UNITTEST_FAIL("Parse xmlDoc error");
        return -1;
    }

    /* Find root element */
    if (!(root = xml.RootElement())) {
        UNITTEST_FAIL("Parse xmlDoc: do not find xml root node!");
        return -1;
    }

    /* Find attribute element */
    if (!(attr = root->FirstChildElement("ATTRIBUTE"))) {
        UNITTEST_FAIL("Parse xmlDoc: do not find attribute node!");
        return -1;
    }

    /* XML parse test */
    test_decode_char(root, attr);
    test_decode_float(root, attr);
    test_decode_double(root, attr);
    test_decode_int32_t(root, attr);
    test_decode_int64_t(root, attr);
    test_decode_uint32_t(root, attr);
    test_decode_uint64_t(root, attr);

    return 0;
}
