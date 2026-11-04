// Behaviour test for CScriptGameResourceObjectScriptedThing::FollowPreCalculatedRoute (0x00903F30), g++ model.
#include <cstdio>
enum EScriptEntityMoveType { SCRIPT_MOVE_WALK = 0, SCRIPT_MOVE_RUN = 1, SCRIPT_MOVE_OTHER = 2 };
static int Convert(EScriptEntityMoveType t) { switch (t) { case SCRIPT_MOVE_WALK: return 3; case SCRIPT_MOVE_RUN: return 5; } return 1; }
struct Thing { bool Dead; bool IsRoute; };
struct Route { bool Alive; Thing* PThing; };
struct Result { int Actions; int LastType; bool LastAvoid; int Waits; };
static void Follow(Thing* self, const Route& route, EScriptEntityMoveType moveType, bool wait, bool avoid, Result& r)
{
    if (self && !self->Dead && route.Alive && route.PThing->IsRoute) {
        ++r.Actions; r.LastType = Convert(moveType); r.LastAvoid = avoid;
        if (wait) ++r.Waits;
    }
}
int main()
{
    Thing self = { false, false }, dead = { true, false }, routeT = { false, true }, plain = { false, false };
    Route r1 = { true, &routeT }, r2 = { true, &plain }, r3 = { false, &routeT };
    Result res = { 0, 0, false, 0 };
    Follow(&self, r1, SCRIPT_MOVE_RUN, true, true, res);
    int ok = res.Actions == 1 && res.LastType == 5 && res.LastAvoid && res.Waits == 1;
    Follow(&self, r1, SCRIPT_MOVE_WALK, false, false, res); ok &= res.Actions == 2 && res.LastType == 3 && res.Waits == 1;
    Follow(&self, r1, SCRIPT_MOVE_OTHER, false, false, res); ok &= res.LastType == 1;
    Follow(&self, r2, SCRIPT_MOVE_RUN, true, true, res); Follow(&self, r3, SCRIPT_MOVE_RUN, true, true, res);
    Follow(&dead, r1, SCRIPT_MOVE_RUN, true, true, res); Follow(0, r1, SCRIPT_MOVE_RUN, true, true, res);
    ok &= res.Actions == 3;
    std::printf(ok ? "00903f30_TEST PASS\n" : "00903f30_TEST FAIL\n");
    return ok ? 0 : 1;
}
