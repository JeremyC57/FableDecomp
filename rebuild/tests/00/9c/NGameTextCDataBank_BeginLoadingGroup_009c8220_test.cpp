#include <cstdio>
struct CDataBank;
struct CDataBankEntry { long loads; long lastIndex; CDataBank* lastBank; void BeginLoading(unsigned long i, CDataBank* b) { ++loads; lastIndex = i; lastBank = b; } };
template <class T> class CArray { public: T* m_pFirst; T* m_pLast; T* m_pEnd; T& operator[](unsigned long i) const { return m_pFirst[i]; } };
struct CDataBank {
    void* __vftable; unsigned char _bankFile[0x1e0 - 4]; CArray<CDataBankEntry*> Entries;
    unsigned long types[8];
    unsigned long GetEntryDataType(unsigned long index) const { return types[index]; }
    void BeginLoadingGroup(unsigned long index) { if (GetEntryDataType(index) == 1) { CDataBankEntry* e = Entries[index]; if (e) e->BeginLoading(index, this); } }
};
int main()
{
    CDataBankEntry e0 = { 0, -1, 0 }, e2 = { 0, -1, 0 }; CDataBankEntry* table[3] = { &e0, 0, &e2 };
    CDataBank bank; bank.Entries.m_pFirst = table; bank.types[0] = 1; bank.types[1] = 1; bank.types[2] = 0;
    bank.BeginLoadingGroup(0); bank.BeginLoadingGroup(1); bank.BeginLoadingGroup(2);
    if (e0.loads != 1 || e0.lastIndex != 0 || e0.lastBank != &bank || e2.loads != 0) { std::printf("009c8220_TEST FAIL\n"); return 1; }
    std::printf("009c8220_TEST PASS\n"); return 0;
}