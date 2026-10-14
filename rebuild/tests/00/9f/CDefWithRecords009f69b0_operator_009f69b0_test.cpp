#include <cstdio>
static int g_arr = 0;
template<class T> struct CArray { T* first; T* last; T* end_of_storage; CArray& operator=(const CArray& o) { first = o.first; last = o.last; end_of_storage = o.end_of_storage; ++g_arr; return *this; } };
struct SRecord34 { unsigned char _body[0x34]; };
struct SQuad { long a; long b; long c; long d; };
struct CDefWithRecords {
    CArray<SRecord34> m_records; long m_count; SQuad m_quad; long m_value0; long m_value1; long m_value2; long m_value3;
    CDefWithRecords& operator=(const CDefWithRecords& o) { m_records = o.m_records; m_count = o.m_count; m_quad = o.m_quad; m_value0 = o.m_value0; m_value1 = o.m_value1; m_value2 = o.m_value2; m_value3 = o.m_value3; return *this; }
};
int main()
{
    CDefWithRecords a, b; SRecord34 rec[2];
    b.m_records.first = rec; b.m_records.last = rec + 2; b.m_records.end_of_storage = rec + 2; b.m_count = 2;
    b.m_quad.a = 1; b.m_quad.b = 2; b.m_quad.c = 3; b.m_quad.d = 4; b.m_value0 = 10; b.m_value1 = 11; b.m_value2 = 12; b.m_value3 = 13;
    a.m_records.first = 0; a.m_count = 0; a.m_quad.d = 0; a.m_value3 = 0;
    CDefWithRecords& r = (a = b);
    if (&r != &a || g_arr != 1 || a.m_records.last != rec + 2 || a.m_count != 2 || a.m_quad.a != 1 || a.m_quad.d != 4 || a.m_value0 != 10 || a.m_value3 != 13) { std::printf("009f69b0_TEST FAIL\n"); return 1; }
    std::printf("009f69b0_TEST PASS\n");
    return 0;
}