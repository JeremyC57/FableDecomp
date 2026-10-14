#include <cstdio>
#include <cstdlib>
#include <cstring>
struct SInkeyEntry { unsigned short m_char; unsigned char m_flags; unsigned char _pad; };
struct CInkeyBuffer { unsigned char _pad[8]; SInkeyEntry m_entries[100]; unsigned long m_read; unsigned long m_write;
    void AddChar(unsigned short character, unsigned char flags) { if ((m_write + 1) % 100 != m_read) { m_entries[m_write].m_char = character; m_entries[m_write].m_flags = flags; m_write = (m_write + 1) % 100; } }
    bool GetChar(unsigned short* character, unsigned char* flags) { if (m_write != m_read) { *character = m_entries[m_read].m_char; *flags = m_entries[m_read].m_flags; m_read = (m_read + 1) % 100; return true; } return false; } };

int main() {
    CInkeyBuffer b; std::memset(&b, 0, sizeof b); unsigned short c = 1; unsigned char f = 1; b.m_read = 99; b.m_write = 99; if (b.GetChar(&c, &f) || c != 1) { std::printf("00a5fac0_TEST FAIL\n"); return 1; }
    b.m_entries[99].m_char = 0x51; b.m_entries[99].m_flags = 7; b.m_write = 3; if (!b.GetChar(&c, &f) || c != 0x51 || f != 7 || b.m_read != 0) { std::printf("00a5fac0_TEST FAIL\n"); return 1; }
    std::printf("00a5fac0_TEST PASS\n"); return 0;
}
