// CTCInventory::OnItemSelectedInHand(CThing& item, bool& handled) -- an item with a carried-action-use component (interface 0x0E) is used in hand by the owner unless the owner is busy with an action the component cannot be used during; an item without one makes a creature owner sheathe its held item back into the inventory
// Leaf: ?OnItemSelectedInHand@CTCInventory@@UAEXAAVCThing@@AA_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0058400A, 156 bytes; frame-pointer size TU (`#pragma optimize("y", off)` +
//   `optimize("s", on)`), `ret 8` (`handled` unused), no EH frame. Identity: both PDB name sets hold
//   ?OnItemSelectedInHand@CTCInventory@@UAEXAAVCThing@@AA_N@Z (virtual; references, not pointers).
// Callees (PDB spellings): std::bitset<274>::test const 0x00410DE0 (out of line in this /Os TU) and
//   CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020 (the item's inline GetTC, interface 0x0E -- retail
//   CTCCarriedActionUseBase vtable 0x0123F894 slot 21 GetInterfaceType is `push 0Eh ; pop eax`);
//   CThingCreatureBase::IsPerformingAnyAction() const 0x006622A0; the component's slot 30 (+0x78, a bool getter returning
//   false in the base: IsUsableWhileThingPerformingAction, probable from the PDB name set) and slot 32 (+0x80,
//   OnActionUseInHand(CThing&), the 0x007ECE60 base body); CCreatureAction_SheatheItemToInventory(CThingCreatureBase&,
//   CCreatureActionBase*, long) 0x0069FFD0, CThingCreatureBase::SetCurrentAction(const CCreatureActionBase&) 0x006644F0
//   and the action's virtual destructor 0x0048C950 (Ego_r.pdb spellings, as the landed 0x0062B7C0 declares them).
// Layout: `this` is engine/CTCInventory.h through the `#define CTCInventory CTCInventoryLayout` include (ParentThing is
//   the flattened CTCBase +0x04, engine/CTCBase.h); retail CThing: type byte +0x10 (2 = creature), bitset<274> +0x20,
//   interface map +0x44 (local view, as the landed siblings).
// Shape: the parked 156v156 residue (owner in esi = the dead item, retail ebx = the dead `this`) closes with the PDB
//   reference spellings: the action constructor takes `CThingCreatureBase&` (`*creature`) and SetCurrentAction a const
//   reference; with pointer stand-ins VC7.1 colours the owner into esi. The action is a temporary passed straight from the
//   constructor's eax.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CTCBase CTCBaseLayout
#include "engine/CTCBase.h"
#undef CTCBase
#define CTCInventory CTCInventoryLayout
#include "engine/CTCInventory.h"
#undef CTCInventory
namespace std {
template <unsigned int _Nb> class bitset {
public:
    unsigned long _M_w[(_Nb + 31) / 32];
    bool test(unsigned int __pos) const;                    // 0x00410DE0 (bitset<274>)
};
}
class CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);                   // 0x0040F020
};
class CThing { public:
    unsigned char _pad00[0x10];
    unsigned char ThingType;                                // +0x10 (2 = creature)
    unsigned char _pad11[0x20 - 0x11];
    std::bitset<274> UsingComponents;                       // +0x20
    CInterfaceMap Components;                               // +0x44
    template<class T> __forceinline bool GetTC(T** out, long interfaceType)
    {
        if (UsingComponents.test(interfaceType)) {
            long type = interfaceType;
            CInterfaceMap* map = &Components;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *out = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CCreatureActionBase { public:                         // retail sizeof 0xa8
    virtual ~CCreatureActionBase();
    unsigned char _pad04[0xa8 - 0x04];
};
class CThingCreatureBase : public CThing { public:
    bool IsPerformingAnyAction() const;                     // 0x006622A0
    bool SetCurrentAction(const CCreatureActionBase& action);   // 0x006644F0
};
class CCreatureAction_SheatheItemToInventory : public CCreatureActionBase { public:   // retail sizeof 0xb0
    unsigned char PNextAction[8];                           // +0xa8 CCountedPointer<CCreatureActionBase>
    CCreatureAction_SheatheItemToInventory(CThingCreatureBase& owner, CCreatureActionBase* next, long kind);   // 0x0069FFD0
    virtual ~CCreatureAction_SheatheItemToInventory();      // 0x0048C950
};
class CTCCarriedActionUseBase { public:                     // interface type 0x0E
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual bool IsUsableWhileThingPerformingAction() const;   // slot 30 (+0x78)
    virtual void OnActionUse(CThing& thing);                // slot 31 (+0x7c)
    virtual void OnActionUseInHand(CThing& thing);          // slot 32 (+0x80)
};
class CTCInventory : public CTCInventoryLayout { public:
    CThing* GetParentThing() const { return reinterpret_cast<const CTCBaseLayout*>(this)->ParentThing; }
    virtual void OnItemSelectedInHand(CThing& item, bool& handled);
};

void CTCInventory::OnItemSelectedInHand(CThing& item, bool& handled)
{
    CTCCarriedActionUseBase* use;
    if (item.GetTC(&use, 0x0e)) {
        if (!static_cast<CThingCreatureBase*>(GetParentThing())->IsPerformingAnyAction() || use->IsUsableWhileThingPerformingAction())
            use->OnActionUseInHand(*GetParentThing());
    } else {
        CThingCreatureBase* creature = static_cast<CThingCreatureBase*>(GetParentThing());
        if (creature->ThingType == 2)
            creature->SetCurrentAction(CCreatureAction_SheatheItemToInventory(*creature, 0, 100));
    }
}