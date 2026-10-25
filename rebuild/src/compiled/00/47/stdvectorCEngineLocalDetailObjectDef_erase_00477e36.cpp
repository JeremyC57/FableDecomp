// std::vector<CEngineLocalDetailObjectDef>::erase(iterator first, iterator last) -- STLport range erase: assign the tail down over [first, last), destroy the surplus, move finish, return first
// Leaf: ?erase@?$vector@VCEngineLocalDetailObjectDef@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00477E36, 86 bytes, thiscall, `ret 8`, returns first; size-optimised
//   frame-pointer def TU (unaligned entry, `push ebp ; mov ebp,esp`, `leave`) -- `#pragma optimize("s", on)`.
// Identity: the manifest's "std::vector::erase" is right but unparameterised. The element is 0x88 bytes (`idiv 88h`), the
//   assignment is the landed CEngineLocalDetailObjectDef::operator= 0x00477964 (Ego_r.pdb 0x8C with the 0x40 parent; retail
//   parent 0x3C), and the range destroy 0x004774B9 is the one the landed CDerived_0047747c destructor 0x0047747C (the
//   def-bank owner of this vector) calls. The only caller 0x00477E12 is this vector's resize(n, x) (erase(begin()+n, end())
//   or _M_fill_insert 0x00477E8E).
// Callees: CEngineLocalDetailObjectDef::operator=(const CEngineLocalDetailObjectDef&) 0x00477964, _Destroy(first, last)
//   0x004774B9 (`__fastcall`, ecx/edx, `ret`; the virtual destructor on each 0x88-byte element).
// Shape: STLport 4.6 `pointer i = __copy_ptrs(last, finish, first, __false_type()); _Destroy(i, finish); finish = i;
//   return first;` with the counted random-access __copy forced inline (the count `(finish - last) / 0x88` homed in the
//   dead `last` slot, the source kept as an offset from the destination in [ebp-4]); unlike the 0xC4 sibling 0x00476F9B,
//   _Destroy itself stays out of line here (no tag push; finish loaded into edx before first into ecx).
#pragma optimize("s", on)

class CEngineLocalDetailObjectDef {                         // retail 0x88 bytes (only the size and the assignment matter here)
public:
    virtual ~CEngineLocalDetailObjectDef();
    unsigned char _body[0x84];
    CEngineLocalDetailObjectDef& operator=(const CEngineLocalDetailObjectDef& other);   // 0x00477964
};

namespace _STL {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

template <class _ForwardIterator>
void __fastcall _Destroy(_ForwardIterator __first, _ForwardIterator __last);   // 0x004774B9

template <class _RandomAccessIter, class _OutputIter, class _Distance>
__forceinline _OutputIter __copy(_RandomAccessIter __first, _RandomAccessIter __last, _OutputIter __result,
                                 const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n) {
        *__result = *__first;
        ++__first;
        ++__result;
    }
    return __result;
}

template <class _InputIter, class _OutputIter>
__forceinline _OutputIter __copy_ptrs(_InputIter __first, _InputIter __last, _OutputIter __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Tp* iterator;
    typedef _Tp* pointer;
    iterator erase(iterator __first, iterator __last)
    {
        pointer __i = __copy_ptrs(__last, this->_M_finish, __first, __false_type());
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

template vector<CEngineLocalDetailObjectDef>::iterator vector<CEngineLocalDetailObjectDef>::erase(iterator, iterator);

} // namespace _STL