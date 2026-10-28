// std::vector<CGraphicInfo>::_M_fill_insert(CGraphicInfo* pos, unsigned int n, const CGraphicInfo& x) -- STLport insert of n copies of a graphic info record: inside the spare capacity shift the tail (copy-construct / assign backwards) and fill with a local copy of x, else grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A01290, 294 bytes, thiscall `ret 0Ch` (pos, n, &x). Callers: the inline
//   insert(pos, n, x) wrapper 0x00A01640 (jmp) and 0x00A017E8 (vector<CGraphicInfo>::resize). The catalog label
//   "Vector_InsertElements" is descriptive.
// Callees: __uninitialized_copy(first, last, result, const __false_type&) @ 0x009FFEB0 (`__fastcall`, `ret 8`),
//   __copy_backward_ptrs(first, last, result, const __false_type&) @ 0x00A00460 (`ret 8`, __copy_backward inlined),
//   fill(first, last, val) @ 0x009FFF90 (landed, `ret 4`), uninitialized_fill_n(first, n, x) @ 0x009FFFC0 (landed,
//   `ret 4`, the __false_type __uninitialized_fill_n inlined), _M_insert_overflow(pos, x, const __false_type&, n, false)
//   @ 0x00A01120.
// Layout: Ego_r.pdb CGraphicInfo (0x2C bytes): Header +0x00, TextureType +0x1C, Bank +0x20, Index +0x24, Frames +0x28;
//   implicit copy (`rep movsd` of 11 dwords into the local x_copy); the destructor's clears of the dead local vanish.
// Shape: STLport 4.6 `_M_fill_insert(pos, n, x)` verbatim over the __false_type helpers (the CGraphicInfoFrame
//   sibling 0x00A00F50); for this element VC7.1 kept __copy_backward_ptrs and uninitialized_fill_n out of line. The
//   helpers are same-TU noinline bodies under `auto_inline(off)` that never touch their tag, so VC7.1 homes the tag
//   temporaries on the live `pos` argument slot.
inline void* operator new(unsigned int, void* where) { return where; }

class CGraphicDataBank;
class CGraphicInfoFrame;

class CGraphicHeader {                              // 0x1C bytes (Ego_r.pdb)
public:
    unsigned short Width;                           // +0x00
    unsigned short Height;                          // +0x02
    unsigned short Depth;                           // +0x04
    unsigned short FrameWidth;                      // +0x06
    unsigned short FrameHeight;                     // +0x08
    unsigned short FrameCount;                      // +0x0A
    long           PixelFormat;                     // +0x0C CPixelFormat (FormatIndex)
    unsigned char  TransparencyType;                // +0x10
    unsigned char  MipmapLevels;                    // +0x11
    unsigned char  IsFlatAnimatedGraphic : 1;       // +0x12
    unsigned long  FrameDataSize;                   // +0x14
    unsigned long  CompressedMipmapSizes[1];        // +0x18
};

enum ETextureType { TEXTURE_TYPE_NONE };

class CGraphicInfo {                                // 0x2C bytes (Ego_r.pdb)
public:
    CGraphicHeader     Header;                      // +0x00
    ETextureType       TextureType;                 // +0x1C
    CGraphicDataBank*  Bank;                        // +0x20
    long               Index;                       // +0x24
    CGraphicInfoFrame* Frames;                      // +0x28
    ~CGraphicInfo() { Bank = 0; Frames = 0; }
};

namespace std {
struct __false_type { __false_type() {} };

template <class _Tp> class allocator {
public:
    allocator() {}
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                                 // 0x009FFEB0
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _BidirectionalIter1, class _BidirectionalIter2>
__declspec(noinline) _BidirectionalIter2 __fastcall __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                                         _BidirectionalIter2 __result, const __false_type&)
{                                                                                 // 0x00A00460
    for (int __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}

template <class _ForwardIter, class _Tp>
__declspec(noinline) void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{                                                                                 // 0x009FFF90
    for (; __first != __last; ++__first)
        *__first = __val;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x)
{                                                                                 // 0x009FFFC0
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
#pragma auto_inline(on)

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;      // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef __false_type _IsPODType;
    typedef __false_type _TrivialAss;
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                                // 0x00A01120
    public:
    void _M_fill_insert(iterator __position, size_type __n, const _Tp& __x);
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_fill_insert(iterator __position, size_type __n, const _Tp& __x)
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

template void vector<CGraphicInfo>::_M_fill_insert(CGraphicInfo*, unsigned int, const CGraphicInfo&);
}