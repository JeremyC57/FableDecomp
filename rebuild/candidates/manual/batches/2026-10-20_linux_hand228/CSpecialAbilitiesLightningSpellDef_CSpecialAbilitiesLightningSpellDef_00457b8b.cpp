// CSpecialAbilitiesLightningSpellDef::CSpecialAbilitiesLightningSpellDef() -- base CParentDefClassBase(), own vtable, the sixteen retail vectors built empty and the four sound CDefStrings set to -1
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00457B8B, length 243 bytes.
// Callees: CParentDefClassBase::CParentDefClassBase @ 0x00430370. Vtable 0x0123460C = CSpecialAbilitiesLightningSpellDef.
// Layout: Ego_r.pdb names; the TLC def root ends at +0x3c (PDB 0x40) and retail STLport vectors are 12 bytes (PDB 16):
// m_faAreaEffect +0x3c ... m_faStaminaPerSecond +0xc0, the four beam angles +0xcc..+0xd8, the decapitation/delay vectors
// +0xdc..+0x100, CombatMultiplierIncreaseTime +0x10c, the CDefStrings +0x110..+0x11c, the theme floats +0x120/+0x124 (0x128 bytes).
// Shape: size-optimised def module (`#pragma optimize("s", on)`): the code generator forms `lea ecx,[esi+N]` only where it is
// shorter than three direct stores, so the five vectors that fit disp8 (+0x3c..+0x74) are stored directly and the rest go
// through ecx; the four -1 CDefStrings share `or eax,-1`. Local model: engine/CParentDefClassBase.h is a flat generated layout.
#pragma optimize("s", on)
struct CParentDefClassBase {
    virtual void v00();
    unsigned char _base[0x38];                  // +0x04..+0x3b: the def root's own members
    CParentDefClassBase();                      // 0x00430370
};

struct CDefString {                             // def-table string index, -1 = none
    long TablePos;
    CDefString() : TablePos(-1) { }
};

template <class T>
struct CAllocProxy {                            // STLport _STLP_alloc_proxy: the end-of-storage pointer with its own ctor
    T* p;
    CAllocProxy(T* v) : p(v) { }
};
template <class T>
struct CRetailVector {                          // STLport _Vector_base: _M_start / _M_finish / _M_end_of_storage
    T* first;
    T* last;
    CAllocProxy<T> end;
    CRetailVector() : first(0), last(0), end(0) { }
};

struct CSpecialAbilitiesLightningSpellDef : CParentDefClassBase {
    CRetailVector<float> m_faAreaEffect;                // +0x3c
    CRetailVector<float> m_faDamagePerHit;              // +0x48
    CRetailVector<float> m_faSubBeamDamagePerHit;       // +0x54
    CRetailVector<float> m_faActivationTime;            // +0x60
    CRetailVector<float> m_faDeActivationTime;          // +0x6c
    CRetailVector<long>  m_naMinMainBeams;              // +0x78
    CRetailVector<long>  m_naMaxMainBeams;              // +0x84
    CRetailVector<long>  m_naMinSubBeams;               // +0x90
    CRetailVector<long>  m_naMaxSubBeams;               // +0x9c
    CRetailVector<float> m_faTemporaryBeamLife;         // +0xa8
    CRetailVector<float> m_faTargettingFOV;             // +0xb4
    CRetailVector<float> m_faStaminaPerSecond;          // +0xc0
    float m_fRandomBeamMinAngleX;                       // +0xcc
    float m_fRandomBeamMaxAngleX;                       // +0xd0
    float m_fRandomBeamMinAngleZ;                       // +0xd4
    float m_fRandomBeamMaxAngleZ;                       // +0xd8
    CRetailVector<float> m_faMainBeamDecapitationTime;  // +0xdc
    CRetailVector<float> m_faSubBeamDecapitationTime;   // +0xe8
    CRetailVector<float> m_faSubBeamDelay;              // +0xf4
    CRetailVector<float> OnhitEveryNSecs;               // +0x100
    float CombatMultiplierIncreaseTime;                 // +0x10c
    CDefString InitSound;                               // +0x110
    CDefString LoopingSound;                            // +0x114
    CDefString EndHeroSound;                            // +0x118
    CDefString EndAttackeeSound;                        // +0x11c
    float LightningThemeTransitionInSecs;               // +0x120
    float LightningThemeTransitionOutSecs;              // +0x124
    virtual void v00();                                 // own vtable 0x0123460C
    CSpecialAbilitiesLightningSpellDef();
};

CSpecialAbilitiesLightningSpellDef::CSpecialAbilitiesLightningSpellDef()
    : CParentDefClassBase()
{
}
