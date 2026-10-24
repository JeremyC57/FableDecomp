// Behaviour test for 0x006DD580 GFSerialiseVectorBinaryIn<CAttackHistoryCombo>: the 4-byte count is read from the
// chunk (position range check, fast path advances the chunk), the vector is resized to it, and every element is read
// in order with a 16-byte stride; a short chunk takes the overflow path.
#include <cstdio>
#include <cstring>

struct Combo { int Words[3]; float Multiplier; };

struct Stream {
    unsigned int position; const unsigned char* chunkBase; int chunkLength; int overflowCalls; int nextOverflowValue;
    void ReadWithSrcChunkOverflow(unsigned char* buffer, int size) { ++overflowCalls; std::memcpy(buffer, &nextOverflowValue, size); }
    void Read(void* buffer, int size)
    {
        long long np = (long long)(unsigned int)(position + size);
        if (!(np >= 0 && np <= 0x7fffffff)) return;
        if (chunkLength >= size) { std::memcpy(buffer, chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
        else ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
};

struct Vec {
    Combo storage[8]; unsigned int n;
    void resize(unsigned int k) { n = k; }
    Combo& operator[](unsigned int i) { return storage[i]; }
};

struct Traits {
    Combo DefaultVal;
    void TransferBinaryIn(Stream& s, Combo* out) const { int v; s.Read(&v, 4); out->Words[0] = v; }
};

static void SerialiseIn(Stream& stream, Vec& out)
{
    Traits traits;
    int count; { int value; stream.Read(&value, 4); count = value; }
    out.resize(count);
    for (int i = 0; i < count; ++i) traits.TransferBinaryIn(stream, &out[i]);
}

int main()
{
    int data[4] = { 3, 11, 22, 33 };
    Stream s; s.position = 0; s.chunkBase = (const unsigned char*)data; s.chunkLength = 16; s.overflowCalls = 0; s.nextOverflowValue = 0;
    Vec v; v.n = 0;
    SerialiseIn(s, v);
    if (sizeof(Combo) != 16 || v.n != 3 || v[0].Words[0] != 11 || v[2].Words[0] != 33 || s.position != 16 || s.chunkLength != 0) {
        std::printf("006dd580_TEST FAIL\n"); return 1;
    }
    Stream t; t.position = 0; t.chunkBase = (const unsigned char*)data; t.chunkLength = 2; t.overflowCalls = 0; t.nextOverflowValue = 0;
    Vec w; w.n = 5;
    SerialiseIn(t, w);
    if (t.overflowCalls != 1 || w.n != 0) { std::printf("006dd580_TEST FAIL\n"); return 1; }
    std::printf("006dd580_TEST PASS\n");
    return 0;
}