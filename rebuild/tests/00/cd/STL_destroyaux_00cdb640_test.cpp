// Behaviour test for 00cdb640 (std::__destroy_aux<TeleportData*>): every element of [first, last) is destroyed (its four
// strings released, last declared first), nothing outside the range is touched, an empty range destroys nothing.
#include <cstdio>
#include <new>
static int g_live = 0;
static int g_order[64];
static int g_count = 0;
struct CCharString {                            // stands in for the engine string (dtor 0x0099EAE0)
    int m_tag;
    explicit CCharString(int tag) : m_tag(tag) { ++g_live; }
    ~CCharString() { --g_live; if (g_count < 64) g_order[g_count++] = m_tag; }
};
struct TeleportData {
    CCharString  defName;
    unsigned int appearanceSeed;
    float        health;
    CCharString  scriptName;
    CCharString  teleportTo;
    CCharString  teleportMkr;
    explicit TeleportData(int k) : defName(k * 10 + 0), appearanceSeed(7u), health(1.0f), scriptName(k * 10 + 1), teleportTo(k * 10 + 2), teleportMkr(k * 10 + 3) { }
};
struct __false_type { __false_type() { } };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
union UStorage { double d; void* p; char bytes[sizeof(TeleportData) * 4]; };
#define FAIL() { std::printf("00cdb640_TEST FAIL\n"); return 1; }
int main()
{
    UStorage storage;
    TeleportData* items = reinterpret_cast<TeleportData*>(storage.bytes);
    for (int i = 0; i < 4; ++i)
        new (&items[i]) TeleportData(i);
    if (g_live != 16) FAIL();
    __destroy_aux(items + 1, items + 1, __false_type());
    if (g_live != 16 || g_count != 0) FAIL();
    __destroy_aux(items + 1, items + 3, __false_type());
    if (g_live != 8 || g_count != 8) FAIL();
    static const int expect[8] = { 13, 12, 11, 10, 23, 22, 21, 20 };   // members in reverse order, elements in order
    for (int i = 0; i < 8; ++i)
        if (g_order[i] != expect[i]) FAIL();
    if (items[0].defName.m_tag != 0 || items[3].teleportMkr.m_tag != 33) FAIL();
    items[0].~TeleportData();
    items[3].~TeleportData();
    if (g_live != 0) FAIL();
    std::printf("00cdb640_TEST PASS\n");
    return 0;
}