// Behaviour test for 00c59ba0: CCodaVoice::CCodaVoice -- base links cleared, +0x2c = 0x80000000
#include <cstdio>
#include <cstring>
static int g_base = 0;
class CPooledNode { public: CPooledNode() : m_pPrev(0), m_pNext(0) { ++g_base; } virtual void v00() { } CPooledNode* m_pPrev; CPooledNode* m_pNext; };
class CCodaVoice : public CPooledNode {
public:
    CCodaVoice() : m_x2c(0x80000000) { }
    unsigned char _pad0c[0x28 - 0x0c]; unsigned long m_flags; unsigned long m_x2c; unsigned char _pad30[0xb4 - 0x30];
};
int main()
{
    CCodaVoice v;
    int ok = g_base == 1 && v.m_pPrev == 0 && v.m_pNext == 0 && v.m_x2c == 0x80000000UL && sizeof(CCodaVoice) == 0xb4;
    std::printf(ok ? "00c59ba0_TEST PASS\n" : "00c59ba0_TEST FAIL\n");
    return ok ? 0 : 1;
}
