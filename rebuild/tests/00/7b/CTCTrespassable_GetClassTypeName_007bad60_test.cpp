#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

// Model of CCharString: records the (ptr,len) it was constructed with.
static const char* g_lastPtr = 0;
static int g_lastLen = 999;

class CCharString
{
public:
    CCharString(const char* pText, long length)
    {
        g_lastPtr = pText;
        g_lastLen = length;
    }
};

// The literal the retail function feeds to the ctor.
static const char* const kActionNameLiteral =
    "CTCTrespassable";

class CTCTrespassable
{
public:
    CCharString GetClassTypeName();
};

CCharString CTCTrespassable::GetClassTypeName()
{
    return CCharString(kActionNameLiteral, -1);
}

int main()
{
    CTCTrespassable obj;
    obj.GetClassTypeName();

    bool ok = (RetailTextEquals(g_lastPtr, "CTCTrespassable")) &&
              (g_lastLen == -1);

    if (ok)
        std::printf("007bad60_TEST PASS\n");
    else
        std::printf("007bad60_TEST FAIL ptr=%p len=%d\n", (void*)g_lastPtr, g_lastLen);
    return 0;
}