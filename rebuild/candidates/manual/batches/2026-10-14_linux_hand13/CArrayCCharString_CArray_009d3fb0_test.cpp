#include <cstdio>
#include <cstdlib>
static int g_erase = 0, g_dtor = 0;
struct CCharString { void* PStringData; ~CCharString() { ++g_dtor; } };
template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }
template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void Erase(T* f, T* l) { if (f == first && l == last) ++g_erase; }
    static void Destroy(T* f, T* l) { for (; f != l; ++f) f->~T(); }
    ~CArray() { if (capacity() > 0) { Erase(first, last); T* p = Exchange(first, (T*)0); T* l = Exchange(last, (T*)0); end_of_storage = 0; Destroy(p, l); if (p) std::free(p); } }
};
int main()
{
    {
        CArray<CCharString> a; a.first = (CCharString*)std::malloc(4 * sizeof(CCharString)); a.last = a.first + 3; a.end_of_storage = a.first + 4;
    }
    if (g_erase != 1 || g_dtor != 3) { std::printf("009d3fb0_TEST FAIL\n"); return 1; }
    { CArray<CCharString> e; e.first = 0; e.last = 0; e.end_of_storage = 0; }
    if (g_erase != 1 || g_dtor != 3) { std::printf("009d3fb0_TEST FAIL\n"); return 1; }
    std::printf("009d3fb0_TEST PASS\n");
    return 0;
}
