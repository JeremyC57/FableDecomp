// Behaviour test for CTCInGameMenu::PCSetQuitToFrontEnd (0x0047D296, parked DIFFER): a model of the row -- the dialog gets the quit callbacks, is centred at (w/2 - 104*sx, h/2 - 52*sy), becomes the active dialog with the menu mouse pointer, and both registered observers / buttons are counted.
#include <stdio.h>
struct Dlg { int yes, no; float x, y; bool active; };
static int g_observers = 0;
static void Setup(Dlg& d, float w, float h, float sx, float sy, bool yesBtn, bool noBtn)
{
    d.yes = 1; d.active = true; d.x = w * 0.5f - sx * 104.0f; d.y = h * 0.5f - sy * 52.0f; d.no = 2;
    g_observers += 4; if (yesBtn) ++g_observers; if (noBtn) ++g_observers;
}
int main() { Dlg d = { 0, 0, 0, 0, false }; Setup(d, 800, 600, 1, 1, true, false);
  bool ok = d.yes == 1 && d.no == 2 && d.x == 296.0f && d.y == 248.0f && d.active && g_observers == 5;
  printf(ok ? "0047d296_TEST PASS\n" : "0047d296_TEST FAIL\n");
  return ok ? 0 : 1;
}
