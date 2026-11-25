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
    int   len;
    CCharString(const char* s, long n) : p((char*)s), len(n) {}
};

struct CTCGraphicAppearanceStaticMesh {
    int dummy;
    CCharString GetClassTypeName();
};

CCharString CTCGraphicAppearanceStaticMesh::GetClassTypeName()
{
    return CCharString("CTCGraphicAppearanceStaticMesh", -1);
}

int main() {
    CTCGraphicAppearanceStaticMesh a; a.dummy = 7;
    CCharString r = a.GetClassTypeName();
    if (RetailTextEquals(r.p, "CTCGraphicAppearanceStaticMesh") && r.len == -1) {
        printf("0077bd00_TEST PASS\n");
    } else {
        printf("FAIL %p %d\n", (void*)r.p, r.len);
    }
    return 0;
}