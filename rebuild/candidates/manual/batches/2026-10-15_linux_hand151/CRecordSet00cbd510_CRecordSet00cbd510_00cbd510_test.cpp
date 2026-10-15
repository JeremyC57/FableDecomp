// behaviour test for 0x00CBD510 ~CRecordSet_00cbd510: records, the two maps, the list, then the base -- in reverse member order
#include <cstdio>
static int g_log[8]; static int g_n = 0;
struct CBase { virtual ~CBase() { g_log[g_n++] = 9; } };
struct M { int id; ~M() { g_log[g_n++] = id; } };
struct CRecordSet : CBase {
    M m_list; M m_connections; M m_surfaces; int m_28; int m_2c; M m_records;
    CRecordSet() { m_list.id = 1; m_connections.id = 2; m_surfaces.id = 3; m_records.id = 4; }
    ~CRecordSet() {}
};
int main()
{
    { CRecordSet r; }
    if (g_n != 5 || g_log[0] != 4 || g_log[1] != 3 || g_log[2] != 2 || g_log[3] != 1 || g_log[4] != 9) { std::printf("00cbd510_TEST FAIL\n"); return 1; }
    std::printf("00cbd510_TEST PASS\n");
    return 0;
}
