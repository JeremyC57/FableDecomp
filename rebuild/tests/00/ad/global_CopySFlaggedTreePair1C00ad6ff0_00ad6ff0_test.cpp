#include <cstdio>
// Behaviour model of 00ad6ff0: the element is reduced to an id plus a counting assignment (the retail element's
// assignment is memberwise / out of line; only the copy order, count and returned pointer are under test).
static int g_assign = 0;
struct SFlaggedTreePair1C {
    long m_id;
    long m_pad[3];
    SFlaggedTreePair1C& operator=(const SFlaggedTreePair1C& other) { m_id = other.m_id; ++g_assign; return *this; }
};
struct SRandomAccessIteratorTag { SRandomAccessIteratorTag() { } };

SFlaggedTreePair1C* Copy_SFlaggedTreePair1C_00ad6ff0(SFlaggedTreePair1C* first, SFlaggedTreePair1C* last, SFlaggedTreePair1C* result, SRandomAccessIteratorTag category, long* distance)
{
    for (long n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}

int main()
{
    SRandomAccessIteratorTag tag;
    // forward copy of [0,4) onto [2,6) in a separate buffer, then an in-place shift left
    SFlaggedTreePair1C src[4], dst[6];
    int i;
    for (i = 0; i < 4; ++i) src[i].m_id = 10 + i;
    for (i = 0; i < 6; ++i) dst[i].m_id = -1;
    SFlaggedTreePair1C* r = Copy_SFlaggedTreePair1C_00ad6ff0(src, src + 4, dst + 2, tag, 0);
    if (r != dst + 6 || g_assign != 4 || dst[0].m_id != -1 || dst[1].m_id != -1) { std::printf("00ad6ff0_TEST FAIL\n"); return 1; }
    for (i = 0; i < 4; ++i) if (dst[2 + i].m_id != 10 + i) { std::printf("00ad6ff0_TEST FAIL\n"); return 1; }
    r = Copy_SFlaggedTreePair1C_00ad6ff0(dst + 3, dst + 6, dst + 2, tag, 0);
    if (r != dst + 5 || dst[2].m_id != 11 || dst[3].m_id != 12 || dst[4].m_id != 13 || dst[5].m_id != 13) { std::printf("00ad6ff0_TEST FAIL\n"); return 1; }
    r = Copy_SFlaggedTreePair1C_00ad6ff0(src, src, dst, tag, 0);
    if (r != dst || g_assign != 7) { std::printf("00ad6ff0_TEST FAIL\n"); return 1; }
    std::printf("00ad6ff0_TEST PASS\n");
    return 0;
}