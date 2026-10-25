// Link test for 0x00A5DDE0: the source object's __uninitialized_copy (first, last, dest, const __false_type&) copies
// 8-byte elements in place and returns the advanced destination.
#include <cstdio>
struct SElement8 { long m_00; long m_04; };
struct __false_type { };
SElement8* __fastcall UninitCopy_SElement8_00a5dde0(SElement8* first, SElement8* last, SElement8* dest, const __false_type&);
int main()
{
    SElement8 src[3] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
    SElement8 dst[3] = { { 0, 0 }, { 0, 0 }, { 9, 9 } };
    __false_type tag;
    SElement8* e = UninitCopy_SElement8_00a5dde0(src, src + 2, dst, tag);
    int ok = e == dst + 2 && dst[0].m_00 == 1 && dst[0].m_04 == 2 && dst[1].m_00 == 3 && dst[1].m_04 == 4 && dst[2].m_00 == 9;
    ok = ok && UninitCopy_SElement8_00a5dde0(src, src, dst + 1, tag) == dst + 1;
    if (!ok) { std::printf("00a5dde0_TEST FAIL\n"); return 1; }
    std::printf("00a5dde0_TEST PASS\n");
    return 0;
}
