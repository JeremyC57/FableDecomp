#include <cstdio>
#include <cstdlib>
struct SElement12 { unsigned char _body[0xc]; };
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };
static int g_copies = 0;
SElement12* __fastcall Copy_SElement12_009b1620(SElement12* first, SElement12* last, SElement12* result, const SRandomAccessIteratorTag&, long*) { ++g_copies; for (long n = last - first; n > 0; --n) { *result = *first; ++first; ++result; } return result; }
template<class P> P Exchange(P& slot, P value) { P old = slot; slot = value; return old; }
template<class T> class CArray { public:
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void erase(T* f, T* l) { last = Copy_SElement12_009b1620(l, last, f, SRandomAccessIteratorTag(), (long*)0); }
    void DeallocateMemory() { if (capacity() > 0) { erase(first, last); T* p = Exchange(first, (T*)0); Exchange(last, (T*)0); end_of_storage = 0; if (p) std::free(p); } }
};
int main()
{
    CArray<SElement12> a; a.first = (SElement12*)std::malloc(4 * sizeof(SElement12)); a.last = a.first + 2; a.end_of_storage = a.first + 4;
    a.DeallocateMemory();
    if (a.first || a.last || a.end_of_storage || g_copies != 1) { std::printf("009b5f30_TEST FAIL\n"); return 1; }
    a.DeallocateMemory();
    if (g_copies != 1) { std::printf("009b5f30_TEST FAIL\n"); return 1; }
    std::printf("009b5f30_TEST PASS\n");
    return 0;
}