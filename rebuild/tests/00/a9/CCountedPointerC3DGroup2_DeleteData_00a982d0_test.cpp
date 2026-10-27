// behaviour test for 0x00A982D0: deleting the object frees its vector storage and the object; null is a no-op
#include <cstdio>
#include <cstdlib>
static int g_frees = 0;
struct V { long* s; long* f; long* e; ~V() { if (s) { std::free(s); ++g_frees; } } };
struct T { long a; V v; };
static void Delete(void* p) { T* t = (T*)p; if (t) { t->~T(); std::free(t); ++g_frees; } }
int main()
{
    T* t = (T*)std::malloc(sizeof(T)); t->v.s = (long*)std::malloc(16); t->v.f = t->v.e = t->v.s;
    Delete(t); Delete(0);
    T* u = (T*)std::malloc(sizeof(T)); u->v.s = u->v.f = u->v.e = 0; Delete(u);
    if (g_frees != 3) { std::printf("00a982d0_TEST FAIL\n"); return 1; }
    std::printf("00a982d0_TEST PASS\n");
    return 0;
}