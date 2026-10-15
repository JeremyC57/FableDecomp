// Behaviour test for 0x00994d20 CDataInputStream::ReadNullTerminated(W)String: characters of 2 byte(s) are appended until a zero
// one; the stream position advances past the terminator, and a short chunk is served by the overflow reader.
#include <cstdio>
#include <cstring>
typedef unsigned short TChar;
struct CString {
    TChar m_text[64]; int m_len;
    CString() : m_len(0) { m_text[0] = 0; }
    CString& operator+=(TChar c) { m_text[m_len++] = c; m_text[m_len] = 0; return *this; }
};
static int g_overflow = 0;
struct CStream {
    unsigned int StreamPos; const unsigned char* CurrentSourcePtr; int SourceChunkBytesRemaining;
    const unsigned char* m_next;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++g_overflow; std::memcpy(buffer, m_next, size); m_next += size; StreamPos += size; }
    static bool IsValidPosition(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, int size)
    {
        long long np = (long long)(unsigned int)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    CString ReadNullTerminated()
    {
        CString text;
        for (;;) { TChar c; Read(&c, 2); if (c == 0) break; text += c; }
        return text;
    }
};
int main()
{
    TChar chunk[3] = { 'A', 'B', 'C' };
    TChar rest[2] = { 'D', 0 };
    CStream s; s.StreamPos = 0; s.CurrentSourcePtr = (const unsigned char*)chunk; s.SourceChunkBytesRemaining = sizeof(chunk); s.m_next = (const unsigned char*)rest;
    CString r = s.ReadNullTerminated();
    if (r.m_len != 4 || r.m_text[0] != 'A' || r.m_text[3] != 'D' || g_overflow != 2 || s.StreamPos != 5 * 2) { std::printf("00994d20_TEST FAIL\n"); return 1; }
    std::printf("00994d20_TEST PASS\n");
    return 0;
}