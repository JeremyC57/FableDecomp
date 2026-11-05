// CThingFilter_HasFreeActionPoint::operator()(CThing* thing) -- true when the thing has a user-action-point component (interface 0x2D) whose action point is free
// Leaf: ??RCThingFilter_HasFreeActionPoint
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006A8B40, length 76 bytes.
// Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x004365B0 (the thing's interface map at +0x44),
// CTCUserActionPointBase::IsActionPointFree @ 0x007FEBF0.
// Layout: CThing keeps the interface-present bitset at +0x20 (type 0x2D -> word 1 at +0x24, bit 13: `test ch,20h`)
// and the sorted interface map at +0x44 (landed GetInterface family, 0x007504E0 / 0x0075D8F0 shape).
// Shape: `return GetInterface(...) && component->IsActionPointFree();` -- the logical-and result is materialised as an
// int (`mov eax,1` / `xor eax,eax`) even though the operator returns bool; the key lives in the dead `thing` slot.
// The filter has no data members (PDB: no layout recorded); `this` is unused.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCUserActionPointBase {
    bool IsActionPointFree() const;                       // 0x007FEBF0
};

class CThingFilter_HasFreeActionPoint { public:
    bool operator()(CThing* thing) const;
};

bool CThingFilter_HasFreeActionPoint::operator()(CThing* thing) const
{
    const CTCUserActionPointBase* actionPoint;
    return thing->PeekTC(TCI_SINGLE_USER_ACTION_POINTS, &actionPoint) && actionPoint->IsActionPointFree();
}