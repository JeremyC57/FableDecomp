// Behaviour test for 00f274d0 (CPersistContext::Transfer of a vector of sub-component records): binary save writes
// the count then each element, binary load reads the count, resizes and transfers each element, mode 4 transfers each
// element only, text load hands the vector to the text helper.
#include <cstdio>

struct Ctx;
struct Entry;
struct EntryVec {
    Entry* data; long n;
    long size() const { return n; }
    Entry& operator[](long i);
    void resize(long count);
};
struct Entry { int transfers; Entry() : transfers(0) {} void Transfer(Ctx&) { ++transfers; } };
static Entry g_store[16];
void EntryVec::resize(long count) { data = g_store; n = count; }
Entry& EntryVec::operator[](long i) { return data[i]; }
struct Ctx {
    long Mode; long written; long toRead; int textLoads; int crc;
    void Transfer(const char* name, EntryVec& v)
    {
        (void)name; ++crc;
        switch (Mode) {
        case 4: { long n = (long)v.size(); for (long i = 0; i < n; ++i) v[i].Transfer(*this); break; }
        case 3: { long n = (long)v.size(); written = n; for (long i = 0; i < n; ++i) v[i].Transfer(*this); break; }
        case 2: { long n = toRead; v.resize(n); for (long i = 0; i < n; ++i) v[i].Transfer(*this); break; }
        case 0: ++textLoads; break;
        }
    }
};
int main()
{
    EntryVec v; v.data = g_store; v.n = 3;
    Ctx c = { 3, -1, 0, 0, 0 };
    c.Transfer("Creatures", v);
    int ok = c.written == 3 && v[0].transfers == 1 && v[2].transfers == 1;
    c.Mode = 2; c.toRead = 5; c.Transfer("Creatures", v);
    if (v.size() != 5 || v[4].transfers != 1 || v[0].transfers != 2) ok = 0;
    c.Mode = 4; c.Transfer("Creatures", v);
    if (v[4].transfers != 2) ok = 0;
    c.Mode = 0; c.Transfer("Creatures", v);
    if (c.textLoads != 1 || c.crc != 4) ok = 0;
    if (!ok) { std::printf("00f274d0_TEST FAIL\n"); return 1; }
    std::printf("00f274d0_TEST PASS\n");
    return 0;
}
