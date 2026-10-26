#include <cstdio>
#include <cstdlib>
// Behaviour test for 0x004E3CB3: the array block is freed (only when present), then the base destructor runs.
static int g_freed = 0, g_baseDtor = 0;
static void FreeBlock(void* p) { ++g_freed; std::free(p); }
class CDefIndex { public: long m_index; };
struct CArrayDefIndex {
    CDefIndex* m_pData; long m_count; long m_capacity;
    ~CArrayDefIndex() { if (m_pData) FreeBlock(m_pData); }
};
struct CBase_009fc550 { virtual void v00() {} ~CBase_009fc550() { ++g_baseDtor; } };
class CDefPointeeBase : public CBase_009fc550 { public: virtual void v00() {} ~CDefPointeeBase() {} };
class CContainerRewardHeroDef : public CDefPointeeBase { public:
    CArrayDefIndex ObjectFamilies;
    virtual void v00() {}
    ~CContainerRewardHeroDef();
};
CContainerRewardHeroDef::~CContainerRewardHeroDef()
{
}
int main()
{
    {
        CContainerRewardHeroDef d; d.ObjectFamilies.m_pData = (CDefIndex*)std::malloc(4 * sizeof(CDefIndex)); d.ObjectFamilies.m_count = 4;
    }
    if (g_freed != 1 || g_baseDtor != 1) { std::printf("004e3cb3_TEST FAIL\n"); return 1; }
    {
        CContainerRewardHeroDef d; d.ObjectFamilies.m_pData = 0;
    }
    if (g_freed != 1 || g_baseDtor != 2) { std::printf("004e3cb3_TEST FAIL\n"); return 1; }
    std::printf("004e3cb3_TEST PASS\n");
    return 0;
}