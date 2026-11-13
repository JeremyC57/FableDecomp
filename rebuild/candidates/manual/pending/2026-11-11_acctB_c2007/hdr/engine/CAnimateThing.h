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
#ifdef FABLE_RETAIL_LOOK_HEAD_ANIM
 CAnimSet* GetFirstCombinationAnimFromType(long);
 void AddCombinationAnim(const CAnimInit&,long,long,bool,bool);
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
 // Retail opt-in (Codex pass17): both PDBs; donor1E964F0 called in actual helper.
 bool AnimsCanBeIgnored()const;
#endif
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
#ifdef FABLE_RETAIL_CANIMATETHING_CLEARALLANIMS
 // Retail opt-in (agentNF11, row 0x0070B4D0): Ego_r.pdb / FableWin.pdb ?ClearAllAnims@CAnimateThing@@QAEXXZ, out of line at 0x0070EC30
 // (landed with that leaf). Declaration only.
 void ClearAllAnims();                                                 // 0x0070EC30
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_SETASFINISHED
 // Retail opt-in (agentNF11, row 0x00835E30): Ego_r.pdb / FableWin.pdb ?SetAsFinished@CAnimateThing@@QAEXXZ, out of line at 0x0070BE80
 // (landed with that leaf). Declaration only.
 void SetAsFinished();                                                 // 0x0070BE80
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_INIT
 // Retail opt-in (agentNF11, rows 0x00732850 / ...): Ego_r.pdb / FableWin.pdb ?Init@CAnimateThing@@QAEXABVCAnimInit@@@Z, out of line at
 // 0x0070D920 (Ego_r donor order SetMainAnim 0x00577F00 / Init 0x00578140 = retail SetMainAnim 0x0070D580 / Init 0x0070D920; the body
 // forwards (init, 1, 0) to SetMainAnim). Declaration only.
 void Init(const CAnimInit& init);                                     // 0x0070D920
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_UPDATEANIMATION
 // Retail opt-in (w22 agent2007, row 0x00665860 CThingCreatureBase::UpdateAnimation): FableWin.pdb ?IsMainAnimFinished@CAnimateThing@@QBE_NXZ
 // (out of line in retail at 0x0070C0E0: no anim index, a first index <= 0 or the main set finished), the inline
 // ?IsInterruptable@CAnimateThing@@QBE_NXZ / ?SetInterruptable@CAnimateThing@@QAEX_N@Z (the bit at +0x94: `test byte [animator+94h],1`,
 // `or byte [animator+94h],1`), ?GetMainAnimType@CAnimateThing@@QBEJXZ (inline: MainAnimSet.Type +0x50) and Ego_r.pdb / FableWin.pdb
 // ?RemoveCombinationAnimsOfType@CAnimateThing@@QAEXJ@Z (0x0070D8C0). Needs FABLE_RETAIL_CANIMATETHING_MAINANIM (PeekMainAnim) and
 // FABLE_RETAIL_CANIMSET_TYPE.
 bool IsMainAnimFinished()const;                                       // 0x0070C0E0
 bool IsInterruptable()const{return Interruptable;}
 void SetInterruptable(bool interruptable){Interruptable=interruptable;}
 long GetMainAnimType()const{return CurrentAnim.MainAnimSet.GetType();}
 void RemoveCombinationAnimsOfType(long type);                         // 0x0070D8C0
#endif
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
