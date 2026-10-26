// Behaviour test for 0x0046BE4D GFSerialiseVectorBinaryOut<CReverbEnvironmentEntry>: count, then one raw 0x54-byte write per entry
#include <cstdio>
#include <cstring>
struct Writer {
    unsigned char bytes[512]; int used; int writes;
    virtual ~Writer() {}
    virtual void Write(const void* data, int size) { std::memcpy(bytes + used, data, size); used += size; ++writes; }
    void WriteSLONG(int v) { Write(&v, 4); }
};
class CReverbEnvironmentEntry { public: float Params[21]; };
struct EntryVector { CReverbEnvironmentEntry* _M_start; CReverbEnvironmentEntry* _M_finish; CReverbEnvironmentEntry* _M_end_of_storage; };
static void SerialiseOut(Writer& stream, EntryVector& value)
{
    int count = (int)(value._M_finish - value._M_start);
    stream.WriteSLONG(count);
    for (int i = 0; i < count; ++i)
        stream.Write(value._M_start + i, (int)sizeof(CReverbEnvironmentEntry));
}
int main()
{
    if (sizeof(CReverbEnvironmentEntry) != 0x54) { std::printf("0046be4d_TEST FAIL\n"); return 1; }
    CReverbEnvironmentEntry e[2];
    for (int i = 0; i < 21; ++i) { e[0].Params[i] = (float)i; e[1].Params[i] = (float)(100 + i); }
    EntryVector v = { e, e + 2, e + 2 };
    Writer w; w.used = 0; w.writes = 0;
    SerialiseOut(w, v);
    int count; std::memcpy(&count, w.bytes, 4);
    float last; std::memcpy(&last, w.bytes + 4 + 0x54 + 20 * 4, 4);
    if (w.writes != 3 || w.used != 4 + 2 * 0x54 || count != 2 || last != 120.0f) { std::printf("0046be4d_TEST FAIL\n"); return 1; }
    std::printf("0046be4d_TEST PASS\n");
    return 0;
}