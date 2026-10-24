// Behaviour test for 0x0046656f vector<NSpeechGainManager::CDialogueLayerDef>::_M_allocate_and_copy: allocate n elements and copy-construct [first, last) into them
#include <cstdio>
#include <cstdlib>
#include <new>
struct T { int GlobalIndex; float Attack, Release, High, Mid, Low, Focus; T() { } T(const T& o) : GlobalIndex(o.GlobalIndex), Attack(o.Attack), Release(o.Release), High(o.High), Mid(o.Mid), Low(o.Low), Focus(o.Focus) { } };
static void Fill(T& t, int k) { t.GlobalIndex = k; t.Attack = k * 1.0f; t.Release = k * 2.0f; t.High = k * 3.0f; t.Mid = k * 4.0f; t.Low = k * 5.0f; t.Focus = k * 6.0f; }
static bool Same(const T& a, const T& b) { return a.GlobalIndex == b.GlobalIndex && a.Attack == b.Attack && a.Release == b.Release && a.High == b.High && a.Mid == b.Mid && a.Low == b.Low && a.Focus == b.Focus; }
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
    T src[3];
    for (int i = 0; i < 3; ++i) Fill(src[i], i + 1);
    T* p = AllocateAndCopy(5, src, src + 3);
    if (p == 0 || g_copies != 3) { std::printf("0046656f_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (!Same(p[i], src[i])) { std::printf("0046656f_TEST FAIL\n"); return 1; }
    std::free(p);
    if (AllocateAndCopy(0, src, src) != 0) { std::printf("0046656f_TEST FAIL\n"); return 1; }
    std::printf("0046656f_TEST PASS\n");
    return 0;
}