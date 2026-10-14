// CActionSummonerLightningAttackTarget::InitParticleMorphs(long morphType) -- scales the particle morphs by 1000 / the morph definition's rate, initialises them from the thing's morph table, then activates the action
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C36430, length 71 bytes.
// Callees: CParticleMorphs::SetScale @ 0x00C45980 (__thiscall(float), the "_Cons_val" label is
// propagated), CParticleMorphs::Init @ 0x00C461E0 (__thiscall(long, void*), labelled
// "CMatrix::InterpolateViewport"), CActionSummonerLightningAttackTarget::OnActivate @ 0x00C36340.
// Shape: `fld [__real@1000.0] ; fdiv [rate]` is 1000.0f / rate, the quotient is spilled into the
// argument slot (`push ecx` reserves it, `fstp [esp]`); the pointer chain this->owner->thing->def->
// morphDef->+0x70 is walked once, and this->owner->thing->+0xc8 is pushed after the argument.
struct CParticleMorphDef {
    unsigned char _pad[0x70];
    float         m_rate;                               // +0x70
};

struct CThingDef {
    void*              _unused;
    CParticleMorphDef* m_pMorphDef;                     // +0x04
};

struct CThing {
    CThingDef*    m_pDef;                               // +0x00
    unsigned char _pad[0xc8 - 0x04];
    void*         m_pMorphTable;                        // +0xc8
};

struct CScriptOwner {
    CThing* m_pThing;                                   // +0x00
};

struct CParticleMorphs {
    unsigned char _body[0x4];
    void SetScale(float scale);                         // 0x00C45980
    void Init(long morphType, void* morphTable);        // 0x00C461E0
};

extern const float g_milliseconds;                      // __real@447a0000 (1000.0f) @ 0x01252370

struct CActionSummonerLightningAttackTarget {
    CScriptOwner*   m_pOwner;                           // +0x00
    unsigned char   _pad[0x4c - 0x04];
    CParticleMorphs m_particleMorphs;                   // +0x4c
    void OnActivate();                                  // 0x00C36340
    void InitParticleMorphs(long morphType);
};

void CActionSummonerLightningAttackTarget::InitParticleMorphs(long morphType)
{
    m_particleMorphs.SetScale(1000.0f / m_pOwner->m_pThing->m_pDef->m_pMorphDef->m_rate);
    m_particleMorphs.Init(morphType, m_pOwner->m_pThing->m_pMorphTable);
    OnActivate();
}
