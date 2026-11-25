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

static const char* g_lastStr = 0;
static int         g_lastLen = 999;

CCharString::CCharString(const char* s, long len)
{
    g_lastStr = s;
    g_lastLen = len;
    p = (char*)s;
}

struct CCombatSequence_TrollDoubleSwipeNonMelee {
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollDoubleSwipeNonMelee::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_TROLL_DOUBLE_SWIPE_NON_MELEE", -1);
}

int main()
{
    CCombatSequence_TrollDoubleSwipeNonMelee obj;
    CCharString r = obj.GetName();

    bool ok = (g_lastLen == -1)
           && (RetailTextEquals(g_lastStr, "COMBAT_SEQUENCE_TROLL_DOUBLE_SWIPE_NON_MELEE"))
           && (RetailTextEquals(r.p, "COMBAT_SEQUENCE_TROLL_DOUBLE_SWIPE_NON_MELEE"));

    if (ok) {
        printf("GETNAME_TROLL_OK\n");
        return 0;
    }
    printf("GETNAME_TROLL_FAIL len=%d\n", g_lastLen);
    return 1;
}