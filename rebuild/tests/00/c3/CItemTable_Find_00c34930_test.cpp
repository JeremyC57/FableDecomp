// Behaviour test for 00c34930: CItemTable::Find -- returns 1 with the row/column of the first entry whose target key matches, else 0
#include <cstdio>
struct CTableTarget { int pad; long m_key; };
struct CTableEntry { int pad; CTableTarget* m_pTarget; };
struct CItemTable {
    virtual unsigned int GetColumnCount(unsigned int row) = 0;
    virtual unsigned int GetRowCount() = 0;
    virtual CTableEntry* GetAt(unsigned int row, unsigned int column) = 0;
    long Find(long key, unsigned int* pRow, unsigned int* pColumn)
    {
        for (*pRow = 0; *pRow < GetRowCount(); ++*pRow)
            for (*pColumn = 0; *pColumn < GetColumnCount(*pRow); ++*pColumn)
                if (GetAt(*pRow, *pColumn)->m_pTarget->m_key == key)
                    return 1;
        return 0;
    }
    virtual ~CItemTable() { }
};
struct CTestTable : CItemTable {
    CTableTarget targets[2][3];
    CTableEntry entries[2][3];
    CTestTable()
    {
        for (int r = 0; r < 2; ++r) for (int c = 0; c < 3; ++c) { targets[r][c].m_key = r * 10 + c; entries[r][c].m_pTarget = &targets[r][c]; }
    }
    unsigned int GetColumnCount(unsigned int row) { return row == 0 ? 2 : 3; }
    unsigned int GetRowCount() { return 2; }
    CTableEntry* GetAt(unsigned int row, unsigned int column) { return &entries[row][column]; }
};
int main()
{
    CTestTable t; unsigned int row = 99, col = 99;
    if (t.Find(12, &row, &col) != 1 || row != 1 || col != 2) { std::printf("00c34930_TEST FAIL\n"); return 1; }
    if (t.Find(1, &row, &col) != 1 || row != 0 || col != 1) { std::printf("00c34930_TEST FAIL\n"); return 1; }
    if (t.Find(2, &row, &col) != 0 || row != 2) { std::printf("00c34930_TEST FAIL\n"); return 1; }   // row 0 has only 2 columns
    std::printf("00c34930_TEST PASS\n");
    return 0;
}