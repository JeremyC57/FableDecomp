#ifndef FABLE_ENGINE_CANIMATETHING_H
#define FABLE_ENGINE_CANIMATETHING_H
#include "engine/CAnimSet.h"
#include "engine/C3DAnimationBlendState.h"
#include "engine/CAnimFromFileInfo.h"
#ifdef FABLE_RETAIL_CANIMATETHING_MAINANIM
#include "engine/CAnimateThingUpdate.h"
#endif
class CThing;
#ifdef FABLE_RETAIL_CANIMATETHING_GETMESHBANK
class CMeshDataBank;
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_PROGRESSFROMALPHA
extern const float DELTA_NOTIONAL_ZERO;                                   // 0x0129BA3C
#endif
class CAnimateThing {public:
#ifdef FABLE_RETAIL_CANIMATETHING_CTOR_PASS26
 // Pass26 native caller0070b418 and both PDBs prove CThing& formal.
 CAnimateThing(CThing& parent_thing);
#endif
#ifdef FABLE_RETAIL_LOOK_HEAD_ANIM
 CAnimSet* GetFirstCombinationAnimFromType(long);
 void AddCombinationAnim(const CAnimInit&,long,long,bool,bool);
#endif
#ifdef FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
 // Retail opt-in (Codex pass17): both PDBs; donor1E964F0 called in actual helper.
 bool AnimsCanBeIgnored()const;
#endif
 class CCompleteAnimation{public:
#ifdef FABLE_RETAIL_CANIMATETHING_CTOR_PASS26
 // Original user constructor, called at native0070e717.
 CCompleteAnimation();
#endif
CAnimSet MainAnimSet;std::list<CAnimSet> TransitionAnimSets,CombinationAnims;bool MainAnimCanTransitionOut;};
 class CDelayedAnimation{public:std::list<CAnimSet> CombinationAnims;};
#ifdef FABLE_RETAIL_CANIMATETHING_UPDATE_CALLEES
 // Opt-in (p25A, row 0x0070E8B0 CAnimateThing::Update): the private members Update calls out of line, Ego_r.pdb / FableWin.pdb
 // ?UpdateCombinationAnimList@CAnimateThing@@AAEXAAV?$list@VCAnimSet@@V?$allocator@VCAnimSet@@@std@@@std@@ABVCAnimateThingUpdate@@@Z
 // (0x0070D390, landed) and ?PreloadAnims@CAnimateThing@@AAEXABVCCompleteAnimation@1@@Z (0x0070C590, landed). Declarations only;
 // needs FABLE_RETAIL_CANIMATETHING_MAINANIM (CAnimateThingUpdate).
private:
 void UpdateCombinationAnimList(std::list<CAnimSet>& anims, const CAnimateThingUpdate& update);   // 0x0070D390
 void PreloadAnims(const CCompleteAnimation& animation);              // 0x0070C590
public:
#endif
 void SetMainAnim(const CAnimInit&,long,long);
#ifdef FABLE_RETAIL_CANIMATETHING_MAINANIM
 // Retail opt-in (chunk 1006, row 0x005B3E4E): FableWin.pdb ?GetMainAnim@CAnimateThing@@QAEAAVCAnimSet@@XZ and
 // ?PeekMainAnim@CAnimateThing@@QBEABVCAnimSet@@XZ (inline: CurrentAnim.MainAnimSet at +0, so the caller reads [animator+4] for the
 // anim indices), and the out-of-line ?Update@CAnimateThing@@QAEXABVCAnimateThingUpdate@@@Z (Ego_r.pdb / FableWin.pdb, retail 0x0070E8B0).
 CAnimSet& GetMainAnim(){return CurrentAnim.MainAnimSet;}
#ifdef FABLE_RETAIL_CANIMATETHING_MAINANIMINDEX_PASS26
 // Both original PDBs; original01d92650 directly indexes the main set Anims.
 long GetMainAnimIndex()const{return CurrentAnim.MainAnimSet.Anims[0];}
#endif
 const CAnimSet& PeekMainAnim()const{return CurrentAnim.MainAnimSet;}
 void Update(const CAnimateThingUpdate& update);                       // 0x0070E8B0
#endif
 void GetAllCombinationAnims(std::vector<CAnimSet*>&);
#ifdef FABLE_RETAIL_CANIMATETHING_REMOVE_COMBINATION_CALL_PASS24
 void RemoveCombinationAnim(const CAnimSet* anim);
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_GETTHINGSCALE
 // Retail opt-in (row 0x0070CE10): FableWin.pdb ?GetThingScale@CAnimateThing@@QBEMXZ, inline in retail (the caller reads Scale +0x88).
 float GetThingScale()const{return Scale;}
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_CLEARALLANIMS
 // Retail opt-in (agentNF11, row 0x0070B4D0): Ego_r.pdb / FableWin.pdb ?ClearAllAnims@CAnimateThing@@QAEXXZ, out of line at 0x0070EC30
 // (landed with that leaf). Declaration only.
 void ClearAllAnims();                                                 // 0x0070EC30
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_CLEARQUEUES
 // NF65 overlay opt-in (row 0x008DF3A0): the out-of-line member at 0x0070D510 that clears the current animation's two anim-set lists
 // (+0x58 then a tail call for +0x64). DESCRIPTIVE name: the landed self row's catalog name CAnimateThing::ClearQueues (one address, one
 // name); no PDB spelling is proven for it (Ego_r has ClearTransitionAnims / ClearCombinationAnims separately). Declaration only.
 void ClearQueues();                                                   // 0x0070D510
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
#ifdef FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED
 // NF59 (queue q10507): 0x0070C0E0 is the private const ?IsAnimationFinished@CAnimateThing@@ABE_NXZ (Ego_r.pdb 0x005752F0 / FableWin.pdb
 // 0x01E96330; retail placement = FableWin order GetMeshBank / IsAnimationFinished / AnimsCanBeIgnored). FableWin's public
 // IsMainAnimFinished (0x01D92820; no Ego_r copy, inline in release) is the wrapper a non-member caller (0x00665860) reaches it through;
 // its body here is the plain forward the retail call site shows (`call 0x0070C0E0`, then the caller's own Interruptable test).
 bool IsMainAnimFinished()const{return IsAnimationFinished();}
#else
 bool IsMainAnimFinished()const;                                       // 0x0070C0E0
#endif
 bool IsInterruptable()const{return Interruptable;}
 void SetInterruptable(bool interruptable){Interruptable=interruptable;}
 long GetMainAnimType()const{return CurrentAnim.MainAnimSet.GetType();}
 void RemoveCombinationAnimsOfType(long type);                         // 0x0070D8C0
#endif
 void ClearUseAnimFromFile(){AnimFromFileInfo.Active=false;}
#ifdef FABLE_RETAIL_CANIMATETHING_MESHANIMS
// agentFAM05 (2026-11-11): Ego_r.pdb / FableWin.pdb ?GetInterpolatedMeshAnim@CAnimateThing@@QBEXAAVC3DAnimationBlendState@@M@Z and
// ?GetMeshAnims@CAnimateThing@@QBEXAAVC3DAnimationBlendState@@0@Z, out of line at 0x0070E650 / 0x0070E6A0 (the CTCAnimationComplex
// slot 28 / 29 overrides 0x0070B440 / 0x0070B450 tail-jump to them with ecx = PAnimation). Declarations only.
 void GetInterpolatedMeshAnim(C3DAnimationBlendState& state, float time) const;   // 0x0070E650
 void GetMeshAnims(C3DAnimationBlendState& state, C3DAnimationBlendState& blend) const;   // 0x0070E6A0
#endif // FABLE_RETAIL_CANIMATETHING_MESHANIMS
#ifdef FABLE_RETAIL_CANIMATETHING_ISFINISHED
 // NF59 opt-in (row 0x00661720; needs FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED): the unreferenced out-of-line accessor at 0x00661720, `IsAnimationFinished() || Interruptable` (the
 // test 0x00665860 inlines next to SetAsFinished). No PDB names it: descriptive name, the landed catalog's (CAnimateThing::IsFinished).
 bool IsFinished()const;                                               // 0x00661720
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_ISANIMATIONFINISHED
private:
 bool IsAnimationFinished()const;                                      // 0x0070C0E0
#endif
private:
 CCompleteAnimation CurrentAnim;
 std::list<CDelayedAnimation> DelayedAnimQueue;
 CDelayedAnimation DelayedAnim;
#ifdef FABLE_RETAIL_CANIMATETHING_CALCCACHEDANIMS
 // Retail opt-in (Codex pass 24). Ego_d const this and native writes at
 // 0x0070E2A0 corroborate mutable cached animation state.
 mutable C3DAnimationBlendState StartAnimState,EndAnimState,LastStartAnimState,LastEndAnimState;
 void CalcCachedAnims() const;
 C3DAnimationBlendState CalcInterpolatedMeshAnim(const CCompleteAnimation&, const CDelayedAnimation&, float) const;
#ifdef FABLE_RETAIL_CANIMATETHING_PROGRESSFROMALPHA
 // Nested opt-in (B24C6, row 0x0070D940): Ego_r.pdb / FableWin.pdb ?GetProgressFromAlpha@CAnimateThing@@ABEMMM@Z, out of line at 0x0070BE40
 // (landed, same body) and expanded in place inside the transition loop of 0x0070D940: an inline body the size-mode inliner
 // expands once its budget allows. DELTA_NOTIONAL_ZERO 0x0129BA3C.
 float GetProgressFromAlpha(float alpha, float totalAlpha) const
 {
  if (totalAlpha >= DELTA_NOTIONAL_ZERO && DELTA_NOTIONAL_ZERO + 1.0f >= totalAlpha) return alpha / totalAlpha;
  return 1.0f;
 }
#endif
 const CThing& GetParentThing() const { return ParentThing; }
#else
 C3DAnimationBlendState StartAnimState,EndAnimState,LastStartAnimState,LastEndAnimState;
#endif
#ifdef FABLE_RETAIL_CANIMATETHING_GETMESHBANK
public:
 // B24C8/B24C9 opt-in (row 0x0070D940): Ego_r.pdb (release animate_thing.obj, 12 bytes) and FableWin.pdb 0x01E92060
 // ?GetMeshBank@CAnimateThing@@QBEABVCMeshDataBank@@XZ (public const); retail's out-of-line copy is 0x0070C0D0 (`mov eax,[PMainGameComponent];
 // mov ecx,[eax+24h]; mov eax,[ecx+3Ch]`). 0x0070D940 expands it as the object of `GetMeshBank().PeekAnimation(index)` at 0x0070DFD3 /
 // 0x0070DFF9 (the object chain evaluated before the argument). Body: engine/CAnimateThing_GetMeshBank.h.
 const CMeshDataBank& GetMeshBank()const;                              // 0x0070C0D0
private:
#endif
 float Scale,ChangeAlphaBy;
 long AnimDelay;
 bool Interruptable:1;
 unsigned long CurrentAnimRefreshStamp,LastAnimRefreshStamp;
 CThing& ParentThing;
public:CAnimFromFileInfo AnimFromFileInfo;
};
FABLE_STATIC_ASSERT_AT(84, sizeof(CAnimateThing)==0xbc);
#endif
