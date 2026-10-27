#include <cstdio>
// Behaviour model: only a kill decrements the parent's counter.
struct Parent { long alive; };
struct Entity { bool killed; Parent* parent; void OnPredicateFail() { if (killed) --parent->alive; } };
int main()
{
    Parent p = { 3 }; Entity a = { false, &p }, b = { true, &p };
    a.OnPredicateFail(); b.OnPredicateFail();
    if (p.alive != 2) { std::printf("00cfdd60_TEST FAIL\n"); return 1; }
    std::printf("00cfdd60_TEST PASS\n");
    return 0;
}