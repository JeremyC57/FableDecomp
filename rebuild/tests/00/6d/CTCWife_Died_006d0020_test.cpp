#include <cstdio>
static long g_frame = 100;
class CThing;
struct CStats { CThing* spouse; long state; void SetMarriageState(CThing* s, long st) { spouse = s; state = st; } };
class CThing { public: bool dead; CStats* stats; };
class CTCWife { public:
    CThing* parent; CThing* hero; long married; int unmarried;
    void Unmarry(bool) { ++unmarried; }
    void Died() {
        if (married > g_frame) return;
        Unmarry(true);
        CThing* h = hero; if (!h || h->dead) return;
        if (h->stats) h->stats->SetMarriageState(parent, 6);
    }
};
int main()
{
    CStats s = { 0, 0 }; CThing p = { false, 0 }, h = { false, &s };
    CTCWife a = { &p, &h, 50, 0 }, b = { &p, &h, 500, 0 };
    b.Died(); if (b.unmarried || s.state) { std::printf("006d0020_TEST FAIL\n"); return 1; }
    a.Died(); if (a.unmarried != 1 || s.state != 6 || s.spouse != &p) { std::printf("006d0020_TEST FAIL\n"); return 1; }
    std::printf("006d0020_TEST PASS\n");
    return 0;
}