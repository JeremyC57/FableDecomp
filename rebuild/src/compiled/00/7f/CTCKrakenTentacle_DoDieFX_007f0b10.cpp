// CTCKrakenTentacle::DoDieFX() -- end the ripple effect, look up the "ON_DIE" special effect of the tentacle's def, create that particle emitter at the tentacle as the new ripple effect and, if it lives, attach it to the tentacle's "EFFECT_WATER" bone
// Leaf: DoDieFX
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F0B10, 340 bytes, plain `ret`. Speed TU (esp frame, no pragma).
//   Ego_r.pdb: `?DoDieFX@CTCKrakenTentacle@@QAEXXZ`.
// Label correction: the catalog row "CTCKraken::CreateHeadEffect" (`AAEXABVCCharString@@@Z`) is wrong: that is
//   0x007FBB00 (`ret 4`, the landed 0x007FBC30 calls it); this body takes no argument, sits among the CTCKrakenTentacle
//   functions (0x007F0770 .. 0x007F15D0) and uses PRippleEffect (+0x20) -> CTCKrakenTentacle::DoDieFX ("ON_DIE").
// Callees: CTCKrakenTentacle::KillRippleFX @ 0x007F07A0 (label "CTCKraken::EndHeadEffects" is propagated),
//   CDefClassBase::GetSubDef<CSpecialEffectsDef> @ 0x00669580 (label names the CAICreatureWillPowerIndicatorDef
//   instantiation: byte-identical COMDAT), CSpecialEffectsDef::GetEffectFromString(const CCharString&, long*)
//   @ 0x00669D30, CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, CThing::GetPosition
//   @ 0x004C73D0, CTCDParticleEmitter::Create(long, const C3DVector&, bool) @ 0x006E0880 (static `__fastcall`),
//   CBaseIntelligentPointer ctor @ 0x00A01B10 / SetPItem @ 0x00A01B90 / GetPItem @ 0x00A01B50 / dtor @ 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CVectorMap LowerBound @ 0x0040F020 (inlined GetInterface, the
//   bitset test inline: 0x4B -> word +0x28 bit 11), CTCDParticleEmitter::AttachToThing(CThing&, long, const
//   CCharString&, long, float) @ 0x006E0BE0, the def's virtual slot 1 (destroy at the last release).
// Shape: GetSubDef / GetEffectFromString have same-TU out-only stand-ins and LowerBound a same-TU body; the tentacle's
//   def is read into a named local before the GetSubDef call (VC7.1 then stages ParentThing in eax and rotates the later
//   scratch registers as retail).
// Data: 0x01271D14 "ON_DIE", 0x01271CF0 "EFFECT_WATER".
// Layout: Ego_r.pdb CTCKrakenTentacle (0x50): PRippleEffect +0x20 (CIntelligentPointer<CThing>); CThing interface
//   bitset +0x20, interface map +0x44, def +0x70, dead flag bit 0 of +0x91.
#include "engine/CTCBase.h"

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
class C3DVector { public: float X, Y, Z; };

template <class T> class CDefPointer;
class CDefClassBase { public:
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long RefCount;                                  // +0x04
    bool GetSubDef(CDefPointer<const class CSpecialEffectsDef>* out) const;   // GetSubDef<CSpecialEffectsDef> 0x00669580
};
template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
class CSpecialEffectsDef : public CDefClassBase { public:
    bool GetEffectFromString(const CCharString& name, long* effect) const;   // 0x00669D30
};

class CTCBaseComponent;
struct CInterfaceMap {                                      // CVectorMap<ETCInterfaceType, CTCBase*> at CThing+0x44
    struct SEntry { long type; CTCBaseComponent* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long* value) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    *value = found;
    return true;
}
#pragma auto_inline(on)
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CInterfaceBits {                                     // bitset<> at CThing+0x20
    unsigned long _M_w[9];
    bool test(unsigned long pos) const { return (_M_w[pos / 32] & (1ul << (pos % 32))) != 0; }
};
class CThing { public:
    unsigned char _pad00[0x20];
    CInterfaceBits present;                                 // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x70 - 0x4c];
    CDefClassBase* PDef;                                    // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
// Same-TU stand-in for the GetSubDef<T> COMDAT (as the landed 0x00801A50): with the body visible VC7.1 sees that the out
// pointer does not escape and keeps the def object in ebp through to its release, as retail.
const CDefClassBase* LookUpSubDef(const CDefClassBase* def);
#pragma auto_inline(off)
bool CDefClassBase::GetSubDef(CDefPointer<const CSpecialEffectsDef>* out) const
{
    const CSpecialEffectsDef* def = (const CSpecialEffectsDef*)LookUpSubDef(this);
    out->Object = def;
    return def != 0;
}
#pragma auto_inline(on)

class CTCDParticleEmitter { public:
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attached);   // 0x006E0880
    void AttachToThing(CThing& thing, long mode, const CCharString& bone, long flags, float offset);   // 0x006E0BE0
};

#pragma pack(push, 1)
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01238C6C
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

class CTCKrakenTentacle : public CTCBase { public:
    unsigned char _pad0c[0x20 - 0x0c];
    CIntelligentPointer<CThing> PRippleEffect;              // +0x20
    void KillRippleFX();                                    // 0x007F07A0
    void DoDieFX();
};

void CTCKrakenTentacle::DoDieFX()
{
    KillRippleFX();
    CDefPointer<const CSpecialEffectsDef> effects;
    long effect = 0;
    const CDefClassBase* thingDef = ParentThing->PDef;
    thingDef->GetSubDef(&effects) && effects->GetEffectFromString("ON_DIE", &effect);
    PRippleEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, ParentThing->GetPosition(), false));
    if (PRippleEffect.IsAlive()) {
        CTCDParticleEmitter* emitter = 0;
        PRippleEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*ParentThing, 0x4a, "EFFECT_WATER", 0, 0.0f);
    }
}