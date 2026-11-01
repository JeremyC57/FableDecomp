// Behavioral fixture for the parked source, with unchanged target body.
// Unused parent interfaces and unrelated component fields are reduced. Actual
// objects are constructed; native offsets are not asserted by this fixture.
// PPersonalityDef_Object is a fixture-only reference to real handle storage,
// so the selected layout projection operates on a live CDefPointer object.
#include <stdio.h>
#include <stdlib.h>
class CDefPointeeBase { public: virtual ~CDefPointeeBase(){} };
class CParentDefClassBase:public CDefPointeeBase {};
template<class T>class CDefPointer{public:CDefPointeeBase*Object;CDefPointer():Object(0){}bool IsNull()const{return Object==0;}T*Get()const{return static_cast<T*>(Object);}T*operator->()const{return Get();}};
enum EOpinion{};
class COpinionPersonalityTraits{public:float BaseOffset,HeroStatScaling,VillageOpinionScaling,TimeScalingRunIn,TimeScalingRunOut,PeakScalingIfPos,PeakScalingIfNeg,PersistScalingIfPos,PersistScalingIfNeg;};
class COpinionPersonalityDef:public CParentDefClassBase{public:COpinionPersonalityTraits*PersonalityTraits;};
class CTCCreatureOpinionOfHero{public:CDefPointer<const COpinionPersonalityDef>PersonalityHandle;CDefPointeeBase*&PPersonalityDef_Object;CTCCreatureOpinionOfHero():PPersonalityDef_Object(PersonalityHandle.Object){}float TransformForcedOpinion(EOpinion,float)const;};
static int cases;
#define CHECK(x)do{if(!(x)){fprintf(stderr,"case %d line %d\n",cases,__LINE__);abort();}}while(0)
float CTCCreatureOpinionOfHero::TransformForcedOpinion(EOpinion opinion,float value)const{
 const CDefPointer<const COpinionPersonalityDef>&def=*reinterpret_cast<const CDefPointer<const COpinionPersonalityDef>*>(&PPersonalityDef_Object);
 if(!def.IsNull()){
  const COpinionPersonalityTraits&traits=def->PersonalityTraits[opinion];
  return traits.BaseOffset+value*traits.HeroStatScaling;
 }
 return value;
}

int main(){
 const float values[]={-8.0f,-2.0f,-0.5f,0.0f,0.5f,2.0f,8.0f};
 COpinionPersonalityDef def;COpinionPersonalityTraits traits[5];def.PersonalityTraits=traits;
 CTCCreatureOpinionOfHero component;
 for(int mode=0;mode<2;++mode)for(int shift=0;shift<7;++shift)for(int scale=0;scale<7;++scale){
  component.PersonalityHandle.Object=mode?&def:0;
  for(int k=0;k<5;++k){traits[k].BaseOffset=values[(shift+k)%7];traits[k].HeroStatScaling=values[(scale+k)%7];traits[k].VillageOpinionScaling=1024;traits[k].TimeScalingRunIn=-1024;traits[k].TimeScalingRunOut=2048;traits[k].PeakScalingIfPos=-2048;traits[k].PeakScalingIfNeg=4096;traits[k].PersistScalingIfPos=-4096;traits[k].PersistScalingIfNeg=8192;}
  for(int opinion=0;opinion<5;++opinion)for(int input=0;input<7;++input){
   float expected=values[input];if(mode)expected=values[(shift+opinion)%7]+values[input]*values[(scale+opinion)%7];
   CHECK(component.TransformForcedOpinion((EOpinion)opinion,values[input])==expected);
   CHECK(component.PersonalityHandle.Object==(mode?&def:0));
   for(int j=0;j<5;++j){CHECK(traits[j].BaseOffset==values[(shift+j)%7]);CHECK(traits[j].HeroStatScaling==values[(scale+j)%7]);CHECK(traits[j].VillageOpinionScaling==1024&&traits[j].PersistScalingIfNeg==8192);}
   ++cases;
  }
 }
 CHECK(cases==3430);puts("00681790_TEST PASS");return 0;
}
