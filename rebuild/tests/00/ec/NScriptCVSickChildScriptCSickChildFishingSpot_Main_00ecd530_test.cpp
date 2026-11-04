// Behaviour test for 0x00ECD530: wait while the spot is enabled; flag the parent when it is disabled and the script is not done.
#include <cstdio>
struct Script { int frames, doneAfter, enabledFor, checks; bool flag;
  bool IsDone() { return frames >= doneAfter; }
  bool Enabled() { return checks++ < enabledFor; }
  void Main() { ++frames; if (IsDone()) return;
    while (!IsDone()) { if (!Enabled()) { if (!IsDone()) flag = true; return; } ++frames; } } };
int main()
{
    Script a = {0, 100, 3, 0, false}; a.Main(); bool ok = a.flag && a.frames == 4;
    Script b = {0, 3, 100, 0, false}; b.Main(); ok = ok && !b.flag && b.frames == 3;
    Script c = {0, 1, 0, 0, false}; c.Main(); ok = ok && !c.flag && c.checks == 0;
    if (!ok) { std::printf("00ecd530_TEST FAIL\n"); return 1; }
    std::printf("00ecd530_TEST PASS\n");
    return 0;
}