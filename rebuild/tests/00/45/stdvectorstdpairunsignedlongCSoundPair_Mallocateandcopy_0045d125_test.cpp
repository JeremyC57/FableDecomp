// Behaviour test for 0x0045d125 vector<pair<unsigned long, CSoundPair> >::_M_allocate_and_copy: allocate n elements and copy-construct [first, last) into them
#include <cstdio>
#include <cstdlib>
#include <new>
class CSoundPair { public: int FirstSound, LastSound; };
struct T { unsigned int first; CSoundPair second; T() { } T(const T& o) : first(o.first), second(o.second) { } };
static void Fill(T& t, int k) { t.first = k; t.second.FirstSound = k * 7; t.second.LastSound = k * 9; }
static bool Same(const T& a, const T& b) { return a.first == b.first && a.second.FirstSound == b.second.FirstSound && a.second.LastSound == b.second.LastSound; }
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
    if (sizeof(T) != 12) { std::printf("0045d125_TEST FAIL\n"); return 1; }
    T src[3];
    for (int i = 0; i < 3; ++i) Fill(src[i], i + 1);
    T* p = AllocateAndCopy(5, src, src + 3);
    if (p == 0 || g_copies != 3) { std::printf("0045d125_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (!Same(p[i], src[i])) { std::printf("0045d125_TEST FAIL\n"); return 1; }
    std::free(p);
    if (AllocateAndCopy(0, src, src) != 0) { std::printf("0045d125_TEST FAIL\n"); return 1; }
    std::printf("0045d125_TEST PASS\n");
    return 0;
}