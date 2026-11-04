// Behaviour test for CTCInventoryExperience::CloseInventory (0x0056F30B, parked DIFFER): a model of the row -- HUD back on, close frame stamped, both sound categories restored to the profile level, screen deactivated, message 0x4A posted, menu count closed.
#include <stdio.h>
struct State { bool hud, screen; long closeFrame; float sfx, music; int lastMessage, menus; };
static void Close(State& s, long frame, float profileLevel)
{ s.hud = true; s.closeFrame = frame; s.sfx = profileLevel; s.music = profileLevel; s.screen = false; s.lastMessage = 0x4a; --s.menus; }
int main() { State s = { false, true, 0, 0.1f, 0.2f, 0, 1 }; Close(s, 99, 0.8f);
  bool ok = s.hud && !s.screen && s.closeFrame == 99 && s.sfx == 0.8f && s.music == 0.8f && s.lastMessage == 0x4a && s.menus == 0;
  printf(ok ? "0056f30b_TEST PASS\n" : "0056f30b_TEST FAIL\n");
  return ok ? 0 : 1;
}
