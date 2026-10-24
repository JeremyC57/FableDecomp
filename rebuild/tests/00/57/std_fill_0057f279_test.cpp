// Behaviour test for 0x0057F279 std::fill over 24-byte CHeroMarriageInfo records: every record in [first, last)
// becomes a copy of val; an empty range is a no-op.
#include <cstdio>
#include <cstring>

struct E { unsigned long long WifeUID; unsigned long long MaritalHomeUID; int State; bool GayMarriage; };
static void Fill(E* first, E* last, const E& val)
{
    for (; first != last; ++first)
        *first = val;
}

int main()
{
    E a[4];
    std::memset(a, 0, sizeof a);
    a[3].State = 7;
    E v; std::memset(&v, 0, sizeof v);
    v.WifeUID = 0x1122334455667788ull; v.MaritalHomeUID = 42; v.State = 2; v.GayMarriage = true;
    Fill(a, a + 3, v);
    bool ok = a[3].State == 7;
    for (int i = 0; i < 3; ++i) ok = ok && std::memcmp(&a[i], &v, sizeof v) == 0;
    Fill(a + 3, a + 3, v);
    ok = ok && a[3].State == 7 && a[3].WifeUID == 0;
    std::printf(ok ? "0057f279_TEST PASS\n" : "0057f279_TEST FAIL\n");
    return ok ? 0 : 1;
}