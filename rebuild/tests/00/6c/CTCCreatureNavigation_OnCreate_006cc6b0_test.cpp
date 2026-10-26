#include <cstdio>
// behaviour test: OnCreate creates one navigator per A* / flyer entry of the def's navigator type list (other types skipped), in
// order, and starts with the first; with no navigators the current one is left alone
class CNavigatorBase { public: int kind; };
static CNavigatorBase g_pool[8]; static int g_used = 0;
static CNavigatorBase* Create(int kind) { g_pool[g_used].kind = kind; return &g_pool[g_used++]; }
class CTCCreatureNavigation { public:
    CNavigatorBase* PNavigator; CNavigatorBase* PossibleNavigators[8]; int count;
    void OnCreate(const int* types, int n)
    {
        for (const int* it = types; it != types + n; ++it) {
            switch (*it) {
            case 1: PossibleNavigators[count++] = Create(1); break;
            case 2: PossibleNavigators[count++] = Create(2); break;
            }
        }
        if (count != 0)
            PNavigator = PossibleNavigators[0];
    }
};
int main()
{
    int types[4] = { 2, 0, 1, 7 };
    CTCCreatureNavigation nav; nav.PNavigator = 0; nav.count = 0;
    nav.OnCreate(types, 4);
    bool ok = nav.count == 2 && nav.PossibleNavigators[0]->kind == 2 && nav.PossibleNavigators[1]->kind == 1 && nav.PNavigator == nav.PossibleNavigators[0];
    CTCCreatureNavigation none; CNavigatorBase sentinel; none.PNavigator = &sentinel; none.count = 0;
    none.OnCreate(types + 1, 1);
    ok = ok && none.count == 0 && none.PNavigator == &sentinel;
    if (!ok) { std::printf("006cc6b0_TEST FAIL\n"); return 1; }
    std::printf("006cc6b0_TEST PASS\n");
    return 0;
}