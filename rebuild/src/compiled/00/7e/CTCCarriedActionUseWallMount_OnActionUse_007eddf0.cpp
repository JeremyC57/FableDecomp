// CTCCarriedActionUseWallMount::OnActionUse(CThing& user) [Ego_r.pdb; was vfunc_28(CThingCreatureBase*)] -- run the base use hook; a live user declares the holder a criminal, otherwise the holder's interface-0x1a component is notified
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). By-value members re-padded to the header layout (re-padded by-value maps +8).
//
// Retail 0x007EDDF0, 158 bytes (`ret 4`). Vtable 0x0123FE6C (CTCCarriedActionUseWallMount) slot 28.
// Callees: the base hook @ 0x007EAB90 (empty `ret 4` in another TU),
// CCreatureAction_DeclarePlayerACriminal::CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase*,
// CThingPlayerCreature*) @ 0x00843610, CThingCreatureBase::SetCurrentAction @ 0x006644F0,
// ~CBaseIntelligentPointer @ 0x00A01C10, ~CCreatureActionBase @ 0x00693EF0,
// CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020, the interface-0x1a component's
// member @ 0x005D8E50 (manifest "CChunkCollectionIndicator::OnPredicateFail", propagated; no arguments).
// Shape: as the landed-shape UseFish 0x007EDCD0 (IsThingAlive on the raw pointer, the holder a named
// local in edi, the 0xb0-byte action a temporary); the other arm tests the holder's bit 26 of
// bitset word 0 (`test dword [eax+20h],4000000h`) and calls the component straight off the
// GetInterface result (`mov ecx,[eax+4]`).
class CTCBase;
class CCreatureActionBase;
class CThingPlayerCreature;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCInterface1A {
    void Notify();                                          // 0x005D8E50
};

class CThingCreatureBase { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                         // +0x20 interface-present bitset
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad54[0x3d];
    unsigned char m_stateFlags;                             // +0x91 (bit 0 = dead)
    unsigned long HasInterface(long type) const { return m_presentBits[type >> 5] & (1UL << (type & 31)); }
    CTCBase* GetInterface(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::value_type* it = map->LowerBound(type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return it->second;
    }
    bool SetCurrentAction(const CCreatureActionBase& action);     // 0x006644F0
};

inline bool IsThingAlive(const CThingCreatureBase* thing)
{
    return thing && !(thing->m_stateFlags & 1);
}

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

class CCreatureActionBase { public:                        // retail sizeof 0xa8
    virtual void v00();
    unsigned char _pad04[0xa8 - 0x04];
    virtual ~CCreatureActionBase();                         // 0x00693EF0
};
class CCreatureAction_DeclarePlayerACriminal : public CCreatureActionBase { public:
    CIntelligentPointer<CThingPlayerCreature> PCriminal;   // +0xa8
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase* owner, CThingPlayerCreature* criminal);   // 0x00843610
};

class CThing;
class CTCActionUseBase { public:                 // Ego_r.pdb base of CTCCarriedActionUseBase (row-local view: the one member called)
    virtual void OnActionUse(CThing& user);   // 0x007EAB90 Ego_r.pdb ?OnActionUse@CTCActionUseBase@@UAEXAAVCThing@@@Z (empty `ret 4`, RTTI slot 28; was OnUseBase(CThingCreatureBase*))
};
class CTCCarriedActionUseBase : public CTCActionUseBase { public:
    virtual void v00();
    CThingCreatureBase* ParentThing;                // +0x04 (CTCBase)
};

struct CTCCarriedActionUseWallMount : CTCCarriedActionUseBase {
    virtual void OnActionUse(CThing& user);   // Ego_r.pdb (UAE; RTTI slot 28, was vfunc_28(CThingCreatureBase*))
};

void CTCCarriedActionUseWallMount::OnActionUse(CThing& thing)
{
    CThingCreatureBase* user = (CThingCreatureBase*)&thing;
    CTCCarriedActionUseBase::OnActionUse(thing);
    if (IsThingAlive(user))
    {
        CThingCreatureBase* holder = ParentThing;
        user->SetCurrentAction(CCreatureAction_DeclarePlayerACriminal(user, (CThingPlayerCreature*)holder));
    }
    else if (ParentThing->HasInterface(0x1a))
        ((CTCInterface1A*)ParentThing->GetInterface(0x1a))->Notify();
}