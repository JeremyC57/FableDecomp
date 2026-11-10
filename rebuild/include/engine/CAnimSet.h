#ifndef FABLE_ENGINE_CANIMSET_H
#define FABLE_ENGINE_CANIMSET_H
#include "rebuild_abi.h"
#include "stlport/fable_containers.h"
#include "engine/CAnimInit.h"
template<class T,class L>inline void FABLE_FASTCALL GFLimitLower(T& value,const L& lower){if(value<lower)value=lower;}
class CAnimateThing;
class CTransitionBlendAlpha{public:float CurrentAlpha,LastAlpha;CTransitionBlendAlpha(float a,float b):CurrentAlpha(a),LastAlpha(b){}};
class CAnimSet{friend class CAnimateThing;public:
 CAnimSet(){}
 CAnimSet(const CAnimSet&);
 bool IsOwnedByMainAnim()const{return OwnedByMainAnim;}
 void SetAsFinished(long frames){if(!Finished){Finished=true;GFLimitLower(frames,1);ChangeAlphaBy=CurrentAlpha/(float)frames;}}
 bool IsValid()const{unsigned int count=Anims.size();return (count==1||(count==2&&Anims[1]>0))&&Anims[0]>0&&count==InternalAlphas.size();}
 float GetChangeAlphaBy()const{return ChangeAlphaBy;}
 void SetChangeAlphaBy(float value){ChangeAlphaBy=value;}
 float GetAlpha()const{return CurrentAlpha;}
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
