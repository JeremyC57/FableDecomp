#include <cstdio>

// Behaviour model of CVtable_012b4e08::Read (0x00C31EB0): a memory input stream; a short read empties it.
#include <cstring>
struct Stream {
    const unsigned char* m_pCursor; unsigned int m_remaining;
    bool Read(void* buffer, unsigned int size)
    {
        if (size > m_remaining) { m_remaining = 0; return false; }
        std::memcpy(buffer, m_pCursor, size); m_remaining -= size; m_pCursor += size; return true;
    }
};

int main()
{

    const unsigned char data[6] = { 10, 11, 12, 13, 14, 15 }; unsigned char out[6];
    Stream s; s.m_pCursor = data; s.m_remaining = 6;
    if (!s.Read(out, 4) || out[0] != 10 || out[3] != 13) { std::printf("00c31eb0_TEST FAIL\n"); return 1; }
    if (s.m_remaining != 2 || s.m_pCursor != data + 4) { std::printf("00c31eb0_TEST FAIL\n"); return 1; }
    if (s.Read(out, 3) || s.m_remaining != 0) { std::printf("00c31eb0_TEST FAIL\n"); return 1; }
    if (!s.Read(out, 0)) { std::printf("00c31eb0_TEST FAIL\n"); return 1; }
    std::printf("00c31eb0_TEST PASS\n");
    return 0;
}