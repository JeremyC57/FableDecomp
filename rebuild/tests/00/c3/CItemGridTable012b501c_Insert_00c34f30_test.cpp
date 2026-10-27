// Behaviour test for 0x00C34F30 CItemGridTable_012b501c::Insert
#include <cstdio>
struct Row { int* m_data; unsigned int m_capacity; unsigned int m_count; int m_store[8];
    Row() : m_data(m_store), m_capacity(8), m_count(0) {}
    void Add(int v) { m_data[m_count++] = v; } };
struct Grid {
    Row* m_data[8]; unsigned int m_capacity; unsigned int m_count;
    int Insert(unsigned int row, int entry)
    {
        unsigned int count = m_count;
        if (row < count) { m_data[row]->Add(entry); return 0; }
        Row* newRow = new Row;
        newRow->Add(entry);
        m_data[m_count++] = newRow;
        return 0;
    }
};
int main()
{
    Grid g; g.m_capacity = 8; g.m_count = 0;
    g.Insert(0, 10);                 // past the end: a new row
    g.Insert(0, 11);                 // existing row 0
    g.Insert(7, 20);                 // past the end again: appended as row 1, not row 7
    if (g.m_count != 2 || g.m_data[0]->m_count != 2 || g.m_data[0]->m_data[1] != 11 || g.m_data[1]->m_data[0] != 20) {
        std::printf("00c34f30_TEST FAIL\n"); return 1;
    }
    delete g.m_data[0]; delete g.m_data[1];
    std::printf("00c34f30_TEST PASS\n");
    return 0;
}