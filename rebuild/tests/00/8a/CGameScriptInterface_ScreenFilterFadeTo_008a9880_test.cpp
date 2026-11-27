#include <stdio.h>
#include <stdlib.h>
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
#define FABLE_RETAIL_STLP_VECTOR_C
#include "stlport/fable_containers.h"
#include "engine/CRGBFloatColour.h"
static void check(bool ok){if(!ok)exit(2);}
class CBaseObject {};
class CThing : public CBaseObject {};
class CBaseObjectPointer {public:CBaseObject* item;unsigned users;};
#include "engine/CIntelligentPointer.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
static CThing objects[5];static CBaseObjectPointer records[5];
static int active;
CBaseIntelligentPointer::CBaseIntelligentPointer():PData(0){++active;}
CBaseIntelligentPointer::~CBaseIntelligentPointer(){SetPItem(0);--active;}
CBaseObject* CBaseIntelligentPointer::GetPItem()const{return PData?PData->item:0;}
void CBaseIntelligentPointer::SetPItem(CBaseObject* p){
 if(PData){check(PData->users>0);--PData->users;PData=0;}
 if(p){for(unsigned i=0;i<5;++i)if(records[i].item==p){PData=&records[i];++PData->users;return;}check(false);}
}
static unsigned queries;
class CScriptThing {public:CThing* p;CScriptThing(CThing* v):p(v){} virtual CThing* GetPThing()const{++queries;return p;}};
#include "engine/CScreenFilterSThingByPass.h"
typedef std::vector<CScreenFilterSThingByPass> Inputs;
typedef NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers Outputs;
static const Inputs* expected;
static const CRGBFloatColour* colour;
static float values[5];static void* handle;static unsigned calls,env_calls,interp_calls;
class CEnvironmentSpellEffectInterpolator {
public:void* ScreenFilterFadeTo(float duration,NScreenEffect::ScreenEffectPriority priority,float alpha,float sat,float contrast,float brightness,const CRGBFloatColour& tint,const Outputs& ss){
 ++calls;check(priority==NScreenEffect::SCREEN_EFFECT_PRIORITY_SCRIPT && &tint==colour);
 check(duration==values[0] && alpha==values[1] && sat==values[2] && contrast==values[3] && brightness==values[4]);
 check(queries==expected->size() && ss.size()==expected->size());
 for(unsigned i=0;i<ss.size();++i){check(ss[i].thing.GetPItem()==(*expected)[i].thing.p && ss[i].bypassSet==(*expected)[i].bypassSet);}
 return handle;
}};
class CEnvironment {public:CEnvironmentSpellEffectInterpolator interpolator;
 CEnvironmentSpellEffectInterpolator& GetSpellEffectInterpolator(){++interp_calls;return interpolator;}};
class CWorld {public:CEnvironment environment;CEnvironment& GetEnvironment(){++env_calls;return environment;}};
class CGameScriptInterface {public:CWorld* World;void* ScreenFilterFadeTo(float,float,float,float,float,const CRGBFloatColour&,const Inputs&)const;};
void* CGameScriptInterface::ScreenFilterFadeTo(float duration, float alpha, float sat, float contrast, float brightness,
    const CRGBFloatColour& tint, const std::vector<CScreenFilterSThingByPass, std::allocator<CScreenFilterSThingByPass> >& s) const
{
    NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers ss;
    if (false) {} else for (long i = 0; i < static_cast<long>(s.size()); ++i) {
        NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser arg(s[i].thing.GetPThing(), s[i].bypassSet);
        ss.push_back(arg);
    }
    return World->GetEnvironment().GetSpellEffectInterpolator().ScreenFilterFadeTo(duration, NScreenEffect::SCREEN_EFFECT_PRIORITY_SCRIPT,
        alpha, sat, contrast, brightness, tint, ss);
}
int main(){
 CWorld world;CGameScriptInterface script;script.World=&world;
 for(unsigned i=0;i<5;++i)records[i].item=&objects[i];
 CRGBFloatColour tint(0.25f,0.5f,0.75f,1.0f);colour=&tint;
 for(unsigned n=0;n<=17;++n)for(unsigned flags=0;flags<8;++flags)
 for(unsigned pat=0;pat<8;++pat)for(unsigned run=0;run<4;++run){
  Inputs items;expected=&items;handle=run?&objects[run]:0;
  for(unsigned j=0;j<5;++j)values[j]=(float)((int)(j*3+run)-(int)flags)*0.25f;
  for(unsigned k=0;k<n;++k){
   CScriptThing thing(((k+pat)%4==0)?0:&objects[(k+pat)%5]);
   items.push_back(CScreenFilterSThingByPass(thing,(flags&(1U<<(k%3)))!=0));
  }
  calls=queries=env_calls=interp_calls=0;
  check(script.ScreenFilterFadeTo(values[0],values[1],values[2],values[3],values[4],tint,items)==handle);
  check(calls==1 && queries==n && env_calls==1 && interp_calls==1 && items.size()==n && active==0);
  for(unsigned j=0;j<5;++j)check(!records[j].users);
 }
 puts("008a9880_TEST PASS");return 0;
}