#include <cstdio>
// Behaviour copy of STLport vector<CEngineGraphic>::erase(first, last): slide [last, finish) down to first, finish = first +
// (finish - last), return first.
#pragma pack(push, 2)
class CEngineGraphic { public: unsigned int BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
struct SVec {
    CEngineGraphic* start; CEngineGraphic* finish; CEngineGraphic* eos;
    CEngineGraphic* erase(CEngineGraphic* first, CEngineGraphic* last)
    {
        CEngineGraphic* result = first;
        for (int n = int(finish - last); n > 0; --n) { *result = *last; ++last; ++result; }
        finish = result;
        return first;
    }
};
int main()
{
    bool ok = true;
    CEngineGraphic buf[6];
    for (int i = 0; i < 6; ++i) { buf[i].BankIndex = i; buf[i].AnimStep = 0; buf[i].RenderSizeX = 0; buf[i].AdditiveAlpha = false; buf[i].Type = (unsigned char)i; }
    SVec v; v.start = buf; v.finish = buf + 6; v.eos = buf + 6;
    CEngineGraphic* r = v.erase(buf + 1, buf + 3);              // remove elements 1 and 2
    if (r != buf + 1 || v.finish != buf + 4) ok = false;
    const unsigned int expect[4] = { 0, 3, 4, 5 };
    for (int i = 0; ok && i < 4; ++i) if (buf[i].BankIndex != expect[i] || buf[i].Type != (unsigned char)expect[i]) ok = false;
    r = v.erase(buf + 2, v.finish);                             // erase the tail
    if (r != buf + 2 || v.finish != buf + 2) ok = false;
    r = v.erase(buf + 1, buf + 1);                              // empty range: nothing moves
    if (r != buf + 1 || v.finish != buf + 2 || buf[1].BankIndex != 3) ok = false;
    if (!ok) { std::printf("004ea40b_TEST FAIL\n"); return 1; }
    std::printf("004ea40b_TEST PASS\n");
    return 0;
}