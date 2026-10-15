// Behaviour test for 0x0041A850: the reconstructed logic over a host model.
#include <cstdio>
#include <cstring>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { g_fail = 1; std::printf("check failed line %d\n", __LINE__); } } while (0)
// Host model of the sprite size: explicit pixel width/height win, else base size scaled per axis.
struct V2 { float X, Y; V2() {} V2(float x, float y) : X(x), Y(y) {} };
struct Sprite { V2 Scale; V2 BaseSize; unsigned int Width, Height;
    V2 GetSize() const
    {
        V2 size = BaseSize;
        float x; if (Width) x = (float)Width; else x = Scale.X * size.X;
        float y; if (Height) y = (float)Height; else y = Scale.Y * size.Y;
        return V2(x, y);
    } };
static void Run()
{
    Sprite s; s.Scale = V2(2.0f, 0.5f); s.BaseSize = V2(10.0f, 8.0f); s.Width = 0; s.Height = 0;
    V2 a = s.GetSize(); CHECK(a.X == 20.0f && a.Y == 4.0f);
    s.Width = 64; V2 b = s.GetSize(); CHECK(b.X == 64.0f && b.Y == 4.0f);
    s.Height = 0x80000000u; V2 c = s.GetSize(); CHECK(c.X == 64.0f && c.Y == 2147483648.0f);
}

int main()
{
    Run();
    if (g_fail) { std::printf("0041a850_TEST FAIL\n"); return 1; }
    std::printf("0041a850_TEST PASS\n");
    return 0;
}
