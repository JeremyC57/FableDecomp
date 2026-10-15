// Behaviour test for 0x006359c0: InitModeClickPastText: mode handed to the yes/no process, two attaches, live character cancels its mutex ability.
// The reconstructed logic runs over a host model of the retail classes and callees.
#include <cstdio>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
struct Iface { int dummy; };
struct Proc { int added; Iface* to; long mode; void* cam; void Add(Iface* i) { ++added; to = i; } };

struct Abil { int cancels; void Cancel() { ++cancels; } };
struct Thing { bool dead; bool hasAbil; Abil* abil; };
struct Player { Iface* PI; Proc* yesno; Proc* look; Thing* ch;
  void Init(long mode) { yesno->mode = mode; yesno->Add(PI); look->Add(PI); if (ch && !ch->dead) { Thing* c = ch; if (c->hasAbil) c->abil->Cancel(); } } };
static int run() { Iface i; Proc y = {0,0,0,0}, l = {0,0,0,0}; Abil a = {0}; Thing t = {false, true, &a};
  Player p = {&i, &y, &l, &t}; p.Init(17); CHECK(y.mode == 17 && y.added == 1 && l.added == 1 && a.cancels == 1);
  t.dead = true; p.Init(3); CHECK(a.cancels == 1); p.ch = 0; p.Init(3); CHECK(y.added == 3); return 0; }
int main()
{
    run();
    if (g_fail) { std::printf("006359c0_TEST FAIL\n"); return 1; }
    std::printf("006359c0_TEST PASS\n");
    return 0;
}