// CTCPhysicalShieldSpell::OnHitAtPos(const C3DVector& position) -- a hit on the shield spawns one of the two PHYSICAL_HIT special effects (a coin toss on the world's random seed) at the hit position, when the shield def's special-effects sub-def maps that name
// Retail: Fable.exe (TLC, VC7.1) @ 0x0079DCC0, 178 bytes (`ret 4`; PDB ?OnHitAtPos@CTCPhysicalShieldSpell@@UAEXABVC3DVector@@@Z).
// Callees: CCharString() @ 0x0099E4B0, CTCBase::GetWorld() @ 0x00686A70, CWorld::GetPRandomSeed() @ 0x0049D810, GFFloatRandom
//   @ 0x004978A0 (`__fastcall`, the float on the stack, the seed in ecx), CCharString::operator=(const char*) @ 0x0099EFE0
//   ("PHYSICAL_HIT1" @ 0x0126C524 / "PHYSICAL_HIT2" @ 0x0126C514), CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString @ 0x00669D30, CTCSpecialAbilities::CreateParticleEffect(long, const C3DVector&)
//   @ 0x0072AB70 (static `__fastcall`, PDB SIPAVCThing@@JABVC3DVector@@@Z), ~CCharString @ 0x0099EAE0. 0.5f @ 0x0122F59C.
// Shape: if/else of the two operator= calls tail-merges into `push A / jmp / push B / call`; the out-slot def and the zero
//   effect type follow the landed 0x008EC320. Tried: `&&` condition, nested if, the call through the slot member, a holder
//   whose destructor releases. Next: lane gap: register choice for the out-slot def copy (chunk 161 0x69A8B0 family).
void ReleaseDef_s7(void* def);   // stand-in: the def's release (count decrement, Destroy on zero)
class C3DVector { public: float X, Y, Z; };
class CThing;
class CCharString { public:
    void* m_pData;
    CCharString();                                      // 0x0099E4B0
    ~CCharString();                                     // 0x0099EAE0
    CCharString& operator=(const char* text);           // 0x0099EFE0
};
class CWorld { public:
    unsigned long* GetPRandomSeed();                    // 0x0049D810
};
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> class CDefPointer { public:                 // the out-slot GetSubDef fills (counted reference handed over)
    T* m_p;
    CDefPointer() : m_p(0) { }
};
template <class T> __forceinline void ReleaseDef(T* def)
{
    if (def && --def->m_refCount == 0)
        def->Destroy();
}
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* FindDef_s7r_0();
#pragma auto_inline(off)
// Same-TU body of GetSubDef (retail instantiates it in this TU): fetch the def, then assign it to the out pointer the way
// CDefPointer::operator= does (read the old pointer, release it when different, store the new one).
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = FindDef_s7r_0();
    if (!def)
        return false;
    void* old = *(void**)&out;
    if (old != def) {
        if (old)
            ReleaseDef_s7(old);
        *(void**)&out = def;
    }
    return true;
}
#pragma auto_inline(on)
class CSpecialAbilitiesPhysicalShieldDef : public CDefClassBase { public: };
class CTCSpecialAbilities { public:
    static CThing* __fastcall CreateParticleEffect(long type, const C3DVector& position);   // 0x0072AB70
};
class CTCBase { public:
    virtual void v00();
    CThing* ParentThing;                                // +0x04
    bool    ToBeRemoved;                                // +0x08
    bool    IsInGlobalUpdateTCs;                        // +0x09
    unsigned char _pad0a[2];
    CWorld* GetWorld();                                 // 0x00686A70
};
class CTCPhysicalShieldSpell : public CTCBase { public:
    CSpecialAbilitiesPhysicalShieldDef* PDef;           // +0x0c CDefPointer<const CSpecialAbilitiesPhysicalShieldDef>::Object
    virtual void OnHitAtPos(const C3DVector& position);
};

void CTCPhysicalShieldSpell::OnHitAtPos(const C3DVector& position)
{
    CCharString effectName;
    if (GFFloatRandom(1.0f, GetWorld()->GetPRandomSeed()) < 0.5f)
        effectName = "PHYSICAL_HIT1";
    else
        effectName = "PHYSICAL_HIT2";
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    bool found = PDef->GetSubDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (found) {
        if (effects->GetEffectFromString(effectName, effectType))
            CTCSpecialAbilities::CreateParticleEffect(effectType, position);
    }
    ReleaseDef(effects);
}