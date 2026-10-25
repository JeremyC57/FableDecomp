// _STL::vector::erase(CFeatDef* first, CFeatDef* last) -- (vector<CFeatDef>) STLport range erase: assign the tail down over [first, last), destroy the leftover elements through their virtual destructor, shrink _M_finish, return first
// Leaf: ?erase@?$vector@VCFeatDef@@V?$allocator@VCFeatDef@@@_STL@@@_STL@@QAEPAVCFeatDef@@PAV3@0@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F345F0, 109 bytes (+ int3 padding), __fastcall `ret 8`; one caller (0x00F345AC).
// Callees: CFeatDef::operator=(const CFeatDef&) @ 0x00F2D200 (out of line), the element's virtual destructor (vtable slot 0 with a
//   zero flag: the explicit `p->~T()` of a polymorphic element).
// Identity: the 0x64 stride and the operator= 0x00F2D200 (CDefClassBase::operator= 0x00431F10, then the members) match
//   Ego_r.pdb CFeatDef (100 bytes, FeatName..CO_ItemName). Catalog "vector_erase_impl_CFeatDef" is descriptive.
// Shape: STLport 4.6 `pointer i = copy(last, _M_finish, first); _Destroy(i, _M_finish); _M_finish = i; return first;` with
//   the landed __forceinline copy -> __copy_ptrs(__false_type) -> random-access __copy chain (vector<CTVertexWaterBackground>
//   ::erase 0x00B7FD70): `for (n = last - first; n > 0; --n) *result++ = *first++;` (signed `jle`, the source folded into
//   an offset from the destination), then the __false_type destroy loop. Local def base: engine/CDefClassBase.h is a
//   0x25-byte pack(1) flat layout (vptr as data), unusable as a polymorphic base.
class CDefClassBase {                               // 0x28 bytes in retail (vptr + 0x24 body), polymorphic def root
public:
    virtual ~CDefClassBase();
    unsigned char _body[0x24];                      // +0x04..+0x27
};
class CFeatDef : public CDefClassBase {             // 0x64 bytes (Ego_r.pdb: FeatName +0x28 .. CO_ItemName +0x60)
public:
    unsigned char _members[0x3c];                   // +0x28..+0x63
    CFeatDef& operator=(const CFeatDef& other);     // 0x00F2D200
    virtual ~CFeatDef();
};

namespace _STL {

struct __false_type { __false_type() { } };
struct input_iterator_tag { input_iterator_tag() { } };
struct forward_iterator_tag : public input_iterator_tag { forward_iterator_tag() { } };
struct bidirectional_iterator_tag : public forward_iterator_tag { bidirectional_iterator_tag() { } };
struct random_access_iterator_tag : public bidirectional_iterator_tag { random_access_iterator_tag() { } };

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

template <class _Tp>
__forceinline _Tp* __copy_ptrs(const _Tp* __first, const _Tp* __last, _Tp* __result, const __false_type&)
{
    return __copy(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}

template <class _Tp>
__forceinline _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result)
{
    return __copy_ptrs(__first, __last, __result, __false_type());
}

template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }

template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _ForwardIterator>
inline void _Destroy(_ForwardIterator __first, _ForwardIterator __last)
{
    __destroy_aux(__first, __last, __false_type());
}

template <class _Tp> class allocator {
public:
    allocator() { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc>
class _Vector_base {
public:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    _Tp* erase(_Tp* __first, _Tp* __last)
    {
        _Tp* __i = copy((const _Tp*)__last, (const _Tp*)this->_M_finish, __first);
        _Destroy(__i, this->_M_finish);
        this->_M_finish = __i;
        return __first;
    }
};

}   // namespace _STL

template CFeatDef* _STL::vector<CFeatDef>::erase(CFeatDef*, CFeatDef*);