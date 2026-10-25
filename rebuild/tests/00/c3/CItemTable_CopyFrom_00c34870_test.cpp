// Behaviour test for 00c34870: CItemTable::CopyFrom -- every source entry is inserted at its row, the target table is returned
#include <cstdio>
struct CTableEntry { int id; };
struct CItemTable {
    virtual long Insert(unsigned int row, CTableEntry* entry) = 0;
    virtual unsigned int GetColumnCount(unsigned int row) = 0;
    virtual unsigned int GetRowCount() = 0;
    virtual CTableEntry* GetAt(unsigned int row, unsigned int column) = 0;
    CItemTable& CopyFrom(CItemTable& source)
    {
        for (unsigned int row = 0; row < source.GetRowCount(); ++row)
            for (unsigned int column = 0; column < source.GetColumnCount(row); ++column)
                Insert(row, source.GetAt(row, column));
        return *this;
    }
    virtual ~CItemTable() { }
};
struct CTestTable : CItemTable {
    CTableEntry* slots[8]; unsigned int rows[8]; int n;
    CTableEntry src[3];
    CTestTable() : n(0) { src[0].id = 10; src[1].id = 11; src[2].id = 20; }
    long Insert(unsigned int row, CTableEntry* entry) { rows[n] = row; slots[n++] = entry; return 0; }
    unsigned int GetColumnCount(unsigned int row) { return row == 0 ? 2 : 1; }
    unsigned int GetRowCount() { return 2; }
    CTableEntry* GetAt(unsigned int row, unsigned int column) { return &src[row * 2 + column]; }
};
int main()
{
    CTestTable a, b;
    if (&b.CopyFrom(a) != &b) { std::printf("00c34870_TEST FAIL\n"); return 1; }
    if (b.n != 3 || b.slots[0] != &a.src[0] || b.rows[0] != 0 || b.slots[1] != &a.src[1] || b.rows[1] != 0 || b.slots[2] != &a.src[2] || b.rows[2] != 1) { std::printf("00c34870_TEST FAIL\n"); return 1; }
    std::printf("00c34870_TEST PASS\n");
    return 0;
}