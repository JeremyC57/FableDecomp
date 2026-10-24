#include <cstdio>
#include <cstring>
// Behaviour test: GFSerialiseMapBinaryOut for map<ETableSprites, long>: the element count, then key and value for each element
// in key order, through the writer's typed calls.
struct Writer {
    long Longs[16]; const char* Strings[16]; char Order[32];
    int NL, NS, NO;
    Writer() : NL(0), NS(0), NO(0) {}
    void WriteSLONG(long v) { Longs[NL++] = v; Order[NO++] = 'l'; }                         // 0x00993EB0
    void WriteNullTerminatedString(const char* s) { Strings[NS++] = s; Order[NO++] = 's'; }  // 0x00995AE0
};
typedef long Val;
struct Entry { long first; Val second; };
struct SortedMap {          // the map's in-order sequence
    Entry E[8]; int N;
    unsigned int size() const { return N; }
    const Entry* begin() const { return E; }
    const Entry* end() const { return E + N; }
};
static void WriteValue(Writer& w, const Val& v) { w.WriteSLONG(v); }
static void GFSerialiseMapBinaryOut(Writer& stream, const SortedMap& value)
{
    stream.WriteSLONG(value.size());
    for (const Entry* it = value.begin(); it != value.end(); ++it) {
        stream.WriteSLONG(it->first);
        WriteValue(stream, it->second);
    }
}
int main()
{
    SortedMap m;
    m.N = 3;
    m.E[0].first = 1; m.E[1].first = 4; m.E[2].first = 9;
    m.E[0].second = 100; m.E[1].second = -5; m.E[2].second = 7;
    Writer w;
    GFSerialiseMapBinaryOut(w, m);
    if (w.NO != 7 || std::strncmp(w.Order, "lllllll", 7) != 0) { std::printf("006327e0_TEST FAIL\n"); return 1; }
    long want[7] = { 3, 1, 100, 4, -5, 9, 7 };
    for (int i = 0; i < 7; ++i) if (w.Longs[i] != want[i]) { std::printf("006327e0_TEST FAIL\n"); return 1; }
    SortedMap empty; empty.N = 0;
    Writer w2;
    GFSerialiseMapBinaryOut(w2, empty);
    if (w2.NO != 1 || w2.Longs[0] != 0) { std::printf("006327e0_TEST FAIL\n"); return 1; }
    std::printf("006327e0_TEST PASS\n");
    return 0;
}