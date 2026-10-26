#include <cstdio>
class CThing { public: bool dead; bool usable; };
class CCharString { public: const char* t; };
static CCharString g_empty = { "" };
class CTCWeapon { public:
    bool carried; CThing* carrier; CCharString name;
    bool IsUsableBy(CThing* t) const { return t->usable; }
    const CCharString& Get() const { if (carried) { CThing* c = carrier; if (c && !c->dead && !IsUsableBy(c)) return g_empty; } return name; }
};
int main()
{
    CThing a = { false, false }, b = { false, true }, d = { true, false };
    CTCWeapon w1 = { true, &a, { "G" } }, w2 = { true, &b, { "G" } }, w3 = { true, &d, { "G" } }, w4 = { false, &a, { "G" } }, w5 = { true, 0, { "G" } };
    if (&w1.Get() != &g_empty || &w2.Get() != &w2.name || &w3.Get() != &w3.name || &w4.Get() != &w4.name || &w5.Get() != &w5.name) { std::printf("005dabe0_TEST FAIL\n"); return 1; }
    std::printf("005dabe0_TEST PASS\n");
    return 0;
}