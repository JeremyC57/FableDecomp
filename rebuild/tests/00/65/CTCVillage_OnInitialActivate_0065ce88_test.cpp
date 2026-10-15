#include <cstdio>
struct CTCVillage {
    bool Limbo; int calls; bool lastArg;
    void SetVillageLimbo(bool limbo) { ++calls; lastArg = limbo; }
    void OnInitialActivate() { if (Limbo) SetVillageLimbo(Limbo); }
};
int main()
{
    CTCVillage v; v.calls = 0; v.lastArg = false;
    v.Limbo = false; v.OnInitialActivate();
    if (v.calls != 0) { std::printf("0065ce88_TEST FAIL\n"); return 1; }
    v.Limbo = true; v.OnInitialActivate();
    if (v.calls != 1 || !v.lastArg) { std::printf("0065ce88_TEST FAIL\n"); return 1; }
    std::printf("0065ce88_TEST PASS\n");
    return 0;
}