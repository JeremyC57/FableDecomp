#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SEntry { unsigned char _pad[0x10]; long m_offset; unsigned char _p[0x18 - 0x10 - 4]; }; struct CMgr { unsigned char _pad[0x1c]; SEntry* m_first; SEntry* m_last; SEntry* m_end; void ResetMusicEntriesOffsets() { for (SEntry* e = m_first; e != m_last; ++e) e->m_offset = 0; } };

int main() {
    SEntry e[3]; for (int i = 0; i < 3; ++i) e[i].m_offset = 7; CMgr* m = new CMgr; m->m_first = e; m->m_last = e + 3; m->ResetMusicEntriesOffsets(); if (e[0].m_offset || e[1].m_offset || e[2].m_offset) { std::printf("006bd870_TEST FAIL\n"); return 1; } e[0].m_offset = 7; m->m_last = e; m->ResetMusicEntriesOffsets(); if (e[0].m_offset != 7) { std::printf("006bd870_TEST FAIL\n"); return 1; } delete m;
    std::printf("006bd870_TEST PASS\n"); return 0;
}