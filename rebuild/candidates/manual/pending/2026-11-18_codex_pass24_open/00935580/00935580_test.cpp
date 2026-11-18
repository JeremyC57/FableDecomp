#include <math.h>
#include <stdio.h>
struct C3DVector {
    float X, Y, Z;
    C3DVector(float x = 0, float y = 0, float z = 0) : X(x), Y(y), Z(z) {}
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X-o.X,Y-o.Y,Z-o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X+o.X,Y+o.Y,Z+o.Z); }
    C3DVector& operator-=(const C3DVector& o) { X-=o.X; Y-=o.Y; Z-=o.Z; return *this; }
    C3DVector operator*(float k) const { return C3DVector(X*k,Y*k,Z*k); }
    C3DVector operator^(const C3DVector& o) const { return C3DVector(Y*o.Z-Z*o.Y,Z*o.X-X*o.Z,X*o.Y-Y*o.X); }
    float Dot(const C3DVector& o) const { return X*o.X+Y*o.Y+Z*o.Z; }
    float GetLengthSquared() const { return Dot(*this); }
    void AccurateNormalise()
    {
        float length = (float)sqrt(GetLengthSquared());
        X/=length; Y/=length; Z/=length;
    }
    void Normalise() { AccurateNormalise(); }
};

struct CTCPerceiveThing {
    bool PlayerSees;
    CTCPerceiveThing() : PlayerSees(true) {}
    bool IsPlayerExtendedLineOfSight() const { return PlayerSees; }
};
struct CTCPhysicsBase {
    C3DVector Direction;
    CTCPhysicsBase() : Direction(1,0,0) {}
    C3DVector GetFacingVector() const { return Direction; }
};
struct CThingPlayerCreature {
    bool Dead;
    C3DVector Position;
    CTCPhysicsBase Physics;
    CThingPlayerCreature() : Dead(false) {}
    bool IsDead() const { return Dead; }
    const C3DVector& GetPos() const { return Position; }
    const C3DVector& PeekPos() const { return Position; }
    const CTCPhysicsBase* GetPhysicsTC() const { return &Physics; }
};
struct CPlayer {
    const CThingPlayerCreature* Hero;
    CPlayer() : Hero(0) {}
    const CThingPlayerCreature* PeekPControlledCreature() const { return Hero; }
};
struct CPlayerManager {
    CPlayer Main;
    const CPlayer& GetMainPlayer() const { return Main; }
};
enum { TCI_PERCEIVE_THING = 0x34 };
struct CThingAICreature {
    C3DVector Position;
    CTCPerceiveThing Perception;
    bool HavePerception;
    CThingAICreature() : Position(1,0,0),HavePerception(true) {}
    bool PeekTC(int interface_type, const CTCPerceiveThing** out)
    {
        *out = HavePerception ? &Perception : 0;
        return interface_type == TCI_PERCEIVE_THING;
    }
    const C3DVector& GetPos() const { return Position; }
    const C3DVector& PeekPos() const { return Position; }
};
struct CAIBrain {
    CThingAICreature* Creature;
};
struct CWorld {
    static unsigned long Frame;
    static unsigned long GetFrame() { return Frame; }
};
unsigned long CWorld::Frame = 0;
struct CMainGameComponent {
    static long GetConstantFPS() { return 30; }
};
static bool GFWorld3DDistanceSmallerThan(const C3DVector& a, const C3DVector& b, float boundary)
{
    return (a-b).GetLengthSquared() < boundary*boundary;
}
struct CAIStateGroup_MinionFaceHero {
    CPlayerManager Players;
    CAIBrain Brain;
    CAIBrain* PBrain;
    unsigned long FrameLastLookedAtHero;
    const CThingPlayerCreature* Hero;
    explicit CAIStateGroup_MinionFaceHero(CThingAICreature& creature)
        : PBrain(&Brain),FrameLastLookedAtHero(0),Hero(0) { Brain.Creature = &creature; }
    const CPlayerManager& PeekPlayerManager() const { return Players; }
    bool IsRunnable();
};

bool CAIStateGroup_MinionFaceHero::IsRunnable()
{
    const CThingPlayerCreature* phero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();
    if (phero && !phero->IsDead()) {
        const CTCPerceiveThing* tc_perception;
        bool face_hero = false;
        if (PBrain->Creature->PeekTC(TCI_PERCEIVE_THING, &tc_perception)
                && tc_perception && tc_perception->IsPlayerExtendedLineOfSight()
                && (FrameLastLookedAtHero == 0 || static_cast<unsigned long>(CWorld::GetFrame()) - FrameLastLookedAtHero
                            > static_cast<unsigned long>(CMainGameComponent::GetConstantFPS() * 35))
                && GFWorld3DDistanceSmallerThan(PBrain->Creature->GetPos(), phero->GetPos(), 2.2f)) {
            C3DVector vec_to_face_me = PBrain->Creature->PeekPos() - phero->PeekPos();
            vec_to_face_me.Z = 0.0f;
            vec_to_face_me.Normalise();
            C3DVector hero_facing_vec = phero->GetPhysicsTC()->GetFacingVector();
            hero_facing_vec.Z = 0.0f;
            hero_facing_vec.Normalise();
            face_hero = hero_facing_vec.Dot(vec_to_face_me) > 0.4f;
            if (face_hero) {
                Hero = phero;
                return true;
            }
        }
    }
    return false;
}

int main()
{
    CThingAICreature minion;
    CThingPlayerCreature hero;
    CAIStateGroup_MinionFaceHero state(minion);
    if (state.IsRunnable() || state.Hero)
        return 1;
    state.Players.Main.Hero = &hero;
    hero.Dead = true;
    if (state.IsRunnable() || state.Hero)
        return 2;
    hero.Dead = false;
    minion.Perception.PlayerSees = false;
    if (state.IsRunnable() || state.Hero)
        return 3;
    minion.Perception.PlayerSees = true;
    state.FrameLastLookedAtHero = 20;
    CWorld::Frame = 1070;
    if (state.IsRunnable() || state.Hero)
        return 4;
    CWorld::Frame = 1071;
    minion.Position.X = 3.0f;
    if (state.IsRunnable() || state.Hero)
        return 5;
    minion.Position.X = 1.0f;
    hero.Physics.Direction = C3DVector(-1,0,1);
    if (state.IsRunnable() || state.Hero)
        return 6;
    hero.Physics.Direction = C3DVector(1,0,1);
    if (!state.IsRunnable() || state.Hero != &hero)
        return 7;
    puts("00935580_TEST PASS");
    return 0;
}
