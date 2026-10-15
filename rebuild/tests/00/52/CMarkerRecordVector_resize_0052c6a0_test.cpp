// Behaviour test for CMarkerRecordVector::resize(unsigned long) (0x0052C6A0)
#include <cstdio>
#include <cstdlib>
static int g_names = 0, g_namesDead = 0;
struct CCharString {
    void* m_pData;
    CCharString() : m_pData(0) { ++g_names; }
    ~CCharString() { ++g_namesDead; }
};
struct CMarkerPointVector {
    void* m_start; void* m_finish; void* m_endOfStorage;
    CMarkerPointVector() : m_start(0), m_finish(0), m_endOfStorage(0) { }
    ~CMarkerPointVector() { if (m_start) std::free(m_start); }
};
struct SMarkerRecord {
    CCharString m_name; CMarkerPointVector m_points; long m_value; bool m_flag;
    SMarkerRecord() : m_value(0), m_flag(false) { }
};
static unsigned long g_n = 0; static bool g_valueOk = false;
struct CMarkerRecordVector {
    void resize(unsigned long n, const SMarkerRecord& value)
    {
        g_n = n;
        g_valueOk = value.m_value == 0 && !value.m_flag && value.m_points.m_start == 0 && value.m_name.m_pData == 0;
    }
    void resize(unsigned long n) { resize(n, SMarkerRecord()); }
};
#define CHECK(c) if (!(c)) { std::printf("0052c6a0_TEST FAIL\n"); return 1; }
int main()
{
    CMarkerRecordVector v;
    v.resize(7);
    CHECK(g_n == 7 && g_valueOk && g_names == 1 && g_namesDead == 1);
    std::printf("0052c6a0_TEST PASS\n");
    return 0;
}