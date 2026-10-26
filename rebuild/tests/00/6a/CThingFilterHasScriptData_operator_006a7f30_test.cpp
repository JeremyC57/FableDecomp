// Behaviour test for 0x006A7F30 CThingFilter_HasScriptData::operator()
#include <cstdio>
#include <cstring>
class CCharStringData { public: const char* m_pChars; unsigned long m_length; };
class CCharString { public: CCharStringData* m_pStringData; };
static int Compare_00411570(const char* a, const char* b) { return std::strcmp(a, b); }
static bool operator==(const CCharString& sa, const CCharString& sb)
{
    CCharStringData* a = sa.m_pStringData;
    CCharStringData* b = sb.m_pStringData;
    if (a == b) return true;
    if (!a || !b) return false;
    if (a->m_length != b->m_length) return false;
    return Compare_00411570(a->m_pChars, b->m_pChars) == 0;
}
class CThing { public: CCharString m_scriptData; };
class CThingFilter_HasScriptData { public:
    CCharString Data;
    bool operator()(CThing* thing) const { return thing->m_scriptData == Data; }
};
int main()
{
    CCharStringData d1 = { "abc", 3 }, d2 = { "abc", 3 }, d3 = { "abd", 3 }, d4 = { "ab", 2 };
    CThing t; CThingFilter_HasScriptData f;
    bool ok = true;
    t.m_scriptData.m_pStringData = 0; f.Data.m_pStringData = 0; if (!f(&t)) ok = false;   // both empty
    f.Data.m_pStringData = &d1; if (f(&t)) ok = false;                                  // one null
    t.m_scriptData.m_pStringData = &d2; if (!f(&t)) ok = false;                         // equal text
    t.m_scriptData.m_pStringData = &d3; if (f(&t)) ok = false;
    t.m_scriptData.m_pStringData = &d4; if (f(&t)) ok = false;                          // length differs
    if (!ok) { std::printf("006a7f30_TEST FAIL\n"); return 1; }
    std::printf("006a7f30_TEST PASS\n");
    return 0;
}