// Behaviour test for CTCObjectAugmentations::RemoveAugmentationFromSlot @ 0x007681D0: release the slot's def, then refresh twice in order.
#include <cstdio>
static int g_destroyed = 0, g_step = 0, g_attachStep = 0, g_morphStep = 0;
struct CDefBase {
    long m_refCount;
    virtual void Destroy() { ++g_destroyed; }
    virtual ~CDefBase() {}
};
struct CDefPtr {
    CDefBase* m_pDef;
    void Release() { if (m_pDef) { CDefBase* def = m_pDef; if (--def->m_refCount == 0) def->Destroy(); m_pDef = 0; } }
    CDefPtr& operator=(int) { Release(); m_pDef = 0; return *this; }
};
class CTCObjectAugmentations { public:
    CDefPtr* m_pAugmentationsFirst;
    void AttachAugmentationParticles() { g_attachStep = ++g_step; }
    void UpdateParticleMorphs() { g_morphStep = ++g_step; }
    void RemoveAugmentationFromSlot(long slot) { m_pAugmentationsFirst[slot] = 0; AttachAugmentationParticles(); UpdateParticleMorphs(); }
};
int main()
{
    CDefBase a, b; a.m_refCount = 1; b.m_refCount = 2;
    CDefPtr slots[3]; slots[0].m_pDef = &a; slots[1].m_pDef = &b; slots[2].m_pDef = 0;
    CTCObjectAugmentations o; o.m_pAugmentationsFirst = slots;
    o.RemoveAugmentationFromSlot(1);
    if (slots[1].m_pDef != 0 || b.m_refCount != 1 || g_destroyed != 0 || g_attachStep != 1 || g_morphStep != 2) { std::printf("007681d0_TEST FAIL\n"); return 1; }
    o.RemoveAugmentationFromSlot(0);
    if (slots[0].m_pDef != 0 || g_destroyed != 1) { std::printf("007681d0_TEST FAIL\n"); return 1; }
    o.RemoveAugmentationFromSlot(2);
    if (slots[2].m_pDef != 0 || g_destroyed != 1 || g_step != 6) { std::printf("007681d0_TEST FAIL\n"); return 1; }
    std::printf("007681d0_TEST PASS\n");
    return 0;
}