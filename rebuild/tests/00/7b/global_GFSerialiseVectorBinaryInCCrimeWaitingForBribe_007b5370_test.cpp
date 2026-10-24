// Behaviour test for 0x007B5370 GFSerialiseVectorBinaryIn<CCrimeWaitingForBribe>: count, resize, then records in place.
#include <cstdio>
#include <cstring>

struct Stream {
    const char* base; long position; long length;
    void Read(void* buffer, long size)
    {
        if (position + size > length) return;             // models the range check: nothing read
        std::memcpy(buffer, base + position, size);
        position += size;
    }
};
struct CCrimeWaitingForBribe { int DeedHandle; int Type; };
struct Vec {
    CCrimeWaitingForBribe storage[8]; unsigned int size;
    void resize(unsigned int n) { for (unsigned int i = size; i < n && i < 8; ++i) { storage[i].DeedHandle = 0; storage[i].Type = 0; } size = n; }
    CCrimeWaitingForBribe& operator[](unsigned int n) { return *(storage + n); }
};
static void TransferBinaryIn(Stream* s, CCrimeWaitingForBribe* v)
{
    int n;
    n = 0; s->Read(&n, 4); v->DeedHandle = n;
    n = 0; s->Read(&n, 4); v->Type = n;
}
static void SerialiseIn(Stream& stream, Vec& out)
{
    int count = 0;
    stream.Read(&count, 4);
    out.resize(count);
    for (int i = 0; i < count; ++i) TransferBinaryIn(&stream, &out[i]);
}
int main()
{
    int data[5] = { 2, 101, 4, 202, 9 };
    Stream s; s.base = (const char*)data; s.position = 0; s.length = sizeof(data);
    Vec v; v.size = 0;
    SerialiseIn(s, v);
    if (v.size != 2 || v.storage[0].DeedHandle != 101 || v.storage[0].Type != 4 || v.storage[1].DeedHandle != 202 || v.storage[1].Type != 9 || s.position != 20) { std::printf("007b5370_TEST FAIL\n"); return 1; }
    std::printf("007b5370_TEST PASS\n");
    return 0;
}