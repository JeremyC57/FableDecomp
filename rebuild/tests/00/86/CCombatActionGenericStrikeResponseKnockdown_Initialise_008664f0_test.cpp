#include <cstdio>
#include <cstring>
#include <cmath>
#include <string>
#include <vector>
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0): X(x),Y(y),Z(z) {}
 C3DVector operator+(const C3DVector& v)const{return C3DVector(X+v.X,Y+v.Y,Z+v.Z);}
 C3DVector operator-(const C3DVector& v)const{return C3DVector(X-v.X,Y-v.Y,Z-v.Z);}
 C3DVector operator*(float k)const{return C3DVector(X*k,Y*k,Z*k);}
 C3DVector operator-()const{return *this * -1.0f;}
 float Dot(const C3DVector&v)const{return X*v.X+Y*v.Y+Z*v.Z;}C3DVector Cross(const C3DVector&v)const{return C3DVector(Y*v.Z-Z*v.Y,Z*v.X-X*v.Z,X*v.Y-Y*v.X);}
 bool IsNonZero()const{return X!=0||Y!=0||Z!=0;}
 void AccurateNormalise(){float n=(float)sqrt(X*X+Y*Y+Z*Z); if(n){X/=n;Y/=n;Z/=n;}}
};
static float DotProduct(const C3DVector&a,const C3DVector&b){return a.X*b.X+a.Y*b.Y+a.Z*b.Z;}
const float g_Epsilon=0.0001f;
typedef std::string CCharString;
static CCharString CopyString(const CCharString&s){return s;}
struct CDefStringTable{CCharString GetString(int i){return i==17?"fall":"unexpected";}} g_DefStringTable;
namespace NGlobalConsole { bool GCheatModeEnableRemoveProjectileModeWhenHit; }
struct SOrientation{C3DVector forward;SOrientation(float,float,float,const C3DVector&f):forward(f){}};
struct CThingGraphic{C3DVector WeaponTip,WeaponBase,face,applied;int writes; CThingGraphic():face(0,1,0),writes(0){}
 const C3DVector&GetPosition()const{return WeaponTip;}const C3DVector&GetLastPosition()const{return WeaponBase;}C3DVector GetFacingVector()const{return face;}void ResetRHSet(const SOrientation&o){SetOrientation(o);}
 void GetFacing(C3DVector*v){*v=face;}void SetOrientation(const SOrientation&o){applied=o.forward;++writes;}};
struct CThing;
struct CPointer{CThing* p; CPointer():p(0){} CThing*GetPItem(){return p;}CThing*operator->()const{return p;}CThing&operator*()const{return *p;}};
struct CTCHeroRangedAttacks{int cancelled,reset;CTCHeroRangedAttacks():cancelled(0),reset(0){} void CancelAimingWeapon(){++cancelled;}void ResetProjectileWeaponMode(){++reset;}};
struct CPlayer{int removed;CPlayer():removed(0){} void RemoveMode(int id){removed=id;}};
struct CModeStrikeKnockdown{CCharString name;CThing* owner;CModeStrikeKnockdown(const CCharString&n,CThing*t):name(n),owner(t){}CModeStrikeKnockdown(CThing&t,const CCharString&n):name(n),owner(&t){}};
struct CModePointer{CModeStrikeKnockdown* p;CModePointer(CModeStrikeKnockdown*v):p(v){}};
template<class T>struct CCountedPointer{T*p;CCountedPointer(T*x):p(x){}};
struct CTCCreatureModeManager{typedef CModeStrikeKnockdown CModeBase; void AddMode(CCountedPointer<CModeBase> mode){AddMode(CModePointer(mode.p));}int type;std::string added;std::vector<int> removed;CTCCreatureModeManager():type(0){} CTCCreatureModeManager& GetCurrentMode(){return *this;}int GetType(){return type;}
 void AddMode(CModePointer m){added=m.p->name;delete m.p;}void RemoveMode(int id){removed.push_back(id);}};
struct CTCHitOnCollision{bool Swinging;bool IsThrowThingsOutLaterally()const{return Swinging;}};typedef CTCHitOnCollision CWeaponInterface;
struct CThing{int Size,Flags91;C3DVector pos;CThingGraphic* Graphic;CTCHeroRangedAttacks*ranged;CTCCreatureModeManager*modes;CPlayer player;bool reorient;CWeaponInterface* weapon;
 CThing():Size(0),Flags91(0),Graphic(0),ranged(0),modes(0),reorient(false),weapon(0){}
 void SetBodyReorienterActive(bool a){reorient=a;}bool GetInterface(CTCHeroRangedAttacks**p,int){*p=ranged;return ranged!=0;}
 bool GetInterface(CTCCreatureModeManager**p,int){*p=modes;return modes!=0;}CPlayer&GetPlayer(){return player;}CThingGraphic*GetPhysicsTC(){return Graphic;}int GetType()const{return Size;}bool PeekTC(int,const CTCHitOnCollision**p){*p=weapon;return weapon!=0;}
 template<class T>bool GetTC(int x,T**p){return GetInterface(p,x);}
 bool GetWeapon(CWeaponInterface**p){*p=weapon;return weapon!=0;}const C3DVector&GetPos(){return pos;}};
struct CModeDef{int NameId;};struct CRef{CModeDef*Data;};struct CCombatActionDef{CRef mode;CRef GetModeDef(){return mode;}};
extern int animation_filter_reads; struct CAnimComponentFollowingOnAnimFilter{CCharString GetNextAnimFilter()const{++animation_filter_reads;return "fall";}};struct CAnimationEntry{CAnimComponentFollowingOnAnimFilter filter;void GetComponent(const CAnimComponentFollowingOnAnimFilter**p){*p=&filter;}};
struct CCombatActionBase{CAnimationEntry entry,*PAnimEntry;CPointer PCreature;CCombatActionDef*Def;std::string anim;int starts; CCombatActionBase():PAnimEntry(&entry),Def(0),starts(0){}void SetCombatAnimation(const CCharString&s){anim=s;}void Initialise(){++starts;}};
struct CCombatAction_GenericStrikeResponseKnockdown: CCombatActionBase{bool FlipFacing;CCharString AnimName;CPointer PThingThatHit;C3DVector PointOfHit,DirectionOfHit;void Initialise();};
namespace NCreatureMode{typedef CModeStrikeKnockdown CModeKnockedDown;}
struct CRightHandedSet:SOrientation{CRightHandedSet(const C3DVector&up,const C3DVector&dir):SOrientation(up.X,up.Y,up.Z,dir){}};
typedef int EPlayerMode;enum {TCI_HERO_RANGED_ATTACKS=0x81,TCI_ENTITY_MODE_MANAGER=0x31,TCI_HIT_ON_COLLISION=0xc8};
static double GFCos(float angle){return cos(angle*(double)6.2831855f);}
static bool IS_THING_ALIVE(const CPointer&p){return p.p&&!(p.p->Flags91&1);}

int animation_filter_reads=0;

void CCombatAction_GenericStrikeResponseKnockdown::Initialise()
{
    FlipFacing = false;
    if (AnimName == "")
        PCreature->SetBodyReorienterActive(true);
    CTCHeroRangedAttacks* ptc_ranged_attacks;
    if (PCreature->GetTC(TCI_HERO_RANGED_ATTACKS,&ptc_ranged_attacks)) {
        ptc_ranged_attacks->CancelAimingWeapon();
        if (NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit)
            ptc_ranged_attacks->ResetProjectileWeaponMode();
        PCreature->GetPlayer().RemoveMode((EPlayerMode)0x22);
    }
    C3DVector new_facing_dir = PCreature->GetPhysicsTC()->GetFacingVector();
    if (AnimName != "") {
        SetCombatAnimation(AnimName);
    } else {
        C3DVector point_of_hit = PointOfHit;
        if (IS_THING_ALIVE(PThingThatHit)) {
            point_of_hit = PThingThatHit->GetPos();
            const CTCHitOnCollision* ptc_hit_on_coll;
            if (PThingThatHit->PeekTC(TCI_HIT_ON_COLLISION,&ptc_hit_on_coll) && ptc_hit_on_coll->IsThrowThingsOutLaterally()) {
                C3DVector velocity = PThingThatHit->GetPhysicsTC()->GetPosition() - PThingThatHit->GetPhysicsTC()->GetLastPosition();
                if (velocity.IsNonZero()) {
                    C3DVector diff = PThingThatHit->GetPos() - PCreature->GetPos();
                    float lambda_param = diff.Dot(velocity) / velocity.Dot(velocity);
                    point_of_hit = PThingThatHit->GetPos() + velocity * lambda_param;
                }
            }
        }
        C3DVector line_2d = point_of_hit - PCreature->GetPos();
        line_2d.Z=0.0f;
        if (line_2d.IsNonZero())
            line_2d.AccurateNormalise();
        C3DVector hit_dir_2d(DirectionOfHit);
        hit_dir_2d.Z=0.0f;
        if (hit_dir_2d.IsNonZero())
            hit_dir_2d.AccurateNormalise();
        C3DVector facing_dir_2d = PCreature->GetPhysicsTC()->GetFacingVector();
        facing_dir_2d.Z = 0.0f;
        if (facing_dir_2d.IsNonZero())
            facing_dir_2d.AccurateNormalise();
        new_facing_dir = facing_dir_2d;
        float cos_thet = line_2d.Dot(facing_dir_2d);
        if (cos_thet < GFCos(1.0f/3.0f)) {
            SetCombatAnimation("RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN");
            if (PCreature->GetType() == 1)
                FlipFacing = true;
            if (IS_THING_ALIVE(PThingThatHit)) {
                C3DVector line_to_target = point_of_hit - PCreature->GetPos();
                line_to_target.Z = 0.0f;
                if (line_to_target.IsNonZero()) {
                    line_to_target.AccurateNormalise();
                    new_facing_dir = -line_to_target;
                } else
                    new_facing_dir = C3DVector(0.0f, -1.0f, 0.0f);
            }
        } else {
            if (IS_THING_ALIVE(PThingThatHit)) {
                C3DVector line_to_target = point_of_hit - PCreature->GetPos();
                line_to_target.Z = 0.0f;
                if (line_to_target.IsNonZero()) {
                    line_to_target.AccurateNormalise();
                    new_facing_dir = line_to_target;
                } else
                    new_facing_dir = C3DVector(0.0f, 1.0f, 0.0f);
            }
            C3DVector facing_vector = PCreature->GetPhysicsTC()->GetFacingVector();
            C3DVector kinda_up = facing_vector.Cross(hit_dir_2d);
            SetCombatAnimation(kinda_up.Z > 0.0f
                                             ? "RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN"
                                             : "RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN");
        }
    }
    CCombatActionBase::Initialise();
    CTCCreatureModeManager* ptc_mode_manager;
    PCreature->GetTC(TCI_ENTITY_MODE_MANAGER,&ptc_mode_manager);
    if (ptc_mode_manager->GetCurrentMode().GetType() != 0x1d)
        PCreature->GetPhysicsTC()->ResetRHSet(CRightHandedSet(C3DVector(0,0,1),new_facing_dir));
    const CAnimComponentFollowingOnAnimFilter* pfilt_comp;
    PAnimEntry->GetComponent(&pfilt_comp);
    CCharString val(pfilt_comp->GetNextAnimFilter());
    ptc_mode_manager->AddMode(CCountedPointer<CTCCreatureModeManager::CModeBase>(new NCreatureMode::CModeKnockedDown(*PCreature,val)));
    ptc_mode_manager->RemoveMode(0x42);
    ptc_mode_manager->RemoveMode(0xd);
    ptc_mode_manager->RemoveMode(0x36);
}

int main(){
 for(int sample=0;sample<192;++sample){
  CCombatAction_GenericStrikeResponseKnockdown action;CThing owner,attacker;CThingGraphic g,h;CTCHeroRangedAttacks ranged;CTCCreatureModeManager modes;CWeaponInterface weapon;CModeDef mode={17};CCombatActionDef def;def.mode.Data=&mode;
  action.PCreature.p=&owner;action.Def=&def;owner.Graphic=&g;owner.modes=&modes;owner.Size=sample%2;owner.ranged=sample%3?&ranged:0;NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit=(sample%4<2);modes.type=(sample%5)?0:0x1d;
  if(sample%8==1)action.AnimName="override";
  int hit=sample%6;attacker.Graphic=&h;attacker.Flags91=(hit==1);attacker.pos=C3DVector(0,hit==5?0:(sample%2?1:-1),0);
  weapon.Swinging=(hit>=3);attacker.weapon=hit>=2?&weapon:0;h.WeaponTip=C3DVector(hit==4?1:0,0,0);if(hit)action.PThingThatHit.p=&attacker;
  action.PointOfHit=C3DVector(0,-1,0);action.DirectionOfHit=C3DVector((sample%3)-1,0,0);
  int reads=animation_filter_reads; action.Initialise(); if(animation_filter_reads!=reads+1)return 6;
  if(action.starts!=1 || modes.added!="fall" || modes.removed.size()!=3 || modes.removed[0]!=0x42 || modes.removed[1]!=13 || modes.removed[2]!=0x36)return 1;
  if(g.writes!=(modes.type!=0x1d) || ranged.cancelled!=(owner.ranged!=0) || ranged.reset!=(owner.ranged&&NGlobalConsole::GCheatModeEnableRemoveProjectileModeWhenHit) || owner.player.removed!=(owner.ranged?0x22:0))return 2;
  if(!action.AnimName.empty()){if(owner.reorient||action.FlipFacing||action.anim!="override")return 3;continue;}
  bool alive=hit>=2,back=alive?attacker.pos.Y<0:true;
  const char* expected=back?"RESPONSE_GENERIC_HIT_BACK_KNOCKDOWN":(action.DirectionOfHit.X<0?"RESPONSE_GENERIC_HIT_LEFT_KNOCKDOWN":"RESPONSE_GENERIC_HIT_RIGHT_KNOCKDOWN");
  if(!owner.reorient || action.anim!=expected || action.FlipFacing!=(back&&owner.Size==1))return 4;
  if(g.writes && (g.applied.X!=0 || g.applied.Y!=1 || g.applied.Z!=0))return 5;
 }
 puts("008664f0_TEST PASS");return 0;
}