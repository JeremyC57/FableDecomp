#include <cstdio>
#include <cstdlib>
#include <new>
struct CEntry { long MorphType; bool Blending; long Location; unsigned long LocationName; long LocationIndex, ParticleIndex, SecondParticleIndex; float StartAtMorphStrength, EndAtMorphStrength; };
struct T { long first; CEntry second; T() { } T(const T& p) : first(p.first), second(p.second) { } };
static void Fill(T& t, int i) { t.first = i; t.second.MorphType = i; t.second.Blending = (i & 1) != 0; t.second.Location = 2 * i; t.second.LocationName = 0x1000u + i; t.second.LocationIndex = i + 5; t.second.ParticleIndex = 9 * i; t.second.SecondParticleIndex = -i; t.second.StartAtMorphStrength = 0.25f * i; t.second.EndAtMorphStrength = 1.0f + i; }
static bool Same(const T& a, const T& b) { return a.first == b.first && a.second.Blending == b.second.Blending && a.second.LocationName == b.second.LocationName && a.second.SecondParticleIndex == b.second.SecondParticleIndex && a.second.EndAtMorphStrength == b.second.EndAtMorphStrength && a.second.ParticleIndex == b.second.ParticleIndex; }
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
    if (!ok) { std::printf("004e7279_TEST FAIL\n"); return 1; }
    std::printf("004e7279_TEST PASS\n");
    return 0;
}