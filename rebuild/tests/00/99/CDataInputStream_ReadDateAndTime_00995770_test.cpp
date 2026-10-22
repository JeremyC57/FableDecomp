// Behaviour test for CDataInputStream::ReadDateAndTime (0x00995770): a long, four bytes and two words are read in
// order from the stream (in-chunk copies advance the pointer, the remaining count and the position; a short chunk
// falls back to the overflow reader) and handed to the CDateAndTime constructor in that order.
#include <cstdio>
#include <cstring>

struct CDateAndTime {
    long y; unsigned long mo, d, h, mi, s, ms;
    CDateAndTime(long a, unsigned long b, unsigned long c, unsigned long e, unsigned long f, unsigned long g, unsigned long k)
        : y(a), mo(b), d(c), h(e), mi(f), s(g), ms(k) { }
};

struct Stream {
    unsigned long StreamPos;
    unsigned char* CurrentSourcePtr;
    long SourceChunkBytesRemaining;
    int overflowCalls;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size)
    {
        ++overflowCalls;
        std::memset(buffer, 0x11, size);
        StreamPos += size;
    }
    void Read(void* buffer, long size)
    {
        long long newPosition = (long long)(unsigned long)(StreamPos + size);
        if (!(newPosition >= 0 && newPosition <= 0x7fffffff))
            return;
        if (SourceChunkBytesRemaining >= size) {
            std::memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr += size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        } else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    template <class T> T ReadValue(T*) { T v = 0; Read(&v, sizeof(T)); return v; }
    CDateAndTime ReadDateAndTime()
    {
        int year = ReadValue((int*)0);
        unsigned long month = ReadValue((unsigned char*)0);
        unsigned long day = ReadValue((unsigned char*)0);
        unsigned long hour = ReadValue((unsigned char*)0);
        unsigned long minute = ReadValue((unsigned char*)0);
        unsigned long second = ReadValue((unsigned short*)0);
        unsigned long milliseconds = ReadValue((unsigned short*)0);
        return CDateAndTime(year, month, day, hour, minute, second, milliseconds);
    }
};

int main()
{
    unsigned char data[12] = { 0xd5, 0x07, 0, 0, 9, 14, 23, 59, 0x3b, 0, 0xe7, 0x03 };
    Stream s;
    s.StreamPos = 0; s.CurrentSourcePtr = data; s.SourceChunkBytesRemaining = 12; s.overflowCalls = 0;
    CDateAndTime t = s.ReadDateAndTime();
    if (t.y != 2005 || t.mo != 9 || t.d != 14 || t.h != 23 || t.mi != 59 || t.s != 59 || t.ms != 999
        || s.StreamPos != 12 || s.SourceChunkBytesRemaining != 0 || s.overflowCalls != 0) {
        std::printf("00995770_TEST FAIL\n");
        return 1;
    }
    Stream u;
    u.StreamPos = 0; u.CurrentSourcePtr = data; u.SourceChunkBytesRemaining = 9; u.overflowCalls = 0;
    CDateAndTime v = u.ReadDateAndTime();
    if (v.y != 2005 || v.mi != 59 || v.s != 0x1111 || v.ms != 0x1111 || u.overflowCalls != 2 || u.StreamPos != 12) {
        std::printf("00995770_TEST FAIL\n");
        return 1;
    }
    std::printf("00995770_TEST PASS\n");
    return 0;
}