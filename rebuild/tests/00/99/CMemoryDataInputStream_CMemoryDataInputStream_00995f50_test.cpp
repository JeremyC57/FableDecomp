#include <cstdio>
#include <cstdlib>
#include <cstring>
class CCharStringData { public: const char* m_pChars; unsigned long m_length; }; static const char g_empty[] = ""; class CCharString { public: CCharStringData* m_pStringData; unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; } const char* GetChars() const { return m_pStringData == 0 ? g_empty : m_pStringData->m_pChars; } };
static long g_base = 0; struct CBaseNC { virtual void v00() { } CBaseNC() { ++g_base; } };
struct SState { long m_position; long m_size; const char* m_pData; long m_flags; long m_sizeCopy; SState() : m_position(0), m_size(0), m_pData(0), m_flags(0), m_sizeCopy(0) { } };
struct CStream : CBaseNC, SState { const char* m_pDataCopy; void SetSource(const char* d, unsigned long n) { m_pData = d; m_pDataCopy = d; m_position = 0; m_flags = 0; m_size = n; m_sizeCopy = n; } CStream(const CCharString& t) : CBaseNC(), SState() { SetSource(t.GetChars(), t.GetLength()); } };

int main() {
    CCharStringData d; d.m_pChars = "abc"; d.m_length = 3; CCharString s; s.m_pStringData = &d; CStream a(s); if (g_base != 1 || a.m_pData != d.m_pChars || a.m_pDataCopy != d.m_pChars || a.m_size != 3 || a.m_sizeCopy != 3 || a.m_position || a.m_flags) { std::printf("00995f50_TEST FAIL\n"); return 1; }
    s.m_pStringData = 0; CStream b(s); if (b.m_pData != g_empty || b.m_size != 0) { std::printf("00995f50_TEST FAIL\n"); return 1; }
    std::printf("00995f50_TEST PASS\n"); return 0;
}