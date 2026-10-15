// Behaviour test for 0x0041B800: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the sprite constructor: base built with the id, a null handle, the defaults, then the def read.
struct Base { int id; Base(int i) : id(i) {} };
struct Handle { void* Object; Handle() : Object(0) {} };
struct Sprite : Base {
    Handle H; unsigned char NoRenderNextFrame; int Sprite2DFlag; unsigned int GraphicIndex; void* TexData; void* TexInfo; int defReads;
    Sprite(int i) : Base(i), NoRenderNextFrame(0), Sprite2DFlag(2), GraphicIndex(0), TexData(0), TexInfo(0), defReads(0) { GetDataFromDef(); }
    void GetDataFromDef() { ++defReads; GraphicIndex = (unsigned int)id * 3; }
};
static void Run()
{
    Sprite s(5);
    CHECK(s.id == 5 && s.H.Object == 0 && s.NoRenderNextFrame == 0 && s.Sprite2DFlag == 2 && s.TexData == 0 && s.TexInfo == 0);
    CHECK(s.defReads == 1 && s.GraphicIndex == 15u);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0041b800_TEST FAIL\n"); return 1; }
    std::printf("0041b800_TEST PASS\n");
    return 0;
}
