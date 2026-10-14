#include <cstdio>
#include <cstdlib>
struct SElement16 { unsigned long a, b, c, d; };
static int g_erase = 0;
template<class T> struct CArray {
    T* first; T* last; T* end_of_storage;
    unsigned long capacity() const { return (unsigned long)(end_of_storage - first); }
    void Erase(T* f, T* l) { if (f == first && l == last) ++g_erase; last = f; }
    void DeallocateMemory() { if (capacity() > 0) { Erase(first, last); T* p = first; first = 0; last = 0; end_of_storage = 0; if (p) std::free(p); } }
};
int main()
{
    CArray<SElement16> a; a.first = (SElement16*)std::malloc(4 * sizeof(SElement16)); a.last = a.first + 2; a.end_of_storage = a.first + 4;
    a.DeallocateMemory();
    if (a.first || a.last || a.end_of_storage || g_erase != 1) { std::printf("00a74c60_TEST FAIL\n"); return 1; }
    a.DeallocateMemory();
    if (g_erase != 1) { std::printf("00a74c60_TEST FAIL\n"); return 1; }
    std::printf("00a74c60_TEST PASS\n");
    return 0;
}
