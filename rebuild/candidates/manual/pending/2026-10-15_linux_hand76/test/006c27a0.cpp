#include <cstdio>
static int g_order = 0, g_vectorAt = 0, g_mapAt = 0;
struct CRecordVector { long n; CRecordVector& operator=(const CRecordVector& o) { n = o.n; g_vectorAt = ++g_order; return *this; } };
struct CRecordMap { long n; CRecordMap& operator=(const CRecordMap& o) { n = o.n; g_mapAt = ++g_order; return *this; } };
struct SMappedRecords {
    CRecordMap m_map; CRecordVector m_records; long m_value;
    void Set(const CRecordVector& records, const CRecordMap& map, long value) { m_records = records; m_map = map; m_value = value; }
};
int main()
{
    CRecordVector v; v.n = 3; CRecordMap m; m.n = 4;
    SMappedRecords r; r.m_map.n = 0; r.m_records.n = 0; r.m_value = 0;
    r.Set(v, m, 99);
    if (r.m_records.n != 3 || r.m_map.n != 4 || r.m_value != 99 || g_vectorAt != 1 || g_mapAt != 2) { std::printf("006c27a0_TEST FAIL\n"); return 1; }
    std::printf("006c27a0_TEST PASS\n");
    return 0;
}
