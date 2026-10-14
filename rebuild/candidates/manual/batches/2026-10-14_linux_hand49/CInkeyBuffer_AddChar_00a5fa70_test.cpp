#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SInkeyEntry { unsigned short m_char; unsigned char m_flags; unsigned char _pad; };
struct CInkeyBuffer { unsigned char _pad[8]; SInkeyEntry m_entries[100]; unsigned long m_read; unsigned long m_write;
    void AddChar(unsigned short character, unsigned char flags) { if ((m_write + 1) % 100 != m_read) { m_entries[m_write].m_char = character; m_entries[m_write].m_flags = flags; m_write = (m_write + 1) % 100; } }
    bool GetChar(unsigned short* character, unsigned char* flags) { if (m_write != m_read) { *character = m_entries[m_read].m_char; *flags = m_entries[m_read].m_flags; m_read = (m_read + 1) % 100; return true; } return false; } };

int main() {
    CInkeyBuffer b; std::memset(&b, 0, sizeof b); b.m_read = 0; b.m_write = 98; b.AddChar(0x41, 3); if (b.m_write != 99 || b.m_entries[98].m_char != 0x41 || b.m_entries[98].m_flags != 3) { std::printf("00a5fa70_TEST FAIL\n"); return 1; }
    b.AddChar(0x42, 4); if (b.m_write != 99 || b.m_entries[99].m_char != 0) { std::printf("00a5fa70_TEST FAIL\n"); return 1; } b.m_read = 5; b.AddChar(0x43, 5); if (b.m_write != 0 || b.m_entries[99].m_char != 0x43) { std::printf("00a5fa70_TEST FAIL\n"); return 1; }
    std::printf("00a5fa70_TEST PASS\n"); return 0;
}
