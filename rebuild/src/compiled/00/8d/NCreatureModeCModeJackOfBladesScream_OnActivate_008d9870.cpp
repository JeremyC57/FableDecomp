// NCreatureMode::CModeJackOfBladesScream::OnActivate() -- the Jack of Blades scream: clear two creature flags (for a thing of type flag 2), look up the "FLOAT_SCREAM" effect in the thing definition's special-effects sub-def, spawn it at the creature, keep it in PEmitter and attach its particle emitter to the creature's "TARGET_CURSOR_POINT" bone
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008D9870, 312 bytes (+ int3 padding), plain `ret` (virtual).
// Callees: CDefClassBase::GetSubDef<CSpecialEffectsDef>(CDefPointer<CSpecialEffectsDef>&) 0x00669580, CCharString(const
//   char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&)
//   0x00669D30, CThing::GetPos 0x004C73D0, __fastcall CreateEffectThing(long, const C3DVector*, bool) 0x006E0880,
//   CBaseIntelligentPointer ctor 0x00A01B10 / SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10,
//   CTCDParticleEmitter::AttachToThing(CThing*, long, const CCharString&, long, float) 0x006E0BE0.
// Layout: Ego_r.pdb CModeJackOfBladesScream (0xBC): PEmitter +0x9C; CModeStandBase ParentThing +0x04. Thing (retail
//   offsets, PDB - 0x10): UsingComponents bitset +0x20 (TC 0x4B = word 2 bit 11), PTCDriver +0x5C, ThingTypeFlags +0x6C,
//   PBaseDef +0x70, Dead bit 0 of +0x91; the creature flag byte +0xBC.
// Shape: GetSubDef and GetEffectFromString both have same-TU out-slot-only stand-in bodies, so neither the sub-def slot
//   nor the effect type escapes: the slot word is reused by the "FLOAT_SCREAM" string, the effect-type word by the
//   intelligent-pointer temporary and the bone-name string (frame 0xc); the creature-flag clear reads ParentThing into a
//   block-scoped local (keeps the scratch-register rotation); the thing's definition is copied (add-ref, GetSubDef,
//   release) in an inline helper; the effect sub-def is released by hand at the end (known non-null: no test).
class C3DVector { public: float X, Y, Z; };

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};

static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CDefVirtual {
    virtual void v00();
    virtual void Destroy();                     // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                            // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail): it writes the out type only, so VC7.1 knows the effect type's address does
// not escape the call.
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    type = found;
    return true;
}
#pragma auto_inline(on)
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};
void* Find_CDefClassBase_GetSubDef(const void* owner);   // stand-in callee of the stand-in body below
// Stand-in body (the real one is out of line in retail): it writes the out slot only, so VC7.1 knows the def
// pointer's address does not escape the call (the landed 0x00848730 idiom).
#pragma auto_inline(off)
template <class T> __declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    void* def = Find_CDefClassBase_GetSubDef(this);
    if (!def)
        return false;
    out.m_p = (T*)def;
    return true;
}
#pragma auto_inline(on)

class CThing;
class CTCDParticleEmitter { public:
    void AttachToThing(CThing& thing, long boneIndex, const CCharString& boneName, long flags, float scale);   // 0x006E0BE0
};

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  UsingComponents[9];          // +0x20
    unsigned char  _pad44[0x5c - 0x44];
    void*          PTCDriver;                   // +0x5C
    unsigned char  _pad60[0x6c - 0x60];
    unsigned long  ThingTypeFlags;              // +0x6C
    CDefClassBase* PBaseDef;                    // +0x70
    unsigned char  _pad74[0x91 - 0x74];
    unsigned char  Dead : 1;                    // +0x91 bit 0
    unsigned char  _pad92[0xbc - 0x92];
    unsigned char  CreatureFlags;               // +0xBC (creature-derived member)
    const C3DVector& GetPos() const;       // 0x004C73D0
    bool HasTC(long type) const
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    void GetSpecialEffectsDef(CDefPointer<CSpecialEffectsDef>& out) const
    {
        CDefClassBase* def = PBaseDef;
        if (def)
            ++def->m_refCount;
        def->GetSubDef(out);
        if (def && --def->m_refCount == 0)
            def->Destroy();
    }
};

CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    CIntelligentPointer() { }
    __forceinline CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

struct CModeStandBase {                         // retail sizeof 0x68
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void OnActivate();                  // slot 5
    CThing*       ParentThing;                  // +0x04
    unsigned char _body[0x60];
};

namespace NCreatureMode {

class CModeJackOfBladesScream : public CModeStandBase { public:
    unsigned char            _pad68[0x9c - 0x68];
    CIntelligentPointer<CThing> PEmitter;          // +0x9C
    virtual void OnActivate();
};

void CModeJackOfBladesScream::OnActivate()
{
    {
        CThing* thing = ParentThing;
        if (thing->ThingTypeFlags & 2)
            thing->CreatureFlags &= 0xfa;
    }
    CDefPointer<CSpecialEffectsDef> effectsSlot;
    long effectType = 0;
    ParentThing->GetSpecialEffectsDef(effectsSlot);
    CSpecialEffectsDef* effects = effectsSlot.m_p;
    if (effects) {
        if (effects->GetEffectFromString("FLOAT_SCREAM", effectType)) {
            CThing* effect = CreateEffectThing(effectType, &ParentThing->GetPos(), false);
            if (effect && !effect->Dead) {
                PEmitter = CIntelligentPointer<CThing>(effect);
                if (effect->HasTC(0x4b)) {
                    CTCDParticleEmitter* emitter = (CTCDParticleEmitter*)effect->PTCDriver;
                    emitter->AttachToThing(*ParentThing, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
                }
            }
        }
        if (DecRefToZero(effects->m_refCount))
            effects->Destroy();
    }
}

}