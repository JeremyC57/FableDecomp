// Behaviour test for CAppearanceDef::Transfer (0x00473504): every field goes through the context in
// declaration order, then the animation set is re-sorted.
#include <cstdio>

static int g_order[16]; static int g_n = 0;
class CEngineGraphic { public: int id; };
class CAnimationSet { public: bool sorted; void EnsureSorted() { sorted = true; g_order[g_n++] = 99; } };
class CPersistContext { public:
    void Transfer(CEngineGraphic* v) { g_order[g_n++] = v->id; }
    void Transfer(CAnimationSet*) { g_order[g_n++] = 2; }
    void Transfer(unsigned char* v) { g_order[g_n++] = *v; }
    void Transfer(bool* v) { g_order[g_n++] = *v ? 51 : 50; }
    void Transfer(int* v) { g_order[g_n++] = *v; }
    void Transfer(float* v) { g_order[g_n++] = (int)*v; }
};
class CAppearanceDef { public:
    CEngineGraphic Graphic; CAnimationSet Animation; unsigned char Ref, Bias; bool Override; int Channel;
    CEngineGraphic Fade; bool HasFade; float D1, D2, D3;
    void Transfer(CPersistContext& c)
    {
        c.Transfer(&Graphic); c.Transfer(&Animation); c.Transfer(&Ref); c.Transfer(&Bias); c.Transfer(&Override);
        c.Transfer(&Channel); c.Transfer(&Fade); c.Transfer(&HasFade); c.Transfer(&D1); c.Transfer(&D2); c.Transfer(&D3);
        Animation.EnsureSorted();
    }
};

int main()
{
    CAppearanceDef d = { { 1 }, { false }, 3, 4, true, 5, { 6 }, false, 7.0f, 8.0f, 9.0f };
    CPersistContext c;
    d.Transfer(c);
    static const int expect[] = { 1, 2, 3, 4, 51, 5, 6, 50, 7, 8, 9, 99 };
    if (g_n != 12 || !d.Animation.sorted) { std::printf("00473504_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 12; ++i) if (g_order[i] != expect[i]) { std::printf("00473504_TEST FAIL\n"); return 1; }
    std::printf("00473504_TEST PASS\n");
    return 0;
}