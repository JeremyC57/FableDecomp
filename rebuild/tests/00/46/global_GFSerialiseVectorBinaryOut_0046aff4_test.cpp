// Behaviour test for 0x0046AFF4 GFSerialiseVectorBinaryOut<CAtmosBankEntry>: count, then ID (unsigned) + file name per entry
#include <cstdio>
struct Writer {
    unsigned int log[32]; char kind[32]; int n;
    void WriteULong(unsigned int v) { kind[n] = 'U'; log[n++] = v; }
    void WriteLong(int v) { kind[n] = 'L'; log[n++] = (unsigned int)v; }
};
class CAtmosBankEntry { public: unsigned int ID; int Filename; };
struct EntryVector { CAtmosBankEntry* _M_start; CAtmosBankEntry* _M_finish; CAtmosBankEntry* _M_end_of_storage; };
static void SerialiseOut(Writer& stream, EntryVector& value)
{
    int count = (int)(value._M_finish - value._M_start);
    stream.WriteLong(count);
    for (int i = 0; i < count; ++i) {
        const CAtmosBankEntry* e = value._M_start + i;
        stream.WriteULong(e->ID);
        stream.WriteLong(e->Filename);
    }
}
int main()
{
    CAtmosBankEntry e[2] = { { 0x80000001u, 3 }, { 7, -1 } };
    EntryVector v = { e, e + 2, e + 2 };
    Writer w; w.n = 0;
    SerialiseOut(w, v);
    if (w.n != 5 || w.kind[0] != 'L' || w.log[0] != 2 || w.kind[1] != 'U' || w.log[1] != 0x80000001u || w.kind[2] != 'L' || w.log[2] != 3
        || w.kind[3] != 'U' || w.log[3] != 7 || w.kind[4] != 'L' || w.log[4] != 0xffffffffu) {
        std::printf("0046aff4_TEST FAIL\n"); return 1;
    }
    std::printf("0046aff4_TEST PASS\n");
    return 0;
}