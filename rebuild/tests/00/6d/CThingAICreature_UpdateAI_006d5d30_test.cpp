#include <stdio.h>
#include <stdlib.h>
#define FABLE_FASTCALL
namespace fixture {
void check(bool v){if(!v) abort();}
const float DELTA_NOTIONAL_ZERO = 0.0001f;
struct C3DVector {
 float X,Y,Z;
 C3DVector(float x=0,float y=0,float z=0):X(x),Y(y),Z(z){}
 C3DVector operator+(const C3DVector& p)const{return C3DVector(X+p.X,Y+p.Y,Z+p.Z);}
 C3DVector operator-(const C3DVector& p)const{return C3DVector(X-p.X,Y-p.Y,Z-p.Z);}
 bool IsNonZero()const{return Y*Y+X*X+Z*Z>DELTA_NOTIONAL_ZERO*DELTA_NOTIONAL_ZERO;}
 bool equals(const C3DVector& p)const{return X==p.X&&Y==p.Y&&Z==p.Z;}
};
namespace NGlobalConsole {bool EnableUpdateAI;}
struct CTCLook{};
struct CCombatActionBase{};
struct CBrainOutput {
 unsigned types; C3DVector point; int nav,action;
 CBrainOutput(unsigned t=0):types(t),point(9,-6,3),nav(17),action(25){}
 bool IsPumpCombatActions()const{return (types&32)!=0;}
 bool IsMoveToPos()const{return (types&1)!=0;}
 bool IsPerformAction()const{return (types&8)!=0;}
 const C3DVector& PeekMoveToPos()const{return point;}
 int PeekNavigationInput()const{return nav;}
 int GetAction()const{return action;}
};
enum { NAV_REACHED_TARGET=0, NAV_CANNOT_REACH_TARGET=1, NAV_MOVING_TO_TARGET=2 };
struct CTCCreatureNavigation {
 bool target;int route,updated;struct CCreatureNavOutput{int NavResult;C3DVector MovementVector;}out;
 CTCCreatureNavigation():target(false),route(0),updated(0){}
 void SetGotNavigationTarget(bool b){target=b;} bool HasGotNavigationTarget()const{return target;}
 void SetNavigationTarget(int n){check(n==17);++updated;}
 CCreatureNavOutput GetNextNavPosOnRoute(int n,bool a,bool b,bool c,bool d,bool e){check(n==17&&!a&&b&&c&&!d&&!e);++route;return out;}
};
struct Brain {bool updated;bool HasStateGroupUpdated()const{return updated;}};
struct OutputPtr {
 CBrainOutput* p;OutputPtr():p(0){}~OutputPtr(){delete p;}
 CBrainOutput* Get()const{return p;}CBrainOutput* operator->()const{return p;}CBrainOutput& operator*()const{return *p;}
 void Reset(CBrainOutput* n){delete p;p=n;}
};
struct CThingAICreature {
 OutputPtr PCurrentBrainOutput;Brain br;Brain* PBrain;CTCCreatureNavigation nav;CTCLook look;CCombatActionBase combat;
 C3DVector position,peek_position,turn;bool melee,still_melee,can_plug,has_action,busy,facing,plugged;
 int polls,requests,attempts,combat_calls,contract_calls,turns,lookups,face_calls;
 CThingAICreature():PBrain(&br),position(1,-2,3),peek_position(4,-5,7),polls(0),requests(0),attempts(0),combat_calls(0),contract_calls(0),turns(0),lookups(0),face_calls(0){}
 void GetTC(CTCCreatureNavigation** p){*p=&nav;++lookups;}void GetTC(CTCLook** p){*p=&look;++lookups;}
 bool IsInMeleeAsAttacker(){return ++polls==1?melee:still_melee;}
 CThingAICreature& GetCombatManager(){return *this;}
 CCombatActionBase* GetNextActionForCreature(CThingAICreature& x){check(&x==this);++requests;return has_action?&combat:0;}
 void SetCreatureAsAttemptedToPerformedAction(CThingAICreature& x,bool b){check(&x==this);++attempts;plugged=b;}
 bool IsPerformingAnyAction()const{return busy;}
 bool SetCurrentAction(CCombatActionBase& x){check(&x==&combat);++combat_calls;return can_plug;}
 void SetCurrentAction(int n){check(n==25);++contract_calls;}
 const C3DVector& GetPos()const{return position;}const C3DVector& PeekPos()const{return peek_position;}
 void UpdateAITurning(const C3DVector& v){turn=v;++turns;}
 bool UpdateAI(bool,const CBrainOutput&,C3DVector&);
};
bool IsFacingCorrectDirectionForMovementToPos(CThingAICreature& c,const CBrainOutput&,const CTCLook& look,const C3DVector& point){
 check(&look==&c.look);++c.face_calls;
 check(point.equals(C3DVector(9,-6,3))||point.equals(C3DVector(-2,0,8)));return c.facing;
}
bool CThingAICreature::UpdateAI(bool brain_updated,const CBrainOutput& brain_output,C3DVector& to_pos)
{
    bool has_ai_pos=false;
    if (NGlobalConsole::EnableUpdateAI) {
        CTCCreatureNavigation* pnav;
        GetTC(&pnav);
        CTCLook* ptc_look;
        GetTC(&ptc_look);
        C3DVector movement_vector(0,0,0);
        if(IsInMeleeAsAttacker() && ((brain_updated && brain_output.IsPumpCombatActions()) ||
           (!brain_updated && PCurrentBrainOutput.Get() && PCurrentBrainOutput->IsPumpCombatActions()))) {
            CCombatActionBase* action=GetCombatManager().GetNextActionForCreature(*this);
            if(IsInMeleeAsAttacker()) {
                bool action_plugged_in=false;
                if(action && !IsPerformingAnyAction() && SetCurrentAction(*action)) action_plugged_in=true;
                GetCombatManager().SetCreatureAsAttemptedToPerformedAction(*this,action_plugged_in);
            }
        }
        if(brain_updated) {
            PCurrentBrainOutput.Reset(new CBrainOutput(brain_output));
            pnav->SetGotNavigationTarget(false);
            if(brain_output.IsMoveToPos()) {
                C3DVector move_to_pos(brain_output.PeekMoveToPos());
                movement_vector = move_to_pos - GetPos();
                if(IsFacingCorrectDirectionForMovementToPos(*this,brain_output,*ptc_look,move_to_pos)) {
                    has_ai_pos=true;
                    to_pos=move_to_pos;
                    pnav->SetGotNavigationTarget(true);
                    pnav->SetNavigationTarget(brain_output.PeekNavigationInput());
                }
            }
            UpdateAITurning(movement_vector);
            if(brain_output.IsPerformAction()) SetCurrentAction(brain_output.GetAction());
        } else if(PCurrentBrainOutput.Get() && !PBrain->HasStateGroupUpdated()) {
            if(PCurrentBrainOutput->IsMoveToPos() && pnav->HasGotNavigationTarget()) {
                CTCCreatureNavigation::CCreatureNavOutput nav_out=pnav->GetNextNavPosOnRoute(PCurrentBrainOutput->PeekNavigationInput(),false,true,true,false,false);
                if(nav_out.NavResult==NAV_MOVING_TO_TARGET) {
                    if(nav_out.MovementVector.IsNonZero()) {
                        if(IsFacingCorrectDirectionForMovementToPos(*this,*PCurrentBrainOutput,*ptc_look,GetPos()+nav_out.MovementVector)) {
                            has_ai_pos=true;
                            to_pos=PeekPos()+nav_out.MovementVector;
                        }
                        movement_vector=nav_out.MovementVector;
                    }
                } else pnav->SetGotNavigationTarget(false);
            }
            UpdateAITurning(movement_vector);
        }
    }
    return has_ai_pos;
}

}
static void TestUpdateAI() {
 using namespace fixture;
 for (unsigned profile=0; profile<3; ++profile) {
 for(unsigned mask=0;mask<65536;++mask) {
  CThingAICreature c;bool enabled=(mask&1)!=0;bool fresh=(mask&2)!=0,prior=(mask&4)!=0,move=(mask&8)!=0,pump=(mask&16)!=0,perform=(mask&32)!=0;
  c.br.updated=(mask&64)!=0;c.nav.target=(mask&128)!=0;c.nav.out.NavResult=(mask&256)?NAV_MOVING_TO_TARGET:(profile?NAV_CANNOT_REACH_TARGET:NAV_REACHED_TARGET);
  c.nav.out.MovementVector=(mask&512)?C3DVector(-3,2,5):(profile==2?C3DVector(0,0.00005f,0):C3DVector());c.facing=(mask&1024)!=0;c.melee=(mask&2048)!=0;
  c.still_melee=(mask&4096)!=0;c.has_action=(mask&8192)!=0;c.busy=(mask&16384)!=0;c.can_plug=(mask&32768)!=0;
  CBrainOutput out((move?1:0)|(pump?32:0)|(perform?8:0));
  const bool previous_move = profile == 1 ? !move : move;
  const bool previous_pump = profile == 1 ? !pump : pump;
  if(prior)c.PCurrentBrainOutput.Reset(new CBrainOutput((previous_move?1:0)|(previous_pump?32:0)));
  C3DVector dest(99,88,77),expected=dest;NGlobalConsole::EnableUpdateAI=enabled;
  bool had_target=c.nav.target,active=enabled&&(fresh||(prior&&!c.br.updated)),route=active&&!fresh&&previous_move&&had_target;
  bool movement=fresh?enabled&&move:route&&c.nav.out.NavResult==NAV_MOVING_TO_TARGET&&c.nav.out.MovementVector.IsNonZero();
  bool result=movement&&c.facing;
  if(result)expected=fresh?out.point:c.peek_position+c.nav.out.MovementVector;
  bool before_pump=enabled&&c.melee&&(fresh?pump:(prior&&previous_pump));
  check(c.UpdateAI(fresh,out,dest)==result);check(dest.equals(expected));
  check(c.lookups==(enabled?2:0)&&c.requests==(before_pump?1:0)&&c.attempts==(before_pump&&c.still_melee?1:0));
  bool attempt=before_pump&&c.still_melee&&c.has_action&&!c.busy;
  check(c.combat_calls==(attempt?1:0));if(c.attempts)check(c.plugged==(attempt&&c.can_plug));
  check(c.contract_calls==(fresh&&enabled&&perform?1:0)&&c.turns==(active?1:0)&&c.nav.route==(route?1:0));
  check(c.face_calls==(movement?1:0));check(c.nav.updated==(enabled&&fresh&&result?1:0));
  check(c.nav.target==(enabled&&fresh?result:route&&c.nav.out.NavResult!=NAV_MOVING_TO_TARGET?false:had_target));
  check((c.PCurrentBrainOutput.Get()!=0)==(prior||(enabled&&fresh)));
  if(active){C3DVector v=movement?(fresh?out.point-c.position:c.nav.out.MovementVector):C3DVector();check(c.turn.equals(v));}
 }
 }
}

namespace turning_fixture {
const float DELTA_NOTIONAL_ZERO = 0.0001f;
using fixture::check;

struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    float GetSquaredMagnitude() const { return (X * X + Y * Y + Z * Z); }
    C3DVector operator+(const C3DVector& rhs) const
    {
        return C3DVector(X + rhs.X, Y + rhs.Y, Z + rhs.Z);
    }
    bool Equals(const C3DVector& rhs) const { return X == rhs.X && Y == rhs.Y && Z == rhs.Z; }
};
struct CTCGraphicAppearance {
    mutable int static_calls, animated_calls;
    CTCGraphicAppearance() : static_calls(0), animated_calls(0) {}
    C3DVector GetStaticFocalPos() const
    {
        ++static_calls;
        return C3DVector(18, 22, 30);
    }
    C3DVector GetFocalPos(float phase) const
    {
        check(phase == 1.0f);
        ++animated_calls;
        return C3DVector(28, 32, 40);
    }
};
struct CThing {
    bool dead;
    C3DVector pos;
    const CTCGraphicAppearance* appearance;
    CThing() : dead(false), pos(38, 42, 50), appearance(0) {}
    bool IsDead() const { return dead; }
    const C3DVector& GetPos() const { return pos; }
    const CTCGraphicAppearance* GetTCGraphicAppearance() const { return appearance; }
};
struct CWitnessThingPointer {
    const CThing* item;
    const CThing* Get() const { return item; }
};
struct CBrainOutput {
    unsigned long Types;
    long LookFlags;
    C3DVector LookAtPos;
    CWitnessThingPointer LookAtThing;
    CBrainOutput() : Types(0), LookFlags(-37), LookAtPos(8, 12, 20) { LookAtThing.item = 0; }
    bool IsLookAtPos() const;
    bool IsLookAtThing() const;
    bool IsFaceMovementDirection() const;
    const C3DVector& PeekLookAtPos() const;
    const CThing* PeekLookAtThing() const;
    long GetLookFlags() const;
};
class CTCLook {
public:
    int calls;
    long flags;
    C3DVector target;
    CTCLook() : calls(0), flags(0) {}
    void LookAtPos(const C3DVector& position, long flags_seen)
    {
        ++calls;
        target = position;
        flags = flags_seen;
    }
};
enum { CREATURE_ANIM_TURNING = 7 };
struct CTCTestAnimation {
    int type;
    int GetMainAnimType() const { return type; }
};
struct CTCAnimationComplex {
    CTCTestAnimation animation;
    const CTCTestAnimation& PeekAnimation() const { return animation; }
};
class CThingAICreature {
public:
    const CBrainOutput* PCurrentBrainOutput;
    C3DVector pos;
    bool melee;
    CTCLook look;
    CTCAnimationComplex animation;
    int lookups;
    mutable int animation_queries;
    CThingAICreature() : PCurrentBrainOutput(0), pos(0, 1, 2), melee(false), lookups(0), animation_queries(0)
    {
        animation.animation.type = 0;
    }
    bool IsInMelee() const { return melee; }
    const C3DVector& GetPos() const { return pos; }
    bool GetTC(CTCLook** p)
    {
        ++lookups;
        *p = &look;
        return true;
    }
    bool PeekTC(const CTCAnimationComplex** p) const
    {
        ++animation_queries;
        *p = &animation;
        return true;
    }
    void UpdateAITurning(const C3DVector& desired_movement_vector);
};
template<class T> bool IS_THING_ALIVE(const T& thing) { return thing != 0 && !thing->IsDead(); }

inline bool CBrainOutput::IsLookAtPos() const { return (Types & 2) != 0; }
inline bool CBrainOutput::IsLookAtThing() const { return (Types & 4) != 0; }
inline bool CBrainOutput::IsFaceMovementDirection() const { return (Types & 16) != 0; }
inline const C3DVector& CBrainOutput::PeekLookAtPos() const { return LookAtPos; }
inline const CThing* CBrainOutput::PeekLookAtThing() const { return LookAtThing.Get(); }
inline long CBrainOutput::GetLookFlags() const { return LookFlags; }

// Strong same-TU owner at 0x006D5B50. The caller sees that the movement reference does not escape.
void CThingAICreature::UpdateAITurning(const C3DVector& desired_movement_vector)
{
    C3DVector look_at_pos;
    bool is_to_look = false;
    if (PCurrentBrainOutput->IsLookAtPos()) {
        is_to_look = true;
        look_at_pos = PCurrentBrainOutput->PeekLookAtPos();
    } else if (PCurrentBrainOutput->IsLookAtThing() &&
               IS_THING_ALIVE(PCurrentBrainOutput->PeekLookAtThing())) {
        is_to_look = true;
        if (PCurrentBrainOutput->PeekLookAtThing()->GetTCGraphicAppearance()) {
            if (IsInMelee()) {
                look_at_pos = PCurrentBrainOutput->PeekLookAtThing()->GetTCGraphicAppearance()->GetStaticFocalPos();
            } else {
                look_at_pos = PCurrentBrainOutput->PeekLookAtThing()->GetTCGraphicAppearance()->GetFocalPos(1.0f);
            }
        } else {
            look_at_pos = PCurrentBrainOutput->PeekLookAtThing()->GetPos();
        }
    } else if (PCurrentBrainOutput->IsFaceMovementDirection() &&
               desired_movement_vector.GetSquaredMagnitude() > DELTA_NOTIONAL_ZERO) {
        is_to_look = true;
        look_at_pos = GetPos() + desired_movement_vector;
    }
    if (is_to_look) {
        CTCLook* ptc_look;
        GetTC(&ptc_look);
        ptc_look->LookAtPos(look_at_pos, PCurrentBrainOutput->GetLookFlags());
    }
}

bool FABLE_FASTCALL IsFacingCorrectDirectionForMovementToPos(const CThingAICreature& creature,const CBrainOutput& output,const CTCLook& tc_look,const C3DVector& pos) {
    const CTCAnimationComplex* ptc_anim;
    creature.PeekTC(&ptc_anim);
    if(ptc_anim->PeekAnimation().GetMainAnimType()==CREATURE_ANIM_TURNING)return false;
    return true;
}

static void TestTurning()
{
    const C3DVector movement[] = {
        C3DVector(), C3DVector(0.003f, -0.004f, 0), C3DVector(0.007f, 0.008f, 0),
        C3DVector(0.0f, 0.0f, -0.02f), C3DVector(-3.0f, 4.0f, 0.5f)
    };
    unsigned cases = 0;
    for (unsigned flags = 0; flags < 8; ++flags) {
        for (unsigned life = 0; life < 3; ++life) {
            for (int has_graphics = 0; has_graphics < 2; ++has_graphics) {
                for (int in_melee = 0; in_melee < 2; ++in_melee) {
                    for (unsigned i = 0; i < sizeof(movement) / sizeof(movement[0]); ++i) {
                        for (int is_turning = 0; is_turning < 2; ++is_turning) {
                            CBrainOutput out;
                            CThing target;
                            CTCGraphicAppearance appearance;
                            CThingAICreature creature;
                            out.Types = ((flags & 1) ? 2UL : 0UL) |
                                        ((flags & 2) ? 4UL : 0UL) |
                                        ((flags & 4) ? 16UL : 0UL);
                            out.LookAtThing.item = life ? &target : 0;
                            out.LookFlags = in_melee ? -37 : 129;
                            target.dead = life == 1;
                            target.appearance = has_graphics ? &appearance : 0;
                            creature.PCurrentBrainOutput = &out;
                            creature.melee = in_melee != 0;
                            creature.animation.animation.type = is_turning ? CREATURE_ANIM_TURNING : 6;
                            const C3DVector unchanged = movement[i];
                            creature.UpdateAITurning(movement[i]);
                            check(movement[i].Equals(unchanged));
                            check(IsFacingCorrectDirectionForMovementToPos(creature, out, creature.look,
                                                                          movement[i]) == (is_turning == 0));
                            check(creature.animation_queries == 1);
                            const bool fixed_point = (flags & 1) != 0;
                            const bool faces_thing = !fixed_point && (flags & 2) && life == 2;
                            const bool faces_movement = !fixed_point && !faces_thing && (flags & 4) && i >= 2;
                            const bool should_look = fixed_point || faces_thing || faces_movement;
                            check(creature.look.calls == should_look && creature.lookups == should_look);
                            check(appearance.static_calls == (faces_thing && has_graphics && in_melee));
                            check(appearance.animated_calls == (faces_thing && has_graphics && !in_melee));
                            if (should_look) {
                                C3DVector expected;
                                if (fixed_point)
                                    expected = out.LookAtPos;
                                else if (!faces_thing)
                                    expected = creature.pos + movement[i];
                                else if (!has_graphics)
                                    expected = target.pos;
                                else if (in_melee)
                                    expected = C3DVector(18, 22, 30);
                                else
                                    expected = C3DVector(28, 32, 40);
                                check(creature.look.target.Equals(expected));
                                check(creature.look.flags == out.LookFlags);
                            }
                            ++cases;
                        }
                    }
                }
            }
        }
    }
    check(cases == 960);
}
}

int main()
{
    TestUpdateAI();
    turning_fixture::TestTurning();
    puts("006d5d30_TEST PASS");
    return 0;
}