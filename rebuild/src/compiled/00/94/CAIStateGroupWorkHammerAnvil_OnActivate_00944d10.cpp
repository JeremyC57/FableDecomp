// CAIStateGroup_WorkHammerAnvil::OnActivate() -- claim a free action point on the anvil, else terminate
// Leaf: ?OnActivate@CAIStateGroup_WorkHammerAnvil_Methods@@QAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944D10, length 154 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x2D = bit 13 of the presence word at +0x24), CTCUserActionPointBase::IsActionPointFree
// @ 0x007FEBF0, CTCUserActionPointBase::GetFreeActionPoint(creature, pos*, rhset*, index*) @ 0x007FEDE0,
// CAIStateGroupBase::Terminate @ 0x008FCF40 (manifest label "CXMVPlaybackInfo" is propagated and rejected).
//
// Shape: one `&&` chain -- IS_THING_ALIVE(PAnvil) (two GetPItem calls), then the third GetPItem feeds the
// bool-returning GetInterface (its bit test is the third `je fail`), then the action-point component (kept in
// esi) is asked whether a point is free and to reserve it into ActionPointPos (+0x1d), ActionPointRHSet
// (+0x29) and ActionPointIndex (+0x44) for the brain's creature. Success sets TookActionPoint (+0x14) and
// returns; any failure clears it and terminates the state group. The creature is a named local taken before
// the reservation call (retail loads PBrain->PCreature before the address pushes; inline it moves between them). `this` is the PDB header layout (pack 1:
// PAnvil sits unaligned at +0x15); PBrain lives in the flattened base bytes at +0x04.
#include "engine/CAIStateGroup_WorkHammerAnvil.h"

class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the landed out-parameter
// GetInterface template (0x007504E0 / 0x00930DC0 shape): the component lands in eax, and a miss leaves the
// caller's out pointer unwritten (the frame slot that also holds the key).
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
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

class C3DVector;
class CRightHandedSet;

struct CTCUserActionPointBase {                 // interface type 0x2D component
    bool IsActionPointFree();                   // 0x007FEBF0
    bool GetFreeActionPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, long* index); // 0x007FEDE0
};

struct CAIStateGroup_WorkHammerAnvil_Methods : CAIStateGroup_WorkHammerAnvil {
    CAIBrain* GetPBrain() const { return *(CAIBrain* const*)_base_0x04; }   // CAIStateGroupBase::PBrain (+0x04)
    const CIntelligentPointer<CThing>& Anvil() const { return *(const CIntelligentPointer<CThing>*)PAnvil; }
    void Terminate();                           // 0x008FCF40
    void OnActivate();
};

void CAIStateGroup_WorkHammerAnvil_Methods::OnActivate()
{
    CTCUserActionPointBase* actionPoint;
    if (IS_THING_ALIVE(Anvil()) && Anvil().GetPItem()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &actionPoint)
        && actionPoint->IsActionPointFree()) {
        CThing* creature = GetPBrain()->PCreature;
        if (actionPoint->GetFreeActionPoint(creature, (C3DVector*)&ActionPointPos_X,
                                            (CRightHandedSet*)ActionPointRHSet, &ActionPointIndex)) {
            TookActionPoint = true;
            return;
        }
    }
    TookActionPoint = false;
    Terminate();
}