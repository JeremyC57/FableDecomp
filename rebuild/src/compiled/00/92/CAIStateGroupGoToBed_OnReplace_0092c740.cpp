// CAIStateGroup_GoToBed::OnReplace() -- give the bed's action point (or the bed itself) back and forget the bed
// Leaf: ?OnReplace@CAIStateGroup_GoToBed@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C740, length 347 bytes (vtable 0x0128B104 slot 10).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
//          (the inlined GetInterface: type 0x2D = bit 13 of the bitset dword +0x24, type 0xAC = bit 12 of +0x34),
//          the action-point owner's FreePoint(long) @ 0x007FEF40 (landed as CPointerListOwner_007fef40::Add: appends the
//          index to its free list), the bed component's GetOccupant @ 0x0087C3E0 and SetOccupant(CThing*) @ 0x0087C390
//          (manifest labels "_Cons_val<...>" / "CVillageTaskCarryCrate::AssignCreatureToTask" are propagated),
//          CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem @ 0x00A01B90, dtor @ 0x00A01C10 (the PBed = 0 temporary,
//          vtable 0x01238C6C).
// Shape: IS_THING_ALIVE is the two-GetPItem inline test, the component comes from a third GetPItem through the
//          out-parameter GetInterface (miss path reads the never-written out slot, which the clearing temporary
//          later reuses); the -1 marker stays in edi for the store; the occupant is compared with the creature
//          read before the call.
// Layout: PDB CAIStateGroup_GoToBed: PBed +0x14, IsBedSleepPointMarker +0x1C, ActionPointIndex +0x2C (retail agrees,
//          see the landed GoToBed rows). No engine header exists for the class; the base follows engine/CAIStateGroupBase.h.
class CTCBase;
class CThing;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CBaseObject;
class CBaseObjectPointer;
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"


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

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

class CTCUserActionPointBase { public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
};
struct CTCActionPointOwner {                    // interface 0x2d
};
class CTCBed { public:                                 // interface 0xac
    CThing* GetOccupant();                      // 0x0087C3E0
    void SetOccupant(CThing* occupant);         // 0x0087C390
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CIntelligentPointer<CThing> PBed;              // +0x14
    bool IsBedSleepPointMarker;                 // +0x1c
    long SleepStartTimeVariation;               // +0x20
    long SleepEndTimeVariation;                 // +0x24
    bool BedToRightOfCreature;                  // +0x28
    long ActionPointIndex;                      // +0x2c
    virtual void OnReplace();
};

void CAIStateGroup_GoToBed::OnReplace()
{
    if (!IsBedSleepPointMarker) {
        if (ActionPointIndex != -1 && IS_THING_ALIVE(PBed)) {
            CTCActionPointOwner* points;
            PBed.Get()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &points);
            ((CTCUserActionPointBase*)points)->SetActionPointAsFree(ActionPointIndex);
            ActionPointIndex = -1;
        }
    }
    else if (IS_THING_ALIVE(PBed)) {
        CTCBed* bed;
        PBed.Get()->GetTC(TCI_SINGLE_PERSON_OCCUPIABLE, &bed);
        CThing* creature = PBrain->PCreature;
        if (bed->GetOccupant() == creature)
            bed->SetOccupant(0);
    }
    PBed = 0;
}