// behaviour test for 0x00891070 CGameScriptInterface::CameraUseCameraPoint(const CScriptThing&, const CScriptThing&, float, long, long) const
#include <cstdio>
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: float m[9]; };
class CTCPhysicsBase { public:
    C3DVector Position; CRightHandedSet RH; int rhCalls;
    const CRightHandedSet& GetRHSet() const { ++const_cast<CTCPhysicsBase*>(this)->rhCalls; return RH; }
    const C3DVector& GetPosition() const { return Position; }
};
class CThing { public:
    CTCPhysicsBase* PhysicsTC; bool Dead;
    bool IsDead() const { return Dead; }
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
};
class CScriptThing { public: CThing* thing; CThing* GetPThing() const { return thing; } };
struct CGSI {
    int calls; const CScriptThing* lastCam; const C3DVector* lastPos; const CRightHandedSet* lastRH; float lastTime; long la, lb;
    void Forward(const CScriptThing& cam, const C3DVector* pos, const CRightHandedSet* rh, float time, long a, long b)
    { ++calls; lastCam = &cam; lastPos = pos; lastRH = rh; lastTime = time; la = a; lb = b; }
    void CameraUseCameraPoint(const CScriptThing& cameraPoint, const CScriptThing& subject, float time, long a, long b)
    {
        CThing* t = subject.GetPThing();
        if (t && !t->IsDead() && t->GetPhysicsTC()) {
            const C3DVector* pos = &t->GetPhysicsTC()->GetPosition();
            const CRightHandedSet* rhSet = &t->GetPhysicsTC()->GetRHSet();
            Forward(cameraPoint, pos, rhSet, time, a, b);
        }
    }
};
int main()
{
    CTCPhysicsBase phys = { {1, 2, 3}, {{0}}, 0 };
    CThing live = { &phys, false }, dead = { &phys, true }, nophys = { 0, false };
    CScriptThing cam = { 0 }, sLive = { &live }, sDead = { &dead }, sNone = { 0 }, sNoPhys = { &nophys };
    CGSI g = { 0 };
    g.CameraUseCameraPoint(cam, sNone, 1.0f, 1, 2);
    g.CameraUseCameraPoint(cam, sDead, 1.0f, 1, 2);
    g.CameraUseCameraPoint(cam, sNoPhys, 1.0f, 1, 2);
    if (g.calls != 0) { std::printf("00891070_TEST FAIL\n"); return 1; }
    g.CameraUseCameraPoint(cam, sLive, 2.5f, 7, 9);
    if (g.calls != 1 || g.lastCam != &cam || g.lastPos != &phys.Position || g.lastRH != &phys.RH || g.lastTime != 2.5f
        || g.la != 7 || g.lb != 9 || phys.rhCalls != 1) { std::printf("00891070_TEST FAIL\n"); return 1; }
    std::printf("00891070_TEST PASS\n");
    return 0;
}