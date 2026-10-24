#include <cstdio>
#include <cstdlib>
#include <new>
struct CEntry { unsigned long MorphName; short Priority; bool SupressesLowerMorphs; };
struct T { unsigned long first; CEntry second; T() { } T(const T& p) : first(p.first), second(p.second) { } };
static void Fill(T& t, int i) { t.first = 0xABC0u + i; t.second.MorphName = 77u * i; t.second.Priority = (short)(-3 * i); t.second.SupressesLowerMorphs = (i % 3) == 0; }
static bool Same(const T& a, const T& b) { return a.first == b.first && a.second.MorphName == b.second.MorphName && a.second.Priority == b.second.Priority && a.second.SupressesLowerMorphs == b.second.SupressesLowerMorphs; }
static int g_mallocs = 0;
static T* AllocateAndCopy(unsigned long n, const T* first, const T* last)
{
    T* result = n != 0 ? static_cast<T*>(std::malloc(n * sizeof(T))) : 0;
    if (result) ++g_mallocs;
    T* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) T(*first);
    return result;
}
int main()
{
    bool ok = true;
    T src[4];
    for (int i = 0; i < 4; ++i) Fill(src[i], i + 1);
    T* p = AllocateAndCopy(6, src, src + 4);              // capacity above the copied count
    if (!p || g_mallocs != 1) ok = false;
    for (int i = 0; ok && i < 4; ++i) if (!Same(p[i], src[i])) ok = false;
    std::free(p);
    T* q = AllocateAndCopy(0, src, src);                  // nothing: no allocation, null result
    if (q != 0 || g_mallocs != 1) ok = false;
    if (!ok) { std::printf("004e741d_TEST FAIL\n"); return 1; }
    std::printf("004e741d_TEST PASS\n");
    return 0;
}