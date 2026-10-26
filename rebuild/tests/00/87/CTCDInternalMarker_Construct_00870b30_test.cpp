// behaviour test for 0x00870B30 CTCDInternalMarker::Construct(const C3DVector&): the owning thing's physics component receives one params object holding the position and the identity axes Up (0,0,1) / Forward (0,1,0)
#include <cstdio>
struct Vec { float X, Y, Z; };
struct Params { Vec Position; Vec Up; Vec Forward; };
struct Physics {
    int calls; Params got;
    void (*ConstructFromParams)(Physics* self, const Params& p);
};
static void Record(Physics* self, const Params& p) { self->calls++; self->got = p; }
struct Thing { Physics* PhysicsTC; };
struct Component { Thing* ParentThing; };
static void Construct(Component* c, const Vec& position)          // the function under test
{
    Physics* physics = c->ParentThing->PhysicsTC;
    Params params;
    params.Position = position;
    params.Up.X = 0.0f; params.Up.Y = 0.0f; params.Up.Z = 1.0f;
    params.Forward.X = 0.0f; params.Forward.Y = 1.0f; params.Forward.Z = 0.0f;
    physics->ConstructFromParams(physics, params);
}
int main()
{
    int fail = 0;
    Physics ph; ph.calls = 0; ph.ConstructFromParams = Record;
    Thing t; t.PhysicsTC = &ph;
    Component c; c.ParentThing = &t;
    Vec pos; pos.X = 12.5f; pos.Y = -3.0f; pos.Z = 7.25f;
    Construct(&c, pos);
    if (ph.calls != 1) fail |= 1;
    if (ph.got.Position.X != 12.5f || ph.got.Position.Y != -3.0f || ph.got.Position.Z != 7.25f) fail |= 2;
    if (ph.got.Up.X != 0.0f || ph.got.Up.Y != 0.0f || ph.got.Up.Z != 1.0f) fail |= 4;
    if (ph.got.Forward.X != 0.0f || ph.got.Forward.Y != 1.0f || ph.got.Forward.Z != 0.0f) fail |= 8;
    if (sizeof(Vec) != (unsigned int)12) fail |= 16;
    if (fail) { std::printf("00870b30_TEST FAIL %d\n", fail); return 1; }
    std::printf("00870b30_TEST PASS\n");
    return 0;
}