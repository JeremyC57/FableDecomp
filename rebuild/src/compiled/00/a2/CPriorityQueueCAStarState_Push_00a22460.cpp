// CPriorityQueue<CAStarState*>::Push(CAStarState* const& element, const CAStarStateTotalGreaterThan& compare) -- append the state and sift it up the binary heap
// Leaf: ??$Push@VCAStarStateTotalGreaterThan@@@?$CPriorityQueue@PAVCAStarState@CNavigatorAStar@@@@QAEXABQAVCAStarState@CNavigatorAStar@@ABVCAStarStateTotalGreaterThan@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A22460, 92 bytes, `ret 8`. Callers 0x00A1D091 / 0x00A1D519 (CNavigatorAStar,
//   OpenList at +0x60). The catalog label "PriorityQueue_Push" is descriptive.
// Identity: Ego_r.pdb `??$Push@VCAStarStateTotalGreaterThan@@@?$CPriorityQueue@PAVCAStarState@CNavigatorAStar@@@@QAEX
//   ABQAVCAStarState@CNavigatorAStar@@ABVCAStarStateTotalGreaterThan@@@Z` (a member template over the comparator).
// Callees: vector<CAStarState*>::_M_insert_overflow 0x00A21CB0 (manifest "PriorityQueue_Reallocate"), STLport
//   __push_heap 0x00A206F0 (__fastcall: first in ecx, holeIndex in edx; topIndex 0, value, comparator on the stack;
//   manifest "_Push_heap<float*,int,float,less<float>>" is an identical-body fold).
// Shape: inline push_back (placement-new null test, `add [esi+4],4`; the overflow tag homed on the element argument
//   slot), then push_heap -> __push_heap_aux inlined: the empty comparator is copied byte-wise into the dead element
//   argument slot and pushed as a dword, value = *(last - 1), holeIndex = (last - first) - 1.
inline void* operator new(unsigned int, void* where) { return where; }

namespace _STL {
struct __true_type { __true_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x00A21CB0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};
template <class _RandomAccessIterator, class _Distance, class _Tp, class _Compare>
void __fastcall __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __value, _Compare __comp);   // 0x00A206F0
template <class _RandomAccessIterator, class _Compare, class _Distance, class _Tp>
inline void __push_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp, _Distance*, _Tp*)
{
    __push_heap(__first, _Distance((__last - __first) - 1), _Distance(0), _Tp(*(__last - 1)), __comp);
}
template <class _Tp> inline int* __distance_type(const _Tp*) { return (int*)0; }
template <class _Tp> inline _Tp* __value_type(const _Tp*) { return (_Tp*)0; }
template <class _RandomAccessIterator, class _Compare>
inline void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp)
{
    __push_heap_aux(__first, __last, __comp, __distance_type(__first), __value_type(__first));
}
}

class CNavigatorAStar {
public:
    class CAStarState;
};

class CAStarStateTotalGreaterThan {
public:
    bool operator()(const CNavigatorAStar::CAStarState* a, const CNavigatorAStar::CAStarState* b) const;
};

template <class T> class CPriorityQueue {
public:
    _STL::vector<T> Elements;                           // +0x00
    template <class C> void Push(const T& element, const C& compare)
    {
        Elements.push_back(element);
        _STL::push_heap(Elements.begin(), Elements.end(), compare);
    }
};

template void CPriorityQueue<CNavigatorAStar::CAStarState*>::Push<CAStarStateTotalGreaterThan>(CNavigatorAStar::CAStarState* const&, const CAStarStateTotalGreaterThan&);