#include <cstdio>

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
};

struct CCombatSequenceWhisperAdvance {
    int dummy;
};

static int   g_len   = 0;
static const char* g_str = 0;
static CCharString* g_self = 0;

CCharString* __fastcall CCharString_ctor(CCharString* self, int /*edx*/, const char* s, int len)
{
    g_self = self;
    g_str  = s;
    g_len  = len;
    self->p = (char*)s;
    return self;
}

static const char* const kName = "COMBAT_SEQUENCE_WHISPER_ADVANCE";

CCharString* __fastcall GetName_CCombatSequenceWhisperAdvance(
        const CCombatSequenceWhisperAdvance* self, int edx, CCharString* __ret)
{
    CCharString_ctor(__ret, 0, kName, -1);
    return __ret;
}

int main()
{
    CCombatSequenceWhisperAdvance obj;
    obj.dummy = 7;
    CCharString ret;
    ret.p = 0;

    CCharString* r = GetName_CCombatSequenceWhisperAdvance(&obj, 0, &ret);

    bool ok = (r == &ret)
           && (g_self == &ret)
           && (g_len == -1)
           && (g_str == kName)
           && (ret.p == (char*)kName)
           && RetailTextEquals(g_str, "COMBAT_SEQUENCE_WHISPER_ADVANCE");

    if (ok) {
        printf("GETNAME_WHISPER_ADVANCE_OK\n");
        return 0;
    }
    printf("FAIL r=%p self=%p len=%d\n", (void*)r, (void*)g_self, g_len);
    return 1;
}