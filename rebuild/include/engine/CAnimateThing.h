#ifndef FABLE_ENGINE_CANIMATETHING_H
#define FABLE_ENGINE_CANIMATETHING_H
#include "engine/CAnimSet.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/CAnimFromFileInfo.h"
class CThing;
class CAnimateThing {public:
 class CCompleteAnimation{public:CAnimSet MainAnimSet;std::list<CAnimSet> TransitionAnimSets,CombinationAnims;bool MainAnimCanTransitionOut;};
 class CDelayedAnimation{public:std::list<CAnimSet> CombinationAnims;};
 void SetMainAnim(const CAnimInit&,long,long);
 void GetAllCombinationAnims(std::vector<CAnimSet*>&);
 void ClearUseAnimFromFile(){AnimFromFileInfo.Active=false;}
private:
 CCompleteAnimation CurrentAnim;
 std::list<CDelayedAnimation> DelayedAnimQueue;
 CDelayedAnimation DelayedAnim;
 C3DAnimationBlendState StartAnimState,EndAnimState,LastStartAnimState,LastEndAnimState;
 float Scale,ChangeAlphaBy;
 long AnimDelay;
 bool Interruptable:1;
 unsigned long CurrentAnimRefreshStamp,LastAnimRefreshStamp;
 CThing& ParentThing;
public:CAnimFromFileInfo AnimFromFileInfo;
};
FABLE_STATIC_ASSERT(sizeof(CAnimateThing)==0xbc);
#endif
