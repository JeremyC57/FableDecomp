// Behaviour test for 0x00A697A0 CManagedTextureData::CreateTexture: power-of-two rounding recorded in Width/Height, texture initialised at that size.
#include <cstdio>
struct Ext { int W, H; };
static int NextPow2(int v) { int p = 1; while (p < v) p <<= 1; return p; }
static Ext GetNextValidDimensions(const Ext& e) { Ext r; r.W = NextPow2(e.W); r.H = NextPow2(e.H); return r; }
struct Texture { Ext last; int levels; bool ok; bool Initialise(const Ext& s, int levels_) { last = s; levels = levels_; return ok; } };
struct Data {
    unsigned short Width, Height; Texture* tex; bool Allocated;
    bool CreateTexture(unsigned int w, unsigned int h, int)
    {
        Ext size; size.W = (int)w; size.H = (int)h;
        Ext actual = GetNextValidDimensions(size);
        Width = (unsigned short)actual.W; Height = (unsigned short)actual.H;
        size = actual;
        Allocated = tex->Initialise(size, -1);
        return Allocated;
    }
};
int main()
{
    Texture t; t.ok = true;
    Data d; d.tex = &t; d.Allocated = false;
    bool ok = d.CreateTexture(300, 100, 4) && d.Width == 512 && d.Height == 128 && t.last.W == 512 && t.last.H == 128 && t.levels == -1 && d.Allocated;
    t.ok = false;
    ok = ok && !d.CreateTexture(64, 64, 1) && !d.Allocated && d.Width == 64;
    if (!ok) { std::printf("00a697a0_TEST FAIL\n"); return 1; }
    std::printf("00a697a0_TEST PASS\n");
    return 0;
}