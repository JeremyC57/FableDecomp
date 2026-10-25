// Behaviour test for 0x00723270: vector<{point, thing pointer}>::push_back (copy in place, else grow)
#include <cstdio>
#include <cstdlib>

struct Entry {
    float X, Y, Z;
    unsigned int vtable;                                    // the intelligent pointer: vtable word, item word
    unsigned int item;
};

struct Vec {
    Entry* start;
    Entry* finish;
    Entry* end_of_storage;
    int    overflows;
    void insert_overflow(Entry* pos, const Entry& x, unsigned int n)
    {
        ++overflows;
        unsigned int old = (unsigned int)(finish - start);
        unsigned int len = old + (old > n ? old : n);
        Entry* ns = (Entry*)std::malloc(len * sizeof(Entry));
        Entry* p = ns;
        for (Entry* q = start; q != pos; ++q, ++p) *p = *q;
        for (unsigned int i = 0; i < n; ++i, ++p) *p = x;
        std::free(start);
        start = ns; finish = p; end_of_storage = ns + len;
    }
    void push_back(const Entry& x)
    {
        if (finish != end_of_storage) {
            if (finish != 0) {
                finish->X = x.X; finish->Y = x.Y; finish->Z = x.Z;
                finish->vtable = 0x1238C6Cu;
                finish->item = x.item;
            }
            ++finish;
        } else
            insert_overflow(finish, x, 1);
    }
};

int main()
{
    Vec v; v.start = v.finish = v.end_of_storage = 0; v.overflows = 0;
    for (int i = 0; i < 9; ++i) {
        Entry e; e.X = (float)i; e.Y = (float)(i * 2); e.Z = 0.5f; e.vtable = 0u; e.item = 7u * (unsigned int)i;
        v.push_back(e);
    }
    if (v.finish - v.start != 9 || v.overflows != 5) { std::printf("00723270_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 9; ++i)
        if (v.start[i].X != (float)i || v.start[i].Y != (float)(i * 2) || v.start[i].Z != 0.5f || v.start[i].item != 7u * (unsigned int)i) {
            std::printf("00723270_TEST FAIL\n"); return 1;
        }
    if (v.start[3].vtable != 0x1238C6Cu || v.start[4].vtable != 0u) { std::printf("00723270_TEST FAIL\n"); return 1; }
    std::free(v.start);
    std::printf("00723270_TEST PASS\n");
    return 0;
}