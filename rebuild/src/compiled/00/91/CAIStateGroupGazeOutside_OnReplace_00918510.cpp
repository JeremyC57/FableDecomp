// CAIStateGroup_GazeOutside::OnReplace() -- CAIStateGroup_Gaze::OnReplace, then ask the creature's idle scheduler for a new idle
// Leaf: ?OnReplace@CAIStateGroup_GazeOutside@@MAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x00918510, 75 bytes, thiscall, plain `ret`.
// Identity (proven): CAIStateGroup_GazeOutside vtable 0x0128FE34 slot 10 (OnReplace); Ego_r.pdb ?OnReplace@CAIStateGroup_GazeOutside@@MAEXXZ (donor 0x0093C280);
//   RTTI bases CAIStateGroup_GazeOutside : CAIStateGroup_Gaze : CAIStateGroupBase.
// Callees: CAIStateGroup_Gaze::OnReplace() 0x009183A0, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound 0x0040F020, CTCIdleScheduler::ChooseNewIdle()
//   0x00801A50.
// Shape (family K045, representative 0x00917D60): the qualified base call, then the landed CAIStateGroup_MinionWatchFires::OnReplace
//   0x00936590 body (its model, verbatim: GetInterface on PBrain->PCreature, interface 0xD3 = bit 19 of the dword at +0x38).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    CAIBrain* PBrain;                           // +0x04
};
class CTCIdleScheduler { public:                // interface type 0xD3 component
    void ChooseNewIdle();                       // 0x00801A50
};
class CAIStateGroup_Gaze : public CAIStateGroupBase {
protected:
    virtual void OnReplace();                   // 0x009183A0
};
class CAIStateGroup_GazeOutside : public CAIStateGroup_Gaze {
protected:
    virtual void OnReplace();
};
void CAIStateGroup_GazeOutside::OnReplace()
{
    CAIStateGroup_Gaze::OnReplace();
    CTCIdleScheduler* pScheduler;
    if (PBrain->PCreature->GetTC(TCI_IDLE_SCHEDULER, &pScheduler))
        pScheduler->ChooseNewIdle();
}