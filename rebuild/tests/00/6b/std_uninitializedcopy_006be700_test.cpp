#include <cstdio>
#include <new>
struct __false_type { __false_type() { } };
static int g_wideCopies = 0;
class CWideString { public: const unsigned short* PData; CWideString() : PData(0) { } CWideString(const CWideString& other) : PData(other.PData) { ++g_wideCopies; } };
struct CRegisteredMusicEntry { CWideString SrcFileName, DstFileName; float Gain, Offset; bool CacheToHD; CRegisteredMusicEntry() : Gain(0), Offset(0), CacheToHD(false) { } };
template <class T1, class T2>
struct pair {
    T1 first;
    T2 second;
    pair() { }
    pair(const pair& other) : first(other.first), second(other.second) { }
};
typedef pair<long, CRegisteredMusicEntry> SMusicEntry;
static const unsigned short g_src[4] = { 1, 2, 3, 4 };
static const unsigned short g_dst[4] = { 5, 6, 7, 8 };
typedef SMusicEntry TElem;
static void Fill(TElem& e, int i)
{
    e.first = 1000 + i; e.second.SrcFileName.PData = &g_src[i]; e.second.DstFileName.PData = &g_dst[i]; e.second.Gain = 0.5f * i; e.second.Offset = 2.0f + i; e.second.CacheToHD = i == 1;
}
static bool Check(const TElem& e, int i)
{
    return e.first == 1000 + i && e.second.SrcFileName.PData == &g_src[i] && e.second.DstFileName.PData == &g_dst[i] && e.second.Gain == 0.5f * i && e.second.Offset == 2.0f + i && e.second.CacheToHD == (i == 1);
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
    if (g_wideCopies != 6) ok = false;
    if (!ok) { std::printf("006be700_TEST FAIL\n"); return 1; }
    std::printf("006be700_TEST PASS\n");
    return 0;
}