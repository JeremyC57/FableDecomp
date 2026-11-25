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

// Mirror the source's declared types.
class CCharString
{ public:
    void* buf;
    CCharString(const char* s, long len);
};

// Local stand-in for the retail ctor 0x0099ebf0: record the args so the test
// can prove GetName built the string from the fixed literal with len -1.
static const char* g_lastPtr = 0;
static int g_lastLen = 999;

CCharString::CCharString(const char* s, long len)
{
    g_lastPtr = s;
    g_lastLen = len;
    this->buf = (void*)s;
}

struct CCombatSequenceWhisperSortYourselfOut
{
    void* vfptr;
    CCharString GetName() const;
};

CCharString CCombatSequenceWhisperSortYourselfOut::GetName() const
{
    return CCharString("COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT", -1);
}

int main()
{
    CCombatSequenceWhisperSortYourselfOut obj;
    obj.vfptr = 0;
    CCharString r = obj.GetName();

    bool ok = (RetailTextEquals(g_lastPtr, "COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT"))
           && (g_lastLen == -1)
           && (RetailTextEquals(r.buf, "COMBAT_SEQUENCE_WHISPER_SORT_YOURSELF_OUT"));

    if (ok)
        printf("WHISPER_GETNAME_OK\n");
    else
        printf("FAIL ptr=%p len=%d\n", (void*)g_lastPtr, g_lastLen);
    return ok ? 0 : 1;
}