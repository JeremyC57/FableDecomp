#include <cstdio>
#include <cstring>
struct P { bool canSteal, child, weapon;
    bool IsExpressionAvailable(const char* n) const { return canSteal && std::strcmp(n, "EXPRESSION_STEAL") == 0; }
    bool IsChild() const { return child; } };
static bool IsThingCarryingWeapon(const P* p) { return p->weapon; }
struct F { bool Accept, Child, Weapon; P* Player; void* Only;
    F(P* p, void* o) : Accept(false), Child(false), Weapon(false), Player(p), Only(o) {
        bool s = p->IsExpressionAvailable("EXPRESSION_STEAL"); if (s) Accept = true;
        Child = p->IsChild(); if (IsThingCarryingWeapon(Player)) Weapon = true; } };
int main()
{
    P a = { true, false, true }, b = { false, true, false };
    int x;
    F fa(&a, &x), fb(&b, 0);
    bool ok = fa.Accept && !fa.Child && fa.Weapon && fa.Player == &a && fa.Only == &x
        && !fb.Accept && fb.Child && !fb.Weapon && fb.Only == 0;
    if (!ok) { std::printf("00805890_TEST FAIL\n"); return 1; }
    std::printf("00805890_TEST PASS\n");
    return 0;
}
