#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __stdcall
#define __fastcall
#define __forceinline inline
#define __declspec(x)
#define __int64 long long
#endif
typedef int LONG32;   // the retail 4-byte long (the g++ host is LP64)
#define TEST_ADDR "00a21c30"
#define CHECK(c) do { if (!(c)) { std::printf("%s_TEST FAIL (line %d)\n", TEST_ADDR, __LINE__); return 1; } } while (0)

struct CAStarState { int total; };
struct CAStarStateTotalGreaterThan { bool operator()(const CAStarState* a, const CAStarState* b) const { return a->total > b->total; } };
static int g_calls = 0, g_hole = -1, g_top = -1; static CAStarState** g_first = 0; static CAStarState* g_value = 0;
void __fastcall __push_heap(CAStarState** first, int holeIndex, int topIndex, CAStarState* value, CAStarStateTotalGreaterThan)
{
    ++g_calls; g_first = first; g_hole = holeIndex; g_top = topIndex; g_value = value;
}
template <class RandomIt, class Compare>
inline void push_heap(RandomIt first, RandomIt last, Compare comp) { __push_heap(first, (int)((last - first) - 1), 0, *(last - 1), comp); }
template <class T> struct CStateVector { T* _M_start; T* _M_finish; T* _M_end_of_storage; T* begin() { return _M_start; } T* end() { return _M_finish; } };
template <class T> struct CPriorityQueue {
    CStateVector<T> Heap;
    template <class Compare> void UpdateElement(const T& element, const Compare& comp)
    {
        for (T* it = Heap.begin(); it != Heap.end(); ++it)
            if (element == *it) {
                push_heap(Heap.begin(), it + 1, comp);
                return;
            }
    }
};
int main()
{
    CAStarState s[4]; CAStarState* heap[4] = { &s[0], &s[1], &s[2], &s[3] };
    CPriorityQueue<CAStarState*> q; q.Heap._M_start = heap; q.Heap._M_finish = heap + 4; q.Heap._M_end_of_storage = heap + 4;
    CAStarStateTotalGreaterThan comp;
    CAStarState* e = &s[2];
    q.UpdateElement(e, comp);
    CHECK(g_calls == 1 && g_first == heap && g_hole == 2 && g_top == 0 && g_value == &s[2]);
    CAStarState other; CAStarState* o = &other;
    q.UpdateElement(o, comp);
    CHECK(g_calls == 1);
    std::printf("00a21c30_TEST PASS\n");
    return 0;
}