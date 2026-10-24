#include <cstdio>
struct CCameraTag { long CameraPositionPerson, CameraPosition, CameraFocusPerson, CameraFocusPosition, CameraZoomType, CameraView; };
struct SEntry {
    float first; CCameraTag second;
    SEntry() : first(0.0f) { second.CameraPositionPerson = second.CameraPosition = second.CameraFocusPerson = 0; second.CameraFocusPosition = second.CameraZoomType = second.CameraView = 0; }
    SEntry(const SEntry& o) : first(o.first), second(o.second) { }
};
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static void UnguardedLinearInsert(SEntry* last, SEntry val, CLess comp)
{
    SEntry* next = last;
    --next;
    while (comp(val, *next)) { *last = *next; last = next; --next; }
    *last = val;
}
static void UnguardedInsertionSortAux(SEntry* first, SEntry* last, SEntry*, CLess comp)
{
    for (SEntry* i = first; i != last; ++i)
        UnguardedLinearInsert(i, SEntry(*i), comp);
}
int main()
{
    SEntry a[7];
    float keys[7] = { -100.0f, 5.0f, 2.0f, 9.0f, 2.0f, -1.0f, 7.0f };
    for (int i = 0; i < 7; ++i) { a[i].first = keys[i]; a[i].second.CameraView = i; a[i].second.CameraZoomType = 10 * i; }
    UnguardedInsertionSortAux(a + 1, a + 7, 0, CLess());
    float want[7] = { -100.0f, -1.0f, 2.0f, 2.0f, 5.0f, 7.0f, 9.0f };
    long view[7] = { 0, 5, 2, 4, 1, 6, 3 };                  // stable for equal keys (2.0 at 2 then 4)
    bool ok = true;
    for (int i = 0; i < 7; ++i)
        if (a[i].first != want[i] || a[i].second.CameraView != view[i] || a[i].second.CameraZoomType != 10 * view[i]) ok = false;
    SEntry b[2]; b[0].first = 1.0f; b[1].first = 3.0f;
    UnguardedInsertionSortAux(b + 2, b + 2, 0, CLess());    // empty range: no work
    if (b[0].first != 1.0f || b[1].first != 3.0f) ok = false;
    if (!ok) { std::printf("004d0460_TEST FAIL\n"); return 1; }
    std::printf("004d0460_TEST PASS\n");
    return 0;
}