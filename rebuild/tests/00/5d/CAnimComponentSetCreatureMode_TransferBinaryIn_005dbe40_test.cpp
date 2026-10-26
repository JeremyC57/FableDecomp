// behaviour test for 0x005DBE40 CAnimComponentSetCreatureMode::TransferBinaryIn: a 4-byte read from the current chunk advances the cursor and position and lands in Mode; a read that crosses the chunk goes to the overflow reader
#include <cstdio>
#include <cstring>
static int gOverflow = 0;
struct Stream {
    unsigned long position; const unsigned char* data; long remaining;
    void Overflow(unsigned char* buffer, long size) { ++gOverflow; std::memset(buffer, 0x5A, size); position += size; }
    void Read(void* buffer, long size)
    {
        double newPosition = (double)position + (double)size;
        if (newPosition < 0.0 || newPosition > 2147483647.0) return;
        if (remaining >= size) { std::memcpy(buffer, data, size); data += size; remaining -= size; position += size; }
        else Overflow((unsigned char*)buffer, size);
    }
};
struct Obj {
    int member;
    void TransferBinaryIn(Stream& s) { int value = 0; s.Read(&value, sizeof(value)); member = value; }   // the function under test
};
int main()
{
    int fail = 0;
    unsigned char buf[8] = { 0x78, 0x56, 0x34, 0x12, 1, 2, 3, 4 };
    Stream s; s.position = 10; s.data = buf; s.remaining = 6;
    Obj o; o.member = 0;
    o.TransferBinaryIn(s);
    if (o.member != 0x12345678 || s.position != 14 || s.remaining != 2 || s.data != buf + 4) fail |= 1;
    o.TransferBinaryIn(s);                                   // only 2 bytes left: the overflow reader
    if (gOverflow != 1 || o.member != 0x5A5A5A5A) fail |= 2;
    if (fail) { std::printf("005dbe40_TEST FAIL %d\n", fail); return 1; }
    std::printf("005dbe40_TEST PASS\n");
    return 0;
}