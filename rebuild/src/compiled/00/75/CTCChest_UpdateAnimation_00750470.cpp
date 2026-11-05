// CTCChest::UpdateAnimation(float time) -- advance the chest's animation (its CTCAnimationComplex's animate-thing) by the given time step
// Leaf: ?UpdateAnimation@CTCChest@@QAEXM@Z
// Plan phase 2 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail 0x00750470, 110 bytes (`ret 4`). Callees: CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound
// @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (`mov eax,[ecx+0Ch]`; manifest
// "CWorld::DrawGetEnvironment" is propagated), CAnimateThing::UpdateFrameLogic(const update&) @ 0x0070E8B0.
// Shape: the component comes from the zero-initialised out-parameter GetInterface (its result is not tested) whose
// entry pick is one end/hit ternary: the preloaded null stays in ecx (`xor ecx,ecx`) and a hit loads the component
// straight into ecx, with the whole-word bit test (`test edx,4000000h`); the 12-byte update record {time, 0, 0} is a
// frame local whose inline constructor zeroes the two reserved words in its init list and assigns the time in the
// body (the zero words are stored before the time).
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

struct CAnimFrameUpdate {                                   // 12-byte update record
    float Time;                                             // +0x00
    long  Reserved0;                                        // +0x04
    long  Reserved1;                                        // +0x08
    CAnimFrameUpdate(float time) : Reserved0(0), Reserved1(0) { Time = time; }
};

class CAnimateThing { public:
    void UpdateFrameLogic(const CAnimFrameUpdate& update);  // 0x0070E8B0
};

class CTCAnimationComplex { public:
    CAnimateThing* GetAnimation();                          // 0x0070B460
};

class CTCChest { public:
    virtual void v00();
    CThing* ParentThing;                                    // +0x04 (CTCBase)
    void UpdateAnimation(float time);
};

void CTCChest::UpdateAnimation(float time)
{
    CTCAnimationComplex* complex = 0;
    ParentThing->GetTC(TCI_ANIMATION, &complex);
    CAnimFrameUpdate update(time);
    CAnimateThing* animation = complex->GetAnimation();
    animation->UpdateFrameLogic(update);
}