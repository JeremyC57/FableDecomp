#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

static const char* g_lastS;
static int g_lastLen;

class CCharString { public:
    void* p;
    CCharString(const char* s, long len);
};

// Local stand-in for the ctor at 0x0099ebf0.
CCharString::CCharString(const char* s, long len)
{
    g_lastS = s;
    g_lastLen = len;
    p = (void*)s;
}

struct CCombatSequence_TrollOpeningGroundPound {
    CCharString GetName() const;
};

CCharString CCombatSequence_TrollOpeningGroundPound::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_TROLL_OPENING_GROUND_POUND", -1);
}

int main()
{
    CCombatSequence_TrollOpeningGroundPound obj;
    CCharString r = obj.GetName();
    bool ok = (RetailTextEquals(g_lastS, "COMBAT_SEQUENCE_TROLL_OPENING_GROUND_POUND"))
           && (g_lastLen == -1)
           && (RetailTextEquals(r.p, "COMBAT_SEQUENCE_TROLL_OPENING_GROUND_POUND"));
    if (ok) printf("GETNAME_GROUNDPOUND_OK\n");
    else    printf("FAIL s=%p len=%d p=%p\n", g_lastS, g_lastLen, r.p);
    return ok ? 0 : 1;
}