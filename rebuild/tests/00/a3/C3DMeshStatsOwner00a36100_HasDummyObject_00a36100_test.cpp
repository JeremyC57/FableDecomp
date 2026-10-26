#include <cstdio>
#include <cstdlib>
#include <cstring>
class CCharStringData { public: const char* m_pChars; unsigned long m_length; }; static const char g_empty[] = ""; class CCharString { public: CCharStringData* m_pStringData; unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; } const char* GetChars() const { return m_pStringData == 0 ? g_empty : m_pStringData->m_pChars; } };
static const char* g_chars = 0; static unsigned long g_hash = 99; static unsigned long Calc(unsigned long, const char* d, unsigned long n) { g_chars = d; return n * 7; } struct CStats { long HasDummyObject(unsigned long h) { g_hash = h; return (long)h; } };
struct COwner { long m_0, m_4; CStats* m_pStats; long HasDummyObject(const CCharString& name) { unsigned long length = name.GetLength(); const char* chars = name.GetChars(); CStats* stats = m_pStats; return stats->HasDummyObject(Calc(0, chars, length)); } };

int main() {
    CStats st; COwner o; o.m_pStats = &st; CCharStringData d; d.m_pChars = "xy"; d.m_length = 2; CCharString s; s.m_pStringData = &d; if (o.HasDummyObject(s) != 14 || g_hash != 14 || g_chars != d.m_pChars) { std::printf("00a36100_TEST FAIL\n"); return 1; } s.m_pStringData = 0; if (o.HasDummyObject(s) != 0 || g_chars != g_empty) { std::printf("00a36100_TEST FAIL\n"); return 1; }
    std::printf("00a36100_TEST PASS\n"); return 0;
}