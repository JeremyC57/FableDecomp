// Behaviour test for 0x0057F29E std::__copy over 24-byte CHeroMarriageInfo records: forward assignment loop, returns result + n.
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
    E a[5], b[5];
    for (int i = 0; i < 5; ++i) { a[i] = Make(i + 1); b[i] = Make(100); }
    E* r = CopyFwd(a + 1, a + 4, b);           // 3 records
    bool ok = r == b + 3 && Same(b[0], Make(2)) && Same(b[1], Make(3)) && Same(b[2], Make(4)) && Same(b[3], Make(100));
    r = CopyFwd(a + 2, a + 2, b + 4);          // empty range
    ok = ok && r == b + 4 && Same(b[4], Make(100));
    // overlapping shift left (vector::erase): a = 1 2 3 4 5 -> 2 3 4 5 5
    r = CopyFwd(a + 1, a + 5, a);
    ok = ok && r == a + 4 && Same(a[0], Make(2)) && Same(a[3], Make(5)) && Same(a[4], Make(5));
    std::printf(ok ? "0057f29e_TEST PASS\n" : "0057f29e_TEST FAIL\n");
    return ok ? 0 : 1;
}