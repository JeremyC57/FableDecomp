// Behaviour test for 006e5360 (_Destroy_StringRecordC): every element of [first, last) is destroyed once, in order; an empty range destroys nothing.
#include <cstdio>
#include <new>
static const void* g_log[8];
static int g_count = 0;
struct CCharString {
    void* m_pData;
    CCharString() : m_pData(0) { }
    ~CCharString() { g_log[g_count++] = this; }   // stands in for 0x0099EAE0
};
struct SStringRecordC {
    CCharString m_name;
    long m_values[2];
};
void _Destroy_StringRecordC(SStringRecordC* first, SStringRecordC* last)
{
    for (; first != last; ++first)
        first->~SStringRecordC();
}
union UStorage { double d; void* p; long l; char bytes[sizeof(SStringRecordC) * 3]; };
int main()
{
    UStorage storage;
    SStringRecordC* items = reinterpret_cast<SStringRecordC*>(storage.bytes);
    for (int i = 0; i < 3; ++i)
        new (&items[i]) SStringRecordC();
    _Destroy_StringRecordC(items, items);
    if (g_count != 0) { std::printf("006e5360_TEST FAIL\n"); return 1; }
    _Destroy_StringRecordC(items, items + 3);
    if (g_count != 3) { std::printf("006e5360_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (g_log[i] != &items[i]) { std::printf("006e5360_TEST FAIL\n"); return 1; }
    std::printf("006e5360_TEST PASS\n");
    return 0;
}