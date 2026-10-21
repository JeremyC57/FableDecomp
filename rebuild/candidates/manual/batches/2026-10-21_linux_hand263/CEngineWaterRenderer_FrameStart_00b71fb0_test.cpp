// behaviour test for 00b71fb0 CEngineWaterRenderer::FrameStart: every render list emptied (finish = start) through erase(begin, end), then sea loading
#include <cstdio>
#include <cstring>
template <class T>
static T* CopyTrivial(const T* first, const T* last, T* result)
{
    if (first == last) return result;
    unsigned int n = (const char*)last - (const char*)first;
    return (T*)((char*)std::memmove(result, first, n) + n);
}
template <class T> struct CArr {
    T* first; T* last; T* end;
    void erase(T* f, T* l) { last = CopyTrivial(l, last, f); }
    void clear() { erase(first, last); }
};
struct SEntry { float box[6]; void* mesh; };
static int g_seaLoads = 0;
struct CWater {
    CArr<void*> lists[9];
    CArr<SEntry> ice;
    void FrameStart()
    {
        for (int i = 0; i < 3; ++i) lists[i].clear();
        ice.clear();
        for (int i = 3; i < 9; ++i) lists[i].clear();
        ++g_seaLoads;
    }
};
int main()
{
    static void* storage[9][4];
    static SEntry iceStorage[3];
    CWater w;
    for (int i = 0; i < 9; ++i) { w.lists[i].first = storage[i]; w.lists[i].last = storage[i] + (i % 4); w.lists[i].end = storage[i] + 4; }
    w.ice.first = iceStorage; w.ice.last = iceStorage + 2; w.ice.end = iceStorage + 3;
    w.FrameStart();
    for (int i = 0; i < 9; ++i)
        if (w.lists[i].last != w.lists[i].first || w.lists[i].end != storage[i] + 4) { std::printf("00b71fb0_TEST FAIL\n"); return 1; }
    if (w.ice.last != w.ice.first || g_seaLoads != 1) { std::printf("00b71fb0_TEST FAIL\n"); return 1; }
    std::printf("00b71fb0_TEST PASS\n");
    return 0;
}
