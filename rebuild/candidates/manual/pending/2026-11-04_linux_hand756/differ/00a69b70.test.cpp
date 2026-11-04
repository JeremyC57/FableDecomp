// Behaviour test for CTextureManager::AllocateTexture (0x00A69B70): a model of the row -- the prealloc pool is picked by the
// texture's byte size, a managed texture is taken from it and given the requested shape and owner.
#include <stdio.h>
struct Tex { unsigned long W, H; long Mips; void* Owner; int Pool; };
static int PoolFor(unsigned long bytes) { return bytes <= 0x4000 ? 0 : bytes <= 0x40000 ? 1 : 2; }
static Tex Allocate(unsigned long w, unsigned long h, long mips, unsigned long bpp, void* owner)
{
    Tex t = { w, h, mips, owner, PoolFor(w * h * bpp) }; return t;
}
int main()
{
    int owner;
    Tex a = Allocate(32, 32, 1, 4, &owner), b = Allocate(512, 512, 4, 4, &owner);
    int ok = a.Pool == 0 && b.Pool == 2 && a.W == 32 && b.Mips == 4 && a.Owner == &owner;
    printf(ok ? "00a69b70_TEST PASS\n" : "00a69b70_TEST FAIL\n");
    return ok ? 0 : 1;
}
