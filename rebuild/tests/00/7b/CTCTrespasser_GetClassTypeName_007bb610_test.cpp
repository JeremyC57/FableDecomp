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
    char* m_data;
    int   m_len;
    int   m_ref;
    CCharString(const char* s, long len) { m_data = (char*)s; m_len = len; m_ref = 0; }
};

static const char* const kActionNameLit = "CTCTrespasser";

class CTCTrespasser { public:
    CCharString GetClassTypeName();
};

CCharString CTCTrespasser::GetClassTypeName()
{
    return CCharString(kActionNameLit, -1);
}

int main()
{
    CTCTrespasser obj;
    CCharString r = obj.GetClassTypeName();
    if (RetailTextEquals(r.m_data, "CTCTrespasser") && r.m_len == -1)
        std::printf("007bb610_TEST PASS\n");
    else
        std::printf("FAIL data=%p len=%d\n", (void*)r.m_data, r.m_len);
    return 0;
}