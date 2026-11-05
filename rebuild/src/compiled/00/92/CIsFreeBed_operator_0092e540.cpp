// CIsFreeBed::operator()(const CIntelligentPointer<CThing>& thing) const -- true for a bed thing (interface 0x35, not flagged +0x91 bit 5) that is not taken (interface 0x0D component byte +0x21) and whose user-action point is free, or (without one) that the embedded filter 0x006A8A30 rejects
// Leaf: ??RCIsFreeBed
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +4.
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092E540, 227 bytes, `ret 4`. Callers: the find_if at 0x0092E750 and 0x0092E7B8.
// Callees: GetPItem @ 0x00A01B50, LowerBound non-const @ 0x0040F020 (type 0x0D) and const @ 0x004365B0 (type 0x2D),
// IsActionPointFree @ 0x007FEBF0, the embedded filter's operator()(CThing*) @ 0x006A8A30 (called on `this`).
// Model: CThing keeps the interface-present bitset at +0x20 (HasTC returns the masked word, so a constant type tests the
// byte/word in place: `test ch,20h`, `test dword ptr [eax+24h],200000h`) and the sorted {type, component} CVectorMap at +0x44
// (LowerBound const @ 0x004365B0, key in the dead argument slot); slot 48 (+0xC0) returns the kind-flag word, read as
// `(unsigned char)(flags >> n) & 1` (`shr eax,n ; test al,1`); +0x10 is the thing type byte, +0x91 a flag byte.
// Same model as the landed CIsFreeHopscotchBoard (0x00941BE0) / CThingFilter_HasFreeActionPoint (0x006A8B40) / 0x00938BF0.
// Shape: one nested `if` with a single trailing `return false;` (early returns shrink-wrap esi and turn the last test into
// `neg/sbb/inc`); the two lookups go through the non-const and const LowerBound copies.
class CTCBase;
// A thing's component table: the presence bitset at +0x20 (bit N of dword N/32) and the sorted {type, component}
// CVectorMap at +0x44 searched by LowerBound (non-const copy 0x0040F020, const copy 0x004365B0).
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_STATEACCESS
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"

struct CTCUserActionPointBase {
    bool IsActionPointFree() const;                     // 0x007FEBF0
    long GetNumberOfActionPointsInUse();          // 0x007FEC10
};

class CTCBed { public:
    unsigned char _pad[0x21];
    bool m_taken;                                 // +0x21
};
class CThingFilter_BedOccupant {                  // empty filter at +0x00 of CIsFreeBed
public:
    bool operator()(CThing* thing) const;         // 0x006A8A30
};
class CIsFreeBed {
public:
    CThingFilter_BedOccupant m_occupied;          // +0x00
    bool operator()(const CIntelligentPointer<CThing>& thing) const;
};
bool CIsFreeBed::operator()(const CIntelligentPointer<CThing>& thing) const
{
    if (thing.Get() && thing.Get()->HasTC(TCI_BED) && !thing.Get()->IsInLimbo()) {
        CTCBed* bed;
        if (!thing.Get()->GetTC(TCI_ON_ACTION_USE, &bed) || !bed->m_taken) {
            const CTCUserActionPointBase* actionPoint;
            if (thing.Get()->PeekTC(TCI_SINGLE_USER_ACTION_POINTS, &actionPoint))
                return actionPoint->IsActionPointFree();
            if (!m_occupied(thing.Get()))
                return true;
        }
    }
    return false;
}