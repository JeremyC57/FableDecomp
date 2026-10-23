#include <cstdio>
// behaviour test: CTCWound::OnSerialise saves the wound count + per-wound duration/morph index, and on load rebuilds wounds whose morph index resolves, capped at 15
struct CWound { unsigned long Duration; long MorphIndex; };
struct CContext {
    bool Saving; long Values[64]; int N, Pos;
    void Transfer(long& v) { if (Saving) Values[N++] = v; else v = Values[Pos++]; }
    void Transfer(unsigned long& v) { if (Saving) Values[N++] = (long)v; else v = (unsigned long)Values[Pos++]; }
};
struct CTCWound {
    CWound Wounds[40]; long Count;
    void OnSerialise(CContext& c)
    {
        if (c.Saving) {
            long count = Count; c.Transfer(count);
            for (long i = 0; i < Count; ++i) { unsigned long d = Wounds[i].Duration; c.Transfer(d); long m = Wounds[i].MorphIndex; c.Transfer(m); }
        } else {
            long count = 0; c.Transfer(count);
            for (long i = 0; i < count; ++i) {
                unsigned long d = 0; c.Transfer(d); long m = -1; c.Transfer(m);
                if (m >= 0) { Wounds[Count].Duration = d; Wounds[Count].MorphIndex = m; ++Count; }
            }
            while (Count > 15) --Count;
        }
    }
};
int main()
{
    CContext c; c.Saving = true; c.N = 0; c.Pos = 0;
    CTCWound a; a.Count = 0;
    for (int i = 0; i < 20; ++i) { a.Wounds[i].Duration = 100 + i; a.Wounds[i].MorphIndex = (i == 3) ? -1 : i; ++a.Count; }
    a.OnSerialise(c);
    bool ok = c.N == 41 && c.Values[0] == 20 && c.Values[1] == 100 && c.Values[2] == 0;
    c.Saving = false; CTCWound b; b.Count = 0; b.OnSerialise(c);
    ok = ok && b.Count == 15 && b.Wounds[3].MorphIndex == 4 && b.Wounds[3].Duration == 104;
    if (!ok) { std::printf("00757c90_TEST FAIL\n"); return 1; }
    std::printf("00757c90_TEST PASS\n");
    return 0;
}
