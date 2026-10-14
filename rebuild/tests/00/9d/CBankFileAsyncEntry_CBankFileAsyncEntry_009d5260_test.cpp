#include <cstdio>
#include <cstdlib>
#include <cstring>
struct CBase { virtual void v00() {} CBase() {} };
struct CBankFileAsync {
    unsigned long m_sizes[4]; unsigned long m_alignment;
    unsigned long GetEntryDataSize(unsigned long i) const { return m_sizes[i]; }
    unsigned long Align(unsigned long size) const { return (size + m_alignment - 1) / m_alignment * m_alignment; }
};
struct CBankFileAsyncEntry : CBase {
    CBankFileAsync* m_pBank; unsigned long m_index; unsigned long m_alignedSize;
    CBankFileAsyncEntry(CBankFileAsync* bank, unsigned long index) : CBase(), m_pBank(bank), m_index(index)
    { m_alignedSize = m_pBank->Align(m_pBank->GetEntryDataSize(index)); }
};
static bool Check()
{
    CBankFileAsync bank; bank.m_sizes[0] = 1; bank.m_sizes[1] = 100; bank.m_sizes[2] = 2048; bank.m_sizes[3] = 2049; bank.m_alignment = 2048;
    CBankFileAsyncEntry e(&bank, 1);
    if (e.m_pBank != &bank || e.m_index != 1 || e.m_alignedSize != 2048) return false;
    CBankFileAsyncEntry f(&bank, 3);
    return f.m_alignedSize == 4096;
}
int main()
{
    if (!Check()) { std::printf("009d5260_TEST FAIL\n"); return 1; }
    std::printf("009d5260_TEST PASS\n");
    return 0;
}