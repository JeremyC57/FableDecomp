#include <cstdio>
// Host model of the NGameText::CGroupEntry constructor: empty id vector, shared group reference (count bumped), id, and
// the resource size 0x34 + capacity*4 + (group ? group size : 0).
struct Group { unsigned long extra; unsigned long Sizeof() const { return extra + 0x48; } };
struct Ptr { Group* data; long* info; };
struct Entry {
    unsigned long resourceSize;
    long* first; long* last; long* end;
    Ptr group; long id;
    Entry(const Ptr& g, long i) : first(0), last(0), end(0), id(i)
    {
        group = g;
        if (group.info) ++*group.info;
        unsigned long size = 0x34 + (unsigned long)(end - first) * 4;
        unsigned long gsize = group.data ? group.data->Sizeof() + 8 : 8;
        resourceSize = size + gsize - 8;
    }
};
int main()
{
    int fail = 0;
    Group g = { 0x10 };
    long count = 1;
    Ptr p = { &g, &count };
    Entry e(p, 42);
    if (e.resourceSize != 0x34 + 0x58 || count != 2 || e.id != 42 || e.first != 0) fail = 1;
    Ptr none = { 0, 0 };
    Entry f(none, 7);
    if (f.resourceSize != 0x34) fail = 1;
    if (fail) { std::printf("009c8060_TEST FAIL\n"); return 1; }
    std::printf("009c8060_TEST PASS\n");
    return 0;
}