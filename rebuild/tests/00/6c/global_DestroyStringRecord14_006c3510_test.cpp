// Behaviour test for 006c3510 (_Destroy_StringRecord14): every element of [first, last) is destroyed once, in order; an empty range destroys nothing.
#include <cstdio>
#include <new>
static const void* g_log[8];
static int g_count = 0;
class CCharString { public:
    void* m_pData;
    CCharString() : m_pData(0) { }
    ~CCharString() { g_log[g_count++] = this; }   // stands in for 0x0099EAE0
};
struct SStringRecord14 {
    CCharString m_name;
    long m_values[4];
};
void _Destroy_StringRecord14(SStringRecord14* first, SStringRecord14* last)
{
    for (; first != last; ++first)
        first->~SStringRecord14();
}
union UStorage { double d; void* p; long l; char bytes[sizeof(SStringRecord14) * 3]; };
int main()
{
    UStorage storage;
    SStringRecord14* items = reinterpret_cast<SStringRecord14*>(storage.bytes);
    for (int i = 0; i < 3; ++i)
        new (&items[i]) SStringRecord14();
    _Destroy_StringRecord14(items, items);
    if (g_count != 0) { std::printf("006c3510_TEST FAIL\n"); return 1; }
    _Destroy_StringRecord14(items, items + 3);
    if (g_count != 3) { std::printf("006c3510_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (g_log[i] != &items[i]) { std::printf("006c3510_TEST FAIL\n"); return 1; }
    std::printf("006c3510_TEST PASS\n");
    return 0;
}