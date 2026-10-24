// _STL::vector<CCardDef>::operator=(const vector<CCardDef>& x) -- STLport copy assignment of a 68-byte card def vector: reallocate-and-copy when x outgrows the capacity, else assign over the live elements and destroy the surplus or copy-construct the tail
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBF32, 234 bytes, `ret 4` (thiscall).
// Caller: CTavernGameCardBaseDef::operator= @ 0x004EBC16 (landed; CardPack at +0x78, after CardOrders through 0x004EBCA9).
// Label correction: the catalog's "CShoveHaPennyRegionDef_CopyAssign_0x44" is wrong; the element is CCardDef (retail 0x44:
//   the 0x3C CParentDefClassBase, CardName +0x3C, CardVal +0x40; vtable 0x01243A44 = CCardDef in rtti-vtables.tsv, stored by
//   the _Construct @ 0x004EC08A).
// Callees: _M_allocate_and_copy(n, first, last) @ 0x004EC10F (this chunk), _M_clear() @ 0x004EC01C (labelled
//   CActiveFile::OnReadFinished, propagated), __copy_ptrs(first, last, result, const __false_type&) @ 0x004EC03F,
//   __destroy_aux(first, last, const __false_type&) @ 0x004DA636, _Construct(T*, const T&) @ 0x004EC08A.
// Shape: the same STLport 4.x `vector::operator=` source as 0x004EBCA9 / 0x004E9AD3; the 0x44 stride (`imul ..,44h` instead
//   of the 72-byte `lea` pair) changes VC7.1's register choice (x._M_start spilled, xlen in a frame word, `sub esp,0Ch`)
//   with no source change. POD empty `_TrivialAss()` = `xor eax,eax ; stosb` into the dead x slot; the destroy tag is a
//   NAMED uninitialised local; _Destroy / __destroy / __uninitialized_copy are `__forceinline` (the size TU out-lines them).
//   Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)`.
// Model: no engine header for CCardDef (Ego_r.pdb gives 72 bytes, retail 0x44); local polymorphic model.
#pragma optimize("y", off)
#pragma optimize("s", on)
struct CParentDefClassBase {                               // retail 0x3C bytes
    virtual void v00();
    unsigned char _body[0x38];                              // +0x04 def-class base body
};
class CCardDef : public CParentDefClassBase {               // 0x44 bytes, vtable 0x01243A44
public:
    long CardName;                                          // +0x3C
    long CardVal;                                           // +0x40
};

namespace _STL {
struct __false_type { };
template <class T> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
template <class T> CCardDef* __fastcall __copy_ptrs(const T* first, const T* last, T* result, const __false_type&);   // 0x004EC03F
template <class T> void __fastcall __destroy_aux(T* first, T* last, const __false_type&);                                // 0x004DA636
template <class T> void __fastcall _Construct(T* p, const T& val);                                                         // 0x004EC08A
template <class T> __forceinline void __destroy(T* first, T* last, T*)
{
    typedef typename __type_traits<T>::has_trivial_destructor _Trivial_destructor;
    _Trivial_destructor tag;
    __destroy_aux(first, last, tag);
}
template <class T> __forceinline void _Destroy(T* first, T* last) { __destroy(first, last, (T*)0); }
template <class T> __forceinline T* __uninitialized_copy(const T* first, const T* last, T* result, const __false_type&)
{
    T* cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(cur, *first);
    return cur;
}
template <class T> class vector {
public:
    typedef typename __type_traits<T>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<T>::is_POD_type _IsPODType;
    T* _M_start;                                            // +0x00
    T* _M_finish;                                           // +0x04
    T* _M_end_of_storage;                                   // +0x08
    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    unsigned int capacity() const { return unsigned int(_M_end_of_storage - _M_start); }
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);   // 0x004EC10F
    void _M_clear();                                        // 0x004EC01C
    vector& operator=(const vector& x);
};
template <class T>
vector<T>& vector<T>::operator=(const vector<T>& x)
{
    if (&x != this) {
        const unsigned int xlen = x.size();
        if (xlen > capacity()) {
            T* tmp = _M_allocate_and_copy(xlen, (const T*)x._M_start + 0, (const T*)x._M_finish + 0);
            _M_clear();
            _M_start = tmp;
            _M_end_of_storage = _M_start + xlen;
        }
        else if (size() >= xlen) {
            T* i = __copy_ptrs((const T*)x._M_start + 0, (const T*)x._M_finish + 0, (T*)_M_start, _TrivialAss());
            _Destroy(i, _M_finish);
        }
        else {
            __copy_ptrs((const T*)x._M_start, (const T*)x._M_start + size(), (T*)_M_start, _TrivialAss());
            __uninitialized_copy((const T*)x._M_start + size(), (const T*)x._M_finish + 0, _M_finish, _IsPODType());
        }
        _M_finish = _M_start + xlen;
    }
    return *this;
}
template class vector<CCardDef>;
}