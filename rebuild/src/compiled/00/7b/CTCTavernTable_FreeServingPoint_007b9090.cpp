// CTCTavernTable::FreeServingPoint(long action_point_index) -- hand a serving point index back to the parent thing's interface-0x2E component
// Leaf: ?FreeServingPoint@CTCTavernTable@@QAEXJ@Z
// Name-fix pass (2026-11-12): Ego_r.pdb 0x007B8090 ?FreeServingPoint@CTCTavernTable@@QAEXJ@Z (the next body after GetServingPoint
//   0x007B7C80, as retail 0x007B9090 follows GetServingPoint 0x007B8CC0) + FableWin.pdb 0x026F1DE0, declared in engine/CTCTavernTable.h;
//   the landed CAIStateGroup_TavernServer row 0x009670A0 binds this symbol. Was the descriptive FreePoint.
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007B9090, length 88 bytes (+ int3 padding).
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020 (the inlined interface lookup,
// type 0x2E = bit 14 of the bitset dword at +0x24, tested with `mov ecx,[eax+24h] ; test ch,40h`), then
// the component's 0x007FEF40 (it appends the long to the vector at its +0x0c: `if (last != end) *last++ =
// v; else insert` -- the manifest's "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" label is
// propagated).
// Identity: the manifest label "CTCActionUseAcceptBoast::OnActionUse" is propagated over the byte-identical
// GetInterface shape. This copy sits directly before CTCTavernTable::OnInitialActivate (0x007B90F0) and is
// called on the interface-0xB8 component by CAIStateGroup_TavernKeeper::FreeBarPoint (0x009670F4, which
// then resets its index to -1), CAIStateGroup_ServeFood::OnReplace and CAIStateGroup_TavernServer::Update;
// the method name is FreeServingPoint (both PDBs).
//
// Shape: the component pointer is an UNINITIALISED local filled by the inlined GetInterface (the landed
// 0x00949320 template): hit path -> the lookup result in eax; miss path -> retail reads the never-written
// local, which VC7.1 homed in the argument's own stack slot (`mov eax,[esp+8]` beside the argument load).
// The frame is one `push ecx` (the key temporary). No engine header models the component table, so the
// thing is local.
class CTCBase;

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            ETCInterfaceType type = (ETCInterfaceType)(interfaceType);
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::value_type* it = map->LowerBound(type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};

class CTCUserActionPointBase { public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
};
struct CTCFreePointList {                       // interface type 0x2E component
};

class CTCTavernTable { public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04 (CTCBase)
    void FreeServingPoint(long action_point_index);   // 0x007B9090
};

void CTCTavernTable::FreeServingPoint(long action_point_index)
{
    CTCFreePointList* points;
    ParentThing->GetInterface(&points, 0x2E);
    ((CTCUserActionPointBase*)points)->SetActionPointAsFree(action_point_index);
}