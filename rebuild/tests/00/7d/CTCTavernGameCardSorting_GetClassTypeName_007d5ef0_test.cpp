#include <cstdio>

// NF410: the row spells retail's literal now, so compare the text it passes, not a retail address.
static bool RetailTextEquals(const void* p, const char* want)
{
    const char* s = (const char*)p;
    if (s == 0) return false;
    while (*want != 0) { if (*s != *want) return false; ++s; ++want; }
    return *s == 0;
}

class CCharString {
public:
    CCharString(const char* str, long len);
    void* m_data;
};

// Local standalone impl of the ctor for the behaviour test.
CCharString::CCharString(const char* str, long len) {
    m_data = (void*)str;
    (void)len;
}

class CTCTavernGameCardSorting {
public:
    CCharString GetClassTypeName();
};

CCharString CTCTavernGameCardSorting::GetClassTypeName()
{
    return CCharString("CTCTavernGameCardSorting", -1);
}

int main() {
    CTCTavernGameCardSorting act;
    CCharString r = act.GetClassTypeName();
    if (RetailTextEquals(r.m_data, "CTCTavernGameCardSorting"))
        printf("OK name=%p\n", r.m_data);
    else
        printf("FAIL name=%p\n", r.m_data);
    return 0;
}