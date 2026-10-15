#include <cstdio>
#include <cstring>
// Behaviour of 0x009CBD30 (chunk 901 piece): forwards to the +0xec object's slot 1 only when it is set
static long g_sum = -1; static int g_hits = 0;
struct CQuestionTarget_Retail {
    virtual long v00() { return 0; }
    virtual void vfunc_1(long a0) { ++g_hits; g_sum = a0; }
};
struct CScriptedThingOwner_Retail {
    unsigned char _pad_0x00[0xec];
    CQuestionTarget_Retail* m_pTarget;
    void func_009cbd30(long a0);
};
void CScriptedThingOwner_Retail::func_009cbd30(long a0) { if (m_pTarget) m_pTarget->vfunc_1(a0); }
int main()
{
    CQuestionTarget_Retail t; CScriptedThingOwner_Retail o; o.m_pTarget = 0;
    o.func_009cbd30(10);
    o.m_pTarget = &t; o.func_009cbd30(10);
    if (g_hits != 1 || g_sum != 10) { std::printf("009cbd30_TEST FAIL\n"); return 1; }
    std::printf("009cbd30_TEST PASS\n"); return 0;
}
