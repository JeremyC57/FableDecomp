#include <cstdio>
#include <cstdlib>
#include <new>
class CTextureMorphEntry { public: long MorphType, TextureLayer, ReplacingTextureIndex, BankIndex, Blend; };
struct T { long first; CTextureMorphEntry second; T() { } T(const T& p) : first(p.first), second(p.second) { } };
static void Fill(T& t, int i) { t.first = i; t.second.MorphType = 10 * i; t.second.TextureLayer = i + 1; t.second.ReplacingTextureIndex = -i; t.second.BankIndex = 3 * i; t.second.Blend = i * i; }
static bool Same(const T& a, const T& b) { return a.first == b.first && a.second.MorphType == b.second.MorphType && a.second.TextureLayer == b.second.TextureLayer && a.second.ReplacingTextureIndex == b.second.ReplacingTextureIndex && a.second.BankIndex == b.second.BankIndex && a.second.Blend == b.second.Blend; }
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
    if (!ok) { std::printf("004e70cf_TEST FAIL\n"); return 1; }
    std::printf("004e70cf_TEST PASS\n");
    return 0;
}