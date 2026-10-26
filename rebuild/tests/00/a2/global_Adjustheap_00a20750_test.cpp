// behaviour test for 00a20750: _Adjust_heap over CAStarState* by TotalCost (greater-than = min-heap),
// driven through make_heap/sort_heap; sorting must give descending TotalCost.
#include <cstdio>
struct CAStarState {
    CAStarState* ParentState;
    void* Node;
    float CostToThisState;
    float TotalCost;
    bool OnOpenList;
    bool OnClosedList;
};
class CAStarStateTotalGreaterThan { public:
    char m_tag;
    bool operator()(const CAStarState* a, const CAStarState* b) const { return a->TotalCost > b->TotalCost; }
};
typedef CAStarState* P;
static void PushHeap(P* first, long hole, long top, P value, CAStarStateTotalGreaterThan pred)
{
    long parent = (hole - 1) / 2;
    while (top < hole && pred(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
static void AdjustHeap(P* first, long hole, long bottom, P value, CAStarStateTotalGreaterThan pred)
{
    long top = hole;
    long idx = 2 * hole + 2;
    for (; idx < bottom; idx = 2 * idx + 2) {
        if (pred(first[idx], first[idx - 1]))
            --idx;
        first[hole] = first[idx], hole = idx;
    }
    if (idx == bottom) {
        first[hole] = first[idx - 1];
        hole = idx - 1;
    }
    PushHeap(first, hole, top, value, pred);
}
int main()
{
    static const float costs[8] = { 3.5f, 1.0f, 9.25f, 4.0f, 4.0f, 0.5f, 7.0f, 2.0f };
    CAStarState states[8];
    P a[8];
    CAStarStateTotalGreaterThan pred;
    pred.m_tag = 0;
    long i = 0;
    long n = 8;
    long last = 0;
    for (i = 0; i < n; ++i) { states[i].TotalCost = costs[i]; states[i].CostToThisState = 0.0f; a[i] = &states[i]; }
    for (i = n / 2; 0 < i; ) { --i; AdjustHeap(a, i, n, a[i], pred); }
    int ok = 1;
    if (a[0]->TotalCost != 0.5f) ok = 0;
    for (last = n; 1 < last; --last) {
        P value = a[last - 1];
        a[last - 1] = a[0];
        AdjustHeap(a, 0, last - 1, value, pred);
    }
    for (i = 1; i < n; ++i) if (a[i - 1]->TotalCost < a[i]->TotalCost) ok = 0;
    if (a[0]->TotalCost != 9.25f || a[7]->TotalCost != 0.5f) ok = 0;
    if (!ok) { std::printf("00a20750_TEST FAIL\n"); return 1; }
    std::printf("00a20750_TEST PASS\n");
    return 0;
}