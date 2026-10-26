// behaviour test for 00af3430: SGI __pop_heap for the registered particle-component table:
// *result = *first, then the saved value is sifted down from the root over [first, last).
#include <cstdio>
#include <cstring>
class CCharString { public:
    const char* Text;
    bool operator<(const CCharString& o) const { return std::strcmp(Text, o.Text) < 0; }
};
struct CRegisteredPSCInfo { void* AllocFunc; int ComponentType; CCharString GUIName; };
struct CPSCPair { CCharString first; CRegisteredPSCInfo second; };
struct CPSCLess { bool operator()(const CPSCPair& a, const CPSCPair& b) const { return a.first < b.first; } };
static void PushHeap(CPSCPair* first, int holeIndex, int topIndex, CPSCPair value, CPSCLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), value)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}
static void AdjustHeap(CPSCPair* first, int holeIndex, int len, CPSCPair value, CPSCLess comp)
{
    int topIndex = holeIndex;
    int secondChild = 2 * holeIndex + 2;
    while (secondChild < len) {
        if (comp(*(first + secondChild), *(first + (secondChild - 1))))
            secondChild--;
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if (secondChild == len) {
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    PushHeap(first, holeIndex, topIndex, value, comp);
}
static void PopHeap(CPSCPair* first, CPSCPair* last, CPSCPair* result, CPSCPair value, CPSCLess comp, int*)
{
    *result = *first;
    AdjustHeap(first, 0, int(last - first), value, comp);
}
int main()
{
    static const char* names[7] = { "Emitter", "Colour", "Size", "Alpha", "Spin", "Decal", "Light" };
    CPSCPair a[7];
    CPSCLess comp;
    int i = 0;
    int n = 7;
    int last = 0;
    for (i = 0; i < n; ++i) { a[i].first.Text = names[i]; a[i].second.ComponentType = i; a[i].second.GUIName.Text = names[i]; a[i].second.AllocFunc = 0; }
    for (i = n / 2; 0 < i; ) { --i; AdjustHeap(a, i, n, a[i], comp); }
    int ok = 1;
    if (std::strcmp(a[0].first.Text, "Spin") != 0) ok = 0;
    for (last = n; 1 < last; --last)
        PopHeap(a, a + last - 1, a + last - 1, a[last - 1], comp, (int*)0);
    for (i = 1; i < n; ++i) if (a[i].first < a[i - 1].first) ok = 0;
    for (i = 0; i < n; ++i) if (a[i].second.GUIName.Text != a[i].first.Text || names[a[i].second.ComponentType] != a[i].first.Text) ok = 0;
    if (std::strcmp(a[0].first.Text, "Alpha") != 0 || std::strcmp(a[6].first.Text, "Spin") != 0) ok = 0;
    if (!ok) { std::printf("00af3430_TEST FAIL\n"); return 1; }
    std::printf("00af3430_TEST PASS\n");
    return 0;
}