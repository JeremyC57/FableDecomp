// Behaviour test for 0x00762010: selection count 0 -> false; 1 -> erase if >3 points, clear selection, true; >1 -> true untouched.
#include <cstdio>
struct Shape { int points; int selected; int erased;
  bool UpdateDeletePoint() { int n = selected; if (n != 0) { if (n == 1) { if (points - 1 > 2) { --points; ++erased; } selected = 0; } return true; } return false; } };
int main()
{
    Shape a = {5, 0, 0}; bool ok = !a.UpdateDeletePoint();
    Shape b = {5, 1, 0}; ok = ok && b.UpdateDeletePoint() && b.erased == 1 && b.points == 4 && b.selected == 0;
    Shape c = {3, 1, 0}; ok = ok && c.UpdateDeletePoint() && c.erased == 0 && c.selected == 0;
    Shape d = {9, 2, 0}; ok = ok && d.UpdateDeletePoint() && d.selected == 2 && d.erased == 0;
    if (!ok) { std::printf("00762010_TEST FAIL\n"); return 1; }
    std::printf("00762010_TEST PASS\n");
    return 0;
}
