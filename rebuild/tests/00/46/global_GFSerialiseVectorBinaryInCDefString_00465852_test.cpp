// Behaviour test for 0x00465852 GFSerialiseVectorBinaryIn<CDefString>: read the count (4 bytes), resize the vector to it with default
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
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } void TransferBinaryIn(Reader& s) { s.Read(&TablePos, 4); } };
struct Traits { CDefString DefaultVal; void TransferBinaryIn(Reader* s, CDefString* v) { v->TransferBinaryIn(*s); } };
struct Vec {
    CDefString _M_data[8]; unsigned int n;
    Vec() : n(0) { }
    void resize(unsigned int count, const CDefString& x) { for (unsigned int i = n; i < count; ++i) _M_data[i] = x; n = count; }
    void resize(unsigned int count) { resize(count, CDefString()); }
    unsigned int size() const { return n; }
    CDefString& operator[](unsigned int i) { return _M_data[i]; }
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
    int src[] = { 4, 40, 41, 42, 43 };
    Reader r; r.position = 0; r.chunkBase = (const unsigned char*)src; r.chunkLength = (int)sizeof(src);
    Vec out;
    GFSerialiseVectorBinaryIn(r, out);
    bool ok = r.chunkLength == 0 && r.position == (int)sizeof(src);
    ok = ok && out.size() == 4 && out[0].TablePos == 40 && out[3].TablePos == 43 && out._M_data[4].TablePos == -1;
    if (!ok) { std::printf("00465852_TEST FAIL\n"); return 1; }
    std::printf("00465852_TEST PASS\n");
    return 0;
}