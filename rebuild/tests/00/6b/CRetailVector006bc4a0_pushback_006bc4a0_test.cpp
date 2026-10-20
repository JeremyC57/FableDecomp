// Behaviour test for 006bc4a0: body copied from src/006bc4a0.cpp (CRetailVector_006bc4a0::push_back), callees stubbed.
#include <cstdio>
#include <cstring>
#include <new>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
struct SScalarTag { SScalarTag() { } };
static int g_grow = 0; static long g_growValue = 0; static long* g_growWhere = 0;
struct CRetailVector_006bc4a0 {
    long* first;
    long* last;
    long* end;
    void InsertGrow(long* where, const long& value, const SScalarTag& tag, unsigned long count, bool noTail)
    {
        ++g_grow; g_growWhere = where; g_growValue = value; CHECK(count == 1 && noTail);
        static long storage[8]; long n = last - first; for (long i = 0; i < n; ++i) storage[i] = first[i]; storage[n] = value; first = storage; last = storage + n + 1; end = storage + 8;
    }
    void push_back(long value);
};
void CRetailVector_006bc4a0::push_back(long value)
{
    if (last != end) {
        new (last) long(value);
        ++last;
    }
    else
        InsertGrow(last, value, SScalarTag(), 1, true);
}
int main()
{
    long buf[2] = { 0, 0 };
    CRetailVector_006bc4a0 v; v.first = buf; v.last = buf; v.end = buf + 2;
    v.push_back(11); CHECK(v.last == buf + 1 && buf[0] == 11 && g_grow == 0);
    v.push_back(22); CHECK(v.last == buf + 2 && buf[1] == 22 && g_grow == 0);
    // full: the growing insert takes the value
    v.push_back(33); CHECK(g_grow == 1 && g_growWhere == buf + 2 && g_growValue == 33); CHECK(v.last - v.first == 3 && v.first[2] == 33 && v.first[0] == 11);
    v.push_back(44); CHECK(g_grow == 1 && v.first[3] == 44);
    if (fails == 0) printf("006bc4a0_TEST PASS\n"); else printf("006bc4a0_TEST FAIL\n");
    return fails;
}