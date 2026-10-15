#include <cstdio>
#include <cstring>
// Behaviour of 0x009CBD50 (chunk 901 piece): forwards to the +0xec object's slot 0 only when it is set
static long g_sum = -1; static int g_hits = 0;
struct CQuestionTarget_Retail {

    virtual void vfunc_0(long a0, long a1) { ++g_hits; g_sum = a0+a1; }
};
struct CScriptedThingOwner_Retail {
    unsigned char _pad_0x00[0xec];
    CQuestionTarget_Retail* m_pTarget;
    void func_009cbd50(long a0, long a1);
};
void CScriptedThingOwner_Retail::func_009cbd50(long a0, long a1) { if (m_pTarget) m_pTarget->vfunc_0(a0, a1); }
int main()
{
    CQuestionTarget_Retail t; CScriptedThingOwner_Retail o; o.m_pTarget = 0;
    o.func_009cbd50(10, 20);
    o.m_pTarget = &t; o.func_009cbd50(10, 20);
    if (g_hits != 1 || g_sum != 30) { std::printf("009cbd50_TEST FAIL\n"); return 1; }
    std::printf("009cbd50_TEST PASS\n"); return 0;
}