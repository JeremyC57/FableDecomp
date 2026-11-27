#include <vector>
#include <list>
#include <functional>
#include <iterator>
#include <algorithm>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
struct C3DVector{float X,Y,Z;};struct C2DBoxI{int ok;};
struct CTCSmashableBase{};struct CTCExplodingObject{long delay;void SetCountdownTimer(long v){delay=v;}};
struct CThing {C3DVector pos;bool explosive,smash,creature,evade;int damage,kills;unsigned bits;CTCSmashableBase ss;CTCExplodingObject ex;
 const C3DVector& GetPos()const{return pos;}unsigned GetTypeFlags()const{return bits;}
 bool GetTC(CTCExplodingObject** out){*out=&ex;return explosive;}bool GetTC(CTCSmashableBase** out){*out=&ss;return smash;}void Kill(bool yes){if(!yes)exit(2);++kills;}};
struct CThingGameObject:CThing{};struct CThingCreatureBase:CThingGameObject{};
template<class T>struct CIntelligentPointer{T* v;CIntelligentPointer(T* p=0):v(p){}};
struct CGetThingFromIntelligentPointer{CThing* operator()(const CIntelligentPointer<CThing>& p)const{return p.v;}};
static CThingCreatureBase obj[5];static float usedRadius;
struct CThingFilter_IsNot {const CThing& x;CThingFilter_IsNot(const CThing& t):x(t){} bool operator()(const CThing& t)const{return &t!=&x;}};
struct CThingFilter_IsNotList{const std::vector<const CThing*>& v;CThingFilter_IsNotList(const std::vector<const CThing*>& p):v(p){} bool operator()(const CThing& t)const{return std::find(v.begin(),v.end(),&t)==v.end();}};
struct CThingFilter_IsWithinArea{bool rad;CThingFilter_IsWithinArea(const C3DVector&,const C3DVector&,float r,bool inc):rad(inc){if(r!=usedRadius)exit(4);}bool operator()(const CThing& t)const{return t.pos.Y<usedRadius+(rad?1:0);}};
struct CThingFilter_IsEvadeExplosion:std::unary_function<CThing,bool>{CThingFilter_IsEvadeExplosion(const C3DVector&,const C3DVector&){}bool operator()(const CThing& t)const{return t.evade;}};
template<class A,class B,class C>struct Bind3{A a;B b;C c;Bind3(A x,B y,C z):a(x),b(y),c(z){}bool operator()(const CThing& t)const{return a(t)&&b(t)&&c(t);}};
template<class A,class B,class C>Bind3<A,B,C> GFFilter_Bind3(A a,B b,C c){return Bind3<A,B,C>(a,b,c);}
template<class A,class B,class C,class D>struct Bind4{Bind3<A,B,C> base;D d;Bind4(A a,B b,C c,D x):base(a,b,c),d(x){}bool operator()(const CThing&t)const{return base(t)&&d(t);}};
template<class A,class B,class C,class D>Bind4<A,B,C,D> GFFilter_Bind4(A a,B b,C c,D d){return Bind4<A,B,C,D>(a,b,c,d);}
struct Tools{
 template<class T,class F>void GetAllThingsInRoughArea(C2DBoxI area,const F& filter,std::list<T*>& out){if(area.ok!=19)exit(3);for(int i=0;i<5;++i)if(filter(obj[i])&&matches(out,obj[i]))out.push_back(static_cast<T*>(&obj[i]));}
 bool matches(std::list<CThingCreatureBase*>&,const CThing&t){return t.creature;}
 bool matches(std::list<CThing*>&,const CThing&){return true;}
};
namespace NPhysicsHelpers{
 C2DBoxI GetLineThingSearchArea(const C3DVector&,const C3DVector&,float r){usedRadius=r;C2DBoxI a={19};return a;}
 float GetPerpendicularDistanceFromFiniteLine(const C3DVector&,const C3DVector&,const C3DVector& t){return t.Y;}
}
struct CMainGameComponent{static int GetConstantFPS(){return 24;}};
struct CTCDExplosion{std::vector<CIntelligentPointer<CThing> > HitThings,ImmuneThings,SpecifiedThingsToHit;float DamageRadius;C3DVector StartPos,EndPos;Tools tools;
 Tools& GetThingSearchTools(){return tools;}CThing& GetParentThing(){return obj[0];}void GiveDamageToThing(CThingGameObject& t){++t.damage;}
 float GetDistanceFromMeAsRatioOfDamageRadius(const CThing&)const;void UpdateHitThings(const C3DVector&,const C3DVector&);};
inline float CTCDExplosion::GetDistanceFromMeAsRatioOfDamageRadius(const CThing& t)const{
 return NPhysicsHelpers::GetPerpendicularDistanceFromFiniteLine(StartPos,EndPos,t.GetPos())/DamageRadius;
}
void CTCDExplosion::UpdateHitThings(const C3DVector& line_start,const C3DVector& line_end)
{
 C2DBoxI area=NPhysicsHelpers::GetLineThingSearchArea(line_start,line_end,DamageRadius);
 std::vector<const CThing*> things_to_ignore;
 std::transform(HitThings.begin(),HitThings.end(),std::back_inserter(things_to_ignore),CGetThingFromIntelligentPointer());
 std::transform(ImmuneThings.begin(),ImmuneThings.end(),std::back_inserter(things_to_ignore),CGetThingFromIntelligentPointer());
 std::vector<const CThing*> specified_things_to_hit;
 std::transform(SpecifiedThingsToHit.begin(),SpecifiedThingsToHit.end(),std::back_inserter(specified_things_to_hit),CGetThingFromIntelligentPointer());
 std::list<CThingCreatureBase*> creature_list;
 GetThingSearchTools().GetAllThingsInRoughArea<CThingCreatureBase>(area,GFFilter_Bind4(CThingFilter_IsNot(GetParentThing()),CThingFilter_IsNotList(things_to_ignore),CThingFilter_IsWithinArea(line_start,line_end,DamageRadius,true),std::not1(CThingFilter_IsEvadeExplosion(line_start,line_end))),creature_list);
 // FableWin preserves the original for-loop scope in the debug branch sequence.
 if (false) {} else for (std::list<CThingCreatureBase*>::iterator it=creature_list.begin();it!=creature_list.end();++it){
    CThingCreatureBase* pcreature=*it;
    if(specified_things_to_hit.empty()||std::find(specified_things_to_hit.begin(),specified_things_to_hit.end(),pcreature)!=specified_things_to_hit.end())GiveDamageToThing(*pcreature);
 }
 std::list<CThing*> thing_list;
 GetThingSearchTools().GetAllThingsInRoughArea<CThing>(area,GFFilter_Bind3(CThingFilter_IsNot(GetParentThing()),CThingFilter_IsNotList(things_to_ignore),CThingFilter_IsWithinArea(line_start,line_end,DamageRadius,false)),thing_list);
 // FableWin preserves the original for-loop scope in the debug branch sequence.
 if (false) {} else for (std::list<CThing*>::iterator it=thing_list.begin();it!=thing_list.end();++it){
    CThing* pthing=*it;
    if(specified_things_to_hit.empty()||std::find(specified_things_to_hit.begin(),specified_things_to_hit.end(),pthing)!=specified_things_to_hit.end()){
        bool thing_set_on_fire=false;
        CTCExplodingObject* ptc_explosion;
        CTCSmashableBase* ptc_smashable;
        if(pthing->GetTC(&ptc_explosion)){
            if(!thing_set_on_fire){
                float distance_ratio=GetDistanceFromMeAsRatioOfDamageRadius(*pthing);
                ptc_explosion->SetCountdownTimer((long)((0.25f+0.75f*distance_ratio)*CMainGameComponent::GetConstantFPS()));
            }else if(pthing->GetTypeFlags()&4){
                CThingGameObject* thing_game_object=static_cast<CThingGameObject*>(pthing);
                GiveDamageToThing(*thing_game_object);
            }
        }else if(pthing->GetTC(&ptc_smashable)){
            if(pthing->GetTypeFlags()&4){
                CThingGameObject* thing_game_object=static_cast<CThingGameObject*>(pthing);
                GiveDamageToThing(*thing_game_object);
            }else if(!thing_set_on_fire)pthing->Kill(true);
        }
    }
 }
}

int main(){for(int seed=0;seed<128;++seed)for(int specified=0;specified<6;++specified)for(int radius=1;radius<=5;++radius){
 CTCDExplosion e;e.DamageRadius=(float)radius;C3DVector a={0,0,0},b={10,0,0};e.StartPos=a;e.EndPos=b;
 if(specified<5)e.SpecifiedThingsToHit.push_back(obj+specified);if(seed&32)e.HitThings.push_back(obj+1);if(seed&64)e.ImmuneThings.push_back(obj+2);
 for(int i=0;i<5;++i){CThing& t=obj[i];t.pos=a;t.pos.Y=float(i);t.bits=(seed+i)&4;t.explosive=((seed+i)&1)!=0;t.smash=((seed+i)&2)!=0;t.creature=((seed+i)&8)!=0;t.evade=((seed+i)&16)!=0;t.kills=t.damage=0;t.ex.delay=-1;}
 e.UpdateHitThings(a,b);
 for(int i=0;i<5;++i){CThing& t=obj[i];bool allowed=i&&(specified==5||specified==i)&&!(i==1&&(seed&32))&&!(i==2&&(seed&64));int damage=allowed&&i<radius+1&&t.creature&&!t.evade;
 int kills=0,delay=-1;if(allowed&&i<radius){if(t.explosive)delay=int((0.25f+0.75f*i/radius)*24);else if(t.smash){if(t.bits&4)++damage;else ++kills;}}
 if(t.damage!=damage||t.kills!=kills||t.ex.delay!=delay)return 1;}
 }puts("0080cd40_TEST PASS");return 0;}