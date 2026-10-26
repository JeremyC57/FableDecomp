#include <cstdio>

static int g_body = 0;
class CTCShop { public: unsigned char pad[0x50]; bool IsStallLike;
    bool CalcIsStallLike() { return true; } void OnInitialActivateBody() { g_body = 1; }
    void OnInitialActivate() { IsStallLike = CalcIsStallLike(); OnInitialActivateBody(); } };
int main()
{
    CTCShop s; s.IsStallLike = false; s.OnInitialActivate();
    if (!s.IsStallLike || g_body != 1) { std::printf("00775b20_TEST FAIL\n"); return 1; }
    std::printf("00775b20_TEST PASS\n");
    return 0;
}