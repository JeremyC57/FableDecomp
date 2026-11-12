// Behaviour test for NCreatureMode::CModeScreamerAggressive::OnActivate (0x008DA410): model of the activation logic.
#include <cstdio>
#include <cstring>
struct Thing { bool dead; bool killed; bool hasEmitter; int attachType; const char* helper; bool readyForMelee; long effect; };
static Thing g_spawned[4]; static int g_nspawn = 0;
struct Def { const char* name; long effect; };
static bool GetEffectFromString(const Def* d, const char* name, long& effect)
{ if (d && std::strcmp(d->name, name) == 0) { effect = d->effect; return true; } return false; }
static Thing* Create(long effect) { Thing* t = &g_spawned[g_nspawn++]; std::memset(t, 0, sizeof *t); t->effect = effect; t->hasEmitter = true; return t; }
struct Mode { Thing* parent; const Def* subdef; Thing* PEffect; bool standActivated;
  void OnActivate() {
    long effect = 0;
    if (subdef) GetEffectFromString(subdef, "AGGRESSIVE", effect);
    if (PEffect && !PEffect->dead) { PEffect->killed = true; PEffect = 0; }
    if (effect > 0) {
      PEffect = Create(effect);
      if (PEffect && !PEffect->dead) { PEffect->attachType = 12; PEffect->helper = "HIT_LOCATION_CYLINDER_01"; }
    }
    parent->readyForMelee = true;
    standActivated = true;
  } };
int main()
{
  Thing parent = {}; Thing old = {}; Def d = { "AGGRESSIVE", 7 };
  Mode m = { &parent, &d, &old, false };
  m.OnActivate();
  if (!old.killed || !m.PEffect || m.PEffect->effect != 7 || m.PEffect->attachType != 12 ||
      std::strcmp(m.PEffect->helper, "HIT_LOCATION_CYLINDER_01") != 0 || !parent.readyForMelee || !m.standActivated)
  { std::printf("008da410_TEST FAIL\n"); return 1; }
  Def other = { "CALM", 3 }; Thing dead = {}; dead.dead = true; Thing parent2 = {};
  Mode m2 = { &parent2, &other, &dead, false };
  m2.OnActivate();
  if (dead.killed || m2.PEffect != &dead || g_nspawn != 1 || !parent2.readyForMelee || !m2.standActivated)
  { std::printf("008da410_TEST FAIL\n"); return 1; }
  std::printf("008da410_TEST PASS\n");
  return 0;
}