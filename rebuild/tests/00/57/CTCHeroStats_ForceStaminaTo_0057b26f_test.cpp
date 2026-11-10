#include <stdio.h>
#include <math.h>
enum ETCInterfaceType{TCI_PHYSICAL_SHIELD_SPELL=0x93};
class CTCPhysicalShieldSpell{public:int Count;CTCPhysicalShieldSpell():Count(0){}void CancelMove(){++Count;}};
class CThing{public:CTCPhysicalShieldSpell* Shield;template<class T>bool GetTC(ETCInterfaceType id,T** out){if(id==TCI_PHYSICAL_SHIELD_SPELL&&Shield){*out=Shield;return true;}return false;}};
class CHeroStatsDef{public:float NumberOfSecondsAfterUseStaminaStartsRefilling;};
class CGameDefinitionManager{public:CHeroStatsDef Definition;const CHeroStatsDef& PeekHeroStatsDef()const{return Definition;}};
class CMainGameComponent{public:static long FPS;static long GetConstantFPS(){return FPS;}};long CMainGameComponent::FPS;
class CTCHeroStats{public:CThing* ParentThing;CGameDefinitionManager* Definitions;long Stamina,MaxStamina,FramesUntilStaminaRefills;const CGameDefinitionManager& GetDefinitionManager()const{return*Definitions;}void ForceStaminaTo(float);};
template<class T,class L,class H>void GFLimit(T& value,const L& lo,const H& hi){if(value<lo)value=lo;else if(value>hi)value=hi;}
static long GFFloatToLongNear(float value){double lower=floor((double)value);double remainder=value-lower;long n=(long)lower;if(remainder>0.5||(remainder==0.5&&(n%2)!=0))++n;return n;}
void CTCHeroStats::ForceStaminaTo(float percentage){
 GFLimit(percentage,0.0f,1.0f);
 Stamina=GFFloatToLongNear(MaxStamina*percentage);
 FramesUntilStaminaRefills=(long)(GetDefinitionManager().PeekHeroStatsDef().NumberOfSecondsAfterUseStaminaStartsRefilling*CMainGameComponent::GetConstantFPS());
 if(Stamina==0){
  CTCPhysicalShieldSpell* listener;
  if(ParentThing->GetTC(TCI_PHYSICAL_SHIELD_SPELL,&listener))listener->CancelMove();
 }
}

int main(){
 const float values[]={-1,-0.5f,0,0.125f,0.25f,0.5f,0.75f,1,2};const long maxima[]={0,1,2,4,10};const long fps[]={0,2,30,60};const float delays[]={0,0.125f,0.5f,1.5f};
 for(int v=0;v<9;++v)for(int m=0;m<5;++m)for(int f=0;f<4;++f)for(int delay=0;delay<4;++delay)for(int present=0;present<2;++present){
  CTCPhysicalShieldSpell shield;CThing thing;thing.Shield=present?&shield:0;CGameDefinitionManager definitions;definitions.Definition.NumberOfSecondsAfterUseStaminaStartsRefilling=delays[delay];CMainGameComponent::FPS=fps[f];
  CTCHeroStats row;row.ParentThing=&thing;row.Definitions=&definitions;row.Stamina=-123;row.MaxStamina=maxima[m];row.FramesUntilStaminaRefills=-123;row.ForceStaminaTo(values[v]);
  const long eighths[]={-8,-4,0,1,2,4,6,8,16};long part=eighths[v]<0?0:eighths[v]>8?8:eighths[v];long numerator=part*maxima[m];long rounded=numerator/8;long remainder=numerator%8;if(remainder>4||(remainder==4&&(rounded%2)!=0))++rounded;
  if(row.Stamina!=rounded||row.FramesUntilStaminaRefills!=(long)(delays[delay]*fps[f])||shield.Count!=(present&&rounded==0?1:0))return 1;
 }
 puts("0057b26f_TEST PASS");return 0;
}