#include <cstdio>
#include <cstdlib>
struct CCharString { void* PStringData; };
struct SStringPair8 { CCharString first; long second; };
struct SAllocator { SAllocator() { } };
static int g_erase = 0, g_destroy = 0;
void __fastcall DestroyRange(SStringPair8* f, SStringPair8* l, const SAllocator&) { g_destroy += (int)(l - f); }
template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }
template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void Erase(T* f, T* l) { if (f == first && l == last) ++g_erase; }
    void DeallocateMemory() { if (capacity() > 0) { Erase(first, last); T* p = Exchange(first, (T*)0); T* l = Exchange(last, (T*)0); end_of_storage = 0; DestroyRange(p, l, SAllocator()); if (p) std::free(p); } }
};
int main()
{
    CArray<SStringPair8> a; a.first = (SStringPair8*)std::malloc(4 * sizeof(SStringPair8)); a.last = a.first + 3; a.end_of_storage = a.first + 4;
    a.DeallocateMemory();
    if (a.first || a.last || a.end_of_storage || g_erase != 1 || g_destroy != 3) { std::printf("009c76d0_TEST FAIL\n"); return 1; }
    a.DeallocateMemory();
    if (g_erase != 1) { std::printf("009c76d0_TEST FAIL\n"); return 1; }
    std::printf("009c76d0_TEST PASS\n");
    return 0;
}