// Behaviour test for 0x00673E10: STLport vector copy constructor over 12-byte pair<unsigned long, CTextureMorph>
#include <cstdio>
#include <cstdlib>
#include <new>

struct Morph { unsigned int NewTexture; unsigned int GroupID; };
struct Entry {
    unsigned int first; Morph second;
    Entry() { }
    Entry(const Entry& o) : first(o.first), second(o.second) { }
};
struct Vec {
    Entry* start; Entry* finish; Entry* eos;
    Vec() : start(0), finish(0), eos(0) { }
    Vec(const Vec& x) : start(0), finish(0), eos(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n != 0 ? (Entry*)std::malloc(n * sizeof(Entry)) : 0;
        finish = start;
        eos = start + n;
        Entry* cur = start;
        for (const Entry* f = x.start; f != x.finish; ++f, ++cur)
            if (cur) new (cur) Entry(*f);
        finish = cur;
    }
};

int main()
{
    Vec empty;
    Vec e2(empty);
    if (e2.start != 0 || e2.finish != 0 || e2.eos != 0) { std::printf("00673e10_TEST FAIL\n"); return 1; }
    Entry src[3];
    for (int i = 0; i < 3; ++i) { src[i].first = 10 + i; src[i].second.NewTexture = 100 + i; src[i].second.GroupID = 1000 + i; }
    Vec a; a.start = src; a.finish = src + 3; a.eos = src + 3;
    Vec b(a);
    if (b.finish - b.start != 3 || b.eos != b.finish || b.start == src) { std::printf("00673e10_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i)
        if (b.start[i].first != (unsigned int)(10 + i) || b.start[i].second.NewTexture != (unsigned int)(100 + i) || b.start[i].second.GroupID != (unsigned int)(1000 + i)) {
            std::printf("00673e10_TEST FAIL\n"); return 1;
        }
    std::printf("00673e10_TEST PASS\n");
    return 0;
}