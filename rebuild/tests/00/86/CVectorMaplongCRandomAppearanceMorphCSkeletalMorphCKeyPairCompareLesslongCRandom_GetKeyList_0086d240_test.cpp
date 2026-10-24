// behaviour test for 0086d240: GetKeyList appends each distinct key of the sorted map once, in order
#include <cstdio>
struct SVec {
    long data[16];
    long* first; long* last; long* end;
    void Grow(long* pos, const long& x) { *pos = x; ++last; end = data + 16; }   // stands in for the grow insert
    void push_back(const long& x) { if (last != end) { if (last) *last = x; ++last; } else Grow(last, x); }
};
struct SEntry { long first; unsigned char _payload[12]; };
struct SMap {
    SEntry* First; SEntry* Last; SEntry* EndOfStorage;
    void GetKeyList(SVec& keys) const
    {
        long lastKey = 0;
        bool first = true;
        for (const SEntry* it = First; it != Last; ++it)
            if (first || it->first != lastKey) { keys.push_back(it->first); lastKey = it->first; first = false; }
    }
};
int main()
{
    static const long k[6] = { 0, 0, 3, 3, 3, 8 };
    SEntry e[6];
    int i = 0;
    for (i = 0; i < 6; ++i) e[i].first = k[i];
    SMap m; m.First = e; m.Last = e + 6; m.EndOfStorage = e + 6;
    SVec v; v.first = v.data; v.last = v.data; v.end = v.data + 2;   // the third key goes through the grow path
    m.GetKeyList(v);
    int ok = (v.last - v.first == 3) && v.data[0] == 0 && v.data[1] == 3 && v.data[2] == 8;
    SMap empty; empty.First = e; empty.Last = e; empty.EndOfStorage = e;
    SVec w; w.first = w.data; w.last = w.data; w.end = w.data + 16;
    empty.GetKeyList(w);
    if (w.last != w.first) ok = 0;
    if (!ok) { std::printf("0086d240_TEST FAIL\n"); return 1; }
    std::printf("0086d240_TEST PASS\n");
    return 0;
}