// behaviour test for 0x00EB6B90 CV_MayorsInvitationScript::CV_MayorsInvitationScript: the script base is built first, then Game / MasterScriptData take the arguments, then destinationHSP is an empty script thing (its CBaseClass base built once, no counted data)
#include <cstdio>
static int gStep = 0, gBaseAt = -1, gThingAt = -1;
struct Thing { int vt; const void* data; const void* info; };
struct Script { const void* game; const void* master; Thing thing; };
static void Construct(Script& s, const void* data, const void* game)   // the function under test
{
    gBaseAt = gStep++;                                       // CScriptBase::CScriptBase
    s.master = data; s.game = game;                          // CLevelScriptBase members
    gThingAt = gStep++; s.thing.vt = 1; s.thing.data = 0; s.thing.info = 0;   // CScriptThing()
}
int main()
{
    int fail = 0;
    int data = 1, game = 2, junk = 3;
    Script s; s.game = &junk; s.master = &junk; s.thing.vt = 0; s.thing.data = &junk; s.thing.info = &junk;
    Construct(s, &data, &game);
    if (gBaseAt != 0 || gThingAt != 1) fail |= 1;
    if (s.game != &game || s.master != &data) fail |= 2;
    if (s.thing.vt != 1 || s.thing.data != 0 || s.thing.info != 0) fail |= 4;
    if (fail) { std::printf("00eb6b90_TEST FAIL %d\n", fail); return 1; }
    std::printf("00eb6b90_TEST PASS\n");
    return 0;
}