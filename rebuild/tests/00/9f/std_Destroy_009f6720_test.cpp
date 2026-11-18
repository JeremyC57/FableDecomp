// Behaviour test for 009f6720 (_Destroy_Record34): every element of [first, last) is destroyed once, in order; an empty range destroys nothing.
#include <cstdio>
#include <new>
static const void* g_log[8];
static int g_count = 0;
struct SRecord34 {
    long m_words[13];
    SRecord34() { m_words[0] = 0; }
    ~SRecord34() { g_log[g_count++] = this; }     // stands in for 0x00A03B30 (empty in retail)
};
void _Destroy_Record34(SRecord34* first, SRecord34* last)
{
    for (; first != last; ++first)
        first->~SRecord34();
}
union UStorage { double d; void* p; long l; char bytes[sizeof(SRecord34) * 3]; };
int main()
{
    UStorage storage;
    SRecord34* items = reinterpret_cast<SRecord34*>(storage.bytes);
    for (int i = 0; i < 3; ++i)
        new (&items[i]) SRecord34();
    _Destroy_Record34(items, items);
    if (g_count != 0) { std::printf("009f6720_TEST FAIL\n"); return 1; }
    _Destroy_Record34(items, items + 3);
    if (g_count != 3) { std::printf("009f6720_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (g_log[i] != &items[i]) { std::printf("009f6720_TEST FAIL\n"); return 1; }
    std::printf("009f6720_TEST PASS\n");
    return 0;
}