#include <cstdio>
#include <cstring>
struct CSlotTable {
    virtual long Kind() { return 1; }
    long m_slots[3];
    long m_count;
    CSlotTable() { std::memset(m_slots, 0, sizeof(m_slots)); m_count = 0; }
};
int main()
{
    int ok = 1;
    CSlotTable t;
    for (int i = 0; i < 3; ++i) if (t.m_slots[i] != 0) ok = 0;
    if (t.m_count != 0 || t.Kind() != 1) ok = 0;
    if (!ok) { std::printf("00c39f10_TEST FAIL\n"); return 1; }
    std::printf("00c39f10_TEST PASS\n");
    return 0;
}
