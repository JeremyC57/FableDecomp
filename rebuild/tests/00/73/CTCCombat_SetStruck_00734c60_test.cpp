#include <stdio.h>
#include <stdlib.h>
#include <string.h>
static unsigned long seed=1,bits,more; static int trial;
#define CHECK(x) do{if(!(x)){printf("case %d line %d\n",trial,__LINE__);exit(1);}}while(0)
static bool B(unsigned n){return ((bits>>n)&1)!=0;}
static unsigned long next(){seed=seed*1664525UL+1013904223UL;return seed^(seed>>13);}
struct CThing;struct CTCCombat;struct CTCCarrying;struct CTCWeapon;struct CTCHeroExperience;struct CTCPhysicalShieldSpell;struct CThingCreatureBase;
struct C3DVector{int id;};struct CMaterialDef{long index;};
template<class T> struct CDefPointer{T*p;CDefPointer():p(0){}T* operator->() const{return p;}};
struct CArmourDef{long BlockMaterial;};static CArmourDef armour;static CMaterialDef material,carriedMat;
namespace NHitLocations{struct CHitLocation{
 CArmourDef*GetArmourDef()const{return &armour;}
 bool IsDefendableAgainstMelee()const{return B(0);}bool IsDefendableAgainstShots()const{return B(1);}
};}static NHitLocations::CHitLocation hit;
struct CThing{unsigned flags;bool dead;long type;
 unsigned GetTypeFlags()const{return flags;}long GetType()const{return type;}
 bool GetTC(CTCCombat**);bool GetTC(CTCPhysicalShieldSpell**);bool GetTC(CTCHeroExperience**);
 bool PeekTC(const CTCCarrying**)const;bool PeekTC(const CTCWeapon**)const;
};struct CThingGameObject:public CThing{CDefPointer<const CMaterialDef> PeekPMaterial()const{CDefPointer<const CMaterialDef>x;x.p=&carriedMat;return x;}};
template<class T>bool IS_THING_ALIVE(T*p){return p&&!p->dead;}
template<class X,class Y>Y GFDebugPDynamicCast(X x,Y){return static_cast<Y>(x);}
template<class X,class Y>Y* GFDebugDynamicCast(X*x,Y*){return static_cast<Y*>(x);}
struct CCreatureActionBase{int kind;bool IsStrikeAction()const{return B(2);}};static CCreatureActionBase current;
struct CCreatureAction_BlockBroken:CCreatureActionBase{CCreatureAction_BlockBroken(CThingCreatureBase&,CThing&){kind=1;}};
struct CCreatureAction_BlockCounter:CCreatureActionBase{CCreatureAction_BlockCounter(CThingCreatureBase&,CThingCreatureBase&){kind=2;}};
struct CCreatureAction_BlockRespond:CCreatureActionBase{CCreatureAction_BlockRespond(CThingCreatureBase&,CThingCreatureBase&,long x){CHECK(!x);kind=3;}};
struct Log{int action[4],finished,reacted,position,shieldDamage,parry,block,blocked,flourish;}static logg;
struct CThingCreatureBase:CThingGameObject{const CCreatureActionBase*PeekPCurrentAction()const{return B(3)?&current:0;}
 void PerformBlockedReaction(CThingCreatureBase*p,bool v){CHECK(!p&&v);++logg.reacted;}
 void FinishCurrentAction(){++logg.finished;}
 bool SetCurrentAction(const CCreatureActionBase&a){++logg.action[a.kind];return a.kind==1?B(4):a.kind==2?B(5):true;}
};static CThingCreatureBase defender,attacker;static CThingGameObject carried,weapon;
enum EWeaponClass{WC_UNARMED=0,WC_LIGHT=1};struct CTCWeapon{
 bool IsAbleToBeUsedToBlock(const CThing&d,const CThing*h,const CThing*)const{CHECK(&d==&defender&&h==&attacker);return B(6);}
 bool IsBlockBrokenOnBlock(const CThing&,const CThing*,const CThing*)const{return B(7);}
 EWeaponClass GetWeaponClass()const{return WC_LIGHT;}
};static CTCWeapon weaponTC;
struct CTCCarrying{CThing* GetThingInPrimarySlot()const{return B(8)?&carried:0;}};static CTCCarrying carrying;
struct CTCHeroExperience{void AddSuccessfulBlockParry(){++logg.parry;}};static CTCHeroExperience hero;
struct CTCPhysicalShieldSpell{bool IsShieldActive()const{return B(9);} void OnHitAtPos(const C3DVector&p){CHECK(p.id==42);++logg.position;}
 void TakeDamage(float f){CHECK(f==3.25f);++logg.shieldDamage;}bool GetAllowReactionsOnHit(CThing*)const{return B(10);}
};static CTCPhysicalShieldSpell shield;
enum ECombatAbilityInterfaceType{COMBAT_ABILITY_INTERFACE_ATTACK_BLOCK_COUNTER=5};
struct CCombatAbilityData{float SecondsDuration;}; static CCombatAbilityData dat;
struct CCombatAbility{bool IsValid(const CCombatAbilityData&d,unsigned long*p)const{CHECK(&d==&dat&&p==&seed);return B(11);}};
struct CombatNode{struct { CCombatAbilityData* first;CCombatAbility second;}second;};static CombatNode node;
struct TCombatAbilityList{typedef CombatNode* const_iterator;};
struct CWorld{unsigned long* GetWorldSeed()const{return &seed;}};static CWorld world;
struct Manager{void GetDef(long key,CDefPointer<const CMaterialDef>*p){CHECK(key==armour.BlockMaterial&&key>0);p->p=&material;}};static Manager manager;
struct CMainGameComponent{static long GetConstantFPS(){return 37;}};
namespace NCreatureMode{bool IsToDeflectAllHits(const CThing&d){CHECK(&d==&defender);return (more&3)==1;}}
struct CHitParameters{
 bool blockCounter,blocked,recoil,guaranteed,cleared;const CMaterialDef* output;C3DVector pos;
 bool IsBlockCounter()const{return blockCounter;}bool IsLocalised()const{return B(12);}const C3DVector&GetHitPosition()const{return pos;}
 CThing* GetPThingHitting()const{return (more&12)==0?0:&attacker;}CThing*GetPWeapon()const{return B(13)?&weapon:0;}
 bool IsDrainHealStrike()const{return B(14);}bool IsDrainStrike()const{return B(15);}
 bool IsMeleeStrike()const{return B(16);}bool IsShotStrike()const{return B(17);}
 void SetToCauseRecoil(bool v){recoil=v;}void SetBlocked(bool v){blocked=v;}void SetBlockCounter(bool v){blockCounter=v;}
 void SetGuaranteeHitResponse(bool v){guaranteed=v;}void ClearAllDamage(){cleared=true;}
 float GetDamageToThing(const CThingGameObject&d){CHECK(&d==&defender);return 3.25f;}
 NHitLocations::CHitLocation*GetPHitLocation()const{return &hit;}
 void SetPHitMaterialDef(CDefPointer<const CMaterialDef>p){output=p.p;}
};
struct CTCCombat{long BlockingNumFrames,BlockHoldTimer;
 CThingCreatureBase&GetParentCreature(){return defender;}CThing&GetParentThing(){return defender;}
 Manager&GetDefinitionManager(){return manager;}CWorld&PeekWorld(){return world;}
 bool IsBlocking()const{return B(18);}bool IsStartingToBlock()const{return B(19);}
 bool IsToBlock(const CHitParameters&)const{return (more&16)==0;}
 bool GetCombatAbilityData(TCombatAbilityList::const_iterator&it,ECombatAbilityInterfaceType t)const{CHECK(t==COMBAT_ABILITY_INTERFACE_ATTACK_BLOCK_COUNTER);it=&node;return B(20);}
 bool GetBlockAbilityIterator(TCombatAbilityList::const_iterator&it,EWeaponClass w)const{CHECK(w==(B(13)&&!weapon.dead&&B(22)?WC_LIGHT:WC_UNARMED));it=&node;return B(21);}
 void SetBlocking(){++logg.block;}void SetBlocked(CThing&a){CHECK(&a==&attacker);++logg.blocked;}void SetMeleeFlourishValid(bool v){CHECK(v);++logg.flourish;}
 void SetStruck(const NHitLocations::CHitLocation&,CHitParameters&);
};static CTCCombat enemyTC;
bool CThing::GetTC(CTCCombat**p){CHECK(this==&attacker);*p=&enemyTC;return B(23);}
bool CThing::GetTC(CTCPhysicalShieldSpell**p){CHECK(this==&defender);*p=&shield;return (more&32)!=0;}
bool CThing::GetTC(CTCHeroExperience**p){CHECK(this==&defender);*p=&hero;return B(24);}
bool CThing::PeekTC(const CTCCarrying**p)const{CHECK(this==&defender);*p=&carrying;return B(25);}
bool CThing::PeekTC(const CTCWeapon**p)const{CHECK(this==&weapon||this==&carried);*p=&weaponTC;return this==&weapon?B(22):B(26);}
void CTCCombat::SetStruck(const NHitLocations::CHitLocation& hit_location, CHitParameters& hit_params)
{
    CThingCreatureBase& defender_creature = GetParentCreature();
    const CCreatureActionBase* pdefender_action = defender_creature.PeekPCurrentAction();
    bool block_counter = hit_params.IsBlockCounter();
    bool counter_attack = false;
    const C3DVector* ppoint_of_hit = 0;
    if (hit_params.IsLocalised())
        ppoint_of_hit = &hit_params.GetHitPosition();
    CThing* pthing_hitting = hit_params.GetPThingHitting();
    if (defender_creature.GetType() == 1 && IsBlocking() &&
        IS_THING_ALIVE(pthing_hitting) && !block_counter) {
        TCombatAbilityList::const_iterator it;
        if (GetCombatAbilityData(it, COMBAT_ABILITY_INTERFACE_ATTACK_BLOCK_COUNTER) &&
            it->second.second.IsValid(*it->second.first, PeekWorld().GetWorldSeed()))
            counter_attack = true;
    }

    CTCPhysicalShieldSpell* ptc_shield;
    if (!hit_params.IsDrainHealStrike() && defender_creature.GetTC(&ptc_shield) &&
        ptc_shield->IsShieldActive()) {
        if (!hit_params.IsDrainStrike())
            hit_params.SetToCauseRecoil(true);
        if (ppoint_of_hit)
            ptc_shield->OnHitAtPos(*ppoint_of_hit);
        ptc_shield->TakeDamage(hit_params.GetDamageToThing(defender_creature));
        if (ptc_shield->GetAllowReactionsOnHit(pthing_hitting)) {
            hit_params.ClearAllDamage();
            hit_params.SetGuaranteeHitResponse(true);
        } else {
            hit_params.SetBlocked(true);
        }
        return;
    }
    if (NCreatureMode::IsToDeflectAllHits(defender_creature)) {
        hit_params.SetBlocked(true);
        hit_params.SetToCauseRecoil(true);
        CDefPointer<const CMaterialDef> pmaterial_def;
        long material_index = hit_params.GetPHitLocation()->GetArmourDef()->BlockMaterial;
        if (material_index > 0)
            GetDefinitionManager().GetDef(material_index, &pmaterial_def);
        hit_params.SetPHitMaterialDef(pmaterial_def);
        return;
    }
    if (!((hit_location.IsDefendableAgainstMelee() && hit_params.IsMeleeStrike()) ||
          (hit_location.IsDefendableAgainstShots() && hit_params.IsShotStrike())) ||
        !IS_THING_ALIVE(pthing_hitting) || !IsToBlock(hit_params))
        return;
    if (BlockingNumFrames <= 6) {
        CTCHeroExperience* ptc_hero_exp;
        if (GetParentThing().GetTC(&ptc_hero_exp))
            ptc_hero_exp->AddSuccessfulBlockParry();
    }
    bool block_effective = false;
    CThing* pweapon = hit_params.GetPWeapon();
    CThing* phitter = pthing_hitting;
    CDefPointer<const CMaterialDef> pblock_material;
    const CTCCarrying* ptc_carrying;
    const CThing* pcarried = defender_creature.PeekTC(&ptc_carrying)
        ? ptc_carrying->GetThingInPrimarySlot() : 0;
    CDefPointer<const CMaterialDef> pcarried_material;
    if (IS_THING_ALIVE(pcarried) && (pcarried->GetTypeFlags() & 4)) {
        const CThingGameObject* pgo = GFDebugPDynamicCast(pcarried, (const CThingGameObject*)0);
        pcarried_material = pgo->PeekPMaterial();
    }
    const CTCWeapon* ptc_weapon;
    if (!IS_THING_ALIVE(pcarried)) {
        long block_mat = hit_params.GetPHitLocation()->GetArmourDef()->BlockMaterial;
        CDefPointer<const CMaterialDef> pdefender_block_material;
        if (block_mat > 0)
            GetDefinitionManager().GetDef(block_mat, &pdefender_block_material);
        block_effective = true;
        pblock_material = pdefender_block_material;
    } else if (pcarried->PeekTC(&ptc_weapon) &&
               ptc_weapon->IsAbleToBeUsedToBlock(defender_creature, phitter, pweapon)) {
        if (ptc_weapon->IsBlockBrokenOnBlock(defender_creature, phitter, pweapon)) {
            block_effective = true;
            block_counter = true;
            pblock_material = pcarried_material;
        } else {
            block_effective = true;
            pblock_material = pcarried_material;
        }
    }
    if (block_effective && IS_THING_ALIVE(phitter) && (phitter->GetTypeFlags() & 8)) {
        CThingCreatureBase& attacker_creature = *GFDebugDynamicCast(phitter, (CThingCreatureBase*)0);
        bool attacker_recoil = true;
        if (pdefender_action && pdefender_action->IsStrikeAction()) {
            defender_creature.PerformBlockedReaction(0, true);
        } else {
            if (defender_creature.GetType() != 2 || IsStartingToBlock())
                defender_creature.FinishCurrentAction();
            if (block_counter && defender_creature.SetCurrentAction(
                    CCreatureAction_BlockBroken(defender_creature, attacker_creature))) {
                attacker_recoil = false;
                hit_params.SetBlockCounter(true);
            } else if (counter_attack && defender_creature.SetCurrentAction(
                         CCreatureAction_BlockCounter(defender_creature, attacker_creature))) {
            } else {
                defender_creature.SetCurrentAction(
                    CCreatureAction_BlockRespond(defender_creature, attacker_creature, 0));
                EWeaponClass wclass = WC_UNARMED;
                if (IS_THING_ALIVE(pweapon) && pweapon->PeekTC(&ptc_weapon))
                    wclass = ptc_weapon->GetWeaponClass();
                SetBlocking();
                SetBlocked(attacker_creature);
                TCombatAbilityList::const_iterator it;
                if (GetBlockAbilityIterator(it, wclass)) {
                    const CCombatAbilityData& comb_ability_dat = *it->second.first;
                    long const_fps = CMainGameComponent::GetConstantFPS();
                    float seconds_duration = comb_ability_dat.SecondsDuration;
                    BlockHoldTimer = (long)(const_fps * seconds_duration);
                }
                CTCCombat* ptc_combat;
                if (attacker_creature.GetTC(&ptc_combat))
                    ptc_combat->SetMeleeFlourishValid(true);
            }
        }
        hit_params.SetBlocked(true);
        hit_params.SetPHitMaterialDef(pblock_material);
        if (attacker_recoil)
            hit_params.SetToCauseRecoil(true);
    }
}
int main(){
 node.second.first=&dat; dat.SecondsDuration=.57f;
 int seen[5]={0,0,0,0,0};
 for(trial=0;trial<131072;++trial){
  bits=next();more=next();memset(&logg,0,sizeof(logg));
  CTCCombat combat;combat.BlockingNumFrames=(more>>10)%10;combat.BlockHoldTimer=-1;
  defender.type=(more>>20)%3;attacker.dead=(more&64)!=0;attacker.flags=(more&128)?8:0;
  carried.dead=B(27);carried.flags=B(28)?4:0;weapon.dead=B(29);
  armour.BlockMaterial=B(30)?7:-1;
  CHitParameters h;memset(&h,0,sizeof(h));h.blockCounter=B(31);h.pos.id=42;
  combat.SetStruck(hit,h);
  const bool initialCounter=B(31),fromShield=!B(14)&&(more&32)&&B(9);
  const bool deflect=!fromShield&&(more&3)==1;
  const bool alive=(more&12)&&!attacker.dead;
  const bool defended=!fromShield&&!deflect&&alive&&!(more&16)&&((B(0)&&B(16))||(B(1)&&B(17)));
  const bool hasCarried=B(25)&&B(8)&&!carried.dead;
  const bool useBlock=defended&&(attacker.flags&8)&&(!hasCarried||(B(26)&&B(6)));
  const bool currentStrike=useBlock&&B(3)&&B(2);
  const bool respond=useBlock&&!currentStrike;
  const bool brokenAttempt=respond&&(initialCounter||(hasCarried&&B(7)));
  const bool broken=brokenAttempt&&B(4);
  const bool counterTry=respond&&!broken&&defender.type==1&&B(18)&&!initialCounter&&B(20)&&B(11);
  const bool ordinary=respond&&!broken&&!(counterTry&&B(5));
  const CMaterialDef* expected=(deflect||(useBlock&&!hasCarried))&&armour.BlockMaterial>0?&material:
                             useBlock&&hasCarried&&(carried.flags&4)?&carriedMat:0;
  CHECK(h.output==expected);CHECK(h.blocked==(deflect||useBlock||(fromShield&&!B(10))));
  CHECK(h.recoil==(deflect||(useBlock&&!broken)||(fromShield&&!B(15))));
  CHECK(h.guaranteed==(fromShield&&B(10)));CHECK(h.cleared==h.guaranteed);
  CHECK(h.blockCounter==(initialCounter||broken));CHECK(logg.position==(fromShield&&B(12)));
  CHECK(logg.shieldDamage==fromShield);CHECK(logg.parry==(defended&&combat.BlockingNumFrames<=6&&B(24)));
  CHECK(logg.reacted==currentStrike);CHECK(logg.finished==(respond&&(defender.type!=2||B(19))));
  CHECK(logg.action[1]==brokenAttempt&&logg.action[2]==counterTry&&logg.action[3]==ordinary);
  CHECK(logg.block==ordinary&&logg.blocked==ordinary&&logg.flourish==(ordinary&&B(23)));
  CHECK(combat.BlockHoldTimer==(ordinary&&B(21)?21:-1));
  seen[0]+=fromShield;seen[1]+=deflect;seen[2]+=broken;seen[3]+=counterTry&&B(5);seen[4]+=ordinary;
 }
 for(int i=0;i<5;++i)CHECK(seen[i]>0);
 puts("00734c60_TEST PASS");return 0;
}