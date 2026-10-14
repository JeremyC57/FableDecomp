#include <cstdio>
static int g_strAssign = 0;
struct CWideString { void* m_pData; CWideString& operator=(const CWideString& o) { m_pData = o.m_pData; ++g_strAssign; return *this; } };
struct STriple_00543530 { long m_x, m_y, m_z; };
struct STextRecord_00543530 { STriple_00543530 m_position; STriple_00543530 m_size; long m_a; long m_b; long m_c; CWideString m_text; long m_d;
    STextRecord_00543530& operator=(const STextRecord_00543530& o) { m_position = o.m_position; m_size = o.m_size; m_a = o.m_a; m_b = o.m_b; m_c = o.m_c; m_text = o.m_text; m_d = o.m_d; return *this; } };
int main() {
    STextRecord_00543530 a, b; a.m_position.m_x = 1; a.m_position.m_y = 2; a.m_position.m_z = 3; a.m_size.m_x = 4; a.m_size.m_y = 5; a.m_size.m_z = 6; a.m_a = 7; a.m_b = 8; a.m_c = 9; a.m_text.m_pData = &a; a.m_d = 10;
    b.m_text.m_pData = 0; STextRecord_00543530& r = (b = a);
    if (&r != &b || b.m_position.m_z != 3 || b.m_size.m_x != 4 || b.m_size.m_z != 6 || b.m_a != 7 || b.m_b != 8 || b.m_c != 9 || b.m_text.m_pData != &a || b.m_d != 10 || g_strAssign != 1) { std::printf("00543530_TEST FAIL\n"); return 1; }
    std::printf("00543530_TEST PASS\n"); return 0;
}
