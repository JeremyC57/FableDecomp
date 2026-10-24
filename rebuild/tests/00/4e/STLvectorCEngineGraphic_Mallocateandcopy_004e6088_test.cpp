#include <cstdio>
#include <cstdlib>
#include <new>
#pragma pack(push, 2)
struct T { unsigned long BankIndex; float AnimStep; float RenderSizeX; bool AdditiveAlpha; unsigned char Type; };
#pragma pack(pop)
static void Fill(T& t, int i) { t.BankIndex = 100 + i; t.AnimStep = 0.5f * i; t.RenderSizeX = 2.0f * i; t.AdditiveAlpha = (i & 1) != 0; t.Type = (unsigned char)(7 * i); }
static bool Same(const T& a, const T& b) { return a.BankIndex == b.BankIndex && a.AnimStep == b.AnimStep && a.RenderSizeX == b.RenderSizeX && a.AdditiveAlpha == b.AdditiveAlpha && a.Type == b.Type; }
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
    if (!ok) { std::printf("004e6088_TEST FAIL\n"); return 1; }
    std::printf("004e6088_TEST PASS\n");
    return 0;
}