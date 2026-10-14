#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[16] = ""; static long g_key = 0; static long g_index = 0;
struct CFontEntry_009e2b40 { bool m_can; virtual void v00() { } virtual void v01() { } virtual bool CanRelease() { std::strcat(g_seq, "C"); return m_can; } virtual void Release() { std::strcat(g_seq, "R"); } };
struct CFontBank_009e2b40 { CFontEntry_009e2b40* m_entry; CFontEntry_009e2b40* GetEntry(long i) { std::strcat(g_seq, "A"); g_index = i; return m_entry; } };
struct CFontBankB_009e2b40 { CFontEntry_009e2b40* m_entry; CFontEntry_009e2b40* GetEntry(long i) { std::strcat(g_seq, "B"); g_index = i; return m_entry; } };
struct CSymbolMap { bool m_has; long m_index; bool FindSymbol(long k) { std::strcat(g_seq, "F"); g_key = k; return m_has; } long Lookup(long) { std::strcat(g_seq, "L"); return m_index; } };
struct CFontManager { long m_0; CFontBank_009e2b40* m_pStaticBank; long m_8; CFontBankB_009e2b40* m_pStreamingBank; long m_10; CSymbolMap* m_pStaticMap; long m_18; CSymbolMap* m_pStreamingMap;
    void ReleaseFont(long key) { CFontEntry_009e2b40* entry; if (m_pStaticMap->FindSymbol(key)) entry = m_pStaticBank->GetEntry(m_pStaticMap->Lookup(key)); else if (m_pStreamingMap->FindSymbol(key)) entry = m_pStreamingBank->GetEntry(m_pStreamingMap->Lookup(key)); else return; if (entry->CanRelease()) entry->Release(); } };
int main() {
    CFontEntry_009e2b40 e; CFontBank_009e2b40 ba; CFontBankB_009e2b40 bb; CSymbolMap ma, mb; ma.m_has = true; ma.m_index = 3; mb.m_has = false; mb.m_index = 4; ba.m_entry = &e; bb.m_entry = &e;
    CFontManager m; m.m_pStaticBank = &ba; m.m_pStreamingBank = &bb; m.m_pStaticMap = &ma; m.m_pStreamingMap = &mb; e.m_can = true;
    m.ReleaseFont(11); if (std::strcmp(g_seq, "FLACR") || g_key != 11 || g_index != 3) { std::printf("009e2b40_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; ma.m_has = false; mb.m_has = true; e.m_can = false; m.ReleaseFont(12); if (std::strcmp(g_seq, "FFLBC") || g_index != 4) { std::printf("009e2b40_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; mb.m_has = false; m.ReleaseFont(13); if (std::strcmp(g_seq, "FF")) { std::printf("009e2b40_TEST FAIL\n"); return 1; }
    std::printf("009e2b40_TEST PASS\n"); return 0;
}