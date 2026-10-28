// std::vector<CCardPositionsDef>::_M_fill_insert(CCardPositionsDef* pos, unsigned int n, const CCardPositionsDef& x) -- STLport vector insert(pos, n, x) for the 72-byte card position defs: shift the tail in place when capacity allows, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004F9DF8, 362 bytes, thiscall `ret 0Ch`, ebp frame (the size-optimised /Gr defs TU).
//   Catalog "Vector_Insert_Multiple_Elements" is descriptive: x_copy's vtable 0x012439DC is CCardPositionsDef's.
// Callees: CParentDefClassBase copy constructor @ 0x0046EE98 and operator= @ 0x00431ED0, vector<C2DVector> copy
//   constructor @ 0x004EBE1A and operator= @ 0x0046EABD (the Offset member), _Construct @ 0x004EBDF1 (__fastcall),
//   __copy_backward_ptrs @ 0x004F9F62 (__fastcall, `stosb` tag), uninitialized_fill_n @ 0x004FA087 (tag-less, __fastcall),
//   ~CCardPositionsDef @ 0x004F9CFC (manifest "CTCInventoryItem::OnDie", propagated), _M_insert_overflow @ 0x004F9FA3.
// Layout: CCardPositionsDef (Ego_r.pdb 0x50 with a 16-byte vector; retail 0x48 = `push 48h`): CParentDefClassBase
//   (0x39 bytes, byte-packed: CDefClassBase + InstantiationName + SubDefInfoMap) + vector<C2DVector> Offset +0x3C.
// Shape: the 0x004EDB3E model; x_copy's copy constructor and the fills' operator= are inline memberwise
//   (`__forceinline`), the destructor out of line.
#pragma optimize("y", off)
#pragma optimize("s", on)

inline void* operator new(unsigned int, void* p) { return p; }

class C2DVector {
public:
    float X;
    float Y;
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
    vector(const vector<_Tp, _Alloc>& __x);                                      // <C2DVector>: 0x004EBE1A
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);              // <C2DVector>: 0x0046EABD
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                               // 0x004F9FA3
    public:
    void _M_fill_insert(pointer __position, size_type __n, const _Tp& __x);
};
}

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
class CDefClassBase : public CDefPointeeBase {       // 0x25 bytes, byte-packed
public:
    unsigned char _body[0x21];                       // +0x04..+0x25
    virtual void v00();
};
class CParentDefClassBase : public CDefClassBase {   // 0x39 bytes, byte-packed
public:
    unsigned char _pbody[0x14];                      // +0x25 InstantiationName, SubDefInfoMap
    virtual void v00();
    CParentDefClassBase(const CParentDefClassBase& other);                     // 0x0046EE98
    CParentDefClassBase& operator=(const CParentDefClassBase& other);          // 0x00431ED0
};
#pragma pack(pop)
class CCardPositionsDef : public CParentDefClassBase {   // vtable 0x012439DC, 0x48 bytes
public:
    std::vector<C2DVector> Offset;                  // +0x3c
    virtual void v00();
    __forceinline CCardPositionsDef(const CCardPositionsDef& o) : CParentDefClassBase(o), Offset(o.Offset) { }
    __forceinline CCardPositionsDef& operator=(const CCardPositionsDef& o)
    {
        CParentDefClassBase::operator=(o);
        Offset = o.Offset;
        return *this;
    }
    ~CCardPositionsDef();                            // 0x004F9CFC
};

namespace std {
template <class _T1, class _T2> void __fastcall _Construct(_T1* __p, const _T2& __val);           // 0x004EBDF1

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
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);    // 0x004FA087

template <class _BidirectionalIter1, class _BidirectionalIter2>
_BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                    _BidirectionalIter2 __result, const __false_type&);  // 0x004F9F62

template <class _ForwardIter, class _Tp>
__forceinline void fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

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

template void vector<CCardPositionsDef>::_M_fill_insert(CCardPositionsDef*, unsigned int, const CCardPositionsDef&);
}