// Behaviour test for 0x00c40750 CEffectOwner_00c40750::Reset (catalog: CScreenEffectPriorityStack::Reset)
#include <cstdio>

struct CManager {
    long status; int calls; void* made;
    long CreateEffect(void** handle) { ++calls; if (status == 0) *handle = made; return status; }
};
static CManager g_manager; static CManager* g_pManager_0143bb5c = &g_manager;
static unsigned long g_flags_0143bc28 = 0x12;

struct SEffectProperties {
    long m_reserved0, m_reserved1; float m_scaleA; long m_reserved2, m_reserved3; float m_scaleB; long m_levelA;
    long m_reserved4, m_reserved5; float m_scaleC, m_ratio, m_scaleD; long m_levelB; float m_delayA; long m_levelC;
    float m_delayB, m_size, m_scaleE, m_scaleF, m_timeA; long m_reserved6; float m_timeB; long m_reserved7;
    long m_referenceHz, m_reference2, m_reserved8, m_mode;
};
struct CEffectOwner {
    SEffectProperties m_properties; unsigned long m_dirtyFlags; void* m_pEffect;
    long Reset()
    {
        long result = g_pManager_0143bb5c->CreateEffect(&m_pEffect);
        if (result != 0)
            return result;
        SEffectProperties& p = m_properties;
        p.m_reserved0 = 0; p.m_reserved1 = 0; p.m_scaleA = 1.0f; p.m_reserved2 = 0; p.m_reserved3 = 0; p.m_scaleB = 1.0f;
        p.m_levelA = -10000; p.m_reserved4 = 0; p.m_reserved5 = 0; p.m_scaleC = 1.0f; p.m_ratio = 0.5f; p.m_scaleD = 1.0f;
        p.m_levelB = -10000; p.m_delayA = 0.02f; p.m_levelC = -10000; p.m_delayB = 0.04f; p.m_size = 7.5f; p.m_scaleE = 1.0f;
        p.m_scaleF = 1.0f; p.m_timeA = 0.075f; p.m_reserved6 = 0; p.m_timeB = 0.04f; p.m_reserved7 = 0; p.m_referenceHz = 5000;
        p.m_reference2 = 200; p.m_reserved8 = 0; p.m_mode = -5;
        m_dirtyFlags = 0xfffff;
        g_flags_0143bc28 = 0x1000000;
        return 0;
    }
};

int main()
{
    int dummy;
    CEffectOwner o; o.m_dirtyFlags = 0; o.m_pEffect = 0; o.m_properties.m_levelA = 7;
    g_manager.status = -5; g_manager.calls = 0; g_manager.made = &dummy;
    if (o.Reset() != -5 || o.m_properties.m_levelA != 7 || o.m_dirtyFlags != 0 || g_flags_0143bc28 != 0x12 || o.m_pEffect != 0) {
        std::printf("00c40750_TEST FAIL\n"); return 1;
    }
    g_manager.status = 0;
    long r = o.Reset();
    float half = 0.5f, size = 7.5f;
    if (r != 0 || o.m_pEffect != &dummy || o.m_dirtyFlags != 0xfffff || g_flags_0143bc28 != 0x1000000 || o.m_properties.m_levelA != -10000
        || o.m_properties.m_levelC != -10000 || o.m_properties.m_ratio != half || o.m_properties.m_size != size
        || o.m_properties.m_referenceHz != 5000 || o.m_properties.m_mode != -5 || g_manager.calls != 2) {
        std::printf("00c40750_TEST FAIL\n"); return 1;
    }
    std::printf("00c40750_TEST PASS\n");
    return 0;
}
