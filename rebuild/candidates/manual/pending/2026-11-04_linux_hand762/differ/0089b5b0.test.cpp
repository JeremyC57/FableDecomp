// Behaviour test for CGameScriptInterface::StartScriptingEntity (0x0089B5B0, parked DIFFER): a model of the row -- refuse at a higher existing priority or when the thing dies while waiting; otherwise the resource gets an object.
#include <stdio.h>
struct T { bool alive, scripted, dead; int priority; int busyFrames; };
static bool Start(T& t, int priority, bool& resourceValid)
{
    resourceValid = false;
    if (t.alive) {
        if (t.scripted && t.priority > priority) return false;
        while (t.busyFrames > 0) { if (!t.alive) return false; --t.busyFrames; }
        if (!t.dead) { resourceValid = true; return true; }
    }
    return false;
}
int main() { bool v; T a = { true, true, false, 5, 2 }; T b = { true, true, false, 1, 3 }; T c = { false, false, false, 0, 0 };
  bool ok = !Start(a, 3, v) && !v && Start(b, 3, v) && v && b.busyFrames == 0 && !Start(c, 3, v) && !v;
  printf(ok ? "0089b5b0_TEST PASS\n" : "0089b5b0_TEST FAIL\n");
  return ok ? 0 : 1;
}
