#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// ---- reproduce the source under test (self-contained) ----
class CCharString
{
public:
    CCharString(const char* str, long len);
    const char* data() const { return m_data; }
private:
    char* m_data;
};

// Local stand-in for CCharString::CCharString(const char*, long) @ 0x0099ebf0.
static const char* g_lastStr = 0;
static int g_lastLen = 0;
CCharString::CCharString(const char* str, long len)
{
    g_lastStr = str;
    g_lastLen = len;
    m_data = (char*)str;
}

class CCombatSequenceNonInterruptable2StrikeCombo
{
public:
    virtual CCharString GetName() const;
};

CCharString CCombatSequenceNonInterruptable2StrikeCombo::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_NON_INTERRUPTABLE_TWO_STRIKE", -1);
}

int main()
{
    CCombatSequenceNonInterruptable2StrikeCombo obj;
    CCharString s = obj.GetName();

    bool ok = true;
    if (!RetailTextEquals(g_lastStr, "COMBAT_SEQUENCE_NON_INTERRUPTABLE_TWO_STRIKE")) ok = false;
    if (g_lastLen != -1) ok = false;
    if (!RetailTextEquals(s.data(), "COMBAT_SEQUENCE_NON_INTERRUPTABLE_TWO_STRIKE")) ok = false;

    if (ok)
        printf("GETNAME_2STRIKECOMBO_OK\n");
    else
        printf("GETNAME_FAIL str=%p len=%d\n", (void*)g_lastStr, g_lastLen);

    return ok ? 0 : 1;
}