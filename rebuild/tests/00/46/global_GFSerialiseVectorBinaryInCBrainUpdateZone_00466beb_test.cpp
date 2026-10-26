// Behaviour test for 0x00466beb GFSerialiseVectorBinaryIn<CBrainUpdateZone>: read the count (4 bytes), resize the vector to it with default
// elements, then let the traits read every element in place (32-bit fields modelled as int on the 64-bit test host)
#include <cstdio>
#include <cstring>
struct Reader {
    int position; const unsigned char* chunkBase; int chunkLength;
    void Read(void* buffer, int size)
    {
        if (chunkLength >= size) { std::memcpy(buffer, chunkBase, size); chunkBase += size; chunkLength -= size; position += size; }
    }
};
class CBrainUpdateZone { public: int Distance; int UpdateFrequency; int InterruptionFrequency; CBrainUpdateZone() : Distance(0), UpdateFrequency(0), InterruptionFrequency(0) { } };
struct Traits { CBrainUpdateZone DefaultVal;
    void TransferBinaryIn(Reader* s, CBrainUpdateZone* v) { s->Read(&v->Distance, 4); s->Read(&v->UpdateFrequency, 4); s->Read(&v->InterruptionFrequency, 4); } };
struct Vec {
    CBrainUpdateZone _M_data[8]; unsigned int n;
    Vec() : n(0) { }
    void resize(unsigned int count, const CBrainUpdateZone& x) { for (unsigned int i = n; i < count; ++i) _M_data[i] = x; n = count; }
    void resize(unsigned int count) { resize(count, CBrainUpdateZone()); }
    unsigned int size() const { return n; }
    CBrainUpdateZone& operator[](unsigned int i) { return _M_data[i]; }
};
static void ReadCount(Reader& stream, int* out) { int value; stream.Read(&value, sizeof(value)); *out = value; }
static void GFSerialiseVectorBinaryIn(Reader& stream, Vec& out)
{
    Traits traits;
    int count;
    ReadCount(stream, &count);
    out.resize(count);
    for (int i = 0; i < count; ++i)
        traits.TransferBinaryIn(&stream, &out[i]);
}
int main()
{
    int src[] = { 3, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    Reader r; r.position = 0; r.chunkBase = (const unsigned char*)src; r.chunkLength = (int)sizeof(src);
    Vec out;
    GFSerialiseVectorBinaryIn(r, out);
    bool ok = r.chunkLength == 0 && r.position == (int)sizeof(src);
    ok = ok && out.size() == 3 && out[0].Distance == 1 && out[1].UpdateFrequency == 5 && out[2].InterruptionFrequency == 9;
    if (!ok) { std::printf("00466beb_TEST FAIL\n"); return 1; }
    std::printf("00466beb_TEST PASS\n");
    return 0;
}