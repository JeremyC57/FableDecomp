#include <cstdio>
#include <cstdlib>
static int g_strings = 0, g_handles = 0, g_frees = 0, g_order = 0;
class CWideString { public: void* m_pData; ~CWideString() { ++g_strings; if (g_handles != g_strings) g_order = 1; } };
struct CTextureHandle { void* m_pTexture; unsigned long m_flags; ~CTextureHandle() { ++g_handles; } };
struct E { unsigned char _pad0[0x14]; CWideString m_name; CTextureHandle m_texture; unsigned char _pad1[0x1c]; };
struct VBase { E* first; E* last; E* e; ~VBase() { if (first) { ++g_frees; std::free(first); } } };
struct V : VBase { ~V() { E* p = first; E* q = last; for (; p != q; ++p) p->~E(); } };
int main()
{
    { V v; v.first = (E*)std::malloc(3 * sizeof(E)); v.last = v.first + 2; v.e = v.first + 3; }
    if (g_strings != 2 || g_handles != 2 || g_frees != 1 || g_order != 0) { std::printf("009c2f50_TEST FAIL\n"); return 1; }
    { V v; v.first = 0; v.last = 0; v.e = 0; }
    if (g_strings != 2 || g_frees != 1) { std::printf("009c2f50_TEST FAIL\n"); return 1; }
    std::printf("009c2f50_TEST PASS\n");
    return 0;
}