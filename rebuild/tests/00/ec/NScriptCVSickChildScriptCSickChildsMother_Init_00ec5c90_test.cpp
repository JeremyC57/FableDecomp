// behaviour test for 0x00EC5C90 NScript::CV_SickChildScript::CSickChildsMother::Init(): one SetIsPushableByHero call on the script's own Game with a by-value copy of Me (sharing its counted data, count bumped once) and pushable = false
#include <cstdio>
struct Info { int RefCount; };
struct Thing {
    void* Data; Info* Ref;
    Thing() : Data(0), Ref(0) { }
    Thing(const Thing& o) : Data(o.Data), Ref(o.Ref) { if (Ref) ++Ref->RefCount; }
};
struct Interface {
    int Calls, Pushable, SeenCount; void* SeenData; const Thing* SeenAddr;
    Interface() : Calls(0), Pushable(-1), SeenCount(-1), SeenData(0), SeenAddr(0) { }
    void SetIsPushableByHero(Thing thing, bool pushable) { ++Calls; Pushable = pushable; SeenData = thing.Data; SeenCount = thing.Ref ? thing.Ref->RefCount : -1; SeenAddr = &thing; }
};
struct Script {
    Interface* Game; Thing Me;
    void Init() { Game->SetIsPushableByHero(Me, false); }   // the function under test
};
int main()
{
    int fail = 0, payload = 5;
    Info info; info.RefCount = 1;
    Interface game;
    Script s; s.Game = &game; s.Me.Data = &payload; s.Me.Ref = &info;
    s.Init();
    if (game.Calls != 1 || game.Pushable != 0) fail |= 1;
    if (game.SeenData != &payload || game.SeenCount != 2) fail |= 2;
    if (game.SeenAddr == &s.Me) fail |= 4;
    if (fail) { std::printf("00ec5c90_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ec5c90_TEST PASS\n");
    return 0;
}