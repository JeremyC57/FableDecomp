#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// ---- standalone model of the CCharString + ctor + method under test ----

class CCharString
{ public:
    const char* p;
    int         len;
    void Construct(const char* s, int length);   // stand-in for retail 0x0099ebf0
};

void CCharString::Construct(const char* s, int length)
{
    p   = s;
    len = length;
}

struct CCombatSequenceWhisperBoast
{
    virtual CCharString GetName() const;
};

CCharString CCombatSequenceWhisperBoast::GetName() const
{
    CCharString result;
    result.Construct("COMBAT_SEQUENCE_WHISPER_BOAST", -1);
    return result;
}

int main()
{
    CCombatSequenceWhisperBoast obj;
    CCharString r = obj.GetName();

    bool ok = (RetailTextEquals(r.p, "COMBAT_SEQUENCE_WHISPER_BOAST")) && (r.len == -1);

    if (ok)
        printf("WHISPERBOAST_GETNAME_OK\n");
    else
        printf("FAIL p=%p len=%d\n", (void*)r.p, r.len);

    return ok ? 0 : 1;
}