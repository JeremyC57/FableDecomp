#ifndef FABLE_ENGINE_CANIMSET_H
#define FABLE_ENGINE_CANIMSET_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CAnimInit.h"
#ifdef FABLE_RETAIL_CANIMSET_MESHANIM
#include "engine/CIVCountedPointer.h"
#include "engine/C3DAnimationStateAlpha.h"
#include "engine/GFGetInterpolatedValue.h"
#endif
template<class T,class L>inline void FABLE_FASTCALL GFLimitLower(T& value,const L& lower){if(value<lower)value=lower;}
class CAnimateThing;
#ifdef FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE
class CAnimateThingUpdate;
#endif
#ifdef FABLE_RETAIL_CANIMSET_UPDATEDISTANCE
class C3DAnimationInfo;
#endif
class CTransitionBlendAlpha{public:float CurrentAlpha,LastAlpha;CTransitionBlendAlpha(float a,float b):CurrentAlpha(a),LastAlpha(b){}};
class CAnimSet{friend class CAnimateThing;public:
#ifdef FABLE_RETAIL_LOOK_HEAD_ANIM
 // Layout: original complete data retained; native percentage pair1c/20 after retail vectors.
 CAnimFrameInfo GetFrame()const{return CAnimFrameInfo(CurrentPercentageComplete,LastPercentageComplete);}
 void SetFrame(const CAnimFrameInfo& frame){CurrentPercentageComplete=frame.Current;LastPercentageComplete=frame.Last;}
#endif
 CAnimSet(){}
 CAnimSet(const CAnimSet&);
 bool IsOwnedByMainAnim()const{return OwnedByMainAnim;}
#ifdef FABLE_RETAIL_CANIMSET_SETASFINISHED_DECL
 // NF66 opt-in (row 0x00694430 CCreatureActionBase::FinishAction): that TU calls Ego_r ?SetAsFinished@CAnimSet@@QAEXJ@Z out of line
 // (0x00668A50). Declaration only; without the macro the inline body below is unchanged.
 void SetAsFinished(long frames);                                     // 0x00668A50
#ifdef FABLE_RETAIL_CANIMSET_SETUPDATEMODE
 // (same row) FableWin.pdb inline ?SetUpdateMode@CAnimSet@@QAEXW4EAnimUpdateMode@@@Z and ?SetAsFinished@CAnimSet@@QAEXXZ (retail stores
 // UpdateMode +0x00 and sets the Finished bit in place, 0x0069451x). Bodies as their names; no layout change.
 void SetUpdateMode(EAnimUpdateMode mode){UpdateMode=mode;}
 void SetAsFinished(){Finished=true;}
#endif
#else
 void SetAsFinished(long frames){if(!Finished){Finished=true;GFLimitLower(frames,1);ChangeAlphaBy=CurrentAlpha/(float)frames;}}
#endif
 bool IsValid()const{unsigned int count=Anims.size();return (count==1||(count==2&&Anims[1]>0))&&Anims[0]>0&&count==InternalAlphas.size();}
 float GetChangeAlphaBy()const{return ChangeAlphaBy;}
 void SetChangeAlphaBy(float value){ChangeAlphaBy=value;}
 float GetAlpha()const{return CurrentAlpha;}
#ifdef FABLE_RETAIL_CANIMSET_ANIMINDICES
 // Retail opt-in (AUTH22 2026-11-22, row 0x0066B370 CTCSoundPlayer::PlayAllSoundEvents): FableWin.pdb
 // ?GetAnimIndices@CAnimSet@@QBEABV?$vector@FV?$allocator@F@std@@@std@@XZ, ?GetAnimBlendStrength@CAnimSet@@QBEMJ@Z,
 // ?GetPercentageComplete@CAnimSet@@QBEMXZ and ?GetLastPercentageComplete@CAnimSet@@QBEMXZ, inline in retail (0x0066B5BA reads
 // Anims +0x04 / +0x08 and InternalAlphas +0x10 in place, 0x0066B6CB `fld [set+1Ch] ; fcomp [set+20h]`). Ego_d.pdb's locals of
 // that row (`const vector<short>& anim_indeces`, `float strength`) and its 11-B / 15-B /Od lines 189 / 195 are one accessor call
 // each. GetAnimBlendStrength's debug body carries asserts (FableWin has EH funclets for it); the release body is the element read.
 // No layout change.
 const std::vector<short,std::allocator<short> >& GetAnimIndices()const{return Anims;}
 float GetAnimBlendStrength(long index)const{return InternalAlphas[index].CurrentAlpha;}
 // The two percentage accessors. All three later VC10 PDBs (Ego_d / Ego_r / FableWin) give them as public, by value
 // (`?GetPercentageComplete@CAnimSet@@QBEMXZ`, `?GetLastPercentageComplete@CAnimSet@@QBEMXZ`) over private members, and hold no friend
 // record of any class. Retail's CTCSoundPlayer::PlayAllSoundEvents passes both to GetAnimEventsBetweenTimes with no temporary
 // (0x0066B6E0 `mov ecx,[set+1Ch] ; mov edx,[set+20h] ; push ecx ; .. ; push edx`, frame 0x74); under VC7.1 an inlined by-value float
 // accessor used as an argument always keeps a stored temporary (frame 0x7C, DIFFER(1844v1808)), and a `const float&` return graded
 // RELOCATION_MATCH on that row with Ego_d's line-208 calls unchanged. Inferred TLC-era form (owner 2026-11-22, as
 // FABLE_RETAIL_CCOMBATMANAGER_PROXIMITY_REF_INFERRED in engine/CCombatManager.h); off by default. No layout change.
#ifdef FABLE_RETAIL_CANIMSET_PERCENTAGE_REF_INFERRED
 const float& GetLastPercentageComplete()const{return LastPercentageComplete;}
#if !(defined(FABLE_RETAIL_CANIMSET_TYPE) && defined(FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24))
 const float& GetPercentageComplete()const{return CurrentPercentageComplete;}
#endif
#else
 float GetLastPercentageComplete()const{return LastPercentageComplete;}
#if !(defined(FABLE_RETAIL_CANIMSET_TYPE) && defined(FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24))
 float GetPercentageComplete()const{return CurrentPercentageComplete;}
#endif
#endif
#endif
#ifdef FABLE_RETAIL_CANIMSET_FRAME
 // Retail opt-in (chunk 1006, row 0x005B3E4E): FableWin.pdb ?GetAnimIndex@CAnimSet@@QBEJXZ (the first anim index, `mov eax,[set+4] ;
 // movsx eax,word ptr [eax]`, 0x005B3FD4) and ?GetFrame@CAnimSet@@QBE?AVCAnimFrameInfo@@XZ (the percentage-complete pair +0x1C/+0x20
 // returned by value, 0x005B4004). Inline in retail. Needs FABLE_RETAIL_CANIMINIT_CTORS (the CAnimFrameInfo(float,float) ctor).
 long GetAnimIndex()const{return Anims[0];}
 CAnimFrameInfo GetFrame()const{return CAnimFrameInfo(CurrentPercentageComplete,LastPercentageComplete);}
#endif
#ifdef FABLE_RETAIL_CANIMSET_TYPE
 // Retail opt-in (w22 agent2007, row 0x00665860 CThingCreatureBase::UpdateAnimation): Ego_r.pdb / FableWin.pdb ?GetType@CAnimSet@@QBEJXZ
 // (inline: the byte +0x50) and ?IsFinished@CAnimSet@@QBE_NXZ (called out of line in that TU: the COMDAT 0x00668A80, `Anims` empty or
 // the Finished bit +0x51 & 2). Declaration of IsFinished only.
 long GetType()const{return Type;}
 unsigned long GetFlags()const{return Flags;}                         // ?GetFlags@CAnimSet@@QBEKXZ (inline: +0x40)
#ifdef FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24
 bool IsFinished() const { return Anims.empty() || Finished; }
 long GetAnimIndex() const { return Anims[0]; }
 float GetPercentageComplete() const { return CurrentPercentageComplete; }
 void SetAsUpdated(bool updated) { HadUpdate = updated; }
 void SetAlpha(float alpha);
 void SetInternalAlpha(long index, float alpha, bool immediate);
#else
 bool IsFinished()const;                                               // 0x00668A80
#endif
#endif
#ifdef FABLE_RETAIL_CANIMSET_MESHANIM
 // Nested opt-in (B24C6, row 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim; needs FABLE_RETAIL_DEF_CLASS_BASE,
 // FABLE_RETAIL_C3DANIMATIONSTATEALPHA(_INDEX_CTOR)): Ego_r.pdb / FableWin.pdb ?GetMeshAnim@CAnimSet@@QBE?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@M@Z
 // (out of line, retail 0x0070C900, landed), ?GetFirstMeshAnim@CAnimSet@@QBE?AVC3DAnimationStateAlpha@@M@Z (FableWin 0x01E99780, inline),
 // and the private ?GetInterpolatedAlpha@CAnimSet@@ABEMM@Z (FableWin 0x01E992B0, inline), ?GetInterpolatedInternalAlpha@CAnimSet@@ABEMMJ@Z
 // (FableWin 0x01E99330, inline), ?GetInterpolatedTime@CAnimSet@@ABEMMJ@Z (out of line, retail 0x0070C800, landed).
 CIVCountedPointer<C3DAnimationStateBase> GetMeshAnim(float alpha)const;   // 0x0070C900
 C3DAnimationStateAlpha GetFirstMeshAnim(float alpha)const{float blend=GetInterpolatedInternalAlpha(alpha,0);return C3DAnimationStateAlpha(Anims[0],GetInterpolatedTime(alpha,0),blend);}
private:
 // Both lerp through Ego_r / FableWin ?GFGetInterpolatedValue@@YIMMMM@Z (by-value floats: retail loads LastAlpha once and keeps it on the
 // x87 stack, `fld last; fld current; fsub st,st(1); fmul alpha; fadd st,st(1); fstp st(0)`, 0x0070DA92 / 0x0070E01B; the member
 // spelling reloads it). FableWin 0x01E992B0 GetInterpolatedAlpha 19i/45b = this call (VC7.1 /Od 16i/36b + FableWin's 3-instruction
 // float pushes, B24C7).
 float GetInterpolatedAlpha(float alpha)const{return GFGetInterpolatedValue(LastAlpha,CurrentAlpha,alpha);}
 float GetInterpolatedInternalAlpha(float alpha,long index)const{return GFGetInterpolatedValue(InternalAlphas[index].LastAlpha,InternalAlphas[index].CurrentAlpha,alpha);}
 float GetInterpolatedTime(float alpha,long index)const;               // 0x0070C800
public:
#endif
#ifdef FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE
 // Opt-in (p25A, row 0x0070E8B0 CAnimateThing::Update): FableWin.pdb ?HasHadUpdate@CAnimSet@@QBE_NXZ (inline in retail: `test byte
 // [set+51h],8`, 0x0070E9F6). Needs FABLE_RETAIL_CANIMSET_TYPE + FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24 (SetAsUpdated / SetAlpha).
 bool HasHadUpdate() const { return HadUpdate; }
#endif
private:
#ifdef FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE
 // (same row) the private members CAnimateThing::Update calls out of line: Ego_r.pdb / FableWin.pdb ?Update@CAnimSet@@AAEXABVCAnimateThingUpdate@@@Z
 // (0x0070D030, landed) and ?UpdateInternalAlphas@CAnimSet@@AAEXXZ (0x0070C2C0, landed). Declarations only.
 void Update(const CAnimateThingUpdate& update);                       // 0x0070D030
 void UpdateInternalAlphas();                                          // 0x0070C2C0
#endif
#ifdef FABLE_RETAIL_CANIMSET_UPDATEDISTANCE
 // Retail opt-in (row 0x0070CE10): Ego_r.pdb ?UpdateDistanceMoved@CAnimSet@@AAEXM@Z, ?PeekAnimInfo@CAnimSet@@ABEABVC3DAnimationInfo@@F@Z
 // and ?UpdatePercentageCompleteTo@CAnimSet@@AAEXM@Z (FableWin.pdb 0x01E9BFF0 / 0x01E9B410 / 0x01E9BA00). The two helpers are
 // expanded inline in retail; their bodies are defined (inline) in the TU that needs them.
 inline const C3DAnimationInfo& PeekAnimInfo(short anim)const;
 inline void UpdatePercentageCompleteTo(float percentage);
 void UpdateDistanceMoved(float distance);
#endif
 void Init(CAnimateThing&,const CAnimInit&,float);
EAnimUpdateMode UpdateMode;
std::vector<short,std::allocator<short> > Anims;
std::vector<CTransitionBlendAlpha,std::allocator<CTransitionBlendAlpha> > InternalAlphas;
float CurrentPercentageComplete;
float LastPercentageComplete;
float Speed;
unsigned long FrameInternalAlphasUpdated;
float CurrentAlpha;
float LastAlpha;
float LastUpdateChange;
float ChangeAlphaBy;
CAnimateThing* PAnimator;
unsigned long Flags;
unsigned long TypeFlags;
long AnimDelay;
long Priority;
unsigned char Type;
bool Looping:1;
bool Finished:1;
bool Forwards:1;
bool HadUpdate:1;
bool OwnedByMainAnim:1;
};
FABLE_STATIC_ASSERT_AT(74, sizeof(CAnimSet)==0x54);
#ifdef FABLE_RETAIL_CANIMSET_ANIMATETHING_UPDATE
// (same row) Ego_r.pdb / FableWin.pdb ?SetAlpha@CAnimSet@@QAEXM@Z: out of line at 0x0070BFC0 (landed, this body) and expanded in
// CAnimateThing::Update twice (main alpha 0x0070E92C..0x0070E97B, each transition set 0x0070EB10..0x0070EB46: the in-register clamp,
// then LastAlpha = CurrentAlpha). The class declares it (FABLE_RETAIL_CANIMSET_STRAFE_ACCESSORS_PASS24); this is its inline definition.
inline void CAnimSet::SetAlpha(float alpha)
{
    if (alpha < 0.0f)
        alpha = 0.0f;
    else if (alpha > 1.0f)
        alpha = 1.0f;
    LastAlpha = CurrentAlpha;
    CurrentAlpha = alpha;
}
#endif
#ifdef FABLE_RETAIL_CANIMSET_GETMESHANIM_INLINE_BODY
// Retail opt-in (row 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim): the body of CAnimSet::GetMeshAnim 0x0070C900
// (Ego_r.pdb / FableWin.pdb ?GetMeshAnim@CAnimSet@@QBE?AV?$CIVCountedPointer@VC3DAnimationStateBase@@@@M@Z). Both PDBs place it in
// animate_thing.obj with CalcInterpolatedMeshAnim (egor_obj_functions.tsv / fablewin_obj_functions.tsv): retail compiled the body in
// the caller's TU and kept it out of line. Without a visible body the caller's leaf grows (2610 v 2395). `inline` here gives the
// caller the body as a pick-any COMDAT instead of a second strong definition of the landed 0x0070C900; the copy equals retail.
#include "engine/C3DAnimationState.h"
#include "engine/C3DAnimationTransitionState.h"
inline CIVCountedPointer<C3DAnimationStateBase> CAnimSet::GetMeshAnim(float fraction) const
{
    CIVCountedPointer<C3DAnimationState> start(new C3DAnimationState(Anims[0],GetInterpolatedTime(fraction,0)));
    if(Anims.size()==1) return start;
    CIVCountedPointer<C3DAnimationState> end(new C3DAnimationState(Anims[1],GetInterpolatedTime(fraction,1)));
    CIVCountedPointer<C3DAnimationTransitionState> transition(new C3DAnimationTransitionState(start,end,GetInterpolatedInternalAlpha(fraction,1)));
    return transition;
}
#endif
#endif
