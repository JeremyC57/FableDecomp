#include <cstdio>
struct CValue { unsigned int Selected; };
struct CAction { CValue* v; };
static int g_exec = 0;
struct CSlider {
    unsigned int Sel, Prev, Start; bool Move; CAction* acts; unsigned int n; unsigned int Original;
    void RevertValue() {
        Sel = Prev = Start = Original;
        for (unsigned int i = 0; i < n; ++i) { acts[i].v->Selected = Original; ++g_exec; }
        Move = true;
    }
};
int main()
{
    CValue v1 = { 0 }, v2 = { 0 }; CAction a[2] = { { &v1 }, { &v2 } };
    CSlider s = { 1, 2, 3, false, a, 2, 7 };
    s.RevertValue();
    if (s.Sel != 7 || s.Prev != 7 || s.Start != 7 || !s.Move || v1.Selected != 7 || v2.Selected != 7 || g_exec != 2) { std::printf("005491a0_TEST FAIL\n"); return 1; }
    std::printf("005491a0_TEST PASS\n");
    return 0;
}