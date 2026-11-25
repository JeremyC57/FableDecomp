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

class CCharString { public:
    char* m_data;
    CCharString() { m_data = 0; }
    CCharString(const char* s, long n) { g_last_str = s; g_last_len = n; m_data = (char*)s; }
    CCharString(const CCharString& o) { m_data = o.m_data; }
    ~CCharString() {}
};

struct CCombatSequenceWhisperAttackBigCombo {
    virtual CCharString GetName();
};

CCharString CCombatSequenceWhisperAttackBigCombo::GetName()
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_ATTACK_COMBO", -1);
}

int main()
{
    CCombatSequenceWhisperAttackBigCombo obj;
    obj.GetName();

    bool ok = (RetailTextEquals(g_last_str, "COMBAT_SEQUENCE_WHISPER_ATTACK_COMBO")) && (g_last_len == -1);
    if (ok) {
        printf("GETNAME_BIGCOMBO_OK len=-1\n");
        return 0;
    }
    printf("GETNAME_FAIL\n");
    return 1;
}