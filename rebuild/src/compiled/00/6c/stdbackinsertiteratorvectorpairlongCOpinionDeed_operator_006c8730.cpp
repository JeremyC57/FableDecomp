// std::back_insert_iterator<vector<pair<long, COpinionDeed*> > >::operator=(const value_type& value) -- push_back the (handle, deed) pair onto the container, return *this
// Leaf: ??4?$back_insert_iterator@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C8730, 69 bytes, thiscall, `ret 4`, returns this.
// Identity: the manifest's "std::vector<pair<long,?>>::emplace_back" is the inlined push_back. The body reads the
//   container pointer at +0 and returns `this`: STLport `back_insert_iterator::operator=(v) { container->push_back(v);
//   return *this; }`. The overflow callee 0x006C80E0 is vector<pair<long, COpinionDeed*> >::_M_insert_overflow (the
//   landed std::__copy 0x006C9B70 of CTCHeroOpinionDeedLog's deed map into such a vector calls it with the same
//   arguments). No direct caller survives (the COMDAT is emitted for the copy instances).
// Callees: vector<pair<long, COpinionDeed*> >::_M_insert_overflow(pos, x, __false_type, 1, true) 0x006C80E0.
// Shape: STLport 4.6 push_back inline (placement copy behind the null test: the pair's user copy constructor copies the
//   two words through edi/edx; else the out-of-line overflow with the tag homed in the value's argument slot, which
//   needs the overflow's same-TU noinline stand-in that never reads the tag).
inline void* operator new(unsigned int, void* p) { return p; }

class COpinionDeed;

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) { }
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp value_type;
    __declspec(noinline) void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                                 unsigned int __fill_len, bool __atend);   // 0x006C80E0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};

// Same-TU stand-in for the out-of-line overflow 0x006C80E0 (landed elsewhere): never reads the tag.
#pragma auto_inline(off)
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&,
                                             unsigned int __fill_len, bool __atend)
{
    if (__atend && __fill_len == 1)
        this->_M_finish = __position + (&__x != 0);
}
#pragma auto_inline(on)

template <class _Container> class back_insert_iterator {
protected:
    _Container* container;                                  // +0x00
public:
    explicit back_insert_iterator(_Container& __x) : container(&__x) { }
    back_insert_iterator<_Container>& operator=(const typename _Container::value_type& __val);
};

template <class _Container>
back_insert_iterator<_Container>& back_insert_iterator<_Container>::operator=(const typename _Container::value_type& __val)
{
    container->push_back(__val);
    return *this;
}

template class back_insert_iterator<vector<pair<long, COpinionDeed*> > >;

} // namespace std