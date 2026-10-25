// Behaviour test for 00f3b220: __uninitialized_fill_n over CScriptEntry -- n copies of x constructed from first, end pointer returned, n == 0 is a no-op
#include <cstdio>
#include <cstring>
#include <new>
struct Entry { unsigned long Flags; long BitCount; unsigned long RangeBias; long RangeShift; unsigned long ShuffleMask; unsigned long OrMask; };
static Entry* FillN(Entry* first, unsigned int n, const Entry& x) { Entry* cur = first; for (; n > 0; --n, ++cur) new (cur) Entry(x); return cur; }
int main()
{
    Entry buf[4]; std::memset(buf, 0, sizeof buf);
    Entry x = { 1, 2, 3, 4, 5, 6 };
    Entry* e = FillN(buf, 3, x);
    int ok = sizeof(Entry) == 24 && e == buf + 3 && buf[0].OrMask == 6 && buf[2].Flags == 1 && buf[2].RangeShift == 4 && buf[3].Flags == 0;
    ok = ok && FillN(buf + 3, 0, x) == buf + 3 && buf[3].Flags == 0;
    std::printf(ok ? "00f3b220_TEST PASS\n" : "00f3b220_TEST FAIL\n");
    return ok ? 0 : 1;
}
