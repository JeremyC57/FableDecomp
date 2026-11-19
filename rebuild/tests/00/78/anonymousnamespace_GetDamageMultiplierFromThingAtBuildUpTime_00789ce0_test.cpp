// Behaviour test for `anonymous namespace'::GetDamageMultiplierFromThingAtBuildUpTime (0x00789CE0): the row body over models of its
// callees (PeekTC, CHeroAbilityStatWeight::GetParameter, the by-value CDefPointer's release). Kills: the stat weighting used on a
// PeekTC miss / skipped on a hit, the wrong experience passed to GetParameter, a swapped speed / scale field (nominal v super, the
// stat weighting field), tanh replaced or dropped, the square on the wrong term or missing, the +1 dropped, a sum turned product,
// and the by-value def pointer not released (count left high / Delete not called on the last reference / called early).
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
static void check(bool v){if(!v){puts("FAIL");exit(1);}}
static bool near(float a,float b){float d=a-b;if(d<0)d=-d;float m=b<0?-b:b;return d<=1e-5f*(m>1?m:1);}
static bool same(float a,float b){return memcmp(&a,&b,4)==0;}
inline double GFTanh(double n){return tanh(n);}
inline float GFSqr(float n){return (n*n);}
class CTCHeroExperience {public:float Strength;};
static int param_calls;static const CTCHeroExperience* param_arg;
class CHeroAbilityStatWeight {public:float Factor;
 float GetParameter(const CTCHeroExperience& e)const{++param_calls;param_arg=&e;return e.Strength*Factor;}};
static int deletes;
class CDefPointeeBase {public:mutable long IVRefCount;long Release()const{return --IVRefCount;}void Delete()const{++deletes;}};
class CHeroCombatDef : public CDefPointeeBase {public:
 CHeroAbilityStatWeight RangedDamageMultiplierWeighting;
 float RangedDamageMultiplierSpeedNominalBuildup,RangedDamageMultiplierScaleNominalBuildup;
 float RangedDamageMultiplierSpeedSuperBuildup,RangedDamageMultiplierScaleSuperBuildup;
 float RangedDamageMultiplierSpeedWithStatParameter;};
template<class T> class CDefPointer {public:T* Object;
 explicit CDefPointer(T* p):Object(p){if(Object)++Object->IVRefCount;}
 CDefPointer(const CDefPointer& o):Object(o.Object){if(Object)++Object->IVRefCount;}
 ~CDefPointer(){if(Object){T* object=Object;if(Object->Release()==0)object->Delete();Object=0;}}
 T* operator->()const{return Object;}};
static int peek_calls;
class CThing {public:CTCHeroExperience* Exp;
 bool PeekTC(const CTCHeroExperience** out)const{++peek_calls;*out=Exp;return Exp!=0;}};
namespace {
float GetDamageMultiplierFromThingAtBuildUpTime(const CThing& thing, CDefPointer<const CHeroCombatDef> pdef, float buildup_time)
{
    float fractional_strength = 1.0f;
    const CTCHeroExperience* ptc_hero_exp;
    if (thing.PeekTC(&ptc_hero_exp))
        fractional_strength = pdef->RangedDamageMultiplierWeighting.GetParameter(*ptc_hero_exp);
    float comp1 = GFTanh(buildup_time * pdef->RangedDamageMultiplierSpeedNominalBuildup) * pdef->RangedDamageMultiplierScaleNominalBuildup;
    float comp2 = GFSqr(buildup_time * pdef->RangedDamageMultiplierSpeedSuperBuildup) * pdef->RangedDamageMultiplierScaleSuperBuildup;
    float damage_m = (pdef->RangedDamageMultiplierSpeedWithStatParameter * fractional_strength + 1.0f) * (comp1 + comp2);
    return damage_m;
}
}
int main(){
 const float t[]={0.0f,0.5f,1.0f,2.25f,-1.5f};long cases=0;
 CHeroCombatDef def;def.IVRefCount=0;def.RangedDamageMultiplierWeighting.Factor=0.75f;
 def.RangedDamageMultiplierSpeedNominalBuildup=1.25f;def.RangedDamageMultiplierScaleNominalBuildup=3.0f;
 def.RangedDamageMultiplierSpeedSuperBuildup=0.5f;def.RangedDamageMultiplierScaleSuperBuildup=5.0f;
 def.RangedDamageMultiplierSpeedWithStatParameter=2.0f;
 CTCHeroExperience exp;CThing thing;
 // Exact dyadic case with the tanh term off: (2*1+1) * (0 + (2*0.5)^2*5) = 15; with stat 4*0.75=3: (2*3+1)*5 = 35.
 {CHeroCombatDef d=def;d.RangedDamageMultiplierScaleNominalBuildup=0.0f;
  thing.Exp=0;{CDefPointer<const CHeroCombatDef> p(&d);check(same(GetDamageMultiplierFromThingAtBuildUpTime(thing,p,2.0f),15.0f));}
  exp.Strength=4.0f;thing.Exp=&exp;{CDefPointer<const CHeroCombatDef> p(&d);check(same(GetDamageMultiplierFromThingAtBuildUpTime(thing,p,2.0f),35.0f));}}
 for(int h=0;h!=2;++h)for(int s=0;s!=3;++s)for(int i=0;i!=5;++i){
  exp.Strength=(float)s*1.5f;thing.Exp=h?&exp:0;
  peek_calls=param_calls=0;param_arg=0;deletes=0;
  float got;
  {CDefPointer<const CHeroCombatDef> p(&def);
   got=GetDamageMultiplierFromThingAtBuildUpTime(thing,p,t[i]);
   check(def.IVRefCount==1&&deletes==0);}            // the callee's copy was released, the caller's still holds
  check(def.IVRefCount==0&&deletes==1);
  check(peek_calls==1&&param_calls==(h?1:0)&&(!h||param_arg==&exp));
  float frac=h?exp.Strength*0.75f:1.0f;
  double c1=tanh((double)(t[i]*1.25f))*3.0;double sq=(double)t[i]*0.5;double c2=sq*sq*5.0;
  float want=(float)((2.0*frac+1.0)*(c1+c2));
  check(near(got,want));++cases;
 }
 // Last reference handed in (a temporary built in the argument slot): the callee's release deletes the def.
 {deletes=0;CHeroCombatDef d=def;d.IVRefCount=0;thing.Exp=0;
  float r=GetDamageMultiplierFromThingAtBuildUpTime(thing,CDefPointer<const CHeroCombatDef>(&d),0.0f);
  check(d.IVRefCount==0&&deletes==1&&same(r,0.0f));}
 check(cases==30);puts("00789ce0_TEST PASS");return 0;
}