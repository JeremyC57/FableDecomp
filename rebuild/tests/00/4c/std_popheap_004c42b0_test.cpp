// Behaviour test for 004c42b0 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
// pop_heap forwards (first, last, comp) to __pop_heap_aux: the root moves to last-1, the rest stays a heap
struct CCameraTag { int A, B; };
struct Pair { float first; CCameraTag second; };
struct Less { bool operator()(const Pair& a, const Pair& b) const { return a.first < b.first; } };
static void adjust_heap(Pair* first, int hole, int len, Pair v, Less comp) {
    int top = hole; int child = 2 * hole + 2;
    while (child < len) { if (comp(first[child], first[child - 1])) child--; first[hole] = first[child]; hole = child; child = 2 * (child + 1); }
    if (child == len) { first[hole] = first[child - 1]; hole = child - 1; }
    int parent = (hole - 1) / 2;
    while (hole > top && comp(first[parent], v)) { first[hole] = first[parent]; hole = parent; parent = (hole - 1) / 2; }
    first[hole] = v;
}
static int g_AuxCalls = 0;
static void pop_heap_aux(Pair* first, Pair* last, Pair*, Less comp) { ++g_AuxCalls; Pair v = *(last - 1); *(last - 1) = *first; adjust_heap(first, 0, (int)(last - 1 - first), v, comp); }
static void pop_heap(Pair* first, Pair* last, Less comp) { pop_heap_aux(first, last, (Pair*)0, comp); }
int main() {
    Pair h[5]; float k[5] = { 9.0f, 7.0f, 8.0f, 1.0f, 3.0f };
    for (int i = 0; i < 5; ++i) { h[i].first = k[i]; h[i].second.A = i; h[i].second.B = -i; }
    pop_heap(h, h + 5, Less());
    if (g_AuxCalls != 1 || h[4].first != 9.0f || h[4].second.A != 0 || h[0].first != 8.0f) { std::printf("004c42b0_TEST FAIL\n"); return 1; }
    { std::printf("004c42b0_TEST PASS\n"); return 0; }
}