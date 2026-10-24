#include <cstdio>
struct CCameraTag { long CameraPositionPerson, CameraPosition, CameraFocusPerson, CameraFocusPosition, CameraZoomType, CameraView; };
struct SEntry {
    float first; CCameraTag second;
    SEntry() : first(0.0f) { second.CameraPositionPerson = second.CameraPosition = second.CameraFocusPerson = 0; second.CameraFocusPosition = second.CameraZoomType = second.CameraView = 0; }
    SEntry(const SEntry& o) : first(o.first), second(o.second) { }
};
struct CLess { bool operator()(const SEntry& a, const SEntry& b) const { return a.first < b.first; } };
static void PushHeap(SEntry* first, int holeIndex, int topIndex, SEntry val, CLess comp)
{
    int parent = (holeIndex - 1) / 2;
    while (holeIndex > topIndex && comp(*(first + parent), val)) {
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = val;
}
static void AdjustHeap(SEntry* first, int holeIndex, int len, SEntry val, CLess comp)
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
    PushHeap(first, holeIndex, topIndex, val, comp);
}
static bool IsHeap(const SEntry* a, int n)
{
    for (int i = 1; i < n; ++i) if (a[(i - 1) / 2].first < a[i].first) return false;
    return true;
}
int main()
{
    const int N = 9;
    float keys[N] = { 3.0f, 8.5f, -2.0f, 4.0f, 11.0f, 0.5f, 7.0f, 7.0f, 1.0f };
    SEntry a[N];
    for (int i = 0; i < N; ++i) { a[i].first = keys[i]; a[i].second.CameraView = i; }
    for (int parent = (N - 2) / 2; ; --parent) {           // make_heap through adjust_heap
        AdjustHeap(a, parent, N, SEntry(a[parent]), CLess());
        if (parent == 0) break;
    }
    bool ok = IsHeap(a, N) && a[0].first == 11.0f && a[0].second.CameraView == 4;
    float last = 1e9f; long viewSum = 0;
    for (int n = N; n > 1; --n) {                           // pop_heap via adjust_heap: descending output
        SEntry v(a[n - 1]);
        a[n - 1] = a[0];
        AdjustHeap(a, 0, n - 1, v, CLess());
        if (a[n - 1].first > last) ok = false;
        last = a[n - 1].first;
        viewSum += a[n - 1].second.CameraView;
        if (!IsHeap(a, n - 1)) ok = false;
    }
    viewSum += a[0].second.CameraView;
    if (a[0].first != -2.0f || viewSum != 36) ok = false;
    if (!ok) { std::printf("004d0660_TEST FAIL\n"); return 1; }
    std::printf("004d0660_TEST PASS\n");
    return 0;
}