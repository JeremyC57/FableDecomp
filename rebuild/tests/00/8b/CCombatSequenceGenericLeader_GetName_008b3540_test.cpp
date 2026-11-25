#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString { public:
    char* p;
    CCharString(const char* s, long len);
};

struct CCombatSequence_GenericLeader {
    CCharString GetName() const;
};

static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* s, long len) {
    g_lastStr = s;
    g_lastLen = len;
    p = (char*)s;
}

CCharString CCombatSequence_GenericLeader::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_GENERIC_LEADER", -1);
}

int main()
{
    CCombatSequence_GenericLeader obj;
    CCharString r = obj.GetName();
    bool ok = (RetailTextEquals(g_lastStr, "COMBAT_SEQUENCE_GENERIC_LEADER"))
           && (g_lastLen == -1)
           && (RetailTextEquals(r.p, "COMBAT_SEQUENCE_GENERIC_LEADER"));
    if (ok) printf("008b3540_TEST PASS\n");
    else    printf("008b3540_TEST FAIL str=%p len=%d p=%p\n", g_lastStr, g_lastLen, r.p);
    return 0;
}