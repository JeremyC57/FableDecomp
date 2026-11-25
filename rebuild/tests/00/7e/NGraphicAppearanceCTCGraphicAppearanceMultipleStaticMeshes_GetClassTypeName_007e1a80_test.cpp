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

// Independent behaviour test for CTCGraphicAppearanceMultipleStaticMeshes::GetActionName.
// We re-declare a self-contained CCharString whose ctor records the literal
// pointer + length it was constructed with, then confirm the factory forwards
// the expected (literal, -1) pair and returns the same buffer.

class CCharString { public:
    const char* rec_s;
    int rec_len;
    CCharString(const char* s, long len) { rec_s = s; rec_len = len; }
};

struct CTCGraphicAppearanceMultipleStaticMeshes {
    CCharString GetClassTypeName();
};

CCharString CTCGraphicAppearanceMultipleStaticMeshes::GetClassTypeName()
{
    return CCharString("CTCGraphicAppearanceMultipleStaticMeshes", -1);
}

int main()
{
    CTCGraphicAppearanceMultipleStaticMeshes obj;
    CCharString r = obj.GetClassTypeName();
    bool ok = (RetailTextEquals(r.rec_s, "CTCGraphicAppearanceMultipleStaticMeshes")) && (r.rec_len == -1);
    if (ok) {
        printf("GETACTIONNAME_OK\n");
        return 0;
    }
    printf("GETACTIONNAME_FAIL s=%p len=%d\n", r.rec_s, r.rec_len);
    return 1;
}