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
#define FABLE_RETAIL_CTCBASE                                 // agentNF19: the virtual CTCBase model (slot 6 OnInitialActivate 0x00686870)
#include "engine/CTCBase.h"
// NF49 (round 7): the def classes come from the shared headers (were local CCharString / CDefClassBase / CDefPointer /
// CSpecialEffectsDef models with same-TU stand-in GetSubDef / GetEffectFromString bodies over made-up LookUpSubDef /
// Lookup_CSpecialEffectsDef_GetEffectFromString callees): the call at 0x00669580 is the member template
// ??$GetSubDef@VCSpecialEffectsDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCSpecialEffectsDef@@@@@Z (Ego_r.pdb / FableWin.pdb),
// its body visible and kept out of line (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE, the 0x0066BC10 idiom), and
// CSpecialEffectsDef::GetEffectFromString 0x00669D30 is the header's same-TU noinline body (FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY,
// the 0x007941D0 idiom), so the caller's out handle and effect index stay uncaptured as with the stand-ins.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_TYPENAME
#define FABLE_RETAIL_CSPECIALEFFECTSDEF_GETEFFECT_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_NOINLINE
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefPointer.h"
#include "engine/CSpecialEffectsDef.h"
#include "engine/CDefClassBase_GetSubDef.h"


class CTCBaseComponent;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
class CTCBase;
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
struct CDefClassBaseT : CDefClassBase {
    bool GetSubDef(CDefPointer<const CKrakenTentacleDef>* out) const;   // GetSubDef<CKrakenTentacleDef> 0x007F16B0
};
template <class T> struct CDefCopy {
    T* Object;
    CDefCopy(T* object) : Object(object) { if (Object) ++Object->IVRefCount; }
    ~CDefCopy() { if (Object && --Object->IVRefCount == 0) Object->Delete(); }
    T* operator->() const { return Object; }
};
struct CThingWithDef : CThing {
    CDefCopy<CDefClassBaseT> GetDef() const { return CDefCopy<CDefClassBaseT>((CDefClassBaseT*)PDef); }
};

class CTCKrakenTentacle : public CTCBase { public:
    CDefPointer<const CKrakenTentacleDef> PTentacleDef;     // +0x0c
    unsigned char _pad10[0x20 - 0x10];
    CIntelligentPointer<CThing> PRippleEffect;              // +0x20
    private: void KillRippleFX(); public:                                    // 0x007F07A0
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
    CTCBase::OnInitialActivate();
}