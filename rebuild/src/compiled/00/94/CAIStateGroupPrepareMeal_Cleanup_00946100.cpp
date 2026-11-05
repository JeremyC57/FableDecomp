// CAIStateGroup_PrepareMeal::Cleanup() -- release the cooking pot's action point this creature had claimed.
// Leaf: ?Cleanup@CAIStateGroup_PrepareMeal@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00946100, length 130 bytes (+ int3 padding). Vtable slot 9.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (three times on PCookingPot at +0x14: the inlined
// IS_THING_ALIVE pair, then the object for the lookup), CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface
// on the pot, type 0x2D = CTCActionPoints, bit 13 of the bitset dword at +0x24: `test ch,20h`), the action-point
// release @ 0x007FEF40 (`ret 4`; the manifest's "EditCyclePreviousKeyCamera" label is propagated) with
// ActionPointIndex.
// Shape: `if (ActionPointIndex != -1) if (IS_THING_ALIVE(PCookingPot)) { PCookingPot.GetPItem()->GetInterface(&ap,
// 0x2d); ap->FreeActionPoint(ActionPointIndex); }` -- the component is an uninitialised local filled by the inlined
// lookup (the miss path reads the slot and duplicates the call). PCookingPot +0x14 and ActionPointIndex +0x1c are the
// engine header names (the header flattens the base so the class is modelled locally).
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
class CTCBase;
class CThing;
class CWorld;
class CThingAICreature;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound @ 0x0040F020 -- the inlined
// GetInterface<T>() shape of the landed AI bodies (hit path: lookup result; miss path: the never-written slot).
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

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void InitStateGroup();              // slot 13 (+0x34)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFC0 (PBrain->+0x1c->+0x24; manifest "CPlayer::GetCurrentMode" is propagated)
    unsigned long* GetPWorldSeed();             // 0x008FCFD0 (world random seed)
};

long __fastcall GFGetCurrentFrame();            // 0x0049D870 (manifest "GFGetPreMainMemoryUsed" is propagated)
unsigned long __fastcall GFROR13(unsigned long value);   // 0x00497890

class CTCUserActionPointBase { public:
    void SetActionPointAsFree(long a0);   // 0x007FEF40 (Ego_r.pdb ?SetActionPointAsFree@CTCUserActionPointBase@@QAEXJ@Z)
};
struct CTCActionPoints {                        // interface type 0x2D
};

struct CAIStateGroup_PrepareMeal : CAIStateGroupBase {
    CIntelligentPointer<CThing> PCookingPot;        // +0x14 CIntelligentPointer<CThing>
    long ActionPointIndex;                      // +0x1c
    float ActionPointPos_X, ActionPointPos_Y, ActionPointPos_Z;    // +0x20
    virtual void Cleanup();
};

void CAIStateGroup_PrepareMeal::Cleanup()
{
    if (ActionPointIndex != -1) {
        if (IS_THING_ALIVE(PCookingPot)) {
            CTCActionPoints* pActionPoints;
            PCookingPot.Get()->GetTC(TCI_SINGLE_USER_ACTION_POINTS, &pActionPoints);
            ((CTCUserActionPointBase*)pActionPoints)->SetActionPointAsFree(ActionPointIndex);
        }
    }
}