// Behaviour test for 0x0046B958 GFSerialiseVectorBinaryOut<CMusicSetEntry>: count, then three longs per entry
#include <cstdio>
struct Writer { int log[32]; int n; void WriteLong(int v) { log[n++] = v; } };
class CMusicSetEntry { public: int NormalMusicID; int DangerMusicID; int LoopCount; };
struct EntryVector { CMusicSetEntry* _M_start; CMusicSetEntry* _M_finish; CMusicSetEntry* _M_end_of_storage; };
static void SerialiseOut(Writer& stream, EntryVector& value)
{
    int count = (int)(value._M_finish - value._M_start);
    stream.WriteLong(count);
    for (int i = 0; i < count; ++i) {
        const CMusicSetEntry* e = value._M_start + i;
        stream.WriteLong(e->NormalMusicID); stream.WriteLong(e->DangerMusicID); stream.WriteLong(e->LoopCount);
    }
}
int main()
{
    CMusicSetEntry e[2] = { { 1, 2, 3 }, { 4, 5, -1 } };
    EntryVector v = { e, e + 2, e + 2 };
    Writer w; w.n = 0;
    SerialiseOut(w, v);
    const int want[7] = { 2, 1, 2, 3, 4, 5, -1 };
    if (w.n != 7) { std::printf("0046b958_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (w.log[i] != want[i]) { std::printf("0046b958_TEST FAIL\n"); return 1; }
    std::printf("0046b958_TEST PASS\n");
    return 0;
}