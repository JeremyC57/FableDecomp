// CAIStateGroup_WhisperRetreat::Cleanup() -- when PTargetThing is alive and has a stealth component, mark it as in danger from this group's creature (DANGER_STATUS_0)
// Leaf: ?Cleanup@CAIStateGroup_WhisperRetreat@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096A350, 112 bytes (body 0x6F + int3), thiscall, `ret`.
// Identity: vtable 0x0128CDCC slot 9 (RTTI .?AVCAIStateGroup_WhisperRetreat@@; bases CAIStateGroup_WhisperRetreat <- CAIStateGroup_EnemyCreatureBase <- CAIStateGroupBase <- CBaseClass); slot 2 is OnActivate and slot 9 Cleanup in the
//   landed CAIStateGroup_* models (e.g. CAIStateGroup_MinionSitOnSeatBase::Cleanup 0x0090FA60). PTargetThing is the
//   CIntelligentPointer at +0x14 (Ego_r CAIStateGroup_EnemyCreatureBase::PTargetThing, CIntelligentPointer<CThingCreatureBase>).
//   Family G003 (12 bodies that differ only in the member offset and the danger status pushed).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (x3: the inlined IS_THING_ALIVE pair, then the item for the lookup),
//   CVectorMap::LowerBound 0x0040F020 (inlined GetInterface, type 0x36 = CTCStealth, bit 22 of the bitset dword at +0x24),
//   CTCStealth::SetAsInDanger(const CThing&, NStealth::EDangerStatus) 0x00747B80 (Ego_r signature) with PBrain->PCreature.
// Layout: local model of the shared AI state-group shapes (the generated engine headers flatten the vptr and the base, so a
//   virtual override cannot be declared on them). EDangerStatus enumerator names are descriptive (values only).
// Shape: nested ifs as in the landed MinionSitOnSeatBase::Cleanup; the creature argument loads after the component.
class CTCBase;
class CThing;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

#pragma pack(push, 1)   // CIntelligentPointer is 1-aligned: Ego_r puts CAIStateGroup_Thunder::PTargetThing at +0x1D
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
#pragma pack(pop)

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

namespace NStealth { enum EDangerStatus { DANGER_STATUS_0 = 0, DANGER_STATUS_2 = 2, DANGER_STATUS_3 = 3 }; }

class CTCStealth { public:                             // interface type 0x36
    void SetAsInDanger(const CThing& source, NStealth::EDangerStatus status);   // 0x00747B80
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01();
    virtual void OnActivate();                  // slot 2
    virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase {
    CIntelligentPointer<CThing> PTargetThing;       // +0x14 CIntelligentPointer<CThingCreatureBase>
};

struct CAIStateGroup_WhisperRetreat : CAIStateGroup_EnemyCreatureBase {
    CIntelligentPointer<CThing> PArenaCentre;       // +0x1c CIntelligentPointer<CThing>
    bool bIsRangeAttack;                       // +0x24
    long NumHits;                              // +0x28
    long FrameOfLastHit;                       // +0x2c
    virtual void Cleanup();
};

void CAIStateGroup_WhisperRetreat::Cleanup()
{
    if (IS_THING_ALIVE(PTargetThing)) {
        CTCStealth* pStealth;
        if (PTargetThing.GetPItem()->GetTC(TCI_STEALTH, &pStealth))
            pStealth->SetAsInDanger(*PBrain->PCreature, NStealth::DANGER_STATUS_0);
    }
}