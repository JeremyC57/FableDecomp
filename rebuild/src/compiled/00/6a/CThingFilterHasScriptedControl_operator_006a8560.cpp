// CThingFilter_HasScriptedControl::operator()(CThing* thing) const -- a script thing with a scripted-control component that, when CheckForActive is set, is also active (locked or with queued script tasks)
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- the interface lookup written as the ternary entry pick (`((it == last || type < it->type) ? last : it)->component`, the landed 0x00584157 idiom): the component loads straight into ecx
// Retail: Fable.exe (TLC, VC7.1) @ 0x006A8560, 99 bytes (`ret 4`).
// Callees: GFHasScriptThing(CThing*) @ 0x004C7E70 (`__fastcall`; label "operator_new" propagated, landed),
//   CInterfaceMap::LowerBound @ 0x004365B0, CTCScriptedControl's task-list empty test @ 0x007122C0 (auto label
//   "CScriptTask_IsRunning"): Ego_r / FableWin ?IsActionListEmpty@CTCScriptedControl@@QBE_NXZ.
// Layout: CThingFilter_HasScriptedControl (Ego_r.pdb 1 byte) = bool CheckForActive; retail CThing: interface bitset +0x20
//   (TC 0x1f = CTCScriptedControl: the sign bit of word +0x20), CVectorMap +0x44; CTCScriptedControl Locked +0x18.
// Shape: the agent160 GetTC form with the lookup key in the dead thing-argument slot (no frame: the function has no
//   named locals); CTCScriptedControl::IsActive inline (`Locked || !IsActionListEmpty()`).
#include "fable_compiler.h"
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;
class CTCScriptedControl { public:
    unsigned char _pad[0x18];
    bool Locked;                                // +0x18
    bool IsActionListEmpty() const;             // 0x007122C0 ?IsActionListEmpty@CTCScriptedControl@@QBE_NXZ (Ego_r / FableWin; the donor IsActive calls it; self Leaf)
    bool IsActive() { return Locked || !IsActionListEmpty(); }
};
class CThing { public:
    bool HasScriptThing() const;                // 0x004C7E70 ?HasScriptThing@CThing@@QBE_NXZ (Ego_r / FableWin; NF108 q4487)
    unsigned char _pad00[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> FABLE_FORCEINLINE T* GetTC(long interfaceType)
    {
        ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::value_type* it = map->LowerBound(type);
        return (T*)((it == map->end() || interfaceType < it->first) ? map->end() : it)->second;
    }
    bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
    }
};

class CThingFilter_HasScriptedControl { public:
    bool CheckForActive;                        // +0x00
    bool operator()(CThing* thing) const;
};

bool CThingFilter_HasScriptedControl::operator()(CThing* thing) const
{
    if (thing->HasScriptThing() && thing->HasTC(0x1f)) {
        if (!CheckForActive || thing->GetTC<CTCScriptedControl>(0x1f)->IsActive())
            return true;
    }
    return false;
}