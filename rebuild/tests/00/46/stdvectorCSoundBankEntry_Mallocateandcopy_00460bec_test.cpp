// Behaviour test for 0x00460bec vector<CSoundBankEntry>::_M_allocate_and_copy: allocate n elements and copy-construct [first, last) into them
#include <cstdio>
#include <cstdlib>
#include <new>
struct T { unsigned int ID; int Filename, SymbolFilename, LipSyncBankHandle; bool DVD; };
static void Fill(T& t, int k) { t.ID = k; t.Filename = k + 10; t.SymbolFilename = k + 20; t.LipSyncBankHandle = k + 30; t.DVD = (k & 1) != 0; }
static bool Same(const T& a, const T& b) { return a.ID == b.ID && a.Filename == b.Filename && a.SymbolFilename == b.SymbolFilename && a.LipSyncBankHandle == b.LipSyncBankHandle && a.DVD == b.DVD; }
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
    if (sizeof(T) != 20) { std::printf("00460bec_TEST FAIL\n"); return 1; }
    T src[3];
    for (int i = 0; i < 3; ++i) Fill(src[i], i + 1);
    T* p = AllocateAndCopy(5, src, src + 3);
    if (p == 0 || g_copies != 3) { std::printf("00460bec_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) if (!Same(p[i], src[i])) { std::printf("00460bec_TEST FAIL\n"); return 1; }
    std::free(p);
    if (AllocateAndCopy(0, src, src) != 0) { std::printf("00460bec_TEST FAIL\n"); return 1; }
    std::printf("00460bec_TEST PASS\n");
    return 0;
}