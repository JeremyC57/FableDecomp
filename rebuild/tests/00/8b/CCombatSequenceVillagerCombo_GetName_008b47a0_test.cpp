#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Standalone behaviour test. Reimplement CCharString locally so we can observe
// that GetName constructs from the fixed literal pointer with length -1.

static const char* g_last_str = 0;
static int         g_last_len = 999;

class CCharString {
public:
    CCharString(const char* s, long n) { g_last_str = s; g_last_len = n; }
};

class CCombatSequence_VillagerCombo {
public:
    CCharString GetName() const;
};

CCharString CCombatSequence_VillagerCombo::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_VILLAGER_COMBO_ATTACK", -1);
}

int main()
{
    CCombatSequence_VillagerCombo obj;
    obj.GetName();

    bool ok = (RetailTextEquals(g_last_str, "COMBAT_SEQUENCE_VILLAGER_COMBO_ATTACK")) && (g_last_len == -1);
    if (ok) {
        printf("GETNAME_OK len=-1\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}