#include <cstdio>
#include <cstring>
#include <new>
// Behaviour copy of STLport uninitialized_fill_n<CEngineGraphic*>: copy-construct x into n slots, return first + n.
#pragma pack(push, 2)
class CEngineGraphic { public: unsigned int BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
static CEngineGraphic* FillN(CEngineGraphic* first, unsigned int n, const CEngineGraphic& x)
{
    CEngineGraphic* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CEngineGraphic(x);
    return cur;
}
int main()
{
    bool ok = true;
    if (sizeof(CEngineGraphic) != 14) ok = false;
    CEngineGraphic buf[5];
    std::memset(buf, 0, sizeof(buf));
    CEngineGraphic x; x.BankIndex = 77; x.AnimStep = 0.25f; x.RenderSizeX = 2.0f; x.AdditiveAlpha = true; x.Type = 3;
    CEngineGraphic* end = FillN(buf, 3, x);
    if (end != buf + 3) ok = false;
    for (int i = 0; ok && i < 3; ++i)
        if (buf[i].BankIndex != 77 || buf[i].AnimStep != 0.25f || buf[i].RenderSizeX != 2.0f || !buf[i].AdditiveAlpha || buf[i].Type != 3) ok = false;
    if (buf[3].BankIndex != 0) ok = false;
    if (FillN(buf, 0, x) != buf) ok = false;
    if (!ok) { std::printf("004ea57f_TEST FAIL\n"); return 1; }
    std::printf("004ea57f_TEST PASS\n");
    return 0;
}