#include <cstdio>
// Behaviour model of 006b6250: the element is reduced to an id plus a counting assignment (the retail element's
// assignment is memberwise / out of line; only the copy order, count and returned pointer are under test).
static int g_assign = 0;
struct CElement1F0_006b6180 {
    long m_id;
    long m_pad[3];
    CElement1F0_006b6180& operator=(const CElement1F0_006b6180& other) { m_id = other.m_id; ++g_assign; return *this; }
};
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

CElement1F0_006b6180* CopyBackward_CElement1F0_006b6180_006b6250(CElement1F0_006b6180* first, CElement1F0_006b6180* last, CElement1F0_006b6180* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    SRandomAccessIteratorTag tag;
    // shift [0,4) right by two inside one array (the vector-insert use), then an empty range
    CElement1F0_006b6180 arr[6];
    int i;
    for (i = 0; i < 6; ++i) arr[i].m_id = 10 + i;
    CElement1F0_006b6180* r = CopyBackward_CElement1F0_006b6180_006b6250(arr, arr + 4, arr + 6, tag, 0);
    if (r != arr + 2 || g_assign != 4) { std::printf("006b6250_TEST FAIL\n"); return 1; }
    long want[6] = { 10, 11, 10, 11, 12, 13 };
    for (i = 0; i < 6; ++i) if (arr[i].m_id != want[i]) { std::printf("006b6250_TEST FAIL\n"); return 1; }
    r = CopyBackward_CElement1F0_006b6180_006b6250(arr, arr, arr + 3, tag, 0);
    if (r != arr + 3 || g_assign != 4) { std::printf("006b6250_TEST FAIL\n"); return 1; }
    std::printf("006b6250_TEST PASS\n");
    return 0;
}