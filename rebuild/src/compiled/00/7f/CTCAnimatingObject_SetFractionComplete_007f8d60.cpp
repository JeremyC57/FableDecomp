// CTCAnimatingObject::SetFractionComplete(float fraction) -- on the thing's animation component's main anim set: the previous completion fraction moves to LastPercentageComplete and the new one is stored
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007F8D60, length 85 bytes.
// Leaf: SetFractionComplete
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end().
// Callees: CVectorMap::LowerBound @ 0x0040F020, CTCAnimationComplex::GetAnimation @ 0x0070B460 (`return *PAnimation`; the
// manifest's "CWorld::DrawGetEnvironment" is propagated). Interface 0x5a (CTCAnimationComplex) = word 2, bit 26 of
// the presence bitset (`test dword ptr [eax+28h],4000000h`). Ego_r.pdb CAnimSet: UpdateMode +0, CurrentPercentageComplete
// +0x24 / LastPercentageComplete +0x28 (retail +0x1c / +0x20 behind two 12-byte vectors); CTCAnimatingObject: Animating
// +0x0c, CurrentAnimationPriority +0x10.
// Shape (beyond the GetInterface template, whose absent path here reads the garbage pointer from the argument's slot):
// the old fraction goes through the x87 (`fld [eax+1Ch] ; fstp [eax+20h]`) while the argument is copied as a dword: the
// old value is a NAMED float local and the new value is stored first in the source (VC7.1 sinks it below the fstp).
// A direct member-to-member copy, or one through an inline float getter, is a pair of integer moves.
struct CTCBaseInterface;
class CTCBase;
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CTCBase { public:                               // engine/CTCBase.h layout, modelled polymorphic
    virtual void v00();
    CThing* ParentThing;                       // +0x04
    bool    ToBeRemoved;                       // +0x08
    bool    IsInGlobalUpdateTCs;               // +0x09
};

class CAnimInit;

class CAnimSet { public:                              // CAnimateThing::CurrentAnim.MainAnimSet (retail layout)
    long  UpdateMode;                          // +0x00 EAnimUpdateMode
    unsigned char _vectors[0x18];              // +0x04 Anims, InternalAlphas (12-byte retail vectors)
    float CurrentPercentageComplete;           // +0x1c
    float LastPercentageComplete;              // +0x20
};

class CAnimateThing { public:
    CAnimSet CurrentAnim;                      // +0x00 (the CCompleteAnimation's MainAnimSet)
    void SetMainAnim(const CAnimInit& init, long mode, long delay);   // 0x0070D580
};

class CTCAnimationComplex { public:
    CAnimateThing& GetAnimation();             // 0x0070B460 (`mov eax,[ecx+0Ch]`, PAnimation)
};

class CTCAnimatingObject : public CTCBase { public:
    bool Animating;                            // +0x0c
    long CurrentAnimationPriority;             // +0x10
    void SetCurrentAnimAsManualUpdate();
    void SetFractionComplete(float fraction);
    void PlayAnimation(const CAnimInit& init, long priority);
};

void CTCAnimatingObject::SetFractionComplete(float fraction)
{
    CTCAnimationComplex* animation;
    ParentThing->GetTC(TCI_ANIMATION, &animation);
    CAnimSet& set = animation->GetAnimation().CurrentAnim;
    float previous = set.CurrentPercentageComplete;
    set.CurrentPercentageComplete = fraction;
    set.LastPercentageComplete = previous;
}