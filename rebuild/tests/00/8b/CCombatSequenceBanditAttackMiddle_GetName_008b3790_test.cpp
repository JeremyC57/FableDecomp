#include <stdio.h>

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
    const char* s;
    int n;
};

// Local definition of the ctor for standalone link.
CCharString::CCharString(const char* str, long num) {
    s = str;
    n = num;
}

class CCombatSequence_BanditAttackMiddle {
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequence_BanditAttackMiddle::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_BANDIT_ATTACK_MIDDLE", -1);
}

int main() {
    CCombatSequence_BanditAttackMiddle obj;
    CCharString r = obj.GetName();
    if (RetailTextEquals(r.s, "COMBAT_SEQUENCE_BANDIT_ATTACK_MIDDLE") && r.n == -1) {
        printf("GETNAME_PASS\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}