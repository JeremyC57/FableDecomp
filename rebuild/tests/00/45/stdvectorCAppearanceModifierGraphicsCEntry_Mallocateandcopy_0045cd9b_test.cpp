// Behaviour test for 0x0045cd9b vector<CAppearanceModifierGraphics::CEntry>::_M_allocate_and_copy: allocate n elements and copy-construct [first, last) into them
#include <cstdio>
#include <cstdlib>
#include <new>
struct T { int Type, MorphType, BankIndex; float RenderSizeX, StartMorphStrength, EndMorphStrength; };
static void Fill(T& t, int k) { t.Type = k; t.MorphType = k * 2; t.BankIndex = k * 3; t.RenderSizeX = k * 0.5f; t.StartMorphStrength = k * 0.25f; t.EndMorphStrength = k * 4.0f; }
static bool Same(const T& a, const T& b) { return a.Type == b.Type && a.MorphType == b.MorphType && a.BankIndex == b.BankIndex && a.RenderSizeX == b.RenderSizeX && a.StartMorphStrength == b.StartMorphStrength && a.EndMorphStrength == b.EndMorphStrength; }
static int g_copies = 0;
static void Construct(T* p, const T& v) { if (p) { new (p) T(v); ++g_copies; } }
static T* AllocateAndCopy(unsigned int n, const T* first, const T* last)
{
    T* result = n != 0 ? static_cast<T*>(std::malloc(n * sizeof(T))) : 0;
    T* cur = result;
    for (; first != last; ++first, ++cur)
        Construct(cur, *first);
    return result;
}
int main()
{
    if (sizeof(T) != 24) { std::printf("0045cd9b_TEST FAIL\n"); return 1; }
    T src[3];
    for (int i = 0; i < 3; ++i) Fill(src[i], i + 1);
    T* p = AllocateAndCopy(5, src, src + 3);
    if (p == 0 || g_copies != 3) { std::printf("0045cd9b_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (!Same(p[i], src[i])) { std::printf("0045cd9b_TEST FAIL\n"); return 1; }
    std::free(p);
    if (AllocateAndCopy(0, src, src) != 0) { std::printf("0045cd9b_TEST FAIL\n"); return 1; }
    std::printf("0045cd9b_TEST PASS\n");
    return 0;
}