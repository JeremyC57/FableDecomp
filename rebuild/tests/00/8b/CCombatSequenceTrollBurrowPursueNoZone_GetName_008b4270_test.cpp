#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString {
public:
    CCharString(const char* s, long len);
    char* m_data;
};

// Local stand-in for the retail ctor at 0x99ebf0: record the args so the
// test can observe what GetName forwarded.
static const char* g_lastStr = 0;
static int g_lastLen = 0;

CCharString::CCharString(const char* s, long len) {
    g_lastStr = s;
    g_lastLen = len;
    m_data = (char*)s;
}

class CCombatSequence_TrollBurrowPursueNoZone {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollBurrowPursueNoZone::GetName() const {
    return CCharString("COMBAT_SEQUENCE_TROLL_BURROW_PURSUE_NO_ZONE", -1);
}

int main() {
    CCombatSequence_TrollBurrowPursueNoZone obj;
    CCharString r = obj.GetName();
    bool ok = (RetailTextEquals(g_lastStr, "COMBAT_SEQUENCE_TROLL_BURROW_PURSUE_NO_ZONE"))
           && (g_lastLen == -1)
           && (RetailTextEquals(r.m_data, "COMBAT_SEQUENCE_TROLL_BURROW_PURSUE_NO_ZONE"));
    if (ok) {
        printf("GETNAME_TROLLBURROW_OK\n");
        return 0;
    }
    printf("GETNAME_TROLLBURROW_FAIL\n");
    return 1;
}