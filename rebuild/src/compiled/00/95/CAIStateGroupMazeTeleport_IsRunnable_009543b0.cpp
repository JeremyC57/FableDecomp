// CAIStateGroup_MazeTeleport::IsRunnable() -- runnable when the boss component is ready for a phase change.
// Leaf: ?IsRunnable@CAIStateGroup_MazeTeleport@@UAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009543B0, length 91 bytes (+ int3 padding).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (the inlined interface lookup, type 0xAD = bit 13 of the bitset
// dword at +0x34), then CTCBoss::IsReadyForPhaseChange @ 0x007AEE90 whose byte result is normalised to a bool
// (`test al,al ; setne al` = `return byte ? true : false;` -- a plain `!= 0` widens through eax with
// `neg al ; sbb eax,eax ; neg eax`).
// Shape: uninitialised component local, lookup bool not tested (the miss path reads the dead slot `[esp]`),
// the call + normalisation duplicated on both paths. Base layout modelled locally (PDB names).
class CTCBase;

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with CVectorMap::LowerBound -- the same shape the landed
// GetInterface_0x.. bodies use, here inlined into the caller through GetTC<T>().
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* Creature;                           // +0x18 CThingAICreature*
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

class CTCBoss { public:                                // interface type 0xAD component
public:
    bool IsReadyForPhaseChange() const;   // 0x007AEE90
};

struct CAIStateGroup_MazeTeleport : CAIStateGroupBase {
    virtual bool IsRunnable();
};

bool CAIStateGroup_MazeTeleport::IsRunnable()
{
    CTCBoss* pBoss;
    PBrain->Creature->GetTC(TCI_BOSS, &pBoss);
    return pBoss->IsReadyForPhaseChange() ? true : false;
}