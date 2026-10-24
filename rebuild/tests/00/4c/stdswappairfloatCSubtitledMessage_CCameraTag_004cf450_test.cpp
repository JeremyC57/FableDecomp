// Behaviour test for 0x004CF450 swap(x, y) over pair<float, CCameraTag> (28 bytes): every field of the two entries
// is exchanged; swapping an entry with itself leaves it unchanged.
#include <cstdio>

struct CameraTag { int PosPerson, Pos, FocusPerson, FocusPos, Zoom, View; };
struct Entry { float first; CameraTag second; Entry() {} Entry(const Entry& o) : first(o.first), second(o.second) {} };
static void Swap(Entry& a, Entry& b) { Entry t = a; a = b; b = t; }
static void IterSwap(Entry* a, Entry* b) { Entry t = *a; *a = *b; *b = t; }
static bool Same(const Entry& e, float f, int base)
{
    return e.first == f && e.second.PosPerson == base && e.second.Pos == base + 1 && e.second.FocusPerson == base + 2 &&
           e.second.FocusPos == base + 3 && e.second.Zoom == base + 4 && e.second.View == base + 5;
}

int main()
{
    Entry x, y;
    x.first = 1.5f; x.second.PosPerson = 10; x.second.Pos = 11; x.second.FocusPerson = 12; x.second.FocusPos = 13; x.second.Zoom = 14; x.second.View = 15;
    y.first = -2.0f; y.second.PosPerson = 20; y.second.Pos = 21; y.second.FocusPerson = 22; y.second.FocusPos = 23; y.second.Zoom = 24; y.second.View = 25;
    Swap(x, y);
    if (!Same(x, -2.0f, 20) || !Same(y, 1.5f, 10)) { std::printf("004cf450_TEST FAIL\n"); return 1; }
    Swap(x, x);
    if (!Same(x, -2.0f, 20)) { std::printf("004cf450_TEST FAIL\n"); return 1; }
    std::printf("004cf450_TEST PASS\n");
    return 0;
}