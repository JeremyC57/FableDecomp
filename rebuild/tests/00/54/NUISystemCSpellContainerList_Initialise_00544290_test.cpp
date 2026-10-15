// Behaviour test for 0x00544290: selection starts at the middle and is stepped to the last container.
#include <cstdio>
struct List { float t, last; unsigned int spellIndex, selected, size; int steps;
  void Next() { ++selected; ++steps; }
  void Initialise() { last = t; selected = 0; spellIndex = 0; selected = size / 2; while (selected != size - 1) Next(); } };
int main()
{
    List a = {2.5f, 0, 7, 9, 5, 0}; a.Initialise();
    bool ok = a.last == 2.5f && a.spellIndex == 0 && a.selected == 4 && a.steps == 2;
    List b = {0, 0, 0, 0, 2, 0}; b.Initialise(); ok = ok && b.selected == 1 && b.steps == 0;
    if (!ok) { std::printf("00544290_TEST FAIL\n"); return 1; }
    std::printf("00544290_TEST PASS\n");
    return 0;
}