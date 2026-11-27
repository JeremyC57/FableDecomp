#include <cstdio>
#include <cstring>
#include <cstdlib>
enum EAIActionReturn{AI_ACTION_NULL,AI_ACTION_TERMINATE_PROCESS,AI_ACTION_FINISHED,AI_ACTION_FAILED,AI_ACTION_IN_PROGRESS};
enum{ACTION_REPEAT_UNTIL_FINISHED=1,ACTION_REPEAT_CALLBACK=2,AI_MOVEMENT_RUNNING=5,UNSHEATHE_MELEE_WEAPON=4};
namespace NStealth{enum{DANGER_STATUS_CHASING=2};}
struct C3DVector{float x;C3DVector(float f=0):x(f){} C3DVector operator-(const C3DVector& p)const{return C3DVector(x-p.x);}bool IsNonZero()const{return x*x>1e-8f;}};
struct CThing;struct CTCStealth{int danger;void SetAsInDanger(const CThing&,int s){if(s!=2)abort();++danger;}};
struct CTCPerceiveThing{const CThing* target;bool seen;C3DVector aware;const CThing* GetPerceivedThing()const{return target;}bool IsThingPerceived()const{return seen;}const C3DVector& GetAwarenessPosition()const{return aware;}};
struct CAnimationEntry{};
struct CThing{bool alive,perform,canFinish;int finished;CTCPerceiveThing perceive;CTCStealth* stealth;C3DVector pos;CAnimationEntry anim;
 CThing():alive(true),perform(false),canFinish(false),finished(0),stealth(0),pos(99){}
 const C3DVector& GetPos()const{return pos;}bool IsPerformingAnyAction()const{return perform;}bool IsFreeToFinishAction()const{return canFinish;}void FinishCurrentAction(){++finished;}
 void PeekTC(const CTCPerceiveThing** p){*p=&perceive;}bool GetTC(CTCStealth** p){*p=stealth;return !!stealth;}
 const CAnimationEntry& GetRandomAnimFromName(const char* n,int idx)const{if(strcmp(n,"WHATS_THAT")||idx!=-1)abort();return anim;}
};
struct Ptr{CThing* p;CThing* Get()const{return p;}CThing* operator->()const{return p;}bool operator==(const CThing* q)const{return p==q;}};
bool IS_THING_ALIVE(const Ptr& p){return p.p&&p.p->alive;}
struct CSoundPair{int n;CSoundPair(int s):n(s){}};
struct CCreatureAction_PlayAnimation{CCreatureAction_PlayAnimation(CThing& c,const CAnimationEntry& a,CSoundPair sound,bool x,bool y,bool z,int i,int j){if(&a!=&c.anim||sound.n||x||!y||z||i||j)abort();}};
struct Def{unsigned EnemySighted;};struct Dialogue{Def def;int speech,cry;bool success;Def* GetPDialogueDef(){return &def;}void SaySpeech(CThing&,unsigned s){if(s!=def.EnemySighted)abort();++speech;}bool PlayBattleCry(CThing&){++cry;return success;}};
struct Combat{Dialogue dialogue;int unsheathe;Dialogue& GetCombatDialogueManager(){return dialogue;}void CreatureUnsheathWeapon(CThing&,int i){if(i!=4)abort();++unsheathe;}};
struct World{Combat combat;unsigned long seed;unsigned long* GetWorldSeed(){return &seed;}Combat& GetCombatManager(){return combat;}};
int GFRandom(int n,unsigned long* p){if(n!=100)abort();return *p;}
class CAIStateGroup_SeekEnemy{public:Ptr PEnemy;CThing creature;World world;int allowed,checks,term,moves,looks,actions,wanders,inprogress;EAIActionReturn moveret,lookret,animret,wanderret;bool change;float target,lastLook;
 CAIStateGroup_SeekEnemy():allowed(20),checks(0),term(0),moves(0),looks(0),actions(0),wanders(0),inprogress(0),moveret(AI_ACTION_FINISHED),lookret(AI_ACTION_FINISHED),animret(AI_ACTION_FINISHED),wanderret(AI_ACTION_FAILED),change(false),target(-99),lastLook(-99){world.combat.unsheathe=world.combat.dialogue.speech=world.combat.dialogue.cry=0;world.combat.dialogue.def.EnemySighted=53;world.combat.dialogue.success=false;world.seed=51;PEnemy.p=0;creature.perceive.target=0;creature.perceive.seen=false;creature.perceive.aware.x=3;}
 CThing& GetCreature(){return creature;}World& GetWorld(){return world;}bool IsStillRunnable(){return ++checks<=allowed;}void Terminate(){++term;}
 EAIActionReturn AIAction_MoveToPosProximity(int r,C3DVector p,float f,bool& route,int m,bool a,bool b,bool c,const CThing* e,bool g,float h){if(r!=2||f!=1||m!=5||!a||!b||!c||e!=PEnemy.p||g||h)abort();target=p.x;route=true;++moves;return moves<=inprogress?AI_ACTION_IN_PROGRESS:moveret;}
 EAIActionReturn AIAction_LookAtPos(int r,C3DVector p,int i,float f){if(r!=1||i!=1||f!=0.125f)abort();lastLook=p.x;++looks;return lookret;}
 EAIActionReturn AIAction_PerformAction(const CCreatureAction_PlayAnimation&){++actions;if(change)creature.perceive.aware.x+=1;return animret;}
 EAIActionReturn AIAction_MinionWander(int m,void* p){if(m!=5||p)abort();++wanders;return wanders==1?wanderret:AI_ACTION_FAILED;}
 void Update();};
void CAIStateGroup_SeekEnemy::Update()
{
    if (!IsStillRunnable()) { Terminate(); return; }
    {
        if (GetCreature().IsPerformingAnyAction() && GetCreature().IsFreeToFinishAction())
            GetCreature().FinishCurrentAction();
        const CTCPerceiveThing* ptc_perceive;
        GetCreature().PeekTC(&ptc_perceive);
        bool nav_route_calculated=false;
        bool HasSpoken=false;
        EAIActionReturn ret;
        do {
            if (!IS_THING_ALIVE(PEnemy)) { Terminate(); return; }
            C3DVector last_aware_pos;
            if (PEnemy==ptc_perceive->GetPerceivedThing() && !ptc_perceive->IsThingPerceived())
                last_aware_pos=ptc_perceive->GetAwarenessPosition();
            else {
                last_aware_pos=PEnemy->GetPos();
                if (!HasSpoken) {
                    if (GFRandom(100,GetWorld().GetWorldSeed())>50) {
                        unsigned long speech_group_to_say=GetWorld().GetCombatManager().GetCombatDialogueManager().GetPDialogueDef()->EnemySighted;
                        GetWorld().GetCombatManager().GetCombatDialogueManager().SaySpeech(GetCreature(),speech_group_to_say);
                        HasSpoken=true;
                    } else HasSpoken=GetWorld().GetCombatManager().GetCombatDialogueManager().PlayBattleCry(GetCreature());
                }
            }
            GetWorld().GetCombatManager().CreatureUnsheathWeapon(GetCreature(),UNSHEATHE_MELEE_WEAPON);
            static float dist_to_attack_from=1.0f;
            ret=AIAction_MoveToPosProximity(ACTION_REPEAT_CALLBACK,last_aware_pos,dist_to_attack_from,nav_route_calculated,AI_MOVEMENT_RUNNING,true,true,true,PEnemy.Get(),false,0.0f);
            if (ret==AI_ACTION_FAILED || !IsStillRunnable()) { Terminate(); return; }
            CTCStealth* ptc_stealth;
            if(PEnemy->GetTC(&ptc_stealth)) ptc_stealth->SetAsInDanger(GetCreature(),NStealth::DANGER_STATUS_CHASING);
        } while(ret==AI_ACTION_IN_PROGRESS);
        if(ret!=AI_ACTION_FINISHED) { Terminate(); return; }
        {
            C3DVector last_aware_pos=ptc_perceive->GetAwarenessPosition();
            ret=AIAction_LookAtPos(ACTION_REPEAT_UNTIL_FINISHED,last_aware_pos,1,0.125f);
            if (ret==AI_ACTION_TERMINATE_PROCESS || !IsStillRunnable()) { Terminate(); return; }
            {
                const CAnimationEntry& anim_entry=GetCreature().GetRandomAnimFromName("WHATS_THAT",-1);
                do {
                    ret=AIAction_PerformAction(CCreatureAction_PlayAnimation(GetCreature(),anim_entry,CSoundPair(0),false,true,false,0,0));
                    if (!IsStillRunnable() || ret!=AI_ACTION_FINISHED) { Terminate(); return; }
                    if ((ptc_perceive->GetAwarenessPosition()-last_aware_pos).IsNonZero()) { Terminate(); return; }
                    ret=AIAction_MinionWander(AI_MOVEMENT_RUNNING,0);
                } while(ret==AI_ACTION_FINISHED || ret==AI_ACTION_IN_PROGRESS);
            }
        }
    }
}

int main(){
 for(int flags=0;flags<512;++flags)for(int moves=0;moves<3;++moves){
  CAIStateGroup_SeekEnemy g;CThing target;CTCStealth stealth;stealth.danger=0;
  g.PEnemy.p=&target;g.creature.perform=flags&1;g.creature.canFinish=flags&2;g.creature.perceive.target=flags&4?&target:0;g.creature.perceive.seen=!!(flags&8);
  g.world.seed=flags&16?51:50;g.world.combat.dialogue.success=!!(flags&32);if(flags&64)target.stealth=&stealth;
  g.inprogress=moves;g.change=!!(flags&128);g.wanderret=flags&256?AI_ACTION_IN_PROGRESS:AI_ACTION_FAILED;g.Update();
  int count=moves+1;bool perceived=(flags&4)&&!(flags&8);if(g.target!=(perceived?3:99)||g.lastLook!=3||g.creature.finished!=int((flags&1)&&(flags&2))||g.world.combat.unsheathe!=count||g.moves!=count||g.looks!=1||stealth.danger!=(flags&64?count:0))return 1;
  if(g.world.combat.dialogue.speech!=int(!perceived&&(flags&16))||g.world.combat.dialogue.cry!=(!perceived&&!(flags&16)?((flags&32)?1:count):0))return 2;
  if(g.actions!=(g.change?1:((flags&256)?2:1))||g.wanders!=(g.change?0:g.actions)||g.term!=int(g.change))return 3;
 }
 for(int limit=0;limit<6;++limit)for(int mode=0;mode<6;++mode){
  CAIStateGroup_SeekEnemy g;CThing target;g.PEnemy.p=&target;g.allowed=limit;
  if(mode==0)g.PEnemy.p=0;if(mode==1)target.alive=false;if(mode==2)g.moveret=AI_ACTION_FAILED;if(mode==3)g.moveret=AI_ACTION_TERMINATE_PROCESS;if(mode==4)g.lookret=AI_ACTION_TERMINATE_PROCESS;if(mode==5)g.animret=AI_ACTION_FAILED;
  g.Update();if(g.term!=1||g.wanders)return 4;int expMoves=mode<=1||limit==0?0:1;if(g.moves!=expMoves)return 5;
  if(g.looks!=int(mode>=4&&limit>=2)||g.actions!=int(mode==5&&limit>=3))return 6;
 }
 puts("0093b9f0_TEST PASS");return 0;
}