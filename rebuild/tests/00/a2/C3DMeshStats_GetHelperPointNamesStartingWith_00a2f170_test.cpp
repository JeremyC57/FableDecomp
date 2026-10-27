// Test for 0x00A2F170 C3DMeshStats::GetHelperPointNamesStartingWith: unpacks the helper-point (not dummy-object) names with the prefix and sort flag passed through.
#include <stdio.h>
#include <string.h>
static const char* g_helpers[] = { "HDL_A", "HDL_B", "FX_C" };
static const char* g_dummies[] = { "HDL_D" };
class C3DMeshStats {
public:
    void GetHelperPointNamesStartingWith(const char** names, int* count, const char* prefix, bool sort) const
    { UnpackNames(names, count, false, prefix, sort); }
    mutable int LastSort;
private:
    void UnpackNames(const char** names, int* count, bool dummyObjects, const char* prefix, bool sort) const
    {
        const char** src = dummyObjects ? g_dummies : g_helpers; int n = dummyObjects ? 1 : 3;
        *count = 0; LastSort = sort ? 1 : 0;
        for (int i = 0; i < n; ++i) if (strncmp(src[i], prefix, strlen(prefix)) == 0) names[(*count)++] = src[i];
    }
};
int main()
{
    C3DMeshStats s; const char* out[4]; int n = 0;
    s.GetHelperPointNamesStartingWith(out, &n, "HDL", true);
    bool ok = n == 2 && strcmp(out[0], "HDL_A") == 0 && strcmp(out[1], "HDL_B") == 0 && s.LastSort == 1;
    puts(ok ? "00a2f170_TEST PASS" : "00a2f170_TEST FAIL");
    return ok ? 0 : 1;
}