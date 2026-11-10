#include <stdio.h>
#include <string.h>
static int live,failures,world_calls,seed_calls,random_calls,lookup_calls,create_calls;
static bool found,known;static float random_value;static long output_effect;static const char* wanted_name;
class C3DVector{public:float X,Y,Z;};class CThing{};static const C3DVector* want_pos;
class CCharString{public:const char* text;CCharString(const char* p,long n=-1):text(p){++live;if(n!=-1)++failures;}~CCharString(){--live;}};
class CDefPointeeBase{public:mutable long IVRefCount;mutable int Deletes;void Delete()const{++Deletes;if(live)++failures;}};
template<class T>class CDefPointer{public:CDefPointeeBase* Object;CDefPointer():Object(0){}~CDefPointer(){if(Object){CDefPointeeBase* object=Object;if(--object->IVRefCount==0)object->Delete();Object=0;}}T* operator->()const{return static_cast<T*>(Object);}};
class CSpecialEffectsDef:public CDefPointeeBase{public:bool GetEffectFromString(const CCharString& name,long& effect)const{++lookup_calls;if(strcmp(name.text,wanted_name)||live!=1)++failures;if(known)effect=output_effect;return known;}};
static CSpecialEffectsDef effects;
class CSpecialAbilitiesBerserkDef:public CDefPointeeBase{public:bool GetSubDef(CDefPointer<const CSpecialEffectsDef>* out)const{if(found){out->Object=&effects;++effects.IVRefCount;}return found;}};
class CWorld{public:mutable unsigned long Seed;unsigned long* GetWorldSeed()const{++seed_calls;return &Seed;}};static CWorld world;
static float GFFloatRandom(float range,unsigned long* seed){++random_calls;if(range!=1||seed!=&world.Seed)++failures;++*seed;return random_value;}
class CTCSpecialAbilities{public:static CThing* CreateParticleEffect(long effect,const C3DVector& pos){++create_calls;if(effect!=output_effect||&pos!=want_pos||live)++failures;return 0;}};
class CTCBerserk{public:CDefPointer<const CSpecialAbilitiesBerserkDef> PDef;void PlayHitEffectAtPos(const C3DVector& pos)const;protected:const CWorld& PeekWorld()const{++world_calls;return world;}};
void CTCBerserk::PlayHitEffectAtPos(const C3DVector& pos)const
{
 CDefPointer<const CSpecialEffectsDef> pspecial_effects;
 long effect=0;
 if(PDef->GetSubDef(&pspecial_effects)) {
  if(GFFloatRandom(1.0f,PeekWorld().GetWorldSeed())>0.5f) {
   if(pspecial_effects->GetEffectFromString("BERSERK_HIT_EFFECT_01",effect)) {
    CThing* pfx=CTCSpecialAbilities::CreateParticleEffect(effect,pos);
   }
  } else {
   if(pspecial_effects->GetEffectFromString("BERSERK_HIT_EFFECT_02",effect)) {
    CThing* pfx=CTCSpecialAbilities::CreateParticleEffect(effect,pos);
   }
  }
 }
}
int main(){
 float values[]={0,0.25f,0.5f,0.75f,1,0,0};union Bits{unsigned int bits;float f;};Bits b;b.bits=0x7fc00000U;values[5]=b.f;b.bits=0x7f800000U;values[6]=b.f;
 long ids[]={0,1,12345};
 for(int f=0;f<2;++f)for(int k=0;k<2;++k)for(int v=0;v<7;++v)for(int refs=0;refs<2;++refs)for(int id=0;id<3;++id)for(int p=0;p<3;++p){
  found=f!=0;known=k!=0;random_value=values[v];output_effect=ids[id];wanted_name=values[v]>0.5f?"BERSERK_HIT_EFFECT_01":"BERSERK_HIT_EFFECT_02";
  CSpecialAbilitiesBerserkDef def;def.IVRefCount=2;def.Deletes=0;CTCBerserk row;row.PDef.Object=&def;effects.IVRefCount=refs;effects.Deletes=0;
  C3DVector pos;pos.X=p*2.0f;pos.Y=p*-4.0f;pos.Z=0.5f;want_pos=&pos;world.Seed=100;world_calls=seed_calls=random_calls=lookup_calls=create_calls=0;row.PlayHitEffectAtPos(pos);
  if(failures||live||world_calls!=f||seed_calls!=f||random_calls!=f||lookup_calls!=f||create_calls!=(f&&k?1:0)||world.Seed!=100UL+f||effects.IVRefCount!=refs||effects.Deletes!=(f&&refs==0?1:0)||pos.X!=p*2.0f||pos.Y!=p*-4.0f||pos.Z!=0.5f)return 1;
 }
 puts("0078c150_TEST PASS");return 0;
}