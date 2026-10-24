#include <cstdio>
#include <cstring>
// Behaviour test: GFSerialiseMapBinaryOut for map<unsigned long, CCharString>: the element count, then key and value for each element
// in key order, through the writer's typed calls.
struct Writer {
    long Longs[16]; const char* Strings[16]; char Order[32];
    int NL, NS, NO;
    Writer() : NL(0), NS(0), NO(0) {}
    void WriteSLONG(long v) { Longs[NL++] = v; Order[NO++] = 'l'; }                         // 0x00993EB0
    void WriteNullTerminatedString(const char* s) { Strings[NS++] = s; Order[NO++] = 's'; }  // 0x00995AE0
};
typedef const char* Val;
struct Entry { long first; Val second; };
struct SortedMap {          // the map's in-order sequence
    Entry E[8]; int N;
    unsigned int size() const { return N; }
    const Entry* begin() const { return E; }
    const Entry* end() const { return E + N; }
};
static void WriteValue(Writer& w, const Val& v) { w.WriteNullTerminatedString(v); }
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
    m.E[0].second = "a"; m.E[1].second = "bb"; m.E[2].second = "";
    Writer w;
    GFSerialiseMapBinaryOut(w, m);
    if (w.NO != 7 || std::strncmp(w.Order, "llslsls", 7) != 0) { std::printf("00632b30_TEST FAIL\n"); return 1; }
    if (w.Longs[0] != 3 || w.Longs[1] != 1 || w.Longs[2] != 4 || w.Longs[3] != 9) { std::printf("00632b30_TEST FAIL\n"); return 1; }
    if (std::strcmp(w.Strings[0], "a") || std::strcmp(w.Strings[1], "bb") || std::strcmp(w.Strings[2], "")) { std::printf("00632b30_TEST FAIL\n"); return 1; }
    SortedMap empty; empty.N = 0;
    Writer w2;
    GFSerialiseMapBinaryOut(w2, empty);
    if (w2.NO != 1 || w2.Longs[0] != 0) { std::printf("00632b30_TEST FAIL\n"); return 1; }
    std::printf("00632b30_TEST PASS\n");
    return 0;
}