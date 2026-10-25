// Behaviour test for 0x00708830: vector<{long, bool}> copy constructor (allocate size, copy range, set finish)
#include <cstdio>
#include <cstdlib>

struct Entry {
    long DefIndex;
    bool Rendered;
};

struct Vec {
    Entry* start;
    Entry* finish;
    Entry* end_of_storage;
    Vec() : start(0), finish(0), end_of_storage(0) { }
    Vec(const Vec& x) : start(0), finish(0), end_of_storage(0)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = n != 0 ? (Entry*)std::malloc(n * sizeof(Entry)) : 0;
        finish = start;
        end_of_storage = start + n;
        Entry* cur = start;
        for (const Entry* p = x.start; p != x.finish; ++p, ++cur)
            if (cur != 0) *cur = *p;
        finish = cur;
    }
};

int main()
{
    Vec empty;
    Vec e2(empty);
    if (e2.start != 0 || e2.finish != 0 || e2.end_of_storage != 0) { std::printf("00708830_TEST FAIL\n"); return 1; }
    Entry data[4];
    for (int i = 0; i < 4; ++i) { data[i].DefIndex = 10 * i + 1; data[i].Rendered = (i % 2) == 0; }
    Vec src; src.start = data; src.finish = data + 4; src.end_of_storage = data + 4;
    Vec c(src);
    if (c.start == data || c.finish - c.start != 4 || c.end_of_storage != c.finish) { std::printf("00708830_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i)
        if (c.start[i].DefIndex != 10 * i + 1 || c.start[i].Rendered != ((i % 2) == 0)) { std::printf("00708830_TEST FAIL\n"); return 1; }
    std::free(c.start);
    std::printf("00708830_TEST PASS\n");
    return 0;
}