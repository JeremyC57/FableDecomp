#include <cstdio>
// Behaviour test for 00a5eea0: every existing entry is flagged for update; the result counts the entries whose
// update flag could not be set; missing entries are skipped.
struct CBankFile {
    unsigned long Size;
    bool          m_exists[8];
    bool          m_accepts[8];
    int           m_setCalls;
    bool EntryExists(unsigned long index) { return m_exists[index]; }
    bool EntrySetUpdate(unsigned long index, bool update) { ++m_setCalls; return update && m_accepts[index]; }
    unsigned long AdjustUpdateEntries();
};
unsigned long CBankFile::AdjustUpdateEntries()
{
    unsigned long failed = 0;
    for (unsigned long index = 0; index < Size; ++index) {
        if (EntryExists(index) && !EntrySetUpdate(index, true))
            ++failed;
    }
    return failed;
}
int main()
{
    CBankFile b; b.Size = 6; b.m_setCalls = 0;
    static const bool ex[6] = { true, false, true, true, false, true };
    static const bool ac[6] = { true, false, false, true, false, false };
    for (int i = 0; i < 8; ++i) { b.m_exists[i] = i < 6 ? ex[i] : false; b.m_accepts[i] = i < 6 ? ac[i] : false; }
    unsigned long r = b.AdjustUpdateEntries();
    CBankFile e; e.Size = 0; e.m_setCalls = 0;
    if (r != 2 || b.m_setCalls != 4 || e.AdjustUpdateEntries() != 0 || e.m_setCalls != 0) {
        std::printf("00a5eea0_TEST FAIL\n"); return 1;
    }
    std::printf("00a5eea0_TEST PASS\n");
    return 0;
}