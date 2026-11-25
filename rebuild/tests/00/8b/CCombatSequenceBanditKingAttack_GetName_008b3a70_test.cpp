#include <cstdio>
#include <cstring>

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
    CCharString(const char* s, long n) {
        // record what the ctor was called with
        g_last_ptr = s;
        g_last_n = n;
        m_data = (char*)s;
    }
    char* m_data;
    static const char* g_last_ptr;
    static int g_last_n;
};
const char* CCharString::g_last_ptr = 0;
int CCharString::g_last_n = 0;

struct CActionDoCreatureAction;

class CCombatSequence_BanditKingAttack {
public:
    virtual CCharString GetName() const;
    int filler;
};

CCharString CCombatSequence_BanditKingAttack::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_KING_ATTACK", -1);
}

int main()
{
    CCombatSequence_BanditKingAttack obj;
    CCharString r = obj.GetName();
    bool ok = (RetailTextEquals(CCharString::g_last_ptr, "COMBAT_SEQUENCE_BANDIT_KING_ATTACK"))
           && (CCharString::g_last_n == -1)
           && (RetailTextEquals(r.m_data, "COMBAT_SEQUENCE_BANDIT_KING_ATTACK"));
    if (ok) printf("PASS_BANDITKINGATTACK_GETNAME\n");
    else    printf("FAIL ptr=%p n=%d\n", CCharString::g_last_ptr, CCharString::g_last_n);
    return ok ? 0 : 1;
}