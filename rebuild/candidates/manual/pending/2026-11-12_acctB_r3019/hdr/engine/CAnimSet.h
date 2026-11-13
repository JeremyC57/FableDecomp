#ifndef FABLE_ENGINE_CANIMSET_H
#define FABLE_ENGINE_CANIMSET_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CAnimInit.h"
template<class T,class L>inline void FABLE_FASTCALL GFLimitLower(T& value,const L& lower){if(value<lower)value=lower;}
class CAnimateThing;
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
 void SetAsFinished(long frames){if(!Finished){Finished=true;GFLimitLower(frames,1);ChangeAlphaBy=CurrentAlpha/(float)frames;}}
 bool IsValid()const{unsigned int count=Anims.size();return (count==1||(count==2&&Anims[1]>0))&&Anims[0]>0&&count==InternalAlphas.size();}
 float GetChangeAlphaBy()const{return ChangeAlphaBy;}
 void SetChangeAlphaBy(float value){ChangeAlphaBy=value;}
 float GetAlpha()const{return CurrentAlpha;}
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
 bool IsFinished()const;                                               // 0x00668A80
#endif
private:
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
FABLE_STATIC_ASSERT(sizeof(CAnimSet)==0x54);
#endif
