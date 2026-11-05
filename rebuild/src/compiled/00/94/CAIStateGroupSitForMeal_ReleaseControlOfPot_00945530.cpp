// CAIStateGroup_SitForMeal::ReleaseControlOfPot() -- free the cooking-pot action point this creature holds
// Leaf: ?ReleaseControlOfPot@CAIStateGroup_SitForMeal@@AAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00945530, length 119 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (x3), CVectorMap::LowerBound @ 0x0040F020 (inlined
// GetInterface, type 0x2D), CTCUserActionPointBase::SetActionPointAsFree(long) @ 0x007FEF40 (manifest label
// "CTCCameraPointScriptedSpline::EditCyclePreviousKeyCamera" is propagated and rejected).
//
// Shape: nothing to do while ActionPointIndex (+0x3c) is negative (`jl` straight to the epilogue). Otherwise,
// when PPot (+0x34) is alive and has the action-point component (bool GetInterface as the `if`, its bit test
// jumps to the reset), the index is released; the index is then forgotten (-1) on every path. No engine
// header exists: members carry their PDB names and offsets (sizeof 0x48).
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
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"


// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.Get() != 0) {
        if (!p.Get()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CTCUserActionPointBase {
public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
public:                 // interface type 0x2D component
};

class CTCCarrying { public:                            // interface type 0x46 component
    CThing* RemoveThingFromPrimarySlot(bool drop);  // 0x006A1E00
};

struct CGameTime {
    unsigned char _pad[0x14];
    long Day;                                   // +0x14
};

class CWorld { public:
    unsigned char _pad[0x1c];
    CGameTime* PGameTime;                       // +0x1c
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    bool Terminated;                            // +0x10
    unsigned char Priority;                     // +0x11
    bool Enabled;                               // +0x12
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24)
};

// PDB layout (sizeof 0x48): the state-group bytes up to +0x34 are a second base's (unnamed in the PDB).
struct CAIStateGroup_SitForMeal : CAIStateGroupBase {
    unsigned char _base2[0x20];                 // +0x14
    CIntelligentPointer<CThing> PPot;               // +0x34 CIntelligentPointer<CThing>
    long ActionPointIndex;                      // +0x3c
    bool HoldingBowl;                           // +0x40
    long DayLastAte;                            // +0x44
private:
    void ReleaseControlOfPot();
public:
    void Cleanup();
};

void CAIStateGroup_SitForMeal::ReleaseControlOfPot()
{
    if (ActionPointIndex >= 0) {
        if (IS_THING_ALIVE(PPot)) {
            CTCUserActionPointBase* actionPoint;
            if (PPot.Get()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &actionPoint))
                ((CTCUserActionPointBase*)actionPoint)->SetActionPointAsFree(ActionPointIndex);
        }
        ActionPointIndex = -1;
    }
}