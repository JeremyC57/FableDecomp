#include <assert.h>
#include <stdio.h>
#include <vector>
struct CThing; struct CThingAICreature;struct CHitParameters;
enum {TCI_ENTITY_MODE_MANAGER,TCI_HERO_OPINION_DEED_LOG,TCI_ENEMY,TCI_CREATURE_OPINION_OF_HERO,TCI_ENTITY_EVENTS};
enum EOpinionDeedType {OPINION_DEED_TYPE_CRIME_GBH=8,OPINION_DEED_TYPE_CRIME_ASSAULT=7,OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH=53};
enum {OPINION_POST_DEED_TYPE_RECIPIENT=1,OPINION_POST_DEED_TYPE_WITNESSES=2,MESSAGE_EVENT_YELL_I_SEE_HIM=12,NULL_DEF=0};
static unsigned config; static bool bit(unsigned n){return (config>>n & 1)!=0;}
static CThing *expectedAttacker,*expectedVictim,*expectedWeapon;
static unsigned fps,baseHits,imposterHits,shouts,updates,enemyQueries,threatQueries,meleeQueries,rangedQueries,events,defQueries;
struct C3DVector {int x; C3DVector(int v=0):x(v){}};
struct CRightHandedSet {int yaw; CRightHandedSet(int v=0):yaw(v){}};
struct Physics {
 C3DVector pos; CRightHandedSet rh;
 const CRightHandedSet& GetRHSet(){return rh;}
 const C3DVector& GetPosition(){return pos;}
 void ResetPosition(const C3DVector& v){pos=v;}
 void ResetRHSet(const CRightHandedSet& v){rh=v;}
};
struct CFaction{};static CFaction faction;
struct CTCEnemy {const CFaction* GetPFaction()const{return bit(14)?&faction:0;}static bool AreThingsEnemies(const CThing&a,const CThing&b){++enemyQueries;assert(&a==expectedVictim&&&b==expectedAttacker);return bit(10);}};
struct CTCCreatureOpinionOfHero {bool OnHit(bool armed){assert(armed==(expectedWeapon!=expectedAttacker));return bit(16);}};
struct CTCCarrying {static bool IsThingCarryingMeleeWeapon(const CThing&t){assert(&t==expectedAttacker);++meleeQueries;return bit(18);}static bool IsThingCarryingRangedWeapon(const CThing&t){assert(&t==expectedAttacker);++rangedQueries;return bit(19);}};
struct CCreatureDef {unsigned CreatureGroup;};
template<class T>struct CDefPointer {const T* p;CDefPointer(int):p(0){} const T*operator->()const{return p;}};
struct Def {CCreatureDef def;bool GetSubDef(CDefPointer<const CCreatureDef>* out){++defQueries;out->p=&def;return bit(7);}};
struct CMainGameComponent {static unsigned GetConstantFPS(){return fps;}};
struct Message {void AddMessage(int type,unsigned delay,const C3DVector& p,CThing* from,CThing* to,float a,float b,long c){assert(type==12&&delay==5*fps&&from==expectedVictim&&to==expectedAttacker&&a==0&&b==5&&c==0);assert(p.x==77||p.x==12);++shouts;}};
struct World {Message msg;Message& GetMessageEventManager(){return msg;}};
struct CThing {
 virtual ~CThing(){}
 CThing():dead(false),flags(0){for(unsigned i=0;i<5;++i)tc[i]=0;}
 void*tc[5];bool dead;unsigned flags;Physics physics;World world;Def base;
 template<class T>bool GetTC(int t,T**v){*v=(T*)tc[t];return *v!=0;}
 template<class T>bool PeekTC(int t,T**v)const{*v=(T*)tc[t];return *v!=0;}
 unsigned GetTypeFlags()const{return flags;} Physics*GetPhysicsTC(){return &physics;} World& GetWorld(){return world;}
 const C3DVector&GetPos()const{return physics.pos;} const C3DVector&PeekPos()const{return physics.pos;} Def&PeekBaseDef(){return base;}
};
template<class T>bool IS_THING_ALIVE(T* p){return p&&!p->dead;}
template<class T>bool IS_THING_DEAD(T* p){return !IS_THING_ALIVE(p);}
struct CHitParameters {CThing*attacker;CThing*weapon;CThing*GetPThingHitting(){return attacker;}CThing*GetPWeapon(){return weapon;}bool IsDecapitate(){return bit(20);}bool IsFlourish(){return bit(21);}};
struct CThingGameObject:CThing {virtual void OnHit(CHitParameters&){++imposterHits;}};
struct CThingCreatureBase:CThingGameObject {float hp;CThingCreatureBase*last;
 float GetHealth()const{return hp;}const CThingCreatureBase*PeekPLastAttackedByCreature()const{return last;}
 bool IsNewAttackerGreaterThreat(const CThingCreatureBase& a,const CThingCreatureBase& b)const{assert(&a==last&&&b==expectedAttacker);++threatQueries;return bit(11);}
 void SetAttackedByCreature(CThingCreatureBase*p){assert(p==expectedAttacker);last=p;++updates;}
 void OnHit(CHitParameters&){++baseHits;}
};
struct CThingAICreature:CThingCreatureBase {void OnHit(CHitParameters&);};
struct Mode {};
namespace NCreatureMode {struct CModeBriarRoseConcentrate:Mode {CThing* imp;bool IsImposter(){return bit(2);}long IsAllowedToHitBriarRose(){return bit(3);}CThing*GetRandomImposter(){return imp;}};}
struct CTCCreatureModeManager {NCreatureMode::CModeBriarRoseConcentrate mode;bool IsModeActive(unsigned n){assert(n==0x3f||n==0x51);return n==0x3f?bit(1):bit(6);}Mode&GetCurrentMode(){return mode;}};
struct Post {int type,who;bool crime;long extra;};
static std::vector<Post> posts;static unsigned searches,handleQueries;
struct CTCHeroOpinionDeedLog {
 long FindCriminalDeed(EOpinionDeedType t,CThing* victim){assert(victim==expectedVictim&&int(t)==(expectedWeapon==expectedAttacker?7:8));++searches;return bit(17)?39:-1;}
 long PostDeed(EOpinionDeedType t,const C3DVector& pos,CThing*target,CThingAICreature*victim,int who,bool crime,bool b,const CFaction*f,bool c,long extra){assert(target==expectedVictim&&victim==expectedVictim&&&pos==&victim->GetPos()&&!b&&!c&&f==&faction);Post p={int(t),who,crime,extra};posts.push_back(p);return 27;}
 const void*GetDeedFromHandle(long n){assert(n==27);++handleQueries;return bit(22)?this:0;}
};
namespace NEntityEvents {struct CEventHitThing {
 CEventHitThing(const CThing& a,CThing& b,bool enemy,bool melee,bool ranged,const CThing*weapon,bool flourish,bool decap){assert(&a==expectedAttacker&&&b==expectedVictim&&enemy==bit(10)&&melee==bit(18)&&ranged==bit(19)&&weapon==expectedWeapon&&flourish==bit(21)&&decap==bit(20));}
};}
struct CTCEntityEvents {void AddEvent(NEntityEvents::CEventHitThing*ev){assert(ev);++events;delete ev;}};
void CThingAICreature::OnHit(CHitParameters& hit_params)
{
    CTCCreatureModeManager* ptc_cmm;
    if (GetTC(TCI_ENTITY_MODE_MANAGER, &ptc_cmm)) {
        if (ptc_cmm->IsModeActive(0x3f)) {
            NCreatureMode::CModeBriarRoseConcentrate* briar_mode = static_cast<NCreatureMode::CModeBriarRoseConcentrate*>(&ptc_cmm->GetCurrentMode());
            if (!briar_mode->IsImposter() && !briar_mode->IsAllowedToHitBriarRose()) {
                CThing* pimposter = briar_mode->GetRandomImposter();
                if (IS_THING_ALIVE(pimposter)) {
                    CRightHandedSet their_rhs(pimposter->GetPhysicsTC()->GetRHSet());
                    C3DVector their_pos(pimposter->GetPhysicsTC()->GetPosition());
                    pimposter->GetPhysicsTC()->ResetRHSet(GetPhysicsTC()->GetRHSet());
                    pimposter->GetPhysicsTC()->ResetPosition(GetPhysicsTC()->GetPosition());
                    GetPhysicsTC()->ResetRHSet(their_rhs);
                    GetPhysicsTC()->ResetPosition(their_pos);
                    if (pimposter->GetTypeFlags() & 4) {
                        CThingGameObject* pgo = static_cast<CThingGameObject*>(pimposter);
                        pgo->OnHit(hit_params);
                        return;
                    }
                }
            }
        }
        if (ptc_cmm->IsModeActive(0x51)) return;
    }
    bool hit_enemy = false;
    CThing* phitting_thing = hit_params.GetPThingHitting();
    if (IS_THING_ALIVE(phitting_thing) && (phitting_thing->GetTypeFlags() & 8)) {
        const unsigned long TIME_TO_LEAVE_SHOUT_MESSAGE = CMainGameComponent::GetConstantFPS() * 5;
        const float ON_HIT_SHOUT_RADIUS = 5.0f;
        GetWorld().GetMessageEventManager().AddMessage(MESSAGE_EVENT_YELL_I_SEE_HIM, TIME_TO_LEAVE_SHOUT_MESSAGE, GetPos(), this, phitting_thing, 0.0f, ON_HIT_SHOUT_RADIUS, 0);
        hit_enemy = CTCEnemy::AreThingsEnemies(*this, *phitting_thing);
        CThingCreatureBase* phitting_creature = static_cast<CThingCreatureBase*>(phitting_thing);
        bool am_i_a_dying_troll = false;
        CDefPointer<const CCreatureDef> pcreature_def(NULL_DEF);
        if (PeekBaseDef().GetSubDef(&pcreature_def))
            am_i_a_dying_troll = GetHealth() <= 0.0f && ((pcreature_def->CreatureGroup & 0x10000) || (pcreature_def->CreatureGroup & 0x2000000) || (pcreature_def->CreatureGroup & 0x8000));
        const CThingCreatureBase* plast_attacker = PeekPLastAttackedByCreature();
        if ((IS_THING_DEAD(plast_attacker) || IsNewAttackerGreaterThreat(*plast_attacker,*phitting_creature)) && !am_i_a_dying_troll) {
            SetAttackedByCreature(phitting_creature);
            bool extra_data_relating_to_hits_killing = false;
            (void)extra_data_relating_to_hits_killing;
            CTCHeroOpinionDeedLog* ptc_deed_log;
            if (phitting_creature->GetTC(TCI_HERO_OPINION_DEED_LOG,&ptc_deed_log)) {
                const CTCEnemy* pmy_enemy_tc;
                if (PeekTC(TCI_ENEMY,&pmy_enemy_tc) && pmy_enemy_tc->GetPFaction()) {
                    CTCCreatureOpinionOfHero* ptc_opinion;
                    if (GetTC(TCI_CREATURE_OPINION_OF_HERO,&ptc_opinion)) {
                        EOpinionDeedType deed = hit_params.GetPWeapon() != phitting_thing ? OPINION_DEED_TYPE_CRIME_GBH : OPINION_DEED_TYPE_CRIME_ASSAULT;
                        bool is_crime = ptc_opinion->OnHit(deed == OPINION_DEED_TYPE_CRIME_GBH);
                        long old_crime = ptc_deed_log->FindCriminalDeed(deed,this);
                        if (old_crime == -1) {
                            long crime_handle = ptc_deed_log->PostDeed(deed,PeekPos(),this,this,OPINION_POST_DEED_TYPE_RECIPIENT,is_crime,false,pmy_enemy_tc->GetPFaction(),false,0);
                            if (ptc_deed_log->GetDeedFromHandle(crime_handle))
                                ptc_deed_log->PostDeed(OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH,PeekPos(),this,this,OPINION_POST_DEED_TYPE_WITNESSES,false,false,pmy_enemy_tc->GetPFaction(),false,is_crime ? crime_handle : -1);
                        } else {
                            long non_crime_handle = ptc_deed_log->PostDeed(deed,PeekPos(),this,this,OPINION_POST_DEED_TYPE_RECIPIENT,false,false,pmy_enemy_tc->GetPFaction(),false,0);
                            if (ptc_deed_log->GetDeedFromHandle(non_crime_handle))
                                ptc_deed_log->PostDeed(OPINION_DEED_TYPE_WITNESSED_ASSAULT_OR_GBH,PeekPos(),this,this,OPINION_POST_DEED_TYPE_WITNESSES,false,false,pmy_enemy_tc->GetPFaction(),false,old_crime);
                        }
                    }
                }
            }
        }
    }
    CThingCreatureBase::OnHit(hit_params);
    CTCEntityEvents* ptc_entity_events;
    if (IS_THING_ALIVE(phitting_thing) && (phitting_thing->GetTypeFlags() & 8) && phitting_thing->GetTC(TCI_ENTITY_EVENTS,&ptc_entity_events)) {
        bool decapitate = hit_params.IsDecapitate();
        bool used_melee_weapon = CTCCarrying::IsThingCarryingMeleeWeapon(*phitting_thing);
        bool used_ranged_weapon = CTCCarrying::IsThingCarryingRangedWeapon(*phitting_thing);
        ptc_entity_events->AddEvent(new NEntityEvents::CEventHitThing(*phitting_thing,*this,hit_enemy,used_melee_weapon,used_ranged_weapon,hit_params.GetPWeapon(),hit_params.IsFlourish(),decapitate));
    }
}
static void run(unsigned mask,unsigned variant) {
 config=mask;fps=variant*11+1;baseHits=imposterHits=shouts=updates=enemyQueries=threatQueries=meleeQueries=rangedQueries=events=defQueries=searches=handleQueries=0;posts.clear();
 CThingAICreature victim;CThingCreatureBase attacker,previous;CThingGameObject imp;CThing weapon;CTCEnemy enemy;CTCCreatureOpinionOfHero opinion;CTCHeroOpinionDeedLog log;CTCEntityEvents ev;CTCCreatureModeManager modes;
 victim.physics.pos.x=77;victim.physics.rh.yaw=91;imp.physics.pos.x=12;imp.physics.rh.yaw=64;
 victim.tc[TCI_ENTITY_MODE_MANAGER]=bit(0)?&modes:0;modes.mode.imp=(variant%5)?&imp:0;imp.dead=bit(4);imp.flags=bit(5)?4:0;
 victim.hp=bit(8)?0.f:1.f;static const unsigned groups[]={0,0x10000,0x2000000,0x8000,8,0x2018000};victim.base.def.CreatureGroup=groups[variant%6];
 victim.last=(variant%3)?&previous:0;previous.dead=bit(9);attacker.flags=(variant%8)?8:0;attacker.dead=variant%13==0;
 victim.tc[TCI_ENEMY]=bit(13)?&enemy:0;victim.tc[TCI_CREATURE_OPINION_OF_HERO]=bit(15)?&opinion:0;
 attacker.tc[TCI_HERO_OPINION_DEED_LOG]=bit(12)?&log:0;attacker.tc[TCI_ENTITY_EVENTS]=(variant%4)?&ev:0;
 CHitParameters hit;hit.attacker=(variant%17)?&attacker:0;hit.weapon=(variant%3==0)?0:(variant%3==1)?(CThing*)&attacker:&weapon;
 expectedAttacker=hit.attacker;expectedVictim=&victim;expectedWeapon=hit.weapon;
 bool swapped=bit(0)&&bit(1)&&!bit(2)&&!bit(3)&&modes.mode.imp&&!imp.dead;
 bool redirected=swapped&&imp.flags==4;
 bool active=!redirected&&!(bit(0)&&bit(6));bool valid=active&&hit.attacker&&!attacker.dead&&attacker.flags==8;
 bool threatened=victim.last&&!previous.dead;
 bool accepted=valid&&(!threatened||bit(11))&&!(bit(7)&&bit(8)&&(victim.base.def.CreatureGroup&0x2018000));
 bool deed=accepted&&bit(12)&&bit(13)&&bit(14)&&bit(15);bool event=valid&&attacker.tc[TCI_ENTITY_EVENTS]!=0;
 victim.OnHit(hit);
 assert(victim.physics.pos.x==(swapped?12:77)&&victim.physics.rh.yaw==(swapped?64:91));
 assert(imp.physics.pos.x==(swapped?77:12)&&imp.physics.rh.yaw==(swapped?91:64));
 assert(imposterHits==unsigned(redirected)&&baseHits==unsigned(active)&&defQueries==unsigned(valid)&&shouts==unsigned(valid)&&enemyQueries==unsigned(valid));
 assert(threatQueries==unsigned(valid&&threatened)&&updates==unsigned(accepted));
 assert(searches==unsigned(deed)&&handleQueries==unsigned(deed)&&posts.size()==unsigned(deed)*(1u+unsigned(bit(22))));
 if(deed){assert(posts[0].who==1&&posts[0].type==(hit.weapon==hit.attacker?7:8)&&posts[0].crime==(!bit(17)&&bit(16))&&posts[0].extra==0);if(bit(22))assert(posts[1].type==53&&posts[1].who==2&&!posts[1].crime&&posts[1].extra==(bit(17)?39:bit(16)?27:-1));}
 assert(meleeQueries==unsigned(event)&&rangedQueries==unsigned(event)&&events==unsigned(event));
}
int main(){unsigned value=37591243;for(unsigned i=0;i<131072;++i){value=value*1664525U+1013904223U;run(value,i%91);}puts("008329b0_TEST PASS");return 0;}