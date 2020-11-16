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

enum STRCONV_ERROR str2int(int *i, char const *s, int base) {
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

enum STRCONV_ERROR str2uint(uint32_t *i, char const *s, int base) {
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

enum STRCONV_ERROR str2float(float *i, char const *s) {
    float f;
    char *end = NULL;

    errno = 0;
    f = strtof(s, &end);

    if ((errno == ERANGE && f == HUGE_VAL)) {
        return CONV_OVERFLOW;
    }

    if ((errno == ERANGE && f == 0)) {
        return CONV_UNDERFLOW;
    }

    if (*s == '\0' || *end != '\0') {
        return CONV_INCONVERTIBLE;
    }

    *i = f;
    return CONV_SUCCESS;
}

enum STRCONV_ERROR str2double(double *i, char const *s) {
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

enum STRCONV_ERROR str2int64(int64_t *i, char const *s, int base) {
    char *end;
    long long l;

    errno = 0;
    l = strtoll(s, &end, base);

    if ((errno == ERANGE && l == LLONG_MAX) || l > LLONG_MAX) {
        return CONV_OVERFLOW;
    }

    if ((errno == ERANGE && l == LLONG_MIN) || l < LLONG_MIN) {
        return CONV_UNDERFLOW;
    }

    if (*s == '\0' || *end != '\0') {
        return CONV_INCONVERTIBLE;
    }

    *i = l;
    return CONV_SUCCESS;
}

enum STRCONV_ERROR str2uint64(uint64_t *i, char const *s, int base) {
    char *end;
    unsigned long long l;

    errno = 0;
    l = strtoull(s, &end, base);

    if ((errno == ERANGE && l == ULLONG_MAX) || l > ULLONG_MAX) {
        return CONV_OVERFLOW;
    }

    if (*s == '\0' || *end != '\0') {
        return CONV_INCONVERTIBLE;
    }

    *i = l;
    return CONV_SUCCESS;
}

std::ostream &operator<<(std::ostream &out, const STRCONV_ERROR &error) {
    switch (error) {
        case CONV_SUCCESS:
            out << "success" << endl;
            break;

        case CONV_OVERFLOW:
            out << "overflow" << endl;
            break;

        case CONV_UNDERFLOW:
            out << "underflow" << endl;
            break;

        case CONV_INCONVERTIBLE:
            out << "inconvertible" << endl;
            break;

        default	:
            out << "unknow status" << endl;
            break;
    }

    return out;
}

/* struct parse_rule operator== mothed */
bool operator== (const xmlParseRule &lhs, const xmlParseRule &hhs) {
    return ((lhs.m_key == hhs.m_key) && \
            (lhs.m_valueType == hhs.m_valueType) && \
            (lhs.m_defValue.d64 == hhs.m_defValue.d64) && \
            (lhs.m_valuePtr.d64 == hhs.m_valuePtr.d64) && \
            (lhs.m_charValue == hhs.m_charValue) && \
            (lhs.m_charDefault == hhs.m_charDefault) && \
            (lhs.m_charValueSize == hhs.m_charValueSize));
}

/* Overloaded operator<< */
std::ostream &operator<<(std::ostream &out, const xmlParseRule &rule) {
    out << "key:\t" << rule.m_key << "\tvalue\t";

    switch (rule.m_valueType) {
        case xmlParseRule::Str:
            out << rule.m_charValue << endl;
            break;

        case xmlParseRule::F32:
            out.precision(6);
            out << *rule.m_valuePtr.f32 << endl;
            break;

        case xmlParseRule::D64:
            out.precision(12);
            out << *rule.m_valuePtr.d64 << endl;
            break;

        case xmlParseRule::I32:
            out << *rule.m_valuePtr.i32 << endl;
            break;

        case xmlParseRule::I64:
            out << *rule.m_valuePtr.i64 << endl;
            break;

        case xmlParseRule::U32:
            out << *rule.m_valuePtr.u32 << endl;
            break;

        case xmlParseRule::U64:
            out << *rule.m_valuePtr.u64 << endl;
            break;

        default:
            out << "None" << endl;
    }

    return out;
}

bool xmlParseRule::check(void) {
    /* Check key */
    if (!m_key.size()) {
        cerr << "Rule error: key is empty!" << endl;
        return false;
    }

    /* Check value type */
    if (m_valueType == None) {
        cerr << "Rule error: value type is None" << endl;
        return false;
    }

    /*	Check value target */
    if (!m_charValue && !m_valuePtr.d64) {
        cerr << "Rule error: value is invalid!" << endl;
        return false;
    }

    return true;
}

bool xmlParseRule::parse(const char *value) {
    enum STRCONV_ERROR result;

    switch (m_valueType) {
        case Str:
            snprintf(m_charValue, m_charValueSize, "%s", value);
            break;

        case I32:
            if ((result = str2int(m_valuePtr.i32, value)) != CONV_SUCCESS) {
                cerr << "Parse[" << m_key << "]\t value is error: " << result << endl;
                return false;
            }

            break;

        case U32:
            if ((result = str2uint(m_valuePtr.u32, value)) != CONV_SUCCESS) {
                cerr << "Parse[" << m_key << "]\t value is error: " << result << endl;
                return false;
            }

            break;

        case I64:
            if ((result = str2int64(m_valuePtr.i64, value)) != CONV_SUCCESS) {
                cerr << "Parse[" << m_key << "]\t value is error: " << result << endl;
                return false;
            }

            break;

        case U64:
            if ((result = str2uint64(m_valuePtr.u64, value)) != CONV_SUCCESS) {
                cerr << "Parse[" << m_key << "]\t value is error: " << result << endl;
                return false;
            }

            break;

        case F32:
            if ((result = str2float(m_valuePtr.f32, value)) != CONV_SUCCESS) {
                cerr << "Parse[" << m_key << "]\t value is error: " << result << endl;
                return false;
            }

            break;

        case D64:
            if ((result = str2double(m_valuePtr.d64, value)) != CONV_SUCCESS) {
                cerr << "Parse[" << m_key << "]\t value is error: " << result << endl;
                return false;
            }

            break;

        default:
            return false;
    }

    return true;
}

void xmlParseRule::loadDefaultValue(void) {
    switch (m_valueType) {
        case Str:
            snprintf(m_charValue, m_charValueSize, "%s", m_charDefault.c_str());
            break;

        case F32:
            *m_valuePtr.f32 = m_defValue.f32;
            break;

        case D64:
            *m_valuePtr.d64 = m_defValue.d64;
            break;

        case I32:
            *m_valuePtr.i32 = m_defValue.i32;
            break;

        case I64:
            *m_valuePtr.i64 = m_defValue.i64;
            break;

        case U32:
            *m_valuePtr.u32 = m_defValue.u32;
            break;

        case U64:
            *m_valuePtr.u64 = m_defValue.u64;
            break;

        default:
            return;
    }
}


xmlParser::xmlParser(const string &desc) {
    /* 	Set parser name */
    parserDesc = desc;

    /* 	Clear parser rules */
    attrParserRules.clear();
    valueParserRules.clear();
}

void xmlParser::addRule(const xmlParseRule &rule) {
    valueParserRules.push_back(rule);
}

void xmlParser::addAttrRule(const xmlParseRule &rule) {
    attrParserRules.push_back(rule);
}

bool xmlParser::checkRules(const XMLElement *root, const ParserRules &rules) {
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


std::ostream &operator<<(std::ostream &out, const xmlParser &parser) {
    ParserRules::const_iterator rule_it;

    out << "=============================================================" << endl;
    out << "Name:\t" << parser.parserDesc << endl;

    if (parser.valueParserRules.size() != 0) {
        out << "Value parser rules num: " << parser.valueParserRules.size() << endl;

        for (rule_it = parser.valueParserRules.begin(); rule_it != parser.valueParserRules.end(); rule_it++) {
            out << *rule_it << endl;
        }
    }

    if (parser.attrParserRules.size()) {
        out << "Attribute parser rules num: " << parser.attrParserRules.size() << endl;

        for (rule_it = parser.attrParserRules.begin(); rule_it != parser.attrParserRules.end(); rule_it++) {
            out << *rule_it << endl;
        }
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
bool xmlParser::parseValue(XMLElement *root, bool debug, ostream &output) {
    XMLElement *key = NULL;
    const char *value = NULL;
    ParserRules::iterator rule;

    /* Check if parser is ready */
    if (!checkRules(root, valueParserRules)) {
        return false;
    }

    /* Parser each rule form valueParserRules */
    for (rule = valueParserRules.begin(); rule != valueParserRules.end(); rule++) {
        /*	Check rule */
        if (!rule->check())
            return false;

        /* 	Find key */
        if (!(key = root->FirstChildElement(rule->key()))) {
            rule->loadDefaultValue();
            continue;
        }

        /* 	Find key value ? */
        if (!(value = key->ToElement()->GetText())) {
            rule->loadDefaultValue();
            continue;
        }

        /* Parse rule */
        if (!rule->parse(value)) {
            return false;
        }
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
bool xmlParser::parseAttr(XMLElement *root, bool debug, ostream &output) {
    const char *attr = NULL;
    ParserRules::iterator rule;

    /* Check parser is ready */
    if (!checkRules(root, attrParserRules)) {
        return false;
    }

    /* Parser each rule from attrParserRules */
    for (rule = attrParserRules.begin(); rule != attrParserRules.end(); rule++) {
        /* Check rule */
        if (!rule->check()) {
            return false;
        }

        /* Find attribute */
        if ((attr = root->Attribute(rule->key())) == NULL) {
            rule->loadDefaultValue();
            continue;
        }

        /* Parse attrValue */
        if (!rule->parse(attr)) {
            return false;
        }
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
