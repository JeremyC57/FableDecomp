// CGameScriptInterface::GetSleepingPositionAndOrientationFromBed(const CScriptThing& sleeper, const CScriptThing& bed, C3DVector& position, C3DVector& orientation) -- for a valid sleeper and a valid bed thing that has a user-action-point component (kind 0x2d), take a free action point: its position into `position`, the forward axis of its orientation into `orientation`, and mark the point used; false otherwise
// Leaf: ?GetSleepingPositionAndOrientationFromBed@CGameScriptInterface@@UAE_NABVCScriptThing@@0AAVC3DVector@@1@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008A1B10, 218 bytes, `ret 10h` (vtable 0x01260F0C slot 756).
// Callees: CScriptThing slots 75 (+0x12c, IsValid) and 11 (+0x2c, GetThing), this interface's slot 682 (+0xaa8,
//   IsThingABed 0x008A0E80; ghidra_out/cgsi_vtable_slots.tsv lists it 4 rows late), the component-map LowerBound @
//   0x0040F020, CTCUserActionPointBase::GetFreeActionPoint(CThing* user, C3DVector* position, CRightHandedSet*
//   orientation, long* index) @ 0x007FEDE0, the action-point "use" member @ 0x007FEF40 (pushes the index on the
//   component's +0x0c vector; its label "EditCyclePreviousKeyCamera" is propagated).
// Layout: engine/CGameScriptInterface.h is data-only (vptr flattened), so the interface is a local polymorphic model;
//   CRightHandedSet {Up, Forward} (Ego_r.pdb, 24 bytes) is the whole 0x18 frame; the index and the lookup key share the
//   dead `sleeper` argument slot.
// Shape: the tests are nested ifs ending in one `return false` (early returns duplicate the epilogue); the component is
//   a named local (`mov esi,[eax+4]`) taken through the inlined HasTC + map find; the claim is a `__forceinline` helper
//   whose locals (the axes and the index) live in the frame and the dead `sleeper` slot, with `((CThing_Retail*)user.GetPThing())` evaluated
//   inside the GetFreeActionPoint argument list (after the other pushes); C3DVector has no user copy/assignment, so the
//   forward axis copies as three dword moves.
struct CTCBase_Retail;
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
typedef std::pair<ETCInterfaceType, CTCBase*> STCEntry_Retail;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:                             // the thing's component map at +0x44: a sorted vector of pairs
    STCEntry_Retail* m_pFirst;
    STCEntry_Retail* m_pLast;
    STCEntry_Retail* m_pEnd;
    STCEntry_Retail* LowerBound(const ETCInterfaceType& kind);   // 0x0040F020 (the key by address)
    __forceinline CTCBase_Retail* Find(long kind)
    {
        STCEntry_Retail* it = LowerBound((ETCInterfaceType)kind);
        if (it == m_pLast || kind < it->first)
            it = m_pLast;
        return ((CTCBase_Retail*)it->second);
    }
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CTCMap_Retail;
struct CThing_Retail {
    unsigned char  _pad00[0x20];
    unsigned long  m_tcMask[8];                    // +0x20: which component kinds the thing holds
    unsigned char  _pad40[4];
    CTCMap_Retail  m_tcs;                          // +0x44
    __forceinline unsigned long HasTC(long kind) const { return m_tcMask[kind >> 5] & (1UL << (kind & 31)); }
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
};
class CRightHandedSet { public:
    C3DVector Up;                                  // +0x00
    C3DVector Forward;                             // +0x0c
    CRightHandedSet() { }
};
struct CTCUserActionPointBase {
    bool GetFreeActionPoint(CThing_Retail* user, C3DVector* position, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
    void UseActionPoint(long index);               // 0x007FEF40
};
class CCPPointerInfo;
template <class T> class CCountedPointer { public: T* Data; CCPPointerInfo* Info; };   // Ego_r.pdb members (Data, Info)
// CScriptThing: the retail model from engine/CScriptThing.h (: CBaseClass, vtable 0x01238C8C, 79 slots, 0xc).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CScriptThing.h"
__forceinline bool ClaimFreeActionPoint(CTCUserActionPointBase* points, const CScriptThing& user, C3DVector& position,
                                        C3DVector& orientation)
{
    CRightHandedSet axes;
    long index;
    if (points->GetFreeActionPoint(((CThing_Retail*)user.GetPThing()), &position, &axes, &index)) {
        orientation = axes.Forward;
        points->UseActionPoint(index);
        return true;
    }
    return false;
}
// CGameScriptInterface is the retail game-script interface from the shared headers (FABLE_RETAIL_NSCRIPT opt-in:
//   CGameScriptInterface : CGameScriptInterfaceBase : CBaseClassNonCopyable : CBaseClass, vtable 0x01260F0C, 918 slots).
#define FABLE_RETAIL_NSCRIPT
#include "engine/CGameScriptInterface.h"

bool CGameScriptInterface::GetSleepingPositionAndOrientationFromBed(const CScriptThing& sleeper, const CScriptThing& bed,
                                                                    C3DVector& position, C3DVector& orientation)
{
    if (sleeper.IsAlive()) {
        if (bed.IsAlive()) {
            if (IsThingABed(bed)) {
                CThing_Retail* bedThing = ((CThing_Retail*)bed.GetPThing());
                if (bedThing->HasTC(0x2d)) {
                    CTCUserActionPointBase* points = (CTCUserActionPointBase*)bedThing->m_tcs.Find(0x2d);
                    if (ClaimFreeActionPoint(points, sleeper, position, orientation))
                        return true;
                }
            }
        }
    }
    return false;
}