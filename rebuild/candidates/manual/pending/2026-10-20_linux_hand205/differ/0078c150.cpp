// CTCBerserk::PlayHitEffectAtPos(const C3DVector& position) -- spawn one of the two berserk hit effects (picked at random) from the berserk def's special-effects sub-def at the position
// Grade: DIFFER(ours v retail) -- frame 0xc v 8: the effect-name strings do not reuse the block-scoped GetSubDef out word (QFE sibling-scope slot sharing, wave 23d), and the def release is the long mov/lea/dec/test/mov form in esi
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078C150, length 192 bytes, `ret 4`.
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) @ 0x00669580 (on PDef, +0x68),
// CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetPRandomSeed @ 0x0049D810, GFFloatRandom(float, unsigned long*) @ 0x004978A0
// (__fastcall), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString
// @ 0x00669D30, the __fastcall particle-effect spawner @ 0x0072AB70 (manifest "CreateParticleEffect"), the def's virtual
// Destroy (slot 1). Literals "BERSERK_HIT_EFFECT_01" 0x0126BC1C / "_02" 0x0126BC04; 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CTCBerserk PDef +0x6C sits at retail +0x68 (12-byte DependentThings vector).
// Shape: the TeleportIn/UndeadReturnToEarth sub-def lookup (0x008EC320): the out slot is block-scoped, so each arm's effect-name
// string reuses its frame word; the def is copied into esi and released by hand at the end; the two arms are complete
// statements that VC7.1 tail-merges after the string constructor, the lookup result riding in bl across the string's destructor.
struct C3DVector { float X, Y, Z; };
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
struct CSpecialEffectsDef : CDefBase {
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> struct CDefPointer {         // the out slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def) {
        if (--def->m_refCount == 0)
            def->Destroy();
    }
}
struct CDefClassBase : CDefBase {
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
struct CWorld {
    unsigned long* GetPRandomSeed();            // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);    // 0x004978A0
struct CThing;
CThing* __fastcall CreateParticleEffect(long type, const C3DVector* position);   // 0x0072AB70

class CTCBerserk {
public:
    virtual void v00();
    unsigned char _body04[0x68 - 4];
    CDefClassBase* PDef;                        // +0x68
    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)
    void PlayHitEffectAtPos(const C3DVector& position);
};

void CTCBerserk::PlayHitEffectAtPos(const C3DVector& position)
{
    CSpecialEffectsDef* effects;
    bool found;
    long effectType = 0;
    {
        CDefPointer<CSpecialEffectsDef> effectsSlot;
        found = PDef->GetSubDef(effectsSlot);
        effects = effectsSlot.m_p;
    }
    if (found) {
        bool known;
        if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) > 0.5f) {
            CCharString name("BERSERK_HIT_EFFECT_01");
            known = effects->GetEffectFromString(name, effectType);
        } else {
            CCharString name("BERSERK_HIT_EFFECT_02");
            known = effects->GetEffectFromString(name, effectType);
        }
        if (known)
            CreateParticleEffect(effectType, &position);
    }
    ReleaseDef(effects);
}
