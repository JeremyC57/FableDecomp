// CEffectOwner_00c40750::Reset() -- create the owner's audio effect object through the manager singleton and reset its 27-word property block to the defaults (catalog label CScreenEffectPriorityStack::Reset is propagated)
// Leaf: Reset
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C40750, length 173 bytes (`ret`, long status in eax).
// Label: NOT NScreenEffect::CScreenEffectPriorityStack (whose Reset is the landed 0x00880A40): this is the object of the
// landed neighbours CTextureOwner_00c40800::ClearErrorMessage / CProjectionOwner_00c40830::SetProjection (same +0x0c
// 27-word block, +0x78 dirty mask, +0x7c handle, manager 0x0143BB5C, flag word 0x0143BC28) in the 0x00C3xxxx-0x00C4xxxx
// audio library. The block's values (three -10000 mB levels, a 7.5 size, a 5000 reference) look like an EAX/I3DL2
// environment, but the layout matches no SDK struct, so the members are named by kind (level, scale, delay...) with the zero-defaulted words as m_reservedN.
// Callees: the manager's create(void** handle) @ 0x00C3E640 (allocates a 0x824-byte object; returns 0 or -5).
// Shape: the status is returned straight through when non-zero (the zero register of the test is reused for every zero
// store); on success the defaults are written in member order (VC groups the 1.0f / -10000 / 0.04f register stores), all
// twenty dirty bits are raised, the global flag word is SET to bit 24 and 0 is returned.
struct CManager_00c3e730 {
    long CreateEffect(void** handle);                  // 0x00C3E640
};
extern CManager_00c3e730* g_pManager_0143bb5c;         // 0x0143BB5C
extern unsigned long g_flags_0143bc28;                 // 0x0143BC28

struct SEffectProperties_00c40750 {                    // the 0x6c-byte block at +0x0c
    long  m_reserved0;          // +0x0c (0)
    long  m_reserved1;          // +0x10 (0)
    float m_scaleA;          // +0x14 (1.0)
    long  m_reserved2;          // +0x18 (0)
    long  m_reserved3;          // +0x1c (0)
    float m_scaleB;          // +0x20 (1.0)
    long  m_levelA;          // +0x24 (-10000 mB)
    long  m_reserved4;          // +0x28 (0)
    long  m_reserved5;          // +0x2c (0)
    float m_scaleC;          // +0x30 (1.0)
    float m_ratio;           // +0x34 (0.5)
    float m_scaleD;          // +0x38 (1.0)
    long  m_levelB;          // +0x3c (-10000 mB)
    float m_delayA;          // +0x40 (0.02)
    long  m_levelC;          // +0x44 (-10000 mB)
    float m_delayB;          // +0x48 (0.04)
    float m_size;            // +0x4c (7.5)
    float m_scaleE;          // +0x50 (1.0)
    float m_scaleF;          // +0x54 (1.0)
    float m_timeA;           // +0x58 (0.075)
    long  m_reserved6;          // +0x5c (0)
    float m_timeB;           // +0x60 (0.04)
    long  m_reserved7;          // +0x64 (0)
    long  m_referenceHz;     // +0x68 (5000)
    long  m_reference2;      // +0x6c (200)
    long  m_reserved8;          // +0x70 (0)
    long  m_mode;            // +0x74 (-5)
};

struct CEffectOwner_00c40750 {
    unsigned char              _pad00[0x0c];
    SEffectProperties_00c40750 m_properties;           // +0x0c
    unsigned long              m_dirtyFlags;           // +0x78
    void*                      m_pEffect;              // +0x7c
    long Reset();
};

long CEffectOwner_00c40750::Reset()
{
    long result = g_pManager_0143bb5c->CreateEffect(&m_pEffect);
    if (result != 0)
        return result;
    SEffectProperties_00c40750& p = m_properties;
    p.m_reserved0 = 0;
    p.m_reserved1 = 0;
    p.m_scaleA = 1.0f;
    p.m_reserved2 = 0;
    p.m_reserved3 = 0;
    p.m_scaleB = 1.0f;
    p.m_levelA = -10000;
    p.m_reserved4 = 0;
    p.m_reserved5 = 0;
    p.m_scaleC = 1.0f;
    p.m_ratio = 0.5f;
    p.m_scaleD = 1.0f;
    p.m_levelB = -10000;
    p.m_delayA = 0.02f;
    p.m_levelC = -10000;
    p.m_delayB = 0.04f;
    p.m_size = 7.5f;
    p.m_scaleE = 1.0f;
    p.m_scaleF = 1.0f;
    p.m_timeA = 0.075f;
    p.m_reserved6 = 0;
    p.m_timeB = 0.04f;
    p.m_reserved7 = 0;
    p.m_referenceHz = 5000;
    p.m_reference2 = 200;
    p.m_reserved8 = 0;
    p.m_mode = -5;
    m_dirtyFlags = 0xfffff;
    g_flags_0143bc28 = 0x1000000;
    return 0;
}