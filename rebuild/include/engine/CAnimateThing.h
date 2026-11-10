#ifndef FABLE_ENGINE_CANIMATETHING_H
#define FABLE_ENGINE_CANIMATETHING_H
#include "engine/CAnimSet.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/CAnimFromFileInfo.h"
#ifdef FABLE_RETAIL_CANIMATETHING_MAINANIM
#include "engine/CAnimateThingUpdate.h"
#endif
class CThing;
class CAnimateThing {public:
 class CCompleteAnimation{public:CAnimSet MainAnimSet;std::list<CAnimSet> TransitionAnimSets,CombinationAnims;bool MainAnimCanTransitionOut;};
 class CDelayedAnimation{public:std::list<CAnimSet> CombinationAnims;};
 void SetMainAnim(const CAnimInit&,long,long);
#ifdef FABLE_RETAIL_CANIMATETHING_MAINANIM
 // Retail opt-in (chunk 1006, row 0x005B3E4E): FableWin.pdb ?GetMainAnim@CAnimateThing@@QAEAAVCAnimSet@@XZ and
 // ?PeekMainAnim@CAnimateThing@@QBEABVCAnimSet@@XZ (inline: CurrentAnim.MainAnimSet at +0, so the caller reads [animator+4] for the
 // anim indices), and the out-of-line ?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z (Ego_r.pdb / FableWin.pdb, retail 0x0070E8B0).
 CAnimSet& GetMainAnim(){return CurrentAnim.MainAnimSet;}
 const CAnimSet& PeekMainAnim()const{return CurrentAnim.MainAnimSet;}
 void Update(const CAnimateThingUpdate& update);                       // 0x0070E8B0
#endif
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
