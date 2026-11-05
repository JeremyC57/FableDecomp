// CTCLook::IsLookTimeOver() -- false while the owner's interface-0xAA component exists and has its "keep looking" byte (+0x7A) clear; otherwise whether the frame counter has reached LookTimer
// Leaf: ?IsLookTimeOver@CTCLook@@QAE_NXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: map pointer made const (the row grades the const LowerBound).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753AD0, length 101 bytes.
// Callees: CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound @ 0x004365B0 (inlined GetInterface, type 0xAA = bit 10 of the dword at
// CThing+0x34: `test ch,4`), CTCBase::PeekComponent @ 0x00686D60 (`[0x013B8A1C]+0x1C`) and CMainGameComponent::GetCurrentWorldFrame @ 0x004167C8
// (a `jmp` to 0x0049D870; the landed UpdateLookTimer 0x00751AD0 uses the same pair).
// Layout (retail, local model): ParentThing +0x04, LookTimer +0x3C (PDB offset); the component's flag byte +0x7A (component type
// not identified, named descriptively).
// Shape: out-pointer GetInterface `&&` the flag test returning false; the tail `return frame >= LookTimer;` (`setge cl ; mov al,cl`).
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CTCLookTimeOwner {                       // the interface-0xAA component
    unsigned char _pad00[0x7a];
    bool KeepLooking;                           // +0x7a
};
class CMainGameComponent { public:
    long GetCurrentWorldFrame() const;             // 0x004167C8 (Ego_r.pdb ?GetCurrentWorldFrame@CMainGameComponent@@QBEJXZ)
};
class CTCBase {
public:
    virtual void v00();
    CThing* ParentThing;                        // +0x04
protected:
    const CMainGameComponent& PeekComponent() const;   // 0x00686D60 (Ego_r.pdb ?PeekComponent@CTCBase@@IBEABVCMainGameComponent@@XZ)
};
class CTCLook : public CTCBase {
public:
    unsigned char _pad08[0x3c - 0x08];
    long LookTimer;                             // +0x3c
    bool IsLookTimeOver();
};

bool CTCLook::IsLookTimeOver()
{
    const CTCLookTimeOwner* owner;
    if (ParentThing->PeekTC(TCI_AI_SCRATCHPAD, &owner) && !owner->KeepLooking)
        return false;
    return PeekComponent().GetCurrentWorldFrame() >= LookTimer;
}