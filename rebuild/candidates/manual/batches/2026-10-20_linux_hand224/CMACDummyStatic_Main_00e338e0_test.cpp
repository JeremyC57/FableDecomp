#include <cstdio>
#include <cstring>
struct Thing {
    Thing* imp; char data[32]; int facing;
    Thing() : imp(0), facing(0) { data[0] = 0; }
    void SetDataString(const char* s) { if (imp) std::strcpy(imp->data, s); }
};
struct Game {
    Thing created; Thing ring; int calls;
    Thing CreateObjectOnEntity(const char* obj, const Thing&, const char* bone) { Thing t; if (!std::strcmp(obj, "OBJECT_STRAW_DUMMY_01") && !std::strcmp(bone, "AC_DummyStatic")) t.imp = &created; return t; }
    Thing GetThingWithScriptName(const char* n) { Thing t; if (!std::strcmp(n, "AC_PlayerRing")) t.imp = &ring; return t; }
    void EntitySetFacingAngleTowardsThing(const Thing& a, const Thing& b, bool f) { if (a.imp == &created && b.imp == &ring && !f) { created.facing = 1; ++calls; } }
};
struct Script {
    Game* G; Thing Me;
    void Main()
    {
        Thing dummy = G->CreateObjectOnEntity("OBJECT_STRAW_DUMMY_01", Me, "AC_DummyStatic");
        dummy.SetDataString(Me.data);
        Thing ring = G->GetThingWithScriptName("AC_PlayerRing");
        G->EntitySetFacingAngleTowardsThing(dummy, ring, false);
    }
};
int main()
{
    Game g; g.calls = 0; Script s; s.G = &g; std::strcpy(s.Me.data, "row7"); s.Main();
    if (std::strcmp(g.created.data, "row7") != 0 || g.created.facing != 1 || g.calls != 1) { std::printf("00e338e0_TEST FAIL\n"); return 1; }
    std::printf("00e338e0_TEST PASS\n");
    return 0;
}
