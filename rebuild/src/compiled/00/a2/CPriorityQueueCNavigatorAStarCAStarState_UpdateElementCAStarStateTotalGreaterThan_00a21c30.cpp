// CPriorityQueue<CNavigatorAStar::CAStarState*>::UpdateElement<CAStarStateTotalGreaterThan>(CNavigatorAStar::CAStarState* const&, const CAStarStateTotalGreaterThan&) -- find an element in the heap and sift it up after its key improved
// Leaf: UpdateElement
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A21C30, 73 bytes, `__fastcall`, `ret 8` (FableWin.pdb:
//   ??$UpdateElement@VCAStarStateTotalGreaterThan@@@?$CPriorityQueue@PAVCAStarState@CNavigatorAStar@@@@QAEXABQAV12@ABV3@@Z).
// Callee: STLport __push_heap<CAStarState**, int, CAStarState*, CAStarStateTotalGreaterThan> @ 0x00A206F0 (`__fastcall`: first in
//   ecx, hole index in edx, then top index 0, the value and the comparator by value on the stack; catalogued under a
//   propagated `_Push_heap<float*,...>` label).
// Layout: Ego_r.pdb CPriorityQueue<T> = `vector<T> Heap` at +0 (retail STLport: start +0, finish +4).
// Shape: a linear search for the element (the key is read once before the rotated loop; `element == *it` with the hit
//   handled and returned inside the loop, a break + `it != end` test adds a second compare); on a hit the inline
//   std::push_heap(begin, it + 1, comp) -> __push_heap_aux passes `(last - first) - 1` (`lea edx,[eax+4] ; sub ; sar ; dec`),
//   0, `*(last - 1)` and the empty comparator copied by value (its byte is staged in the dead element-argument slot).
namespace CNavigatorAStar { class CAStarState; }
class CAStarStateTotalGreaterThan {
public:
    bool operator()(const CNavigatorAStar::CAStarState* a, const CNavigatorAStar::CAStarState* b) const;
};

void __fastcall __push_heap(CNavigatorAStar::CAStarState** first, int holeIndex, int topIndex,
                            CNavigatorAStar::CAStarState* value, CAStarStateTotalGreaterThan comp);   // 0x00A206F0

namespace std {
template <class RandomIt, class Compare, class Distance, class T>
inline void __push_heap_aux(RandomIt first, RandomIt last, Compare comp, Distance*, T*)
{
    __push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)), comp);
}
template <class RandomIt, class Compare>
inline void push_heap(RandomIt first, RandomIt last, Compare comp)
{
    __push_heap_aux(first, last, comp, (int*)0, (CNavigatorAStar::CAStarState**)0);
}
}

template <class T> struct CStateVector {             // STLport vector<T>
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    T* begin() { return _M_start; }
    T* end() { return _M_finish; }
};

template <class T> class CPriorityQueue {
public:
    CStateVector<T> Heap;                             // +0x00
    template <class Compare> void UpdateElement(const T& element, const Compare& comp)
    {
        for (T* it = Heap.begin(); it != Heap.end(); ++it)
            if (element == *it) {
                std::push_heap(Heap.begin(), it + 1, comp);
                return;
            }
    }
};

template void CPriorityQueue<CNavigatorAStar::CAStarState*>::UpdateElement<CAStarStateTotalGreaterThan>(
    CNavigatorAStar::CAStarState* const& element, const CAStarStateTotalGreaterThan& comp);