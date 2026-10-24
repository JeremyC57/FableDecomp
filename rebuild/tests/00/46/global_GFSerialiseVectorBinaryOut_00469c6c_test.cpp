// Behaviour test for 0x00469C6C GFSerialiseVectorBinaryOut<CTextureReplacementDef::CEntry>: count, then both indices per entry
#include <cstdio>
struct Writer { int log[32]; int n; void WriteLong(int v) { log[n++] = v; } };
struct CEntry { int OriginalTexture; int NewTexture; };
struct EntryVector { CEntry* _M_start; CEntry* _M_finish; CEntry* _M_end_of_storage; };
static void SerialiseOut(Writer& stream, EntryVector& value)
{
    int count = (int)(value._M_finish - value._M_start);
    stream.WriteLong(count);
    for (int i = 0; i < count; ++i) {
        const CEntry* e = value._M_start + i;
        stream.WriteLong(e->OriginalTexture);
        stream.WriteLong(e->NewTexture);
    }
}
int main()
{
    CEntry entries[3] = { { 1, 2 }, { 3, 4 }, { 5, 6 } };
    EntryVector v = { entries, entries + 3, entries + 3 };
    Writer w; w.n = 0;
    SerialiseOut(w, v);
    const int want[7] = { 3, 1, 2, 3, 4, 5, 6 };
    if (w.n != 7) { std::printf("00469c6c_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 7; ++i) if (w.log[i] != want[i]) { std::printf("00469c6c_TEST FAIL\n"); return 1; }
    EntryVector e = { entries, entries, entries }; w.n = 0;
    SerialiseOut(w, e);
    if (w.n != 1 || w.log[0] != 0) { std::printf("00469c6c_TEST FAIL\n"); return 1; }
    std::printf("00469c6c_TEST PASS\n");
    return 0;
}