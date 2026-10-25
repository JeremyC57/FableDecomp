// Behaviour test for 00c348f0: CItemTable::Validate -- visits every row and column count once per slot (empty body)
#include <cstdio>
struct CItemTable {
    virtual unsigned int GetColumnCount(unsigned int row) = 0;
    virtual unsigned int GetRowCount() = 0;
    void Validate()
    {
        for (unsigned int row = 0; row < GetRowCount(); ++row)
            for (unsigned int column = 0; column < GetColumnCount(row); ++column) {
            }
    }
    virtual ~CItemTable() { }
};
struct CTestTable : CItemTable {
    int rowCalls, colCalls;
    unsigned int GetColumnCount(unsigned int row) { ++colCalls; return row + 1; }
    unsigned int GetRowCount() { ++rowCalls; return 3; }
};
int main()
{
    CTestTable t; t.rowCalls = 0; t.colCalls = 0;
    t.Validate();
    // rows: 4 tests; columns: row r is tested (r+1)+1 times -> 2+3+4 = 9
    if (t.rowCalls != 4 || t.colCalls != 9) { std::printf("00c348f0_TEST FAIL\n"); return 1; }
    std::printf("00c348f0_TEST PASS\n");
    return 0;
}