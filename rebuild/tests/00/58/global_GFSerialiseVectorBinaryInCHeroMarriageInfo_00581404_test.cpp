// Behaviour test for 0x00581404 GFSerialiseVectorBinaryIn<CHeroMarriageInfo> (model copy)
#include <cstdio>
#include <cstring>
struct Rec { unsigned int wife, home; long state; bool gay; };
struct Stream {
    long position; const unsigned char* chunkBase; long chunkLength; int slowReads;
    void ReadSlow(unsigned char*, long) { ++slowReads; }
    static bool IsValid(long long p) { return p >= 0 && p <= 0x7fffffff; }
    void Read(void* buffer, long size)
    {
        long long np = (long long)(unsigned int)(position + size);
        if (!IsValid(np)) return;
        if (chunkLength >= size) { std::memcpy(buffer, chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
        else ReadSlow((unsigned char*)buffer, size);
    }
};
struct Vec { Rec items[8]; long n; void resize(unsigned int k) { for (unsigned int i = (unsigned int)n; i < k; ++i) { Rec z = { 0, 0, 0, false }; items[i] = z; } n = (long)k; } Rec& operator[](unsigned int i) { return items[i]; } };
struct Traits { Rec DefaultVal; void TransferBinaryIn(Stream& s, Rec* out) const { int v; s.Read(&v, 4); out->wife = (unsigned int)v; out->state = 1; } };
static void SerialiseIn(Stream& s, Vec& out)
{
    Traits traits;
    int count; { int value; s.Read(&value, sizeof(value)); count = value; }
    out.resize(count);
    for (int i = 0; i < count; ++i) traits.TransferBinaryIn(s, &out[i]);
}
int main()
{
    unsigned char buf[16]; int vals[4] = { 3, 11, 22, 33 };
    for (int i = 0; i < 4; ++i) { int v = (int)vals[i]; std::memcpy(buf + 4 * i, &v, 4); }
    Stream s; s.position = 0; s.chunkBase = buf; s.chunkLength = 16; s.slowReads = 0;
    Vec v; v.n = 0;
    SerialiseIn(s, v);
    if (v.n != 3 || v.items[0].wife != 11 || v.items[2].wife != 33 || v.items[1].state != 1 || s.position != 16 || s.slowReads != 0) { std::printf("00581404_TEST FAIL\n"); return 1; }
    std::printf("00581404_TEST PASS\n");
    return 0;
}