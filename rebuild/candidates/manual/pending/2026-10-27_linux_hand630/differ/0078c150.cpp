// CTCBerserk::PlayHitEffectAtPos(const C3DVector& position) -- spawn one of the two berserk hit effects (picked at random) from the berserk def's special-effects sub-def at the position
// Grade: DIFFER(192v192) -- frame 0xc v 8: retail lets the effect-name CCharString reuse the dead GetSubDef out-slot word (ours gives it its own word, every lea 4 higher), and retail pops ebx after the CreateParticleEffect call (ours before the known test's je) | tried (chunk 630): function-scope out slot + pointer copy (fixed the release to retail's dec [esi+4] ; jne, 202->192), block-scoped slot / inline helper slot (both give the long mov/lea/dec/test/mov release), release spellings (nested if, split --/test, Release() accessor, unsigned count), CCharString temporaries in the call (190) | next: lane gap: dead-slot reuse of a function-scope out slot by a later temporary
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078C150, length 192 bytes, `ret 4`.
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) @ 0x00669580 (on PDef, +0x68),
// CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetPRandomSeed @ 0x0049D810, GFFloatRandom(float, unsigned long*) @ 0x004978A0
// (__fastcall), CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString
// @ 0x00669D30, the __fastcall particle-effect spawner @ 0x0072AB70 (manifest "CreateParticleEffect"), the def's virtual
// Destroy (slot 1). Literals "BERSERK_HIT_EFFECT_01" 0x0126BC1C / "_02" 0x0126BC04; 0.5f 0x0122F59C.
// Layout: Ego_r.pdb CTCBerserk PDef +0x6C sits at retail +0x68 (12-byte DependentThings vector).
// Shape: the landed UndeadReturnToEarth 0x008EC320 lookup: a FUNCTION-SCOPE out slot whose def is copied into a named pointer
// (esi) and released by hand at the end with the `p && --p->count == 0` form -- that gives retail's `dec [esi+4] ; jne`
// (a block-scoped or inline-helper slot gives the long mov/lea/dec/test/mov release); the two arms are complete statements
// that VC7.1 tail-merges after the string constructor, the lookup result riding in bl across the string's destructor.
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
    if (def && --def->m_refCount == 0)
        def->Destroy();
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
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    bool found = PDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found) {
        bool known;
        if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) > 0.5f)
            known = effects->GetEffectFromString("BERSERK_HIT_EFFECT_01", effectType);
        else
            known = effects->GetEffectFromString("BERSERK_HIT_EFFECT_02", effectType);
        if (known)
            CreateParticleEffect(effectType, &position);
    }
    ReleaseDef(effects);
}
