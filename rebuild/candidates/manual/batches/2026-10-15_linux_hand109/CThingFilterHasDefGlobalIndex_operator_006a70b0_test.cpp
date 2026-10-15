// Behaviour test for CThingFilter_HasDefGlobalIndex::operator() (0x006A70B0)
#include <cstdio>
#include <cstring>

struct CThing { unsigned short m_defGlobalIndex; };
struct CThingFilter_HasDefGlobalIndex {
    long m_defGlobalIndex;
    bool operator()(const CThing& thing) const { return thing.m_defGlobalIndex == m_defGlobalIndex; }
};
#define CHECK(c) if (!(c)) { std::printf("006a70b0_TEST FAIL\n"); return 1; }
int main()
{
    CThing t; t.m_defGlobalIndex = 0xfffe;
    CThingFilter_HasDefGlobalIndex f = { 0xfffe };
    CHECK(f(t));
    f.m_defGlobalIndex = -2;                        // the word is zero-extended: no match with a negative long
    CHECK(!f(t));
    t.m_defGlobalIndex = 5; f.m_defGlobalIndex = 5;
    CHECK(f(t));
    std::printf("006a70b0_TEST PASS\n");
    return 0;
}
