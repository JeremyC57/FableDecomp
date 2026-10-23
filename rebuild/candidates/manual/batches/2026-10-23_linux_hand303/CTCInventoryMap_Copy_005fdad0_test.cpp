// Behaviour test for CTCInventoryMap::Copy @ 0x005FDAD0: the three name lists and the id list become copies of the
// other map's lists (old contents dropped, order kept), including growth from an empty id list
#include <cstdio>
#include <cstdlib>
struct IdVec {
    int* s; int* f; int* e;
    unsigned long size() const { return f - s; }
    void clear() { f = s; }
    void push_back(const int& x)
    {
        if (f != e) { *f = x; ++f; return; }
        unsigned long old = size();
        unsigned long len = old + (old < 1 ? 1 : old);
        int* ns = len ? (int*)std::malloc(len * sizeof(int)) : 0;
        int* nf = ns;
        for (int* p = s; p != f; ++p, ++nf) *nf = *p;
        *nf = x; ++nf;
        if (s) std::free(s);
        e = ns + len; s = ns; f = nf;
    }
};
struct Map { IdVec names[3]; IdVec ids; };
static void Copy(Map& m, const Map& o)
{
    int k;
    for (k = 0; k < 3; ++k) m.names[k].clear();
    m.ids.clear();
    for (k = 0; k < 3; ++k)
        for (unsigned long i = 0; i < o.names[k].size(); ++i) m.names[k].push_back(o.names[k].s[i]);
    for (unsigned long i = 0; i < o.ids.size(); ++i) m.ids.push_back(o.ids.s[i]);
}
int main()
{
    Map a, b;
    IdVec z = { 0, 0, 0 };
    for (int k = 0; k < 3; ++k) { a.names[k] = z; b.names[k] = z; }
    a.ids = z; b.ids = z;
    for (int v = 0; v < 5; ++v) { b.ids.push_back(v * 3); b.names[1].push_back(v); }
    a.ids.push_back(99); a.names[0].push_back(7);
    Copy(a, b);
    int ok = a.ids.size() == 5 && a.names[0].size() == 0 && a.names[1].size() == 5 && a.names[2].size() == 0;
    for (int v = 0; ok && v < 5; ++v) if (a.ids.s[v] != v * 3 || a.names[1].s[v] != v) ok = 0;
    if (a.ids.e - a.ids.s < 5) ok = 0;
    if (!ok) { std::printf("005fdad0_TEST FAIL\n"); return 1; }
    std::printf("005fdad0_TEST PASS\n");
    return 0;
}
