// CTCKrakenTentacle::OnInitialActivate() -- fetch the tentacle def, end the ripple effect, spawn the "DRIP" special effect attached at the tentacle's "TARGET_CURSOR_POINT", spawn the "MEETS_WATER" effect as the new ripple effect attached at "EFFECT_WATER", then run the base OnInitialActivate
// Leaf: OnInitialActivate
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F08B0, 593 bytes, plain `ret`. Vtable 0x012407BC (CTCKrakenTentacle) slot 6.
// Callees: CDefClassBase::GetSubDef<CKrakenTentacleDef> @ 0x007F16B0, CTCKrakenTentacle::KillRippleFX @ 0x007F07A0,
//   GetSubDef<CSpecialEffectsDef> @ 0x00669580, GetEffectFromString @ 0x00669D30, CCharString @ 0x0099EBF0/0x0099EAE0,
//   CThing::GetPosition @ 0x004C73D0, CTCDParticleEmitter::Create @ 0x006E0880, AttachToThing @ 0x006E0BE0,
//   CBaseIntelligentPointer @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10, LowerBound @ 0x0040F020,
//   CTCBase::OnInitialActivate @ 0x00686870 (empty base body).
// Model: the landed sibling CTCKrakenTentacle::DoDieFX 0x007F0B10 (its second half is this function's tail).
// Data: 0x01271D0C "DRIP", 0x012651BC "TARGET_CURSOR_POINT", 0x01271D00 "MEETS_WATER", 0x01271CF0 "EFFECT_WATER".
#include "engine/CTCBase.h"

struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
struct C3DVector { float X, Y, Z; };

template <class T> struct CDefPointer;
struct CDefClassBase {
    virtual void v00();
    virtual void Destroy() const;                           // slot 1
    mutable long RefCount;                                  // +0x04
    bool GetSubDef(CDefPointer<const struct CSpecialEffectsDef>* out) const;   // GetSubDef<CSpecialEffectsDef> 0x00669580
};
template <class T> struct CDefPointer {
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer()
    {
        if (Object && --Object->RefCount == 0)
            Object->Destroy();
    }
    T* operator->() const { return Object; }
};
struct CSpecialEffectsDef : CDefClassBase {
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
struct CThing {
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

struct CTCDParticleEmitter {
    static CThing* __fastcall Create(long effect, const C3DVector& position, bool attached);   // 0x006E0880
    void AttachToThing(CThing& thing, long mode, const CCharString& bone, long flags, float offset);   // 0x006E0BE0
};

#pragma pack(push, 1)
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                          // +0x04
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    CThing* GetPItem() const;                               // 0x00A01B50
    void SetPItem(CThing* item);                            // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer(T* item) { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
    bool IsAlive() const { return GetPItem() && !(GetPItem()->Flags91 & 1); }
};
#pragma pack(pop)

struct CKrakenTentacleDef;
struct CTCBaseActivate : CTCBase {
    void OnInitialActivate();                               // 0x00686870 (CTCBase, empty)
};
struct CDefClassBaseT : CDefClassBase {
    bool GetSubDef(CDefPointer<const CKrakenTentacleDef>* out) const;   // GetSubDef<CKrakenTentacleDef> 0x007F16B0
};
template <class T> struct CDefCopy {
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->RefCount; }
    ~CDefCopy() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};
struct CThingWithDef : CThing {
    CDefCopy<CDefClassBaseT> GetDef() const { return CDefCopy<CDefClassBaseT>((CDefClassBaseT*)PDef); }
};

struct CTCKrakenTentacle : CTCBaseActivate {
    CDefPointer<const CKrakenTentacleDef> PTentacleDef;     // +0x0c
    unsigned char _pad10[0x20 - 0x10];
    CIntelligentPointer<CThing> PRippleEffect;              // +0x20
    void KillRippleFX();                                    // 0x007F07A0
    void OnInitialActivate();
};

void CTCKrakenTentacle::OnInitialActivate()
{
    ((CThingWithDef*)ParentThing)->GetDef()->GetSubDef(&PTentacleDef);
    KillRippleFX();
    CDefPointer<const CSpecialEffectsDef> effects;
    long effect = 0;
    const CDefClassBase* thingDef = ParentThing->PDef;
    thingDef->GetSubDef(&effects) && effects->GetEffectFromString("DRIP", &effect);
    CThing* drip = CTCDParticleEmitter::Create(effect, ParentThing->GetPosition(), false);
    if (drip && !(drip->Flags91 & 1)) {
        CTCDParticleEmitter* emitter = 0;
        drip->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*ParentThing, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
    }
    thingDef = ParentThing->PDef;
    thingDef->GetSubDef(&effects) && effects->GetEffectFromString("MEETS_WATER", &effect);
    PRippleEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, ParentThing->GetPosition(), false));
    if (PRippleEffect.IsAlive()) {
        CTCDParticleEmitter* emitter = 0;
        PRippleEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*ParentThing, 0x4a, "EFFECT_WATER", 0, 0.0f);
    }
    CTCBaseActivate::OnInitialActivate();
}
