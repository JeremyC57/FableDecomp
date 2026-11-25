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
    int d;
    CCharString(const char* s, long n);
};

// Fake engine ctor: record the args it was called with.
static const char* g_last_s;
static int         g_last_n;
CCharString::CCharString(const char* s, long n) {
    g_last_s = s;
    g_last_n = n;
    this->d = (int)s;
}

class CTCDiscovery { public:
    CCharString GetClassTypeName();
};

CCharString CTCDiscovery::GetClassTypeName() {
    return CCharString("CTCDiscovery", -1);
}

int main() {
    CTCDiscovery a;
    CCharString r = a.GetClassTypeName();
    if (RetailTextEquals(g_last_s, "CTCDiscovery") && g_last_n == -1 && r.d == (int)g_last_s) {
        std::printf("00750a40_TEST PASS\n");
    } else {
        std::printf("FAIL s=%p n=%d d=%x\n", g_last_s, g_last_n, r.d);
    }
    return 0;
}