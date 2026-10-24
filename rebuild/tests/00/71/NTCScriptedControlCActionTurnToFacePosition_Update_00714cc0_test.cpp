// Behaviour test for 0x00714CC0 NTCScriptedControl::CActionTurnToFacePosition::Update
#include <cstdio>
struct C3DVector { float X, Y, Z; };
struct CTCLook {
    const C3DVector* lookedAt; long priority; bool inView; float angleSeen;
    void LookAtPos(const C3DVector* p, long pri) { lookedAt = p; priority = pri; }
    bool IsPosInViewRange(const C3DVector&, float angle, bool, bool) { angleSeen = angle; return inView; }
};
struct CThing {
    unsigned long UsingComponents[9]; CTCLook* look;
    bool GetInterface(CTCLook** out, long type)
    {
        if (UsingComponents[type >> 5] & (1UL << (type & 31))) { *out = look; return true; }
        return false;
    }
};
struct CAction {
    CThing* thing; bool Terminated; C3DVector Position; float Angle;
    void Update()
    {
        CTCLook* look;
        if (thing->GetInterface(&look, 0x43)) {
            look->LookAtPos(&Position, 0x21);
            if (look->IsPosInViewRange(Position, Angle, false, false))
                Terminated = true;
        } else
            Terminated = true;
    }
};
int main()
{
    CTCLook look; look.lookedAt = 0; look.priority = 0; look.inView = false; look.angleSeen = 0.0f;
    CThing thing; int i; for (i = 0; i < 9; ++i) thing.UsingComponents[i] = 0;
    thing.UsingComponents[2] = 8; thing.look = &look;
    CAction a; a.thing = &thing; a.Terminated = false; a.Position.X = 1.0f; a.Position.Y = 2.0f; a.Position.Z = 3.0f; a.Angle = 0.5f;
    a.Update();
    int ok = !a.Terminated && look.lookedAt == &a.Position && look.priority == 0x21 && look.angleSeen == 0.5f;
    look.inView = true;
    a.Update();
    ok = ok && a.Terminated;
    a.Terminated = false; thing.UsingComponents[2] = 0; look.lookedAt = 0;
    a.Update();
    ok = ok && a.Terminated && look.lookedAt == 0;
    if (!ok) { std::printf("00714cc0_TEST FAIL\n"); return 1; }
    std::printf("00714cc0_TEST PASS\n");
    return 0;
}