// std::vector<vector<CCard*> >::insert(vector<CCard*>* position, const vector<CCard*>& x) -- STLport single-element insert into the blackjack hands vector: copy-construct at the end, or copy the last hand up one slot, copy the rest backward and assign a copy of x; grow through _M_insert_overflow when full; returns the iterator to the inserted hand
// Leaf: ?insert@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DCB10, 232 bytes, thiscall `ret 8`.
// Identity: the only caller is CTCTavernGameCardBlackjack::Split (0x007DB5C0, call at 0x007DB615:
//   PlayersCards.insert(hand, empty hand)). The catalog label "vector_PushBack_ComplexElement" is descriptive.
// Callees: vector<CCard*>::vector(const vector&) @ 0x007DC350 and operator= @ 0x007DC3F0 (out of line here),
//   __copy_backward<vector<CCard*>*, vector<CCard*>*, int>(first, last, result, const random_access_iterator_tag&,
//   int*) @ 0x007DC600 (/Gr __fastcall), free @ 0x00BFEA14 (the copy's destructor),
//   vector<vector<CCard*> >::_M_insert_overflow(pointer, const T&, const __false_type&, size_type, bool) @ 0x007DC9A0.
// Shape: STLport 4.5 insert for a non-POD element: `size_type __n = __position - begin();` (the divide-by-12
//   multiply) ... `_Construct(_M_finish, *(_M_finish - 1)); ++_M_finish; _Tp __x_copy = __x;
//   __copy_backward_ptrs(__position, _M_finish - 2, _M_finish - 1, __false_type()); *__position = __x_copy;` ...
//   `else _M_insert_overflow(__position, __x, __false_type(), 1UL); return begin() + __n;` -- both empty tags are
//   homed on x's argument slot.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
inline void* operator new(unsigned int, void* p) { return p; }

struct CCard;

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _BidirectionalIter1, class _BidirectionalIter2, class _Distance>
_BidirectionalIter2 __fastcall __copy_backward(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                               _BidirectionalIter2 __result, const random_access_iterator_tag&,
                                               _Distance*);   // 0x007DC600
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    vector(const vector& __x);                              // 0x007DC350 for vector<CCard*>
    ~vector()
    {
        if (_M_start)
            free(_M_start);
    }
    vector& operator=(const vector& __x);                   // 0x007DC3F0 for vector<CCard*>
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x007DC9A0
    iterator insert(iterator __position, const _Tp& __x)
    {
        unsigned int __n = __position - begin();
        if (this->_M_finish != this->_M_end_of_storage) {
            if (__position == end()) {
                _Construct(this->_M_finish, __x);
                ++this->_M_finish;
            }
            else {
                _Construct(this->_M_finish, *(this->_M_finish - 1));
                ++this->_M_finish;
                _Tp __x_copy = __x;
                __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __false_type());
                *__position = __x_copy;
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), 1UL);
        return begin() + __n;
    }
};

template vector<CCard*>* vector<vector<CCard*> >::insert(vector<CCard*>*, const vector<CCard*>&);
}