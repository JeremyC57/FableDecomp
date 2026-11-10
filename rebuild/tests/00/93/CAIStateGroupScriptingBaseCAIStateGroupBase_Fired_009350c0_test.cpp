#include <stdio.h>
#include <assert.h>
#include <math.h>
#ifndef _MSC_VER
#define __fastcall
#endif
class CTCAIScratchpad{public:long ActionFrequency;float ActionFrequencyVariation;long GetActionFrequency()const{return ActionFrequency;}float GetActionFrequencyVariation()const{return (ActionFrequencyVariation);}};
class CThingAICreature{public:CTCAIScratchpad* scratch;void GetTC(CTCAIScratchpad** out){*out=scratch;}};
class CMainGameComponent{public:static long fps;static long __fastcall GetConstantFPS(){return fps;}};long CMainGameComponent::fps=30;
class CWorld{public:static long frame;static long __fastcall GetFrame(){return frame;}};long CWorld::frame=100;
static float fraction;static unsigned long seed;static int seed_calls,random_calls;
float __fastcall GFFloatRandom(float range,unsigned long* p){assert(p==&seed);++random_calls;++*p;return range*fraction;}
class CAIStateGroupBase{protected:CThingAICreature creature;CThingAICreature&GetCreature()const{return const_cast<CThingAICreature&>(creature);}unsigned long*GetPWorldSeed()const{++seed_calls;return &seed;}public:void SetScratch(CTCAIScratchpad*p){creature.scratch=p;}};
template<class TBase>class CAIStateGroup_ScriptingBase:public TBase{protected:void Fired();long NextFrameFireable;public:void Run(){Fired();}long Next()const{return NextFrameFireable;}};
template<> void CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired(){
 CTCAIScratchpad* ai_scratchpad;
 GetCreature().GetTC(&ai_scratchpad);
 int frame_offset=ai_scratchpad->GetActionFrequency()*CMainGameComponent::GetConstantFPS();
 frame_offset*=(1.0f+ai_scratchpad->GetActionFrequencyVariation())-GFFloatRandom(ai_scratchpad->GetActionFrequencyVariation()*2.0f,GetPWorldSeed());
 NextFrameFireable=CWorld::GetFrame()+frame_offset;
}
int main(){for(int frequency=0;frequency<=8;++frequency)for(int fps=15;fps<=60;fps+=15)for(int v=0;v<=8;++v)for(int f=0;f<=8;++f){CTCAIScratchpad scratch;scratch.ActionFrequency=frequency;scratch.ActionFrequencyVariation=v*.125f;fraction=f*.125f;CMainGameComponent::fps=fps;CWorld::frame=123;seed=41;seed_calls=random_calls=0;CAIStateGroup_ScriptingBase<CAIStateGroupBase> group;group.SetScratch(&scratch);group.Run();float multiplier=(1.0f+scratch.ActionFrequencyVariation)-2.0f*scratch.ActionFrequencyVariation*fraction;int delay=(int)(multiplier*(frequency*fps));assert(group.Next()==123+delay);assert(seed==42&&seed_calls==1&&random_calls==1);assert(group.Next()>=123-1&&group.Next()<=123+2*frequency*fps+1);}puts("009350c0_TEST PASS");return 0;}