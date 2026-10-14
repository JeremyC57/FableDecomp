#include <cstdio>
struct CDataBankEntry { long loads; long lastIndex; void* lastBank; };
template <class T> struct CArray { T* m_pFirst; T* m_pLast; T* m_pEnd; T& operator[](unsigned long i) const { return m_pFirst[i]; } };
struct CDataBank {
    void* __vftable; unsigned char _bankFile[0x1e0 - 4]; CArray<CDataBankEntry*> Entries;
    unsigned long types[8];
    unsigned long GetEntryDataType(unsigned long index) const { return types[index]; }
    CDataBankEntry* GetTextInfo(unsigned long index) const { if (GetEntryDataType(index) == 0) return Entries[index]; return 0; }
};
int main()
{
    CDataBankEntry e0, e1; CDataBankEntry* table[2] = { &e0, &e1 };
    CDataBank bank; bank.Entries.m_pFirst = table; bank.types[0] = 0; bank.types[1] = 1;
    if (bank.GetTextInfo(0) != &e0 || bank.GetTextInfo(1) != 0) { std::printf("009c7d00_TEST FAIL\n"); return 1; }
    std::printf("009c7d00_TEST PASS\n"); return 0;
}