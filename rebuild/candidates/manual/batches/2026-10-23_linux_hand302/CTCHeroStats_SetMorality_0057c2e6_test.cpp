// Behaviour test for CTCHeroStats::SetMorality @ 0x0057C2E6
#include <cstdio>

struct Stats { long Morality; bool Enabled; bool Child; long Max; long ChildMax; };
static long g_guiChange, g_event, g_expr, g_sound;
template <class T> inline const T& LimitRef(const T& v, const T& lo, const T& hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}
static void SetMorality(Stats* s, long morality)
{
    if (!s->Enabled) return;
    long maxM = s->Max; long minM = -maxM;
    long newM = LimitRef(morality, minM, maxM);
    long change = newM - s->Morality;
    if (change != 0) {
        g_guiChange = change;
        if (s->Child) {
            long maxC = s->ChildMax; long minC = -maxC;
            change = LimitRef(morality, minC, maxC);
            newM = change;
            change -= s->Morality;
        }
        if (change != 0) { g_event = change; s->Morality = newM; ++g_expr; ++g_sound; }
    }
}
int main()
{
    Stats s; s.Morality = 0; s.Enabled = true; s.Child = false; s.Max = 100; s.ChildMax = 10;
    SetMorality(&s, 250);
    if (s.Morality != 100 || g_guiChange != 100 || g_event != 100 || g_expr != 1) { std::printf("0057c2e6_TEST FAIL\n"); return 1; }
    s.Child = true; SetMorality(&s, -50);
    if (s.Morality != -10 || g_guiChange != -150 || g_event != -110 || g_expr != 2) { std::printf("0057c2e6_TEST FAIL\n"); return 1; }
    s.Enabled = false; SetMorality(&s, 5);
    if (s.Morality != -10 || g_expr != 2) { std::printf("0057c2e6_TEST FAIL\n"); return 1; }
    s.Enabled = true; SetMorality(&s, -10);
    if (g_expr != 2) { std::printf("0057c2e6_TEST FAIL\n"); return 1; }
    std::printf("0057c2e6_TEST PASS\n");
    return 0;
}
