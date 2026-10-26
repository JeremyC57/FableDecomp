// Behaviour test for 00410d10 DestroyRangeWideString (STLport _Destroy_Range, __false_type)
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
static int g_destroyed = 0;
static void* g_last = 0;
class CWideString { public:
    void* m_pData;
    ~CWideString() { ++g_destroyed; g_last = m_pData; }
};
struct __false_type {};
void __fastcall DestroyRangeWideString(CWideString* first, CWideString* last, __false_type)
{
    for (; first != last; ++first)
        first->~CWideString();
}
int main()
{
    static int tags[3];
    unsigned char raw[3 * sizeof(CWideString)];
    CWideString* a = (CWideString*)raw;
    for (int i = 0; i < 3; ++i) a[i].m_pData = &tags[i];
    DestroyRangeWideString(a, a + 3, __false_type());
    if (g_destroyed != 3 || g_last != &tags[2]) { std::printf("00410d10_TEST FAIL\n"); return 1; }
    DestroyRangeWideString(a, a, __false_type());
    if (g_destroyed != 3) { std::printf("00410d10_TEST FAIL\n"); return 1; }
    std::printf("00410d10_TEST PASS\n");
    return 0;
}