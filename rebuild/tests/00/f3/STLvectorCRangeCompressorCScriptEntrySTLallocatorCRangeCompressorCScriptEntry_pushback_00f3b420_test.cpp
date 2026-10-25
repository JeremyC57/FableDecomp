// Behaviour test for 00f3b420: vector<CScriptEntry>::push_back -- in-place copy while there is room, one grow call (at end, count 1) when full
#include <cstdio>
#include <cstdlib>
#include <cstring>
struct Entry { unsigned long Flags; long BitCount; unsigned long RangeBias; long RangeShift; unsigned long ShuffleMask; unsigned long OrMask; };
struct Vec {
    Entry* start; Entry* finish; Entry* end; int grows;
    void Grow(Entry* pos, const Entry& x, unsigned int fill, bool atEnd)
    {
        ++grows; unsigned int old = (unsigned int)(finish - start); unsigned int len = old + (old > fill ? old : fill);
        Entry* n = (Entry*)std::malloc(len * sizeof(Entry)); std::memcpy(n, start, (pos - start) * sizeof(Entry));
        Entry* f = n + (pos - start); *f++ = x; if (!atEnd) { std::memcpy(f, pos, (finish - pos) * sizeof(Entry)); f += finish - pos; }
        std::free(start); start = n; finish = f; end = n + len;
    }
    void push_back(const Entry& x) { if (finish != end) { *finish = x; ++finish; } else Grow(finish, x, 1, true); }
};
int main()
{
    Vec v; v.start = v.finish = v.end = 0; v.grows = 0;
    for (unsigned long i = 0; i < 5; ++i) { Entry e = { i, (long)i, i, (long)i, i, i * 2 }; v.push_back(e); }
    int ok = v.finish - v.start == 5 && v.start[4].OrMask == 8 && v.start[0].Flags == 0 && v.start[3].BitCount == 3 && v.grows == 4;
    std::free(v.start);
    std::printf(ok ? "00f3b420_TEST PASS\n" : "00f3b420_TEST FAIL\n");
    return ok ? 0 : 1;
}
