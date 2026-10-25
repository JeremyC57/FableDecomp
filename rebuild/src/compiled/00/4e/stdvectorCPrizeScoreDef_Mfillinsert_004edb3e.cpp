// std::vector<CPrizeScoreDef>::_M_fill_insert(CPrizeScoreDef* pos, unsigned int n, const CPrizeScoreDef& x) -- STLport vector insert(pos, n, x) for the 48-byte prize score defs: shift the tail in place when capacity allows, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004EDB3E, 349 bytes, thiscall `ret 0Ch`, ebp frame (the size-optimised /Gr defs TU).
//   The catalog label "Vector_InsertRange_CCardDef" is wrong twice over: a fill insert, and the element is CPrizeScoreDef
//   (x_copy's vtable 0x012437D4 is CPrizeScoreDef's in rtti-vtables.tsv; manifest 0x004EDC9B "CCardDef::CCardDef" is
//   its copy constructor).
// Callees: CPrizeScoreDef copy constructor @ 0x004EDC9B (out of line: the implicit one under "s"), _Construct @ 0x004EAC30
//   (__fastcall), __copy_backward_ptrs @ 0x004EDCC2 (__fastcall, tag zero-stored with `stosb`), CDefClassBase::operator=
//   @ 0x00431F10 (the inline fill), uninitialized_fill_n @ 0x004EDDF9 (tag-less, __fastcall), the base destructor
//   @ 0x009FC550 after the inline CDefPointeeBase destructor's vtable store 0x01230BA0, _M_insert_overflow @ 0x004EDD13.
// Layout: CPrizeScoreDef (Ego_r.pdb, 0x30 = `push 30h ; pop ecx ; idiv`): CDefClassBase (0x25 bytes, byte-packed) +
//   Score +0x28 + Mult +0x2C -- the 0x004ED161 model.
// Shape: STLport 4.6 `_M_fill_insert` verbatim under `optimize("y", off)` + `optimize("s", on)`; helpers retail inlined are
//   `__forceinline`, the out-of-line ones declarations (which keeps the zero-filled tag stores).
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

class CBase_009fc550 {
public:
    virtual void v00();
    ~CBase_009fc550();                               // 0x009FC550 (labelled "~CMemoryBlock")
};
class CDefPointeeBase : public CBase_009fc550 {      // vtable 0x01230BA0
public:
    virtual void v00();
    ~CDefPointeeBase() { }
};
#pragma pack(push, 1)
class CDefClassBase : public CDefPointeeBase {       // 0x25 bytes, byte-packed (landed 0x004E9C39 / 0x004E9BDA model)
public:
    unsigned char _body[0x21];                       // +0x04..+0x25
    virtual void v00();
    CDefClassBase(const CDefClassBase& other);       // 0x0046644C
    CDefClassBase& operator=(const CDefClassBase& other);   // 0x00431F10
};
#pragma pack(pop)
class CPrizeScoreDef : public CDefClassBase {        // vtable 0x012437D4, 0x30 bytes (Ego_r.pdb)
public:
    float Score;                                     // +0x28
    long  Mult;                                      // +0x2c
    virtual void v00();
    // copy constructor: the implicit one (under "s" VC7.1 emits it out of line, 0x004EDC9B)
    __forceinline CPrizeScoreDef& operator=(const CPrizeScoreDef& o)
    {
        CDefClassBase::operator=(o);
        Score = o.Score;
        Mult = o.Mult;
        return *this;
    }
};

namespace std {
struct __false_type { };
template <class _Tp> struct __type_traits {
    typedef __false_type has_trivial_assignment_operator;
    typedef __false_type is_POD_type;
};

template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val);           // 0x004EAC30

template <class _InputIter, class _ForwardIter>
__forceinline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                                const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x004EDDF9

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&);  // 0x004EDCC2

template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _TrivialAss;
    typedef typename __type_traits<_Tp>::is_POD_type _IsPODType;
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x004EDD13
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(pointer __position, size_type __n, const _Tp& __x)
{
    if (__n != 0) {
        if (size_type(this->_M_end_of_storage._M_data - this->_M_finish) >= __n) {
            _Tp __x_copy = __x;
            const size_type __elems_after = this->_M_finish - __position;
            pointer __old_finish = this->_M_finish;
            if (__elems_after > __n) {
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, _TrivialAss());
                fill(__position, __position + __n, __x_copy);
            } else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, _IsPODType());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        } else
            _M_insert_overflow(__position, __x, _IsPODType(), __n);
    }
}

template void vector<CPrizeScoreDef>::_M_fill_insert(CPrizeScoreDef*, unsigned int, const CPrizeScoreDef&);
}