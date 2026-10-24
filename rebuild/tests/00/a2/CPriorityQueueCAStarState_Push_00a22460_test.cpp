// Behaviour test for CPriorityQueue<CAStarState*>::Push (0x00A22460): pushing states keeps the vector a valid binary
// heap under CAStarStateTotalGreaterThan (smallest total on top), through both the in-place and the overflow path.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

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
    vector() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    typedef _Tp* iterator;
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool)
    {
        unsigned int __old = (unsigned int)(_M_finish - _M_start);
        unsigned int __len = __old + (__old > __fill_len ? __old : __fill_len);
        _Tp* __new_start = (_Tp*)std::malloc(__len * sizeof(_Tp));
        unsigned int __before = (unsigned int)(__position - _M_start);
        if (__before) std::memmove(__new_start, _M_start, __before * sizeof(_Tp));
        _Tp* __new_finish = __new_start + __before;
        for (unsigned int i = 0; i < __fill_len; ++i) *__new_finish++ = __x;
        std::free(_M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage._M_data = __new_start + __len;
    }
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
void __push_heap(_RandomAccessIterator __first, _Distance __holeIndex, _Distance __topIndex, _Tp __value, _Compare __comp)
{
    _Distance __parent = (__holeIndex - 1) / 2;
    while (__holeIndex > __topIndex && __comp(*(__first + __parent), __value)) {
        *(__first + __holeIndex) = *(__first + __parent);
        __holeIndex = __parent;
        __parent = (__holeIndex - 1) / 2;
    }
    *(__first + __holeIndex) = __value;
}
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

class CAStarState { public: float Total; };

class CAStarStateTotalGreaterThan {
public:
    bool operator()(const CAStarState* a, const CAStarState* b) const { return a->Total > b->Total; }
};

template <class T> class CPriorityQueue {
public:
    _STL::vector<T> Elements;
    template <class C> void Push(const T& element, const C& compare)
    {
        Elements.push_back(element);
        _STL::push_heap(Elements.begin(), Elements.end(), compare);
    }
};

int main()
{
    int ok = 1;
    float totals[9] = { 5.0f, 3.0f, 8.0f, 1.0f, 9.0f, 2.0f, 7.0f, 0.5f, 4.0f };
    CAStarState states[9];
    CPriorityQueue<CAStarState*> q;
    CAStarStateTotalGreaterThan cmp;
    for (int i = 0; i < 9; ++i) {
        states[i].Total = totals[i];
        CAStarState* p = &states[i];
        q.Push(p, cmp);
        int n = (int)(q.Elements._M_finish - q.Elements._M_start);
        if (n != i + 1) ok = 0;
        for (int c = 1; c < n; ++c)                                    // heap property: parent <= child
            if (q.Elements._M_start[(c - 1) / 2]->Total > q.Elements._M_start[c]->Total) ok = 0;
    }
    if (q.Elements._M_start[0] != &states[7]) ok = 0;                 // smallest total (0.5) on top
    std::free(q.Elements._M_start);

    if (!ok) { std::printf("00a22460_TEST FAIL\n"); return 1; }
    std::printf("00a22460_TEST PASS\n");
    return 0;
}