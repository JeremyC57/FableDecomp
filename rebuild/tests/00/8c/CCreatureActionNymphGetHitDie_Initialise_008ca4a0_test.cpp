// Behaviour test for 0x008CA4A0 CCreatureAction_NymphGetHitDie::Initialise: animation, the three physics calls in order,
// StopFlyingAround(-1) only with a nymph component, HeightOnInit = Z - ground height, then the base Initialise.
#include <cstdio>
#include <cstring>
class C3DVector { public: float X, Y, Z; };
struct CPhysics { int log[3]; int n; void SetMovementType(long t) { log[n++] = (int)t; } void SetCollisionType(long t) { log[n++] = 10 + (int)t; } void ResetMovement() { log[n++] = 99; } };
class CTCNymph { public: long stopped; void StopFlyingAround(long m) { stopped = m; } };
class CThing { public:
    CPhysics physics; C3DVector pos; bool isNymph; CTCNymph nymph;
    CPhysics* GetPhysics() { return &physics; }
    const C3DVector& GetPosition() const { return pos; }
    bool GetNymph(CTCNymph** out) { if (!isNymph) return false; *out = &nymph; return true; }
};
class CWorldMap { public: float ground; float GetHeightZBelowPos(const C3DVector*) { return ground; } };
struct CPtr { CThing* item; CThing* GetPItem() const { return item; } };
struct CAction {
    CPtr PCreature; CWorldMap* map; char anim[16]; float HeightOnInit; int baseInit;
    CWorldMap* GetWorldMap() { return map; }
    void SetCombatAnimation(const char* s) { std::strcpy(anim, s); }
    void Initialise()
    {
        SetCombatAnimation("FALL_DEAD");
        PCreature.GetPItem()->GetPhysics()->SetMovementType(1);
        PCreature.GetPItem()->GetPhysics()->SetCollisionType(2);
        PCreature.GetPItem()->GetPhysics()->ResetMovement();
        CTCNymph* nymph;
        if (PCreature.GetPItem()->GetNymph(&nymph))
            nymph->StopFlyingAround(-1);
        C3DVector position = PCreature.GetPItem()->GetPosition();
        HeightOnInit = position.Z - GetWorldMap()->GetHeightZBelowPos(&position);
        ++baseInit;
    }
};
int main()
{
    CThing n; n.physics.n = 0; n.pos.X = 1; n.pos.Y = 2; n.pos.Z = 7.5f; n.isNymph = true; n.nymph.stopped = 0;
    CWorldMap m; m.ground = 2.0f;
    CAction a; a.PCreature.item = &n; a.map = &m; a.baseInit = 0;
    a.Initialise();
    int ok = std::strcmp(a.anim, "FALL_DEAD") == 0 && n.physics.n == 3 && n.physics.log[0] == 1 && n.physics.log[1] == 12
          && n.physics.log[2] == 99 && n.nymph.stopped == -1 && a.HeightOnInit == 5.5f && a.baseInit == 1;
    n.isNymph = false; n.nymph.stopped = 0; n.physics.n = 0;
    a.Initialise();
    ok = ok && n.nymph.stopped == 0;
    if (!ok) { std::printf("008ca4a0_TEST FAIL\n"); return 1; }
    std::printf("008ca4a0_TEST PASS\n");
    return 0;
}