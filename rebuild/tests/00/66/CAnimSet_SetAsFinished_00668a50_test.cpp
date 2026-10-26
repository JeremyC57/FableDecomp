#include <cstdio>
class CAnimSet { public:
    unsigned char _pad0[0x2c]; float CurrentAlpha; unsigned char _pad1[0x8]; float ChangeAlphaBy; unsigned char _pad2[0x15];
    unsigned char Looping : 1; unsigned char Finished : 1; unsigned char Forwards : 1; unsigned char HadUpdate : 1; unsigned char OwnedByMainAnim : 1;
    void SetAsFinished(long n) { if (!Finished) { Finished = 1; if (n < 1) n = 1; ChangeAlphaBy = CurrentAlpha / (float)n; } }
};
int main()
{
    CAnimSet s; s.Finished = 0; s.Looping = 1; s.CurrentAlpha = 8.0f; s.ChangeAlphaBy = 0.0f;
    s.SetAsFinished(4);
    if (!s.Finished || !s.Looping || s.ChangeAlphaBy != 2.0f) { std::printf("00668a50_TEST FAIL\n"); return 1; }
    s.CurrentAlpha = 1.0f; s.SetAsFinished(100);
    if (s.ChangeAlphaBy != 2.0f) { std::printf("00668a50_TEST FAIL\n"); return 1; }
    s.Finished = 0; s.SetAsFinished(-3);
    if (s.ChangeAlphaBy != 1.0f) { std::printf("00668a50_TEST FAIL\n"); return 1; }
    std::printf("00668a50_TEST PASS\n");
    return 0;
}