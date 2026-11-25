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

class CCharString { public:
    char* p;
    CCharString(const char* s, long n);
};

struct CCombatSequenceWhisperTooCloseLeapBack {
    void* vtbl;
    CCharString GetName() const;
};

// Local stand-in for the real CCharString ctor at 0x99ebf0.
static const char* g_lastStr;
static int g_lastN;
CCharString::CCharString(const char* s, long n)
{
    g_lastStr = s;
    g_lastN = n;
    p = (char*)s;
}

CCharString CCombatSequenceWhisperTooCloseLeapBack::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_TOO_CLOSE_LEAP_BACK", -1);
}

int main()
{
    CCombatSequenceWhisperTooCloseLeapBack obj;
    obj.vtbl = 0;
    CCharString r = obj.GetName();
    if (RetailTextEquals(g_lastStr, "COMBAT_SEQUENCE_WHISPER_TOO_CLOSE_LEAP_BACK") && g_lastN == -1 && RetailTextEquals(r.p, "COMBAT_SEQUENCE_WHISPER_TOO_CLOSE_LEAP_BACK")) {
        printf("LEAPBACK_GETNAME_OK\n");
        return 0;
    }
    printf("FAIL\n");
    return 1;
}