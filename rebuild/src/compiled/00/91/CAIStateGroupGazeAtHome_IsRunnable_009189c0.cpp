// CAIStateGroup_GazeAtHome::IsRunnable() -- runnable when the idle scheduler prefers this group, the creature has a home and a free gaze position exists
// Leaf: ?IsRunnable@CAIStateGroup_GazeAtHome@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009189C0, length 114 bytes (+ int3 padding). Vtable slot 11
// of CAIStateGroup_GazeAtHome (0x0128FE7C).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined interface lookup, type 0xd3 = the idle scheduler),
// CTCIdleScheduler::IsPreferredIdle @ 0x00801BE0, virtual GetOwner (slot 14, +0x38, 0x00919000),
// CAIStateGroup_Gaze::FindFreeGazePos @ 0x00918880.
//
// Shape: the scheduler test is a nested `if` whose false exit returns the call's own `al` (VC knows it is 0
// after `test al,al ; jne`), the tail is a `&&` of two pointer tests, which VC materialises as a full-register
// `mov eax,1` / `xor eax,eax` result. The key temporary of the inlined lookup lands in the `push ecx` slot.
// The engine headers are flattened data-only layouts without the vptr chain: `this` is a local model.
class CTCBase;
class CThingBuilding;
class CThingGameObject;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase;

class CTCIdleScheduler { public:                       // interface type 0xd3
    bool IsPreferredIdle(CAIStateGroupBase* group);   // 0x00801BE0
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void v12(); virtual void v13();
    virtual CThingBuilding* GetOwner();         // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_Gaze : CAIStateGroupBase {
    unsigned char GazeMarker[0x08];             // +0x14 CIntelligentPointer<CThingGameObject>
    float ChanceOfRunning;                      // +0x1C
    float MinJourneyForRunning;                 // +0x20
    float MaxDistanceForCreaturesToLookAt;      // +0x24
    long PrimaryMarkerDef;                      // +0x28
    long SecondaryMarkerDef;                    // +0x2C
    CThingGameObject* FindFreeGazePos();        // 0x00918880
};

struct CAIStateGroup_GazeAtHome : CAIStateGroup_Gaze {
    virtual bool IsRunnable();
};

bool CAIStateGroup_GazeAtHome::IsRunnable()
{
    CTCIdleScheduler* scheduler;
    if (PBrain->PCreature->GetTC(TCI_IDLE_SCHEDULER, &scheduler)) {
        if (!scheduler->IsPreferredIdle(this))
            return false;
    }
    return GetOwner() && FindFreeGazePos();
}