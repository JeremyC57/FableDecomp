// behaviour test for 0x00443930: the deleter destroys the member, every handle, frees the handle block and the object
#include <cstdio>
static int g_handles = 0, g_member = 0, g_frees = 0;
struct H { virtual ~H() { ++g_handles; } long h; };
struct M { ~M() { ++g_member; } };
struct D { H* s; H* f; M m; };
static void DeleteData(void* data)
{
    D* p = (D*)data; if (!p) return;
    p->m.~M(); for (H* h = p->s; h != p->f; ++h) h->~H(); if (p->s) ++g_frees; ++g_frees;
}
int main()
{
    H hs[3]; D d; d.s = hs; d.f = hs + 3;
    DeleteData(&d); DeleteData(0);
    if (g_handles != 3 || g_member != 1 || g_frees != 2) { std::printf("00443930_TEST FAIL\n"); return 1; }
    std::printf("00443930_TEST PASS\n");
    return 0;
}