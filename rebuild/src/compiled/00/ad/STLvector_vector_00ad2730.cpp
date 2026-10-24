// _STL::vector::vector(const vector& x) -- (vector<COptimisedBone>) STLport copy constructor: allocate x.size() 16-byte bones through the _Vector_base constructor and copy-construct x's elements into them
// Leaf: ??0?$vector@VCOptimisedBone@QuadricOptimiserInternals@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AD2730, length 87 bytes (+9 int3), `ret 4`.
// Callees: malloc @ 0x00BFEA0E (allocator::allocate), __uninitialized_copy(const COptimisedBone*, const COptimisedBone*,
//   COptimisedBone*, const __false_type&) @ 0x00AD1B20 (/Gr __fastcall: the element copy = CCharString copy constructor
//   0x0099EC30, the long, the bool and the long).
// Identity: the element of 0x00AD1B20 is 16 bytes {CCharString, long, bool, long} = Ego_r.pdb
//   QuadricOptimiserInternals::COptimisedBone {BoneName, Parent, Remapped, NewIndex} (COptimisedSubMesh::Bones); the
//   five callers are in the QuadricOptimiser TU (0x00AD5908 .. 0x00AD8744). The manifest's
//   "Vector_Construct_CFeatDef" is descriptive (the propagated `_Uninit_copy<CFeatDef*,...>` callee label is wrong).
// Shape: STLport 4.6 `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, _IsPODType()); }` -- the inline base constructor zeroes the
//   three words from one register that also serves as the null allocation, then start/finish/end; the __false_type tag
//   is homed on x's argument slot.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

class CCharString {
public:
    void* m_pData;                                     // CBasicString<char> data block
    CCharString(const CCharString& other);             // 0x0099EC30
};

namespace QuadricOptimiserInternals {
class COptimisedBone {                                 // 16 bytes (Ego_r.pdb)
public:
    CCharString BoneName;                              // +0x0
    long        Parent;                                // +0x4
    bool        Remapped;                              // +0x8
    long        NewIndex;                              // +0xc
};
}
typedef QuadricOptimiserInternals::COptimisedBone COptimisedBone;

namespace _STL {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);   // 0x00AD1B20

template <class _Tp, class _Alloc> class _Vector_base {
public:
    typedef _Alloc allocator_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Alloc allocator_type;
    typedef const _Tp* const_pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    allocator_type get_allocator() const { return allocator_type(); }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                               this->_M_start, __false_type());
    }
};

template vector<COptimisedBone>::vector(const vector<COptimisedBone>&);

}   // namespace _STL