// Behaviour test for 005684a0 (_Destroy_LongWideStringPair): every pair's CWideString is destroyed once, in order; an empty range destroys nothing.
#include <cstdio>
#include <new>
static const void* g_log[8];
static int g_count = 0;
struct CWideString {
    void* m_pData;
    CWideString() : m_pData(0) { }
    ~CWideString() { g_log[g_count++] = this; }   // stands in for 0x0099B510
};
struct SLongWideString {
    long first;
    CWideString second;
};
void _Destroy_LongWideStringPair(SLongWideString* first, SLongWideString* last)
{
    for (; first != last; ++first)
        first->~SLongWideString();
}
union UStorage { double d; void* p; char bytes[sizeof(SLongWideString) * 3]; };
int main()
{
    UStorage storage;
    SLongWideString* items = reinterpret_cast<SLongWideString*>(storage.bytes);
    for (int i = 0; i < 3; ++i) {
        new (&items[i]) SLongWideString();
        items[i].first = i;
    }
    _Destroy_LongWideStringPair(items, items);
    if (g_count != 0) { std::printf("005684a0_TEST FAIL\n"); return 1; }
    _Destroy_LongWideStringPair(items, items + 3);
    if (g_count != 3) { std::printf("005684a0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (g_log[i] != &items[i].second) { std::printf("005684a0_TEST FAIL\n"); return 1; }
    std::printf("005684a0_TEST PASS\n");
    return 0;
}