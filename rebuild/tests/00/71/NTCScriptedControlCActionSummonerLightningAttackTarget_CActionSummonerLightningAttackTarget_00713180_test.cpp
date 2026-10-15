// Behaviour test for CActionSummonerLightningAttackTarget::CActionSummonerLightningAttackTarget (0x00713180).
#include <cstdio>

struct CThing;
struct CScriptThing { CThing* PImp_Data; CScriptThing(CThing* t) : PImp_Data(t) {} };
struct C3DVector { float X, Y, Z; };
struct CThing {
    C3DVector pos;
    CScriptThing GetScriptThing() { return CScriptThing(this); }
    C3DVector* GetPosition() { return &pos; }
};

struct CActionBase {
    void* PTCScript; bool Activated; bool Terminated;
    CActionBase() : PTCScript(0), Activated(false), Terminated(false) {}
};

struct CActionTurnToFacePosition : CActionBase {
    float Position_X, Position_Y, Position_Z, Tolerance;
    CActionTurnToFacePosition(const C3DVector& p)
        : CActionBase(), Position_X(p.X), Position_Y(p.Y), Position_Z(p.Z), Tolerance(1.0f / 72.0f) {}
};

struct CActionSummonerLightningAttackTarget : CActionBase {
    CScriptThing PTarget; bool StartedAttack; bool TurnedToFace; CActionTurnToFacePosition TurnToFaceAction;
    CActionSummonerLightningAttackTarget(CThing* target)
        : CActionBase(), PTarget(target->GetScriptThing()), StartedAttack(false), TurnedToFace(false),
          TurnToFaceAction(*target->GetPosition()) {}
};

int main()
{
    CThing t; t.pos.X = 1.0f; t.pos.Y = 2.0f; t.pos.Z = 3.0f;
    CActionSummonerLightningAttackTarget a(&t);
    if (a.PTarget.PImp_Data != &t || a.StartedAttack || a.TurnedToFace || a.TurnToFaceAction.Position_X != 1.0f
        || a.TurnToFaceAction.Position_Y != 2.0f || a.TurnToFaceAction.Position_Z != 3.0f
        || a.TurnToFaceAction.Tolerance != 1.0f / 72.0f || a.TurnToFaceAction.Terminated) {
        std::printf("00713180_TEST FAIL\n");
        return 1;
    }
    std::printf("00713180_TEST PASS\n");
    return 0;
}