#include <stdio.h>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString
{
public:
    CCharString(const char* s, long n);
    void* m_data;
};

class CCombatSequence_BanditKingSpinAround
{
public:
    virtual CCharString GetName() const;
};

// Local stub of the CCharString ctor so the test links standalone.
static const char* g_seen_s = 0;
static int g_seen_n = 0;
CCharString::CCharString(const char* s, long n)
{
    g_seen_s = s;
    g_seen_n = n;
    m_data = (void*)0xABCD;
}

CCharString CCombatSequence_BanditKingSpinAround::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND", -1);
}

int main()
{
    CCombatSequence_BanditKingSpinAround obj;
    CCharString r = obj.GetName();

    int ok = 1;
    if (!RetailTextEquals(g_seen_s, "COMBAT_SEQUENCE_BANDIT_KING_SPIN_AROUND")) ok = 0;
    if (g_seen_n != -1) ok = 0;
    if (r.m_data != (void*)0xABCD) ok = 0;

    if (ok)
        printf("BANDITKING_GETNAME_OK\n");
    else
        printf("FAIL s=%p n=%d\n", g_seen_s, g_seen_n);
    return ok ? 0 : 1;
}