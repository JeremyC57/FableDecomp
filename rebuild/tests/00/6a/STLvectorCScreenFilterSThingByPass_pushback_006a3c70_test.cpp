// Behaviour test for 0x006A3C70: vector<9-byte packed {pointer, flag}>::push_back (copy in place, else grow)
#include <cstdio>
#include <cstdlib>
#include <cstring>

#pragma pack(push, 1)
struct Entry {
    unsigned int vtable;                                    // 32-bit words, as on x86-32
    unsigned int item;
    bool         bypassSet;
};
#pragma pack(pop)

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
                finish->vtable = 0x1238C6Cu;
                finish->item = x.item;
                finish->bypassSet = x.bypassSet;
            }
            ++finish;
        } else
            insert_overflow(finish, x, 1);
    }
};

int main()
{
    if (sizeof(Entry) != 9) { std::printf("006a3c70_TEST FAIL\n"); return 1; }
    Vec v; v.start = v.finish = v.end_of_storage = 0; v.overflows = 0;
        for (int i = 0; i < 5; ++i) {
        Entry e; e.vtable = 0; e.item = 100u + (unsigned int)i; e.bypassSet = (i & 1) != 0;
        v.push_back(e);
    }
    if (v.finish - v.start != 5 || v.overflows != 4) { std::printf("006a3c70_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 5; ++i)
        if (v.start[i].item != 100u + (unsigned int)i || v.start[i].bypassSet != ((i & 1) != 0)) { std::printf("006a3c70_TEST FAIL\n"); return 1; }
    if (v.start[3].vtable != 0x1238C6Cu || v.start[4].vtable != 0u) { std::printf("006a3c70_TEST FAIL\n"); return 1; }
    std::free(v.start);
    std::printf("006a3c70_TEST PASS\n");
    return 0;
}