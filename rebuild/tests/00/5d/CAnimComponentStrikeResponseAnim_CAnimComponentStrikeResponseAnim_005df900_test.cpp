#include <cstdio>
static int g_strings = 0;
class CCharString { public: void* PStringData; CCharString() : PStringData(0) { ++g_strings; } };
class CAnimComponentBase { public: virtual void v00() { } unsigned long m_refCount; CAnimComponentBase() : m_refCount(1) { } };
class CAnimComponentStrikeResponseAnim : public CAnimComponentBase { public: CCharString ResponseAnimName; CAnimComponentStrikeResponseAnim() { } };
int main()
{
    CAnimComponentStrikeResponseAnim a;
    if (sizeof(a) != 0xc || a.m_refCount != 1 || g_strings != 1 || a.ResponseAnimName.PStringData != 0 || (char*)&a.ResponseAnimName != (char*)&a + 8) { std::printf("005df900_TEST FAIL\n"); return 1; }
    std::printf("005df900_TEST PASS\n"); return 0;
}