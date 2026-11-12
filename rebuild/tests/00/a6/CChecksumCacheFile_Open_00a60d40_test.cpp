#include <cstdio>
#include <map>
#include <string>
#include <vector>
#include <cstring>
// Behaviour model of CChecksumCacheFile::Open: version-4 cache = SLONG version, SLONG count, then per entry
// NUL-terminated name, 7-dword CDateAndTime, ULONG checksum; anything else leaves the cleared map empty.
struct DT { long y; unsigned long m, d, h, mi, s, ms; };
struct Entry { DT TimeStamp; unsigned long Checksum; };
struct Stream {
    const unsigned char* p; long left;
    void Read(void* dst, long n) { if (n <= 0 || n > left) return; std::memcpy(dst, p, n); p += n; left -= n; }
    long ReadSLONG() { long v = 0; Read(&v, 4); return v; }
    unsigned long ReadULONG() { unsigned long v = 0; Read(&v, 4); return v; }
    std::string ReadNT() { std::string s; while (left > 0) { char c = (char)*p++; --left; if (!c) break; s += c; } return s; }
    DT ReadDT() { DT d; std::memset(&d, 0, sizeof d); Read(&d, sizeof d); return d; }
};
struct Cache {
    std::string Filename; std::map<std::string, Entry> Entries; long BufferSize; bool NeedsSaving;
    void Open(const std::string& fn, const std::vector<unsigned char>* file) {
        Filename = fn; Entries.clear(); NeedsSaving = false;
        if (!file) return;
        Stream st = { file->empty() ? 0 : &(*file)[0], (long)file->size() };
        if (st.ReadSLONG() != 4) return;
        long count = st.ReadSLONG();
        for (long i = 0; i < count; ++i) {
            std::string name = st.ReadNT();
            Entry e; e.TimeStamp = st.ReadDT(); e.Checksum = st.ReadULONG();
            Entries[name] = e;
        }
    }
};
static void put32(std::vector<unsigned char>& v, unsigned long x) { for (int i = 0; i < 4; ++i) v.push_back((unsigned char)(x >> (8 * i))); }
int main()
{
    std::vector<unsigned char> f; put32(f, 4); put32(f, 2);
    const char* names[2] = { "a.def", "b.def" };
    for (int k = 0; k < 2; ++k) { for (const char* c = names[k]; ; ++c) { f.push_back((unsigned char)*c); if (!*c) break; }
        for (int j = 0; j < 7; ++j) put32(f, 2000 + k + j); put32(f, 0xABC0 + k); }
    Cache c; c.NeedsSaving = true; Entry junk; std::memset(&junk, 0, sizeof junk); c.Entries["old"] = junk;
    c.Open("cache.bin", &f);
    if (c.NeedsSaving || c.Entries.size() != 2 || c.Entries.count("old") || c.Entries["b.def"].Checksum != 0xABC1 ||
        c.Entries["a.def"].TimeStamp.y != 2000 || c.Filename != "cache.bin") { std::printf("00a60d40_TEST FAIL\n"); return 1; }
    std::vector<unsigned char> bad; put32(bad, 3); put32(bad, 5);
    c.Open("x", &bad);
    if (!c.Entries.empty()) { std::printf("00a60d40_TEST FAIL\n"); return 1; }
    c.Entries["z"] = junk; c.Open("missing", 0);
    if (!c.Entries.empty() || c.Filename != "missing") { std::printf("00a60d40_TEST FAIL\n"); return 1; }
    std::printf("00a60d40_TEST PASS\n");
    return 0;
}