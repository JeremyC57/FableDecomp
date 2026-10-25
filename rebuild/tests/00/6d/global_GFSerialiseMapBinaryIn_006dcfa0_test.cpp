// Behaviour test for 0x006DCFA0 GFSerialiseMapBinaryIn of map<long, CComboMultiplierData> (model copy): clear the map,
// read the count from the memory stream (fast path and the invalid-position early out), then read and insert each entry.
#include <cstdio>
#include <cstring>

struct Combo { int entries; float a, b; Combo() : entries(0) { } };

struct Map {
    int keys[16]; Combo vals[16]; int n; int clears;
    void clear() { if (n != 0) { n = 0; ++clears; } }
    void insert(int k, const Combo& v)
    {
        for (int i = 0; i < n; ++i) if (keys[i] == k) return;      // unique keys, first wins
        keys[n] = k; vals[n] = v; ++n;
    }
};

struct Stream {
    unsigned int position; const unsigned char* chunkBase; int chunkLength;
    void Read(void* buffer, int size)
    {
        unsigned int newPosition = position + (unsigned int)size;   // zero-extended to 64 bits in retail: never negative
        if (!(newPosition <= 0x7fffffffU)) return;
        if (chunkLength >= size) {
            std::memcpy(buffer, chunkBase, size);
            chunkBase += size; chunkLength -= size; position += size;
        }
    }
    int ReadLong() { int v = 0; Read(&v, 4); return v; }
};

static void SerialiseIn(Stream& s, Map& out)
{
    out.clear();
    int count = 0;
    s.Read(&count, 4);
    for (; count > 0; --count) {
        int key = s.ReadLong();
        Combo value;
        value.entries = (int)s.ReadLong();
        out.insert(key, value);
    }
}

int main()
{
    int data[7] = { 3, 7, 70, 2, 20, 7, 99 };                     // count 3; (7,70) (2,20) (7,99 duplicate)
    Stream s; s.position = 0; s.chunkBase = (const unsigned char*)data; s.chunkLength = sizeof(data);
    Map m; m.n = 1; m.keys[0] = 5; m.clears = 0;
    SerialiseIn(s, m);
    int ok = m.clears == 1 && m.n == 2 && m.keys[0] == 7 && m.vals[0].entries == 70 && m.keys[1] == 2 &&
             m.vals[1].entries == 20 && s.position == sizeof(data) && s.chunkLength == 0;

    int zero[1] = { 0 };
    Stream z; z.position = 0; z.chunkBase = (const unsigned char*)zero; z.chunkLength = sizeof(zero);
    SerialiseIn(z, m);
    ok = ok && m.n == 0 && m.clears == 2 && z.position == 4;

    Stream bad; bad.position = 0x7ffffffeU; bad.chunkBase = (const unsigned char*)data; bad.chunkLength = sizeof(data);
    m.n = 0;
    SerialiseIn(bad, m);                                            // position + 4 overflows: nothing read
    ok = ok && m.n == 0 && bad.position == 0x7ffffffeU;

    if (!ok) { std::printf("006dcfa0_TEST FAIL\n"); return 1; }
    std::printf("006dcfa0_TEST PASS\n");
    return 0;
}