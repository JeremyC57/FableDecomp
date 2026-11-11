#include <cstdio>
// Behaviour model of 0075f0e0: the element is reduced to an id plus a counting assignment (the retail element's
// assignment is memberwise / out of line; only the copy order, count and returned pointer are under test).
static int g_assign = 0;
struct CElement38_0075ef00 {
    long m_id;
    long m_pad[3];
    CElement38_0075ef00& operator=(const CElement38_0075ef00& other) { m_id = other.m_id; ++g_assign; return *this; }
};
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement38_0075ef00* CopyBackward_CElement38_0075ef00_0075f0e0(CElement38_0075ef00* first, CElement38_0075ef00* last, CElement38_0075ef00* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    SRandomAccessIteratorTag tag;
    // shift [0,4) right by two inside one array (the vector-insert use), then an empty range
    CElement38_0075ef00 arr[6];
    int i;
    for (i = 0; i < 6; ++i) arr[i].m_id = 10 + i;
    CElement38_0075ef00* r = CopyBackward_CElement38_0075ef00_0075f0e0(arr, arr + 4, arr + 6, tag, 0);
    if (r != arr + 2 || g_assign != 4) { std::printf("0075f0e0_TEST FAIL\n"); return 1; }
    long want[6] = { 10, 11, 10, 11, 12, 13 };
    for (i = 0; i < 6; ++i) if (arr[i].m_id != want[i]) { std::printf("0075f0e0_TEST FAIL\n"); return 1; }
    r = CopyBackward_CElement38_0075ef00_0075f0e0(arr, arr, arr + 3, tag, 0);
    if (r != arr + 3 || g_assign != 4) { std::printf("0075f0e0_TEST FAIL\n"); return 1; }
    std::printf("0075f0e0_TEST PASS\n");
    return 0;
}