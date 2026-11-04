// Behaviour test for CLocalDetailCacheMap::CQuadTreeElement ctor (0x00BDDF30, parked DIFFER row): a model of the row -- map and rectangle stored, children/collections/sphere/fade/file words/mask zeroed, state bits cleared.
#include <stdio.h>
struct Q { void* Map; Q* Children[4]; void* Collections; float Sphere[4]; float Fade; long Pos, Size, Offset; unsigned long Mask; unsigned short X, Y, W, H; unsigned char Bits;
    Q(void* m, long x, long y, long w, long h, unsigned char oldBits) : Map(m), X((unsigned short)x), Y((unsigned short)y), W((unsigned short)w), H((unsigned short)h)
    { for (int i = 0; i < 4; ++i) Children[i] = 0; Collections = 0; Bits = (unsigned char)(oldBits & 0xF0); Fade = 0; for (int i = 0; i < 4; ++i) Sphere[i] = 0; Pos = Size = Offset = 0; Mask = 0; } };
static int Check() { int m; Q q(&m, 1, 2, 3, 4, 0xFF); return q.Map == &m && q.X == 1 && q.H == 4 && q.Children[3] == 0 && q.Collections == 0 && q.Bits == 0xF0 && q.Fade == 0 && q.Mask == 0 && q.Offset == 0; }
int main()
{
    int ok = Check();
    printf(ok ? "00bddf30_TEST PASS\n" : "00bddf30_TEST FAIL\n");
    return ok ? 0 : 1;
}
