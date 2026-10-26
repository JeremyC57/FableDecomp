#include <cstdio>
// Behaviour copy of CCreatureAction_DragonTakeOffIntoFlying::Initialise (0x008F3E70): animation name, base init, unliftable,
// clear the interface-9 byte, make the dragon not solid to the hero (adding the component when missing), destination = pos + (0,0,35.148).
class C3DVector { public: float X, Y, Z; C3DVector() {} C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {} };
struct Solid { int notSolidTo; void AddThingNotSolidTo(int t) { notSolidTo = t; } };
struct Flags9 { unsigned char byte10; };
struct Thing {
    int id; bool has4d; bool has9; Solid solid; Flags9 flags; C3DVector pos; int addedCount; bool liftable;
    bool HasTC(long t) { return t == 0x4d ? has4d : has9; }
    Solid* AddTC(const char*, bool, void*) { ++addedCount; has4d = true; return &solid; }
    Solid* GetSolid() { return &solid; }
    const C3DVector& GetPosition() { return pos; }
};
struct Action {
    Thing* creature; Thing* hero; const char* anim; bool baseInit; C3DVector Dest;
    void Initialise() {
        anim = "TAKE_OFF"; baseInit = true;
        creature->liftable = false;
        creature->flags.byte10 = 0;
        Solid* s;
        if (hero->HasTC(0x4d)) s = hero->GetSolid(); else s = hero->AddTC("CTCSolidToThing", false, 0);
        if (s) s->AddThingNotSolidTo(creature->id);
        const C3DVector& p = creature->GetPosition();
        Dest = C3DVector(p.X, p.Y, p.Z + 35.14837646484375f);
    }
};
int main()
{
    Thing dragon = { 7, false, true, { 0 }, { 5 }, C3DVector(1.0f, 2.0f, 3.0f), 0, true };
    Thing hero = { 1, false, false, { 0 }, { 0 }, C3DVector(0, 0, 0), 0, true };
    Action a; a.creature = &dragon; a.hero = &hero; a.baseInit = false;
    a.Initialise();
    bool ok = a.baseInit && !dragon.liftable && dragon.flags.byte10 == 0 && hero.addedCount == 1 && hero.solid.notSolidTo == 7
        && a.Dest.X == 1.0f && a.Dest.Y == 2.0f && a.Dest.Z == 3.0f + 35.14837646484375f;
    a.Initialise();                                   // the component now exists: no second add
    ok = ok && hero.addedCount == 1 && hero.solid.notSolidTo == 7;
    std::printf(ok ? "008f3e70_TEST PASS\n" : "008f3e70_TEST FAIL\n");
    return ok ? 0 : 1;
}