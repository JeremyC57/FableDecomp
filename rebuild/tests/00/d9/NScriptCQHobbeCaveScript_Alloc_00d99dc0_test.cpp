// behaviour test for 0x00D99DC0 NScript::CQ_HobbeCaveScript::Alloc: one 0x78-byte block; the script base is built first, Game and MasterScriptData take the arguments, the BanditComment timer is registered once, last; a failed allocation returns null and registers nothing
#include <cstdio>
#include <cstdlib>
static int gStep = 0, gBaseAt = -1, gTimerAt = -1, gTimers = 0, gFailAlloc = 0;
static unsigned int gAsked = 0;
struct Script { const void* game; const void* master; long timer; char pad[0x78 - 12]; };
static long RegisterTimer() { ++gTimers; gTimerAt = gStep++; return 41 + gTimers; }
static void* Allocate(unsigned int n) { gAsked = n; return gFailAlloc ? 0 : std::malloc(n); }
static Script* Alloc(const void* data, const void* game)   // the function under test
{
    Script* s = (Script*)Allocate(0x78);
    if (!s) return 0;
    gBaseAt = gStep++;                                       // CScriptBase::CScriptBase
    s->game = game; s->master = data;                        // CLevelScriptBase members
    s->timer = RegisterTimer();                              // CTimer member
    return s;
}
int main()
{
    int fail = 0;
    int data = 1, game = 2;
    Script* s = Alloc(&data, &game);
    if (!s || gAsked != 0x78u) fail |= 1;
    if (gBaseAt != 0 || gTimerAt != 1 || gTimers != 1) fail |= 2;
    if (s && (s->game != &game || s->master != &data || s->timer != 42)) fail |= 4;
    std::free(s);
    gFailAlloc = 1;
    if (Alloc(&data, &game) != 0 || gTimers != 1) fail |= 8;
    if (fail) { std::printf("00d99dc0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00d99dc0_TEST PASS\n");
    return 0;
}