// std::vector<CCardPositionsDef>::operator=(const vector<CCardPositionsDef>& x) -- STLport copy assignment of a 72-byte card-positions def vector: reallocate-and-copy when x outgrows the capacity, else assign over the live elements and destroy the surplus or copy-construct the tail
// Leaf: ??4?$vector@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EBCA9, 228 bytes, `ret 4` (thiscall); the same bytes as 0x004E9AD3 (vector<CShopItemDef>)
//   but for the callees.
// Caller: 0x004EBC91 (an owner's operator= assigning its CCardPositionsDef vector at +0x6C, then the 0x44-byte
//   CShoveHaPennyRegionDef vector at +0x78 through 0x004EBF32).
// Label correction: the catalog's "Pair_CCharString_CArray_CopyAssign" is propagated; the element is CCardPositionsDef
//   (retail 0x48 bytes: the 0x3C CParentDefClassBase + vector<C2DVector> Offset) by the landed _Construct @ 0x004EBDF1.
// Callees: _M_allocate_and_copy(n, first, last) @ 0x004EBEDB (thiscall), _M_clear() @ 0x004EBD8D (labelled
//   CActiveFile::OnReadFinished, propagated), __copy_ptrs(first, last, result, const __false_type&) @ 0x004EBDB0 (this chunk),
//   __destroy_aux(first, last, const __false_type&) @ 0x004DA61A (the virtual-destructor range loop, ICF-shared), and
//   _Construct(T*, const T&) @ 0x004EBDF1 (landed; /Gr: free template functions are __fastcall).
// Shape: STLport 4.x `vector::operator=` verbatim. `_TrivialAss()` value-initialises a POD empty __false_type (`xor eax,eax ;
//   stosb` into the high byte of the dead x slot, hoisted above the `size() >= xlen` test with the shared `push tag ; push
//   _M_start`); `_Destroy(i, _M_finish)` reaches the out-of-line __destroy_aux through the inline __destroy, whose tag must
//   be a NAMED uninitialised local (retail passes the same dead slot with no second `stosb`; the landed 0x00471CC5 recipe),
//   and the tail's __uninitialized_copy (the `__false_type` loop of _Construct calls) is inlined. Under "s" the three inline
//   helpers (_Destroy, __destroy, __uninitialized_copy) must be `__forceinline` or they are emitted out of line (__cdecl).
//   Frame-pointer size TU: `#pragma optimize("y", off)` + `("s", on)` (`push 48h ; pop edi ; idiv edi`, `leave`).
// Model: no engine header for CCardPositionsDef (Ego_r.pdb gives 80 bytes, retail 0x48); the local model follows the landed
//   _Construct @ 0x004EBDF1 (polymorphic CParentDefClassBase body + the 12-byte STLport vector<C2DVector>).
#pragma optimize("y", off)
#pragma optimize("s", on)
class CParentDefClassBase { public:                               // retail 0x3C bytes
    virtual void v00();
    unsigned char _body[0x38];                              // +0x04 def-class base body
};
class C2DVector { public: float X, Y; };
struct C2DVectorVector {                                    // STLport vector<C2DVector>, 12 bytes
    C2DVector* _M_start;
    C2DVector* _M_finish;
    C2DVector* _M_end_of_storage;
};
class CCardPositionsDef : public CParentDefClassBase {      // 0x48 bytes
public:
    C2DVectorVector Offset;                                 // +0x3C
};

namespace std {
struct __false_type { };
template <class T> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type has_trivial_destructor;
    typedef __false_type is_POD_type;
};
template <class T> CCardPositionsDef* __fastcall __copy_ptrs(const T* first, const T* last, T* result, const __false_type&);   // 0x004EBDB0
template <class T> void __fastcall __destroy_aux(T* first, T* last, const __false_type&);                                // 0x004DA61A
template <class T> void __fastcall _Construct(T* p, const T& val);                                                         // 0x004EBDF1
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
    T* _M_allocate_and_copy(unsigned int n, const T* first, const T* last);   // 0x004EBEDB
    void _M_clear();                                        // 0x004EBD8D
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
template class vector<CCardPositionsDef>;
}