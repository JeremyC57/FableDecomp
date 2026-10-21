// Behaviour test for CAIStateGroup_MeleeCombatNearbyBase::IsEnemyRunningAway (0x0093D3A0): model copy with stub callees.
#include <cstdio>

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
};
static C3DVector operator-(const C3DVector& a, const C3DVector& b) { return C3DVector(a.X - b.X, a.Y - b.Y, a.Z - b.Z); }
static float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }

struct CPhysicsBody {
    C3DVector Position, PrevPosition, Facing;
    const C3DVector& GetPosition() const { return Position; }
    const C3DVector& GetPrevPosition() const { return PrevPosition; }
    C3DVector GetFacing() const { return Facing; }
};
struct CThing;
struct CTCPerceiveThing {
    CThing* perceived; float range;
    CThing* GetPerceivedThing() { return perceived; }
    float GetPerceptionRange() { return range; }
};
struct CTCAIScratchpad { float CombatNearbyEnemyFleeingBreakOffRange; };
struct CThing {
    CPhysicsBody* m_pBody;
    CTCPerceiveThing* perceive;
    CTCAIScratchpad* scratchpad;
    const C3DVector& GetPosition() const { return m_pBody->Position; }
    C3DVector GetFacing() const { return m_pBody->GetFacing(); }
    bool HasTC(long t) const { return t == 0x34 ? perceive != 0 : scratchpad != 0; }
    void GetTC(CTCPerceiveThing** out, long) const { *out = perceive; }
    template<class T> T* GetTC(long) const { return scratchpad; }
};
struct CPointer { CThing* item; CThing* GetPItem() const { return item; } };
struct CAIBrain { CThing* PCreature; };

struct CAIStateGroup_MeleeCombatNearbyBase {
    CAIBrain* PBrain; CPointer PEnemy;
    bool IsEnemyRunningAway();
};

bool CAIStateGroup_MeleeCombatNearbyBase::IsEnemyRunningAway()
{
    const CThing* me = PBrain->PCreature;
    if (me->HasTC(0x34)) {
        CTCPerceiveThing* perceive;
        me->GetTC(&perceive, 0x34);
        const CThing* perceived = perceive->GetPerceivedThing();
        if (PEnemy.GetPItem() == perceived) {
            const CThing* creature = PBrain->PCreature;
            const CThing* enemy = PEnemy.GetPItem();
            C3DVector toEnemy = enemy->GetPosition() - creature->GetPosition();
            if (Dot(PEnemy.GetPItem()->m_pBody->GetPosition() - PEnemy.GetPItem()->m_pBody->GetPrevPosition(), toEnemy) > 0.0f) {
                C3DVector facing = PEnemy.GetPItem()->GetFacing();
                if (Dot(facing, toEnemy) > 0.0f) {
                    float distSq = Dot(toEnemy, toEnemy);
                    const CThing* self = PBrain->PCreature;
                    const CTCAIScratchpad* scratchpad = 0;
                    if (self->HasTC(0xaa))
                        scratchpad = self->GetTC<CTCAIScratchpad>(0xaa);
                    float breakOff = scratchpad->CombatNearbyEnemyFleeingBreakOffRange;
                    bool beyondBreakOff = false;
                    if (distSq > breakOff * breakOff)
                        beyondBreakOff = true;
                    float range = perceive->GetPerceptionRange();
                    if (distSq > range * range || beyondBreakOff)
                        return true;
                }
            }
        }
    }
    return false;
}

int main()
{
    int ok = 1;
    CPhysicsBody meBody = { C3DVector(0, 0, 0), C3DVector(0, 0, 0), C3DVector(1, 0, 0) };
    CPhysicsBody enBody = { C3DVector(10, 0, 0), C3DVector(9, 0, 0), C3DVector(1, 0, 0) };
    CThing enemy = { &enBody, 0, 0 };
    CTCPerceiveThing perceive = { &enemy, 20.0f };
    CTCAIScratchpad pad = { 5.0f };
    CThing me = { &meBody, &perceive, &pad };
    CAIBrain brain = { &me };
    CAIStateGroup_MeleeCombatNearbyBase s; s.PBrain = &brain; s.PEnemy.item = &enemy;
    // moving away, facing away, 10 > 5 break-off: running away
    if (!s.IsEnemyRunningAway()) ok = 0;
    // inside both ranges: not running away
    pad.CombatNearbyEnemyFleeingBreakOffRange = 15.0f;
    if (s.IsEnemyRunningAway()) ok = 0;
    // beyond the perception range alone
    perceive.range = 8.0f;
    if (!s.IsEnemyRunningAway()) ok = 0;
    perceive.range = 20.0f; pad.CombatNearbyEnemyFleeingBreakOffRange = 5.0f;
    // facing towards the creature
    enBody.Facing = C3DVector(-1, 0, 0);
    if (s.IsEnemyRunningAway()) ok = 0;
    enBody.Facing = C3DVector(1, 0, 0);
    // moving towards the creature
    enBody.PrevPosition = C3DVector(11, 0, 0);
    if (s.IsEnemyRunningAway()) ok = 0;
    enBody.PrevPosition = C3DVector(9, 0, 0);
    // perceiving something else
    perceive.perceived = &me;
    if (s.IsEnemyRunningAway()) ok = 0;
    perceive.perceived = &enemy;
    // no perceiver
    me.perceive = 0;
    if (s.IsEnemyRunningAway()) ok = 0;
    me.perceive = &perceive;
    if (!s.IsEnemyRunningAway()) ok = 0;
    if (!ok) { std::printf("0093d3a0_TEST FAIL\n"); return 1; }
    std::printf("0093d3a0_TEST PASS\n");
    return 0;
}
