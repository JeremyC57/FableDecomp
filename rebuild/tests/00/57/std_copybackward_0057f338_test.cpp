// Behaviour test for 0x0057F338 std::__copy_backward over 24-byte CHeroMarriageInfo records: backward assignment loop, returns result - n.
#include <cstdio>
#include <cstring>

struct E { unsigned long long WifeUID; unsigned long long MaritalHomeUID; int State; bool GayMarriage; };
static E Make(int k) { E e; std::memset(&e, 0, sizeof e); e.WifeUID = 0x100000000ull * k + k; e.MaritalHomeUID = k * 7; e.State = k % 3; e.GayMarriage = (k & 1) != 0; return e; }
static bool Same(const E& x, const E& y) { return std::memcmp(&x, &y, sizeof(E)) == 0; }
template <class I, class O> static O CopyFwd(I first, I last, O result)
{
    for (long n = (long)(last - first); n > 0; --n) { *result = *first; ++first; ++result; }
    return result;
}
template <class I, class O> static O CopyBwd(I first, I last, O result)
{
    for (long n = (long)(last - first); n > 0; --n) *--result = *--last;
    return result;
}

int main()
{
    E a[6];
    for (int i = 0; i < 6; ++i) a[i] = Make(i + 1);
    // overlapping shift right (insert): copy [0,4) to end at 6 -> 1 2 1 2 3 4
    E* r = CopyBwd(a, a + 4, a + 6);
    bool ok = r == a + 2 && Same(a[2], Make(1)) && Same(a[3], Make(2)) && Same(a[4], Make(3)) && Same(a[5], Make(4)) &&
              Same(a[0], Make(1)) && Same(a[1], Make(2));
    r = CopyBwd(a + 3, a + 3, a + 5);          // empty range
    ok = ok && r == a + 5 && Same(a[4], Make(3));
    std::printf(ok ? "0057f338_TEST PASS\n" : "0057f338_TEST FAIL\n");
    return ok ? 0 : 1;
}