// CTCKrakenTentacle::OnInitialActivate() -- fetch the tentacle def, end the ripple effect, spawn the "DRIP" special effect attached at the tentacle's "TARGET_CURSOR_POINT", spawn the "MEETS_WATER" effect as the new ripple effect attached at "EFFECT_WATER", then run the base OnInitialActivate
// Leaf: OnInitialActivate
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007F08B0, 593 bytes, plain `ret`. Vtable 0x012407BC (CTCKrakenTentacle) slot 6.
// Callees: CDefClassBase::GetSubDef<CKrakenTentacleDef> @ 0x007F16B0, CTCKrakenTentacle::KillRippleFX @ 0x007F07A0,
//   GetSubDef<CSpecialEffectsDef> @ 0x00669580, GetEffectFromString @ 0x00669D30, CCharString @ 0x0099EBF0/0x0099EAE0,
//   CThing::GetPos @ 0x004C73D0, CTCDParticleEmitter::Create @ 0x006E0880, AttachToThing @ 0x006E0BE0,
//   CBaseIntelligentPointer @ 0x00A01B10/0x00A01B50/0x00A01B90/0x00A01C10, LowerBound @ 0x0040F020,
//   CTCBase::OnInitialActivate @ 0x00686870 (empty base body).
// Model: the landed sibling CTCKrakenTentacle::DoDieFX 0x007F0B10 (its second half is this function's tail).
// Data: 0x01271D0C "DRIP", 0x012651BC "TARGET_CURSOR_POINT", 0x01271D00 "MEETS_WATER", 0x01271CF0 "EFFECT_WATER".
#include "fable_compiler.h"
#include "engine/CTCBase.h"

class CCharStringData;
class CCharString { public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);        // 0x0099EBF0
    ~CCharString();                                         // 0x0099EAE0
};
#include "engine/C3DVector.h"

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
    bool GetEffectFromString(const CCharString& name, long& effect) const;   // 0x00669D30 // agentNF16: Ego_r/FableWin ?GetEffectFromString@CSpecialEffectsDef@@QBE_NABVCCharString@@AAJ@Z
};

class CTCBaseComponent;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
long Lookup_CSpecialEffectsDef_GetEffectFromString(const void* def, const CCharString& name);   // stand-in callee
// Stand-in body (out of line in retail @ 0x00669D30): it writes the out value only, so VC7.1 knows the local's address
// does not escape the call (the 0x008D9740 idiom).
#pragma auto_inline(off)
__declspec(noinline) bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& value) const
{
    long found = Lookup_CSpecialEffectsDef_GetEffectFromString(this, name);
    if (found < 0)
        return false;
    value = found;
    return true;
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
    unsigned char _pad54[0x1c];
    CDefClassBase* PDef;                                    // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    template<class T> FABLE_FORCEINLINE bool GetInterface(T** ppOut, long interfaceType)
    {
        if (present.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
    const C3DVector& GetPos() const;                   // 0x004C73D0
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

class CKrakenTentacleDef;
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

class CTCKrakenTentacle : public CTCBaseActivate { public:
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
    thingDef->GetSubDef(&effects) && effects->GetEffectFromString("DRIP", effect);
    CThing* drip = CTCDParticleEmitter::Create(effect, ParentThing->GetPos(), false);
    if (drip && !(drip->Flags91 & 1)) {
        CTCDParticleEmitter* emitter = 0;
        drip->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*ParentThing, 0xc, "TARGET_CURSOR_POINT", 0, 0.0f);
    }
    thingDef = ParentThing->PDef;
    thingDef->GetSubDef(&effects) && effects->GetEffectFromString("MEETS_WATER", effect);
    PRippleEffect = CIntelligentPointer<CThing>(CTCDParticleEmitter::Create(effect, ParentThing->GetPos(), false));
    if (PRippleEffect.IsAlive()) {
        CTCDParticleEmitter* emitter = 0;
        PRippleEffect.GetPItem()->GetInterface(&emitter, 0x4b);
        emitter->AttachToThing(*ParentThing, 0x4a, "EFFECT_WATER", 0, 0.0f);
    }
    CTCBaseActivate::OnInitialActivate();
}