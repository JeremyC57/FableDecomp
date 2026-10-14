#include <cstdio>
struct CBankFileAsyncData { bool finished; bool IsFinished() const { return finished; } };
struct CTextEntry { unsigned char _pad[0x38]; CBankFileAsyncData* m_pAsyncData; };
struct CDataBankTextEntry {
    CTextEntry* PText; long NarratorIndex;
    bool IsLoaded() const { if (PText != 0) { if (PText->m_pAsyncData == 0 || PText->m_pAsyncData->IsFinished()) return true; } return false; }
};
int main()
{
    CBankFileAsyncData pending = { false }, done = { true }; CTextEntry t; CDataBankTextEntry e = { 0, 0 };
    if (e.IsLoaded()) { std::printf("009c7be0_TEST FAIL\n"); return 1; }
    e.PText = &t; t.m_pAsyncData = 0; if (!e.IsLoaded()) { std::printf("009c7be0_TEST FAIL\n"); return 1; }
    t.m_pAsyncData = &pending; if (e.IsLoaded()) { std::printf("009c7be0_TEST FAIL\n"); return 1; }
    t.m_pAsyncData = &done; if (!e.IsLoaded()) { std::printf("009c7be0_TEST FAIL\n"); return 1; }
    std::printf("009c7be0_TEST PASS\n"); return 0;
}