#include <cstdio>
#include <cstdlib>
#include <cstring>
static char g_seq[64] = "";
struct CArrayChar { int x; }; static unsigned long g_a = 0, g_entry = 0, g_slotEntry = 99; static const CArrayChar* g_data = 0; struct SEntry44 { unsigned char _pad[0x24]; long m_slot; unsigned char _pad28[4]; };
struct CEntryVector { SEntry44* m_pFirst; SEntry44* m_pLast; unsigned long size() const { return m_pLast - m_pFirst; } SEntry44& operator[](unsigned long i) { return m_pFirst[i]; } };
class CGraphicDataBank { public: virtual void OnEntryUpdated(unsigned long a, unsigned long entry, const CArrayChar& data) = 0; virtual void OnEntrySlotUpdated(unsigned long entry) = 0; CEntryVector m_entries;
    void Notify(unsigned long a, unsigned long entry, const CArrayChar& data) { if (entry < m_entries.size() && m_entries[entry].m_slot != -1) OnEntrySlotUpdated(entry); OnEntryUpdated(a, entry, data); } };
struct CBank : CGraphicDataBank { virtual void OnEntryUpdated(unsigned long a, unsigned long entry, const CArrayChar& data) { std::strcat(g_seq, "U"); g_a = a; g_entry = entry; g_data = &data; } virtual void OnEntrySlotUpdated(unsigned long entry) { std::strcat(g_seq, "S"); g_slotEntry = entry; } };

int main() {
    CBank b; SEntry44 e[3]; e[0].m_slot = 4; e[1].m_slot = -1; e[2].m_slot = 0; b.m_entries.m_pFirst = e; b.m_entries.m_pLast = e + 2; CArrayChar data;
    b.Notify(7, 0, data); if (std::strcmp(g_seq, "SU") || g_slotEntry != 0 || g_a != 7 || g_entry != 0 || g_data != &data) { std::printf("009fd910_TEST FAIL\n"); return 1; }
    g_seq[0] = 0; b.Notify(8, 1, data); if (std::strcmp(g_seq, "U") || g_entry != 1) { std::printf("009fd910_TEST FAIL\n"); return 1; } g_seq[0] = 0; b.Notify(9, 2, data); if (std::strcmp(g_seq, "U") || g_entry != 2) { std::printf("009fd910_TEST FAIL\n"); return 1; } g_seq[0] = 0; b.Notify(9, 0xffffffffUL, data); if (std::strcmp(g_seq, "U")) { std::printf("009fd910_TEST FAIL\n"); return 1; }
    std::printf("009fd910_TEST PASS\n"); return 0;
}