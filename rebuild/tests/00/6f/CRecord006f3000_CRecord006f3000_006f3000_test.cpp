#include <stdio.h>
static int g_cs, g_vs;
struct CCharString { void* m_pStringData; CCharString() : m_pStringData(0) {} CCharString(const CCharString& o) : m_pStringData(o.m_pStringData) { ++g_cs; } };
template <class T> struct CRetailVector { T* first; T* last; T* end; CRetailVector() : first(0), last(0), end(0) {} CRetailVector(const CRetailVector& o) : first(o.first), last(o.last), end(o.end) { ++g_vs; } };
struct C3DVector { float X, Y, Z; };
struct CRecord_006f3000 {
    long m_id; unsigned char m_flag04; long m_value; unsigned char m_flag0c; unsigned char m_flag0d; CCharString m_name; CRetailVector<C3DVector> m_points0; CRetailVector<C3DVector> m_points1;
    CRecord_006f3000() {}
    CRecord_006f3000(const CRecord_006f3000& other);
};
CRecord_006f3000::CRecord_006f3000(const CRecord_006f3000& other)
    : m_id(other.m_id), m_flag04(other.m_flag04), m_value(other.m_value), m_flag0c(other.m_flag0c), m_flag0d(other.m_flag0d), m_name(other.m_name), m_points0(other.m_points0), m_points1(other.m_points1) { }
int main()
{
    C3DVector pts[3]; CRecord_006f3000 a; a.m_id = 11; a.m_flag04 = 1; a.m_value = 22; a.m_flag0c = 0; a.m_flag0d = 1; a.m_name.m_pStringData = (void*)9; a.m_points0.first = pts; a.m_points0.last = pts + 3; a.m_points1.first = pts + 1; a.m_points1.last = pts + 2;
    CRecord_006f3000 b(a);
    bool ok = g_cs == 1 && g_vs == 2 && b.m_id == 11 && b.m_flag04 == 1 && b.m_value == 22 && b.m_flag0c == 0 && b.m_flag0d == 1 && b.m_name.m_pStringData == (void*)9 && b.m_points0.last == pts + 3 && b.m_points1.first == pts + 1 && sizeof(CRecord_006f3000) == 0x2c;
    if (!ok) { printf("006f3000_TEST FAIL\n"); return 1; }
    printf("006f3000_TEST PASS\n"); return 0;
}