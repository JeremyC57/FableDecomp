// _STL::vector<long>::insert(long* position, const long& x) -- STLport single-element insert into a vector of longs (the blackjack bets): append at the end, or shift the tail up one slot (memmove) and store the value; grow through _M_insert_overflow when full; returns the iterator to the inserted value
// Leaf: ?insert@?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x007DC750, 167 bytes, thiscall `ret 8`.
// Identity: CTCTavernGameCardBlackjack::Split (0x007DB5C0, call at 0x007DB68D) inserts the second bet with it
//   (Bets.insert(bet, BaseBet)); the overflow 0x00440990 is vector<long>::_M_insert_overflow (the same callee as
//   CTCHeroReceiveItems::SetItemAsPickedUp 0x007CB850's push_back). The body is the vector<CCard*> insert 0x007D78A0
//   over a 4-byte long. The catalog label "vector_PushBack_uint" is descriptive.
// Callees: memmove @ 0x00BFEAE6 (STLport __copy_trivial_backward), vector<long>::_M_insert_overflow(long*,
//   const long&, const __true_type&, size_type, bool) @ 0x00440990.
// Shape: STLport 4.5 insert, as 0x007D78A0: the overflow's empty tag is homed on x's argument slot.
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6
inline void* operator new(unsigned int, void* p) { return p; }

namespace _STL {
struct __true_type { __true_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

inline void* __copy_trivial_backward(const void* __first, const void* __last, void* __result)
{
    const int _Num = (const char*)__last - (const char*)__first;
    return (_Num > 0) ? memmove((char*)__result - _Num, __first, _Num) : 0;
}
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __true_type&)
{
    return (_BidirectionalIter2)__copy_trivial_backward(__first, __last, __result);
}

template <class _Tp> class vector {
public:
    typedef _Tp* iterator;
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _Tp* _M_end_of_storage;                                 // +0x08
    iterator begin() { return _M_start; }
    iterator end() { return _M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend = false);   // 0x00440990
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
                __copy_backward_ptrs(__position, this->_M_finish - 2, this->_M_finish - 1, __true_type());
                *__position = __x_copy;
            }
        }
        else
            _M_insert_overflow(__position, __x, __true_type(), 1UL);
        return begin() + __n;
    }
};

template long* vector<long>::insert(long*, const long&);
}