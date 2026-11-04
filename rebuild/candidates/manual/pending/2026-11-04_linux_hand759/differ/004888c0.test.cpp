// Behaviour test for CPlayer::SwitchPlayerCharactersWith (0x004888C0, parked DIFFER): a model of the row -- refuses when a main player has no living character; otherwise swaps the characters and creatures and renumbers them.
#include <stdio.h>
struct Ch { bool alive; int number; int mode; };
struct Pl { bool main; Ch* ch; int creature; int number; };
static bool Switch(Pl& a, Pl& b)
{
    if ((a.main && !(a.ch && a.ch->alive)) || (b.main && !(b.ch && b.ch->alive))) return false;
    Ch* t = a.ch; a.ch = b.ch; b.ch = t; int c = a.creature; a.creature = b.creature; b.creature = c;
    if (a.ch && a.ch->alive) { a.ch->mode = 0; a.ch->number = a.number; }
    if (b.ch && b.ch->alive) { b.ch->mode = 0; b.ch->number = b.number; }
    return true;
}
int main() { Ch c1 = { true, 0, 5 }, c2 = { true, 1, 5 }, dead = { false, 0, 0 };
  Pl p1 = { true, &c1, 10, 0 }, p2 = { false, &c2, 20, 1 }, p3 = { true, &dead, 0, 2 };
  bool ok = Switch(p1, p2) && p1.ch == &c2 && p2.ch == &c1 && c2.number == 0 && c1.number == 1 && p1.creature == 20 && !Switch(p3, p2);
  printf(ok ? "004888c0_TEST PASS\n" : "004888c0_TEST FAIL\n");
  return ok ? 0 : 1;
}
