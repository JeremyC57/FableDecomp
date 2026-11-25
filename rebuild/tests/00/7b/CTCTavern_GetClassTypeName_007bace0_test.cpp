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
    const char* p;
    int n;
    CCharString(const char* s, long len) : p(s), n(len) {}
};

class CTCTavern { public:
    char pad;
    CCharString GetClassTypeName();
};

CCharString CTCTavern::GetClassTypeName()
{
    return CCharString("CTCTavern", -1);
}

int main()
{
    CTCTavern obj;
    CCharString r = obj.GetClassTypeName();
    if (RetailTextEquals(r.p, "CTCTavern") && r.n == -1) {
        std::printf("007bace0_TEST PASS\n");
    } else {
        std::printf("FAIL p=%p n=%d\n", (void*)r.p, r.n);
    }
    return 0;
}