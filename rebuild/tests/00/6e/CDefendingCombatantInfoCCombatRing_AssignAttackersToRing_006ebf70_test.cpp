#include <map>
#include <algorithm>
#include <vector>
#include <stdio.h>
#include <math.h>
static const float DELTA_NOTIONAL_ZERO=0.0001f;
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 bool IsNonZero()const{return X*X+Y*Y+Z*Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
 void AccurateNormalise(){float m=(float)sqrt(X*X+Y*Y+Z*Z);X/=m;Y/=m;Z/=m;}
 float Dot(const C3DVector& v)const{return X*v.X+Y*v.Y+Z*v.Z;}
 C3DVector Cross(const C3DVector& v)const{return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}
};
static float GFArcCos(double x){return (float)acos(x)*0.15915494f;}
struct CWorldMap{};
struct CCombatTypeDef {long DefenderWheelOuterRingStartIndex;};
struct CThingCreatureBase{
 CCombatTypeDef def;C3DVector pos;int id;
 const C3DVector& GetPos()const{return pos;}
 const CCombatTypeDef& GetCombatTypeDef()const{return def;}
};
struct CAttackingCombatantInfo{int id;};
static unsigned accepted,assigns,prongCalls,attackerCalls;
static bool invalid,outerExpected;
class CDefendingCombatantInfo{
public:
 struct CProng{bool usable;int id;C3DVector direction;C3DVector GetProngVector(float rotation)const{
 if(rotation!=0.0f)invalid=true;return direction;}};
 struct CCombatWheel{};
 struct CCombatRing{
 float RotationAngle,DeltaRotation;long Index;std::vector<CProng> Prongs;
 bool IsOuterRing(const CCombatTypeDef&)const;
 bool IsRotating()const{return fabs(DeltaRotation)>DELTA_NOTIONAL_ZERO;}
 void AssignAttackerToProng(CProng* p,CThingCreatureBase* c,CAttackingCombatantInfo* info,bool outer){
 if(outer!=outerExpected||!p||!c||!info||!p->usable||c->id!=info->id||((accepted>>c->id)&1))invalid=true;
 accepted|=1u<<c->id;++assigns;p->usable=false;
 }
 void AssignAttackersToRing(const CWorldMap&,const CCombatWheel&,std::map<CThingCreatureBase*,CAttackingCombatantInfo*>&,const CThingCreatureBase&);
 };
};
struct CIsProngUsable{bool operator()(const CDefendingCombatantInfo::CProng& p)const{return p.usable;}};
static unsigned allow;
struct CGetBestAttackerForProng {
 CThingCreatureBase* best;int prong;
 CGetBestAttackerForProng(const CWorldMap&,const CDefendingCombatantInfo::CCombatWheel&,const CDefendingCombatantInfo::CCombatRing&,
    const CDefendingCombatantInfo::CProng& p,const CThingCreatureBase&):best(0),prong(p.id){}
 void operator()(const std::pair<CThingCreatureBase* const,CAttackingCombatantInfo*>& e){
 ++attackerCalls;if(allow&(1u<<e.first->id))if(!best||e.first->id<best->id)best=e.first;}
 CThingCreatureBase* GetPBestAttacker()const{return best;}
};
struct CGetBestFreeProngForAttacker {
 CDefendingCombatantInfo::CProng* best;int who;
 CGetBestFreeProngForAttacker(const CWorldMap&,const CDefendingCombatantInfo::CCombatWheel&,const CDefendingCombatantInfo::CCombatRing&,
    const CThingCreatureBase& a,const CThingCreatureBase&,const CAttackingCombatantInfo& info):best(0),who(a.id){
    if(info.id!=who)invalid=true;}
 void operator()(CDefendingCombatantInfo::CProng& p){
 ++prongCalls;if((allow&(1u<<who))&&p.usable&&(!best||p.id<best->id))best=&p;}
 CDefendingCombatantInfo::CProng* GetPBestProng()const{return best;}
};
inline bool CDefendingCombatantInfo::CCombatRing::IsOuterRing(const CCombatTypeDef& defender_combat_type_def) const
{
    return Index >= defender_combat_type_def.DefenderWheelOuterRingStartIndex;
}

void CDefendingCombatantInfo::CCombatRing::AssignAttackersToRing(const CWorldMap& wmap,const CCombatWheel& wheel,
    std::map<CThingCreatureBase*,CAttackingCombatantInfo*>& attackers,const CThingCreatureBase& defender)
{
    typedef std::map<CThingCreatureBase*,CAttackingCombatantInfo*> AttackerMap;
    const CCombatTypeDef& def_cmbt_type=defender.GetCombatTypeDef();
    bool outer_ring=IsOuterRing(def_cmbt_type);
    std::vector<CProng>::iterator it_usable_end=std::partition(Prongs.begin(),Prongs.end(),CIsProngUsable());
    long num_assign_attackers=attackers.size();
    long num_free_prongs=it_usable_end-Prongs.begin();
    if(num_free_prongs==0)return;
    if(Index<def_cmbt_type.DefenderWheelOuterRingStartIndex && attackers.size()==1 && !IsRotating()) {
        const CThingCreatureBase& one_true_attacker=*attackers.begin()->first;
        const CProng& one_true_prong=*Prongs.begin();
        C3DVector preferred_prong_vector=one_true_attacker.GetPos()-defender.GetPos();
        preferred_prong_vector.Z=0.0f;
        if(preferred_prong_vector.IsNonZero())preferred_prong_vector.AccurateNormalise();
        C3DVector prong_vector=one_true_prong.GetProngVector(0.0f);
        float cos_thet=prong_vector.Dot(preferred_prong_vector);
        if(cos_thet < -1.0f)cos_thet=-1.0f;
        else if(cos_thet > 1.0f)cos_thet=1.0f;
        float new_rotation_angle=GFArcCos(cos_thet);
        C3DVector perp=prong_vector.Cross(preferred_prong_vector);
        if(perp.Z>0.0f)new_rotation_angle*=-1.0f;
        RotationAngle=new_rotation_angle;
    }
    if(num_assign_attackers>=num_free_prongs){
        for(std::vector<CProng>::iterator it=Prongs.begin();it!=it_usable_end && num_assign_attackers!=0;++it){
            CProng& prong=*it;
            CThingCreatureBase* pbest_attacker=std::for_each(attackers.begin(),attackers.end(),
                CGetBestAttackerForProng(wmap,wheel,*this,prong,defender)).GetPBestAttacker();
            if(pbest_attacker){
                AttackerMap::const_iterator it_look=attackers.find(pbest_attacker);
                CAttackingCombatantInfo* pattacker_inf=it_look->second;
                AssignAttackerToProng(&prong,pbest_attacker,pattacker_inf,outer_ring);
                attackers.erase(pbest_attacker);
                --num_free_prongs;
                --num_assign_attackers;
            }
        }
    } else {
        for(AttackerMap::iterator it=attackers.begin();it!=attackers.end();){
            CThingCreatureBase* pattacker=it->first;
            CAttackingCombatantInfo* pattacker_inf=it->second;
            CProng* pbest_prong=std::for_each(Prongs.begin(),it_usable_end,
                CGetBestFreeProngForAttacker(wmap,wheel,*this,*pattacker,defender,*pattacker_inf)).GetPBestProng();
            if(pbest_prong){
                AssignAttackerToProng(pbest_prong,pattacker,pattacker_inf,outer_ring);
                attackers.erase(it++);
                --num_assign_attackers;
                --num_free_prongs;
            }else ++it;
        }
    }
}

int main() {
 CWorldMap world;CDefendingCombatantInfo::CCombatWheel wheel;
 for(unsigned used=0;used<8;++used)for(unsigned count=0;count<=3;++count)for(allow=0;allow<8;++allow)
 for(unsigned outer=0;outer<2;++outer)for(unsigned rotating=0;rotating<2;++rotating)for(unsigned mode=0;mode<5;++mode){
  CThingCreatureBase defender,creatures[3];CAttackingCombatantInfo infos[3];
  std::map<CThingCreatureBase*,CAttackingCombatantInfo*> attackers;CDefendingCombatantInfo::CCombatRing ring;
  defender.pos=C3DVector(0,0,2);defender.def.DefenderWheelOuterRingStartIndex=1;
  ring.Index=outer;ring.RotationAngle=0.875f;ring.DeltaRotation=rotating?0.5f:0;
  unsigned freeCount=0;
  for(int i=0;i<3;++i){CDefendingCombatantInfo::CProng p;p.id=i;p.usable=(used&(1u<<i))!=0;freeCount+=unsigned(p.usable);
   p.direction=C3DVector(mode==1?-2.0f:2.0f,0,0);ring.Prongs.push_back(p);
   infos[i].id=creatures[i].id=i;creatures[i].pos=C3DVector(mode<2?1.0f:0.0f,mode==2?1.0f:mode==3?-1.0f:0.0f,8.0f);
   if(unsigned(i)<count)attackers[&creatures[i]]=&infos[i];}
  accepted=assigns=prongCalls=attackerCalls=0;invalid=false;outerExpected=outer!=0;
  ring.AssignAttackersToRing(world,wheel,attackers,defender);
  unsigned eligibleCount=0;for(unsigned id=0;id<count;++id)if(allow&(1u<<id))++eligibleCount;
  unsigned expected=eligibleCount<freeCount?eligibleCount:freeCount;
  if(invalid||assigns!=expected||attackers.size()!=count-expected||(accepted&~allow))return 1;
  for(unsigned id=0;id<count;++id)if(bool(attackers.count(&creatures[id]))==bool(accepted&(1u<<id)))return 2;
  if(count>=freeCount&&prongCalls || count<freeCount&&attackerCalls || freeCount==0&&(attackerCalls||prongCalls))return 3;
  float angle=0.875f;
  if(freeCount && !outer && count==1 && !rotating)angle=mode==0?0.0f:mode==1?0.5f:mode==2?-0.25f:0.25f;
  if(fabs(ring.RotationAngle-angle)>0.00001f)return 4;
 }
 puts("006ebf70_TEST PASS");return 0;
}