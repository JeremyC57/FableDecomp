// Behaviour test for 0x006D4B40 GFSerialiseVectorBinaryIn<pair<CCharString, long> >: the 4-byte count is read from the
// chunk, the vector is resized to it, and every element is read in order through the pair traits. When the range check
// rejects the read nothing is consumed (retail then uses an uninitialised count; the model starts it at 0).
#include <cstdio>
#include <cstring>

struct Stream { unsigned int position; const unsigned char* chunk; int chunkLength; };
struct Element { int text; long value; };
struct Vec { Element items[8]; int size; };

static void Read(Stream& s, void* buffer, int size)
{
    unsigned long long newPosition = (unsigned long long)(unsigned int)(s.position + size);
    if (newPosition > 0x7fffffffULL) return;
    if (s.chunkLength >= size) { std::memcpy(buffer, s.chunk, size); s.chunk += size; s.chunkLength -= size; s.position += size; }
}

static void TransferPair(Stream& s, Element* out)                    // stands in for CPersistTraits<pair>::TransferBinaryIn
{
    int text = 0; long value = 0;
    Read(s, &text, 4); Read(s, &value, 4);
    out->text = text; out->value = value;
}

static void VectorIn(Stream& s, Vec& out)
{
    int count = 0;
    Read(s, &count, 4);
    out.size = count;
    for (int i = 0; i < count; ++i)
        TransferPair(s, &out.items[i]);
}

int main()
{
    unsigned char data[4 + 3 * 8];
    int count = 3; std::memcpy(data, &count, 4);
    for (int i = 0; i < 3; ++i) { int t = 10 + i; int v = 100 * i; std::memcpy(data + 4 + 8 * i, &t, 4); std::memcpy(data + 8 + 8 * i, &v, 4); }
    Stream s; s.position = 0; s.chunk = data; s.chunkLength = sizeof(data);
    Vec v; v.size = -1;
    VectorIn(s, v);
    if (v.size != 3 || v.items[0].text != 10 || v.items[2].text != 12 || v.items[2].value != 200 || s.position != 28) { std::printf("006d4b40_TEST FAIL\n"); return 1; }
    Stream bad; bad.position = 0x7ffffffe; bad.chunk = data; bad.chunkLength = sizeof(data);
    VectorIn(bad, v);                                 // range check fails: nothing consumed
    if (v.size != 0 || bad.position != 0x7ffffffe) { std::printf("006d4b40_TEST FAIL\n"); return 1; }
    std::printf("006d4b40_TEST PASS\n");
    return 0;
}