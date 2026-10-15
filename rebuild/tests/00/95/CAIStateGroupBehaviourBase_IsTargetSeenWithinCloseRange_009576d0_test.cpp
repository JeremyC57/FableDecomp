#include <cstdio>
// Behaviour test for CAIStateGroupBehaviourBase::IsTargetSeenWithinCloseRange (0x009576D0).
struct C3DVector { float X, Y, Z; };
struct CThing { C3DVector pos; const C3DVector* GetPosition() const { return &pos; } };
struct CAIBrain { CThing* Creature; };
struct CTCPerceiveThing { float AwarenessTimer; bool LineOfSight; bool DirectAwareness; bool GetDirectAwareness() { return DirectAwareness; } };
struct CBaseIntelligentPointer { CThing* p; CThing* GetPItem() { return p; } };
struct CAIStateGroupBehaviourBase {
    CAIBrain* PBrain; CBaseIntelligentPointer PTargetThing; CTCPerceiveThing* PTCPerception;
    int IsTargetSeenWithinCloseRange()
    {
        if (PTCPerception->AwarenessTimer != 0.0f && PTCPerception->GetDirectAwareness() && PTCPerception->LineOfSight) {
            CThing* creature = PBrain->Creature;
            const C3DVector* targetPos = PTargetThing.GetPItem()->GetPosition();
            const C3DVector* creaturePos = creature->GetPosition();
            float dx = creaturePos->X - targetPos->X;
            float dy = creaturePos->Y - targetPos->Y;
            float distSq = dx * dx;
            distSq += dy * dy;
            if (distSq < 25.0f)
                return 1;
        }
        return 0;
    }
};
int main()
{
    CThing me = {{0.0f, 0.0f, 0.0f}}, target = {{3.0f, 3.0f, 50.0f}};
    CAIBrain brain = {&me}; CTCPerceiveThing per = {1.0f, true, true};
    CAIStateGroupBehaviourBase g = {&brain, {&target}, &per};
    int ok = 1;
    if (g.IsTargetSeenWithinCloseRange() != 1) ok = 0;          // 3,3 is 4.24 away: close, height ignored
    target.pos.X = 4.0f; target.pos.Y = 3.0f;                   // exactly 5 away: not close
    if (g.IsTargetSeenWithinCloseRange() != 0) ok = 0;
    target.pos.X = 1.0f; per.LineOfSight = false;
    if (g.IsTargetSeenWithinCloseRange() != 0) ok = 0;
    per.LineOfSight = true; per.DirectAwareness = false;
    if (g.IsTargetSeenWithinCloseRange() != 0) ok = 0;
    per.DirectAwareness = true; per.AwarenessTimer = 0.0f;
    if (g.IsTargetSeenWithinCloseRange() != 0) ok = 0;
    std::printf(ok ? "009576d0_TEST PASS\n" : "009576d0_TEST FAIL\n");
    return ok ? 0 : 1;
}