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
    /* Constructor */
    xmlParseRule(const char *k, char *v, size_t size, const string &def = "") :
        m_key(k), m_valueType(Str), m_charValue(v), m_charDefault(def), m_charValueSize(size) {
    }

    xmlParseRule(const char *k, float *v, float def = 0.0) : m_key(k), m_valueType(F32) {
        m_valuePtr.f32 = v, m_defValue.f32 = def;
    }

    xmlParseRule(const char *k, double *v, double def = 0.0) : m_key(k), m_valueType(D64) {
        m_valuePtr.d64 = v, m_defValue.d64 = def;
    }

    xmlParseRule(const char *k, int32_t *v, int32_t def = 0) : m_key(k), m_valueType(I32) {
        m_valuePtr.i32 = v, m_defValue.i32 = def;
    }

    xmlParseRule(const char *k, int64_t *v, int64_t def = 0) : m_key(k), m_valueType(I64) {
        m_valuePtr.i64 = v, m_defValue.i64 = def;
    }

    xmlParseRule(const char *k, uint32_t *v, uint32_t def = 0) : m_key(k), m_valueType(U32) {
        m_valuePtr.u32 = v, m_defValue.u32 = def;
    }

    xmlParseRule(const char *k, uint64_t *v, uint64_t def = 0) : m_key(k), m_valueType(U64) {
        m_valuePtr.u64 = v, m_defValue.u64 = def;
    }

    const char *key() const {
        return m_key.c_str();
    }

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
    string m_key;

    enum {
        None, Str, F32, D64, I32, I64, U32, U64
    } m_valueType;

    union {
        float *f32;
        double *d64;
        int32_t *i32;
        int64_t *i64;
        uint32_t *u32;
        uint64_t *u64;
    } m_valuePtr;

    union {
        float f32;
        double d64;
        int32_t i32;
        int64_t i64;
        uint32_t u32;
        uint64_t u64;
    } m_defValue;

    /* Data */
    char *m_charValue;
    string m_charDefault;
    size_t m_charValueSize;
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
enum STRCONV_ERROR {
    CONV_SUCCESS,
    CONV_OVERFLOW,
    CONV_UNDERFLOW,
    CONV_INCONVERTIBLE
};

enum STRCONV_ERROR str2float(float *i, char const *s);
enum STRCONV_ERROR str2double(double *i, char const *s);

enum STRCONV_ERROR str2int (int *i, char const *s, int base = 0);
enum STRCONV_ERROR str2uint (uint32_t *i, char const *s, int base = 0);

enum STRCONV_ERROR str2int64(int64_t *i, char const *s, int base = 0);
enum STRCONV_ERROR str2uint64(uint64_t *i, char const *s, int base = 0);

#endif
