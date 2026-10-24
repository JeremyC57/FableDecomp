// behaviour test for 0x00a206f0 _Push_heap (CAStarState* elements)
#include <cstdio>
struct CAStarState { CAStarState* ParentState; void* Node; float CostToThisState; float TotalCost; bool OnOpenList; bool OnClosedList; };
struct CCmp { char m_tag; bool operator()(const CAStarState* a, const CAStarState* b) const { return a->TotalCost > b->TotalCost; } };
static void _Push_heap(CAStarState** first, long hole, long top, CAStarState* value, CCmp comp)
{
    long parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], value)) {
        first[hole] = first[parent];
        hole = parent;
        parent = (hole - 1) / 2;
    }
    first[hole] = value;
}
int main()
{
    CCmp c; c.m_tag = 0;
    static const float costs[7] = { 5.0f, 1.5f, 8.0f, 3.0f, 0.5f, 2.0f, 4.0f };
    static CAStarState states[7];
    CAStarState* heap[7];
    for (int i = 0; i < 7; ++i) { states[i].TotalCost = costs[i]; _Push_heap(heap, i, 0, &states[i], c);
        for (int k = 1; k <= i; ++k) if (c(heap[(k - 1) / 2], heap[k])) { std::printf("00a206f0_TEST FAIL\n"); return 1; } }
    if (heap[0] != &states[4]) { std::printf("00a206f0_TEST FAIL\n"); return 1; }
    // hole == top: the value is stored without a comparison
    CAStarState* keep = heap[0];
    _Push_heap(heap, 0, 0, heap[6], c);
    if (heap[0] != heap[6]) { std::printf("00a206f0_TEST FAIL\n"); return 1; }
    heap[0] = keep;
    std::printf("00a206f0_TEST PASS\n");
    return 0;
}