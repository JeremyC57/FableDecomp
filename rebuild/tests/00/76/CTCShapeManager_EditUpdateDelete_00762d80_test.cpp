#include <cstdio>
// Behaviour model of the CTCShapeManager editor rebuild (0x00762d80): edit the selected shape, then replace its list entry by
// a fresh shape of the same type built from a copy of its points (or drop it when the delete leaves nothing).
struct Shape { int type; int npoints; float firstX; };
struct Manager {
    Shape list[4]; int count; int sel;
    void Rebuild() { Shape s = list[sel]; for (int i = sel + 1; i < count; ++i) list[i - 1] = list[i]; --count; sel = 0; list[count++] = s; }
    void Op(int which, float x) {
        if (sel == count) return;
        Shape& s = list[sel];
        if (which == 0) { if (--s.npoints > 0) Rebuild(); else { for (int i = sel + 1; i < count; ++i) list[i - 1] = list[i]; --count; } }
        else if (which == 1) { if (s.npoints > 2) { s.npoints++; Rebuild(); } }
        else { s.firstX = x; Rebuild(); }
    }
};
int main()
{
    Manager m = { { {0, 3, 1.0f}, {1, 1, 2.0f} }, 2, 1 };
    int which = 0;
    m.Op(which, 5.0f);
    bool ok = which == 0 ? (m.count == 1) : which == 1 ? (m.count == 2 && m.list[1].npoints == 1) : (m.count == 2 && m.list[1].firstX == 5.0f && m.list[1].type == 1);
    if (!ok) { std::printf("00762d80_TEST FAIL\n"); return 1; }
    std::printf("00762d80_TEST PASS\n");
    return 0;
}