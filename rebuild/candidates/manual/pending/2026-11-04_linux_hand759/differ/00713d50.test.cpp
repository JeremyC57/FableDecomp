// Behaviour test for NTCScriptedControl::CActionFollowCreature::SetAsSneaking (0x00713D50, parked DIFFER): a model of the row -- on: add a stealth component when missing, flag it and enter mode 7; off: clear, leave mode 7 and remove the component this action added.
#include <stdio.h>
struct C { bool hasStealth, sneaking, inMode7, added; };
static void SetAsSneaking(C& c, bool on)
{
    if (on) { if (!c.hasStealth) { c.hasStealth = true; c.added = true; } if (!c.sneaking) c.inMode7 = true; c.sneaking = true; }
    else { c.sneaking = false; c.inMode7 = false; if (c.added) { c.hasStealth = false; c.added = false; } }
}
int main() { C c = { false, false, false, false }; SetAsSneaking(c, true); bool a = c.hasStealth && c.sneaking && c.inMode7;
  SetAsSneaking(c, false); C d = { true, false, false, false }; SetAsSneaking(d, true); SetAsSneaking(d, false);
  bool ok = a && !c.hasStealth && !c.sneaking && !c.inMode7 && d.hasStealth;
  printf(ok ? "00713d50_TEST PASS\n" : "00713d50_TEST FAIL\n");
  return ok ? 0 : 1;
}
