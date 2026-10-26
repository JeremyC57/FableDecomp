// Behaviour test for 0x007B84C0 __copy_backward over intelligent pointers: backward element assignment (insert shift).
#include <cstdio>
class CThing { public: int id; };
struct IP {
    CThing* PItem;
    CThing* GetPItem() const { return PItem; }
    void SetPItem(CThing* p) { PItem = p; }
    IP& operator=(const IP& o) { SetPItem(o.GetPItem()); return *this; }
};
static IP* CopyBackward(IP* first, IP* last, IP* result)
{
    for (int n = (int)(last - first); n > 0; --n) *--result = *--last;
    return result;
}
int main()
{
    CThing t[4];
    IP a[4]; for (int i = 0; i < 4; ++i) a[i].PItem = &t[i];
    // overlapping right shift (insert at a[0]): a[1..3] = a[0..2]
    IP* start = CopyBackward(a, a + 3, a + 4);
    if (start != a + 1 || a[1].PItem != &t[0] || a[2].PItem != &t[1] || a[3].PItem != &t[2] || a[0].PItem != &t[0]) { std::printf("007b84c0_TEST FAIL\n"); return 1; }
    if (CopyBackward(a, a, a + 2) != a + 2) { std::printf("007b84c0_TEST FAIL\n"); return 1; }
    std::printf("007b84c0_TEST PASS\n");
    return 0;
}