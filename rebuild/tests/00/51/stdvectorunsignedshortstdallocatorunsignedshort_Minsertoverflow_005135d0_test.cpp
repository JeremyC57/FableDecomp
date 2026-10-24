// Behaviour test for 0x005135D0 vector<unsigned short>::_M_insert_overflow(position, x, __true_type, n, atEnd): the
// block grows to size + max(size, n); the prefix [start, position) is moved, n copies of x follow, the suffix
// [position, finish) is moved unless atEnd; the old block is freed and start/finish/end are re-set.
#include <cstdio>
#include <cstdlib>
#include <cstring>

struct VecModel {
    unsigned short* start; unsigned short* finish; unsigned short* end;
};

static void* copy_trivial(const void* first, const void* last, void* result)
{
    return (last == first) ? result : (char*)std::memmove(result, first, (const char*)last - (const char*)first) + ((const char*)last - (const char*)first);
}

static void insert_overflow(VecModel& v, unsigned short* position, const unsigned short& x, unsigned int n, bool atEnd)
{
    const unsigned int oldSize = (unsigned int)(v.finish - v.start);
    const unsigned int length = oldSize + (oldSize < n ? n : oldSize);
    unsigned short* newStart = length != 0 ? (unsigned short*)std::malloc(length * sizeof(unsigned short)) : 0;
    unsigned short* newFinish = (unsigned short*)copy_trivial(v.start, position, newStart);
    for (unsigned int k = n; k > 0; --k, ++newFinish) *newFinish = x;
    if (!atEnd) newFinish = (unsigned short*)copy_trivial(position, v.finish, newFinish);
    if (v.start) std::free(v.start);
    v.start = newStart; v.finish = newFinish; v.end = newStart + length;
}

static int fail() { std::printf("005135d0_TEST FAIL\n"); return 1; }

int main()
{
    VecModel v;
    v.start = (unsigned short*)std::malloc(4 * sizeof(unsigned short));
    for (int i = 0; i < 4; ++i) v.start[i] = (unsigned short)(100 + i);
    v.finish = v.end = v.start + 4;
    unsigned short x = 7;
    insert_overflow(v, v.start + 1, x, 2, false);
    static const unsigned short want1[6] = { 100, 7, 7, 101, 102, 103 };
    if (v.finish - v.start != 6 || v.end - v.start != 8 || std::memcmp(v.start, want1, sizeof(want1)) != 0) return fail();
    v.end = v.finish;
    unsigned short y = 9;
    insert_overflow(v, v.finish, y, 1, true);       // push_back growth
    if (v.finish - v.start != 7 || v.end - v.start != 12 || v.start[6] != 9 || v.start[5] != 103) return fail();
    VecModel e = { 0, 0, 0 };
    insert_overflow(e, 0, x, 3, true);              // empty vector: length = n, no prefix copy
    if (e.finish - e.start != 3 || e.end - e.start != 3 || e.start[2] != 7) return fail();
    std::free(v.start); std::free(e.start);
    std::printf("005135d0_TEST PASS\n");
    return 0;
}