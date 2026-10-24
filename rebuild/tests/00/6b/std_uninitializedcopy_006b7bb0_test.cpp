#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
struct CGameMapCell { unsigned char AtmosThemes[3]; unsigned char AtmosBlends[2]; signed char SoundTheme; unsigned char Music; };
typedef CGameMapCell TElem;
static void Fill(TElem& e, int i)
{
    e.AtmosThemes[0] = (unsigned char)i; e.AtmosThemes[1] = (unsigned char)(i + 1); e.AtmosThemes[2] = (unsigned char)(i + 2); e.AtmosBlends[0] = (unsigned char)(10 * i); e.AtmosBlends[1] = (unsigned char)(11 * i); e.SoundTheme = (signed char)-i; e.Music = (unsigned char)(200 + i);
}
static bool Check(const TElem& e, int i)
{
    return e.AtmosThemes[0] == i && e.AtmosThemes[1] == i + 1 && e.AtmosThemes[2] == i + 2 && e.AtmosBlends[0] == 10 * i && e.AtmosBlends[1] == 11 * i && e.SoundTheme == -i && e.Music == 200 + i;
}
static TElem* UninitCopy(const TElem* first, const TElem* last, TElem* result, const __false_type&)
{
    TElem* cur = result;
    for (; first != last; ++first, ++cur)
        new ((void*)cur) TElem(*first);
    return cur;
}
int main()
{
    static double srcStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    static double dstStorage[4 * sizeof(TElem) / sizeof(double) + 2];
    TElem* src = (TElem*)srcStorage;
    TElem* dst = (TElem*)dstStorage;
    int i;
    for (i = 0; i < 3; ++i) { new ((void*)(src + i)) TElem(); Fill(src[i], i + 1); }
    __false_type tag;
    TElem* end = UninitCopy(src, src + 3, dst, tag);
    bool ok = end == dst + 3;
    for (i = 0; i < 3; ++i) if (!Check(dst[i], i + 1)) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3) ok = false;
    
    if (!ok) { std::printf("006b7bb0_TEST FAIL\n"); return 1; }
    std::printf("006b7bb0_TEST PASS\n");
    return 0;
}