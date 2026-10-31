// behaviour test for C2DBoxI::Overlapping 0x004FAB40: separating-axis test on centres and half extents (signed /2)
#include <cstdio>
#include <cstdlib>
struct B { long TLX, TLY, BRX, BRY;
    long HW() const { return (BRX - TLX) / 2; } long HH() const { return (BRY - TLY) / 2; }
    int Overlapping(const B& o) const {
        if (labs((TLX + HW()) - (o.TLX + o.HW())) <= HW() + o.HW()) {
            long dy = labs((TLY + HH()) - (o.TLY + o.HH()));
            if (dy <= HH() + o.HH()) return 1;
        }
        return 0;
    } };
int main()
{
    B a = { 0, 0, 10, 10 }, b = { 5, 5, 15, 15 }, c = { 20, 0, 30, 10 }, d = { 0, 20, 10, 30 }, e = { 10, 10, 20, 20 };
    if (a.Overlapping(b) != 1 || a.Overlapping(c) != 0 || a.Overlapping(d) != 0 || a.Overlapping(e) != 1 || c.Overlapping(a) != 0)
        { std::printf("004fab40_TEST FAIL\n"); return 1; }
    std::printf("004fab40_TEST PASS\n"); return 0;
}