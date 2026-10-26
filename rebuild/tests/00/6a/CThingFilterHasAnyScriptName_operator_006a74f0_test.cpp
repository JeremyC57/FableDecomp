// Behaviour test for CThingFilter_HasAnyScriptName::operator() (0x006A74F0)
#include <cstdio>
#include <cstring>

class CCharString { public:
    const char* m_text;
    bool operator!=(const char* text) const { return std::strcmp(m_text ? m_text : "", text) != 0; }
};
class CThing { public: CCharString m_scriptName; };
class CThingFilter_HasAnyScriptName { public:
    bool operator()(const CThing& thing) const
    {
        const CCharString* name = &thing.m_scriptName;
        if (*name != "" && *name != "NULL")
            return true;
        return false;
    }
};
#define CHECK(c) if (!(c)) { std::printf("006a74f0_TEST FAIL\n"); return 1; }
int main()
{
    CThing t; CThingFilter_HasAnyScriptName f;
    t.m_scriptName.m_text = 0;      CHECK(!f(t));
    t.m_scriptName.m_text = "";     CHECK(!f(t));
    t.m_scriptName.m_text = "NULL"; CHECK(!f(t));
    t.m_scriptName.m_text = "Hero"; CHECK(f(t));
    std::printf("006a74f0_TEST PASS\n");
    return 0;
}