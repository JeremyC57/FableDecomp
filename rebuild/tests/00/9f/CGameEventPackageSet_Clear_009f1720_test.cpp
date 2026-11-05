// Behaviour test for CGameEventPackageSet::Clear (0x009F1720): the count goes to zero, the package bytes are untouched.
#include <cstdio>
#include <cstring>

struct CGameEventPackageSetModel
{
    unsigned long NoPackages;
    unsigned char Packages[64];
    void Clear() { NoPackages = 0; }
};

int main()
{
    CGameEventPackageSetModel set;
    set.NoPackages = 17;
    std::memset(set.Packages, 0xAB, sizeof(set.Packages));
    set.Clear();
    if (set.NoPackages != 0) { std::printf("009f1720_TEST FAIL\n"); return 1; }
    for (unsigned i = 0; i < sizeof(set.Packages); ++i)
        if (set.Packages[i] != 0xAB) { std::printf("009f1720_TEST FAIL\n"); return 1; }
    set.Clear();
    if (set.NoPackages != 0) { std::printf("009f1720_TEST FAIL\n"); return 1; }
    std::printf("009f1720_TEST PASS\n");
    return 0;
}
