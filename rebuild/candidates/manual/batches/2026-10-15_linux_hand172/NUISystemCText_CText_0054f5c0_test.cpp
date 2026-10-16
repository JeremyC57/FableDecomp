#include <cstdio>
struct CHandle { void* Data; CHandle() : Data(0) {} };
struct CText {
    void* ValueData; void* FontData; void* FontInfo; CHandle Handle, Shadow; bool ScaleText, Unavailable; unsigned char NoRenderNextFrame; int defReads;
    void GetDataFromDef() { ++defReads; ScaleText = true; Unavailable = true; }
    CText() : ValueData(0), FontData(0), FontInfo(0), NoRenderNextFrame(0), defReads(0) { GetDataFromDef(); Unavailable = false; }
};
int main()
{
    CText t;
    if (t.defReads != 1 || t.Unavailable || !t.ScaleText || t.NoRenderNextFrame || t.FontData || t.FontInfo || t.Handle.Data || t.Shadow.Data) { std::printf("0054f5c0_TEST FAIL\n"); return 1; }
    std::printf("0054f5c0_TEST PASS\n");
    return 0;
}
