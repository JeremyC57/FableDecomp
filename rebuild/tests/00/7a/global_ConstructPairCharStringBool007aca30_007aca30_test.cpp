#include <cstdio>
#include <new>
static int g_strCopies = 0;
class CCharString { public:
    const char* m_pData;
    CCharString(const char* s) : m_pData(s) { }
    CCharString(const CCharString& other) : m_pData(other.m_pData) { ++g_strCopies; }
};
struct CCharStringBoolPair {
    const CCharString first;
    bool second;
};
void Construct_PairCharStringBool_007aca30(CCharStringBoolPair* p, const CCharStringBoolPair& value)
{
    if (p) new (p) CCharStringBoolPair(value);   // VC null-tests the placement result itself
}
int main()
{
    CCharStringBoolPair src = { CCharString("key"), true };
    static double storage[4];
    CCharStringBoolPair* dst = (CCharStringBoolPair*)(void*)storage;
    Construct_PairCharStringBool_007aca30(dst, src);
    if (g_strCopies != 1 || dst->first.m_pData != src.first.m_pData || !dst->second) { std::printf("007aca30_TEST FAIL\n"); return 1; }
    Construct_PairCharStringBool_007aca30(0, src);
    if (g_strCopies != 1) { std::printf("007aca30_TEST FAIL\n"); return 1; }
    std::printf("007aca30_TEST PASS\n");
    return 0;
}