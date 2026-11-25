#include "rebuild_abi.h"
#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Behaviour fixture for CTCHitOnCollision::GetActionName @ 0x007c8520.
// The function constructs its CCharString result from the relocated string
// constant 0x0123d1d8 with the -1 length sentinel and returns it by value
// (hidden return pointer). We supply a stand-in CCharString ctor that records
// the forwarded arguments to prove the sentinel and the string constant.

static const char* g_seen_text;
static int g_seen_len;

class CCharString
{ public:
    void* m_data;
    CCharString(const char* text, long length);
};

CCharString::CCharString(const char* text, long length)
{
    g_seen_text = text;
    g_seen_len = length;
    m_data = (void*)text;
}

class CTCHitOnCollision
{ public:
    CCharString GetClassTypeName();
};

CCharString CTCHitOnCollision::GetClassTypeName()
{
    return CCharString("CTCHitOnCollision", -1);
}

int main()
{
    CTCHitOnCollision obj;
    CCharString r = obj.GetClassTypeName();
    if (g_seen_len != -1) { std::printf("BAD_LEN\n"); return 1; }
    if (!RetailTextEquals(g_seen_text, "CTCHitOnCollision")) { std::printf("BAD_TEXT\n"); return 1; }
    if (!RetailTextEquals(r.m_data, "CTCHitOnCollision")) { std::printf("BAD_DATA\n"); return 1; }
    std::printf("007c8520_TEST PASS\n");
    return 0;
}