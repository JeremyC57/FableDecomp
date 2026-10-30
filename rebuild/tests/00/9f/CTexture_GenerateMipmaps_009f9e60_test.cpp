#include <cstdio>

typedef unsigned long ulong;

class CTexture { public:
    void* field0;
};

static void* g_p;
static int   g_zero = -1;
static ulong g_b, g_a;

// Stands in for D3DXFilterTexture (d3dx9_25 import thunk 0x00BFEFCE): (pBaseTexture, pPalette, SrcLevel, Filter).
extern "C" long __stdcall D3DXFilterTexture(void* p, const void* palette, unsigned int b, ulong a)
{
    g_p = p; g_zero = (int)palette; g_b = b; g_a = a;
    return 0;
}

void __fastcall GenerateMipmaps(CTexture* texture, int edx, ulong a, ulong b);

int main()
{
    CTexture t; t.field0 = (void*)0xDEAD;
    GenerateMipmaps(&t, 0, 11u, 22u);
    if (g_p == (void*)0xDEAD && g_zero == 0 && g_b == 22u && g_a == 11u)
        printf("009f9e60_TEST PASS\n");
    else
        printf("FAIL p=%p z=%d b=%lu a=%lu\n", g_p, g_zero, g_b, g_a);
    return 0;
}