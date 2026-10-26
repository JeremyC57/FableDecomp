// Behaviour test for 0x0046712b GFSerialiseVectorBinaryIn<CBrainBehaviour>: read the count (4 bytes), resize the vector to it with default
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
class CDefString { public: int TablePos; CDefString() : TablePos(-1) { } };
class CBrainBehaviour { public: CDefString Name; int Priority; int ScriptAIPriority; CBrainBehaviour() : Priority(0), ScriptAIPriority(0) { } };
static int g_traitsBuilt = 0;
struct Traits { CBrainBehaviour DefaultVal; Traits() { ++g_traitsBuilt; }
    void TransferBinaryIn(Reader* s, CBrainBehaviour* v) { s->Read(&v->Name.TablePos, 4); s->Read(&v->Priority, 4); s->Read(&v->ScriptAIPriority, 4); } };
struct Vec {
    CBrainBehaviour _M_data[8]; unsigned int n;
    Vec() : n(0) { }
    void resize(unsigned int count, const CBrainBehaviour& x) { for (unsigned int i = n; i < count; ++i) _M_data[i] = x; n = count; }
    void resize(unsigned int count) { resize(count, CBrainBehaviour()); }
    unsigned int size() const { return n; }
    CBrainBehaviour& operator[](unsigned int i) { return _M_data[i]; }
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
    int src[] = { 2, 11, 12, 13, 21, 22, 23 };
    Reader r; r.position = 0; r.chunkBase = (const unsigned char*)src; r.chunkLength = (int)sizeof(src);
    Vec out;
    GFSerialiseVectorBinaryIn(r, out);
    bool ok = r.chunkLength == 0 && r.position == (int)sizeof(src);
    ok = ok && g_traitsBuilt == 1 && out.size() == 2 && out[0].Name.TablePos == 11 && out[0].ScriptAIPriority == 13 && out[1].Priority == 22 && out[1].ScriptAIPriority == 23;
    ok = ok && out._M_data[2].Name.TablePos == -1 && out._M_data[2].Priority == 0;   /* untouched default slot */
    if (!ok) { std::printf("0046712b_TEST FAIL\n"); return 1; }
    std::printf("0046712b_TEST PASS\n");
    return 0;
}