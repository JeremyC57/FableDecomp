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
    CCharString(const char* s, long n);
    const char* m_data;
};

// local definition of the CCharString(const char*, long) ctor for the test
CCharString::CCharString(const char* s, long n)
{
    (void)n;
    m_data = s;
}

class CCombatSequence_TrollRockIdle {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_TrollRockIdle::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_TROLL_ROCK_IDLE", -1);
}

int main()
{
    CCombatSequence_TrollRockIdle obj;
    CCharString r = obj.GetName();
    if (RetailTextEquals(r.m_data, "COMBAT_SEQUENCE_TROLL_ROCK_IDLE")) {
        printf("TROLLROCKIDLE_GETNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}