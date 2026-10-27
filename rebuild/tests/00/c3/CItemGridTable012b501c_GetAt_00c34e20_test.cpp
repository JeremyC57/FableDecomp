// Test for 0x00C34E20 CItemGridTable_012b501c::GetAt: row lookup through the outer array, column through the row.
#include <cstdio>
struct CTableEntry { int id; };
struct CRow { CTableEntry** m_data; unsigned int m_capacity; unsigned int m_count;
    CTableEntry* GetAt(unsigned int i) { return m_data[i]; } };
struct CRows { CRow** m_data; unsigned int m_capacity; unsigned int m_count; };
struct CItemGridTable { CRows m_rows;
    CTableEntry* GetAt(unsigned int row, unsigned int column) { CRow* entry = m_rows.m_data[row]; return entry->GetAt(column); } };
int main()
{
    CTableEntry a = {1}, b = {2}, c = {3};
    CTableEntry* r0[2] = {&a, &b}; CTableEntry* r1[1] = {&c};
    CRow row0 = {r0, 2, 2}, row1 = {r1, 1, 1};
    CRow* rows[2] = {&row0, &row1};
    CItemGridTable t; t.m_rows.m_data = rows; t.m_rows.m_capacity = 2; t.m_rows.m_count = 2;
    if (t.GetAt(0, 1) != &b || t.GetAt(1, 0) != &c || t.GetAt(0, 0) != &a) { std::printf("00c34e20_TEST FAIL\n"); return 1; }
    std::printf("00c34e20_TEST PASS\n");
    return 0;
}