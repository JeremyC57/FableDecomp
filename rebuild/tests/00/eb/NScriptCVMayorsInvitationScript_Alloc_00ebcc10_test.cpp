// behaviour test for 0x00EBCC10 NScript::CV_MayorsInvitationScript::Alloc: one 0x6C-byte block; the script base is built first, Game and MasterScriptData take the arguments, destinationHSP is an empty script thing; a failed allocation returns null and builds nothing
#include <cstdio>
#include <cstdlib>
static int gStep = 0, gBaseAt = -1, gThingAt = -1, gFailAlloc = 0;
static unsigned int gAsked = 0;
struct Thing { int vt; const void* data; const void* info; };
struct Script { const void* game; const void* master; Thing thing; char pad[0x6C - 20]; };
static void* Allocate(unsigned int n) { gAsked = n; return gFailAlloc ? 0 : std::malloc(n); }
static Script* Alloc(const void* data, const void* game)   // the function under test
{
    Script* s = (Script*)Allocate(0x6C);
    if (!s) return 0;
    gBaseAt = gStep++;                                       // CScriptBase::CScriptBase
    s->game = game; s->master = data;                        // CLevelScriptBase members
    gThingAt = gStep++; s->thing.vt = 1; s->thing.data = 0; s->thing.info = 0;   // CScriptThing()
    return s;
}
int main()
{
    int fail = 0;
    int data = 1, game = 2;
    Script* s = Alloc(&data, &game);
    if (!s || gAsked != 0x6Cu) fail |= 1;
    if (gBaseAt != 0 || gThingAt != 1) fail |= 2;
    if (s && (s->game != &game || s->master != &data || s->thing.vt != 1 || s->thing.data != 0 || s->thing.info != 0)) fail |= 4;
    std::free(s);
    gFailAlloc = 1; gStep = 0; gBaseAt = -1;
    if (Alloc(&data, &game) != 0 || gBaseAt != -1) fail |= 8;
    if (fail) { std::printf("00ebcc10_TEST FAIL %d\n", fail); return 1; }
    std::printf("00ebcc10_TEST PASS\n");
    return 0;
}