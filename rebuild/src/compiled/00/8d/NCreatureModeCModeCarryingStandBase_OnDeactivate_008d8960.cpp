// NCreatureMode::CModeCarryingStandBase::OnDeactivate() -- drop the carrying combination animations (type 0x18) from the creature's animation, then the stand-mode deactivation
// Leaf: ?OnDeactivate@CModeCarryingStandBase@NCreatureMode@@UAEXXZ
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Retail: Fable.exe (TLC, VC7.1) @ 0x008D8960, 90 bytes (+ int3 padding), `ret`; vtable 0x01267400 slot 6.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface, type 0x5a = bit 26 of the dword at +0x28 =
//   CTCAnimationComplex), CTCAnimationComplex::GetAnimation @ 0x0070B460 (manifest "CWorld::DrawGetEnvironment" is
//   propagated), CAnimateThing::RemoveCombinationAnimsOfType(long) @ 0x0070D8C0 (manifest "CSpellContainerList::AddChild"
//   is propagated), CModeStandBase::OnDeactivate @ 0x00861D00 (qualified).
// Name: FableWin.pdb `?OnDeactivate@CModeCarryingStandBase@NCreatureMode@@UAEXXZ` (slot 6).
// Shape: the landed sibling CModeCarryingStandBase::FrameUpdate @ 0x008D88A0: the GetInterface result is not tested, so the
//   absent path joins with the never-written out variable, which shares the lookup key's frame word (`mov eax,[esp+4]`).
//   The animation is a named reference: chained `GetAnimation().Remove(0x18)` schedules the `push 18h` before the getter call.
class CTCBase;

#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CAnimateThing { public:
    void RemoveCombinationAnimsOfType(long type);   // 0x0070D8C0
};
class CTCAnimationComplex { public:               // interface type 0x5a
    CAnimateThing& GetAnimation();         // 0x0070B460
};

struct CModeBase {                         // CTCCreatureModeManager::CModeBase, retail 0x1c
    virtual void v00();
    CThing*       ParentThing;             // +0x04
    unsigned char _body[0x14];
};
struct CModeStandBase : CModeBase {        // retail 0x68
    unsigned char _stand[0x4c];
public:
    virtual void OnDeactivate();   // 0x00861D00
};

namespace NCreatureMode {
struct CModeCarryingStandBase : CModeStandBase {   // vtable 0x01267400
    void OnDeactivate();
};
}

void NCreatureMode::CModeCarryingStandBase::OnDeactivate()
{
    CTCAnimationComplex* animation;
    ParentThing->GetTC(TCI_ANIMATION, &animation);
    CAnimateThing& animate = animation->GetAnimation();
    animate.RemoveCombinationAnimsOfType(0x18);
    CModeStandBase::OnDeactivate();
}