#include <cstdio>
// Behaviour model of the CTCParamsPhysicsBase hand-off (CTCDCameraPoint::Construct, 0x00778D60).
struct V { float X, Y, Z; };
struct Params { V Position; V Up; V Forward; };
struct Physics { Params last; int calls; void ConstructFromParams(const Params* p) { last = *p; ++calls; } };
struct Thing { Physics* phys; };
struct Def {
    Thing* owner;
    void Construct(const V& pos)
    {
        Physics* physics = owner->phys;
        Params p; p.Position = pos; p.Up.X = 0; p.Up.Y = 0; p.Up.Z = 1; p.Forward.X = 0; p.Forward.Y = 1; p.Forward.Z = 0;
        physics->ConstructFromParams(&p);
    }
};
int main()
{
    Physics ph; ph.calls = 0; Thing t = { &ph }; Def d = { &t };
    V pos = { 1, 2, 3 };
    d.Construct(pos);
    if (ph.calls != 1 || ph.last.Position.Y != 2 || ph.last.Up.Z != 1 || ph.last.Forward.Y != 1 || ph.last.Forward.Z != 0) { std::printf("00778d60_TEST FAIL\n"); return 1; }
    std::printf("00778d60_TEST PASS\n");
    return 0;
}