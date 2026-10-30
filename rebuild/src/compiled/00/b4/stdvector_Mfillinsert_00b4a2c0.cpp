// std::vector::_M_fill_insert(CEngineLightingManager::CThemeChannel* position, unsigned int n, const CEngineLightingManager::CThemeChannel& x) -- (vector<CEngineLightingManager::CThemeChannel>) STLport insert of n copies of a 112-byte lighting theme channel: slide the tail inside the spare capacity and fill the hole from a copy of x, or grow through _M_insert_overflow
// Leaf: ?_M_fill_insert@?$vector@UCThemeChannel@CEngineLightingManager@@V?$allocator@UCThemeChannel@CEngineLightingManager@@@std@@@std@@QAEXPAUCThemeChannel@CEngineLightingManager@@IABU34@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B4A2C0, 327 bytes, thiscall `ret 0Ch`, no EH frame (STLport built without exceptions).
// Identity: the landed vector<CEngineLightingManager::CThemeChannel>::resize(n, x) 0x00B4A4C0 (Ego_r.pdb names that resize;
//   CEngineLightingManager::ThemeChannels, a CArray<CThemeChannel>) calls this row through the inline `insert(end(), n, x)`;
//   the landed _M_insert_overflow 0x00B4A0E0 names this row as its only caller. The 0x92492493 / sar 6 reciprocal and the
//   28-dword `rep movsd` x_copy are the 112-byte CThemeChannel (Ego_r.pdb struct, key U). STLport 4.6 vector.c verbatim.
//   Access: public (QAE) as STLport 4.6 _vector.h declares `_M_fill_insert`; the resize binding's IAE spelling is wrong
//   (NAME_FIXES.tsv).
// Callees (free helpers __fastcall, all declared only): __uninitialized_copy 0x00B49760 (twice), __copy_backward<..., int>
//   0x00B491D0 (five arguments: behind the inline __copy_backward_ptrs), uninitialized_fill_n 0x00B497B0 (the three-argument
//   front end), fill 0x00B48EF0 (twice), _M_insert_overflow 0x00B4A0E0.
// Shape: the frame is x_copy (0x70) + the cached _M_finish / reused count word + one tag word: here retail homes the empty
//   tags in its own frame byte (esp+13h), not on a live argument slot, so the helpers stay declarations.
// Layout: local model -- engine/CEngineLightingManager.h has no nested CThemeChannel (ThemeChannels is an opaque
//   unsigned char[0xc]); the nested struct follows the landed 0x00B4A4C0 / 0x00B4A0E0 models (header debt, NOTES.md).
class CRGBFloatColour {
public:
    float R, G, B, A;                                     // +0x00
};
class C4DVector {
public:
    float X, Y, Z, W;                                     // +0x00
};
class CEngineLightingManager {
public:
    struct CThemeChannel {                                 // Ego_r.pdb (struct), 112 bytes
        CRGBFloatColour DiffuseColour;                    // +0x00
        CRGBFloatColour AmbientColour;                    // +0x10
        CRGBFloatColour BacklightColour;                  // +0x20
        CRGBFloatColour ReflectionColour;                 // +0x30
        CRGBFloatColour FogColour;                        // +0x40
        float FogStartZ;                                  // +0x50
        float FogEndZ;                                    // +0x54
        float ShadowFactor;                               // +0x58
        float FadedShadowFactor;                          // +0x5C
        C4DVector FogTransform;                           // +0x60
    };
};

namespace std {
struct __false_type { __false_type() { } };
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);                                       // 0x00B49760
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __fastcall uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x);         // 0x00B497B0
template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __fastcall __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last,
                                              _BidirectionalIter __result, const random_access_iterator_tag&,
                                              _Distance*);                                                // 0x00B491D0
template <class _BidirectionalIter1, class _BidirectionalIter2>
inline _BidirectionalIter2 __copy_backward_ptrs(_BidirectionalIter1 __first, _BidirectionalIter1 __last,
                                                _BidirectionalIter2 __result, const __false_type&)
{
    return __copy_backward(__first, __last, __result, random_access_iterator_tag(), (int*)0);
}
template <class _ForwardIter, class _Tp>
void __fastcall fill(_ForwardIter __first, _ForwardIter __last, const _Tp& __val);                     // 0x00B48EF0

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);                       // 0x00B4A0E0
public:
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);
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
                __uninitialized_copy(this->_M_finish - __n, this->_M_finish, this->_M_finish, __false_type());
                this->_M_finish += __n;
                __copy_backward_ptrs(__position, __old_finish - __n, __old_finish, __false_type());
                fill(__position, __position + __n, __x_copy);
            }
            else {
                uninitialized_fill_n(this->_M_finish, __n - __elems_after, __x_copy);
                this->_M_finish += __n - __elems_after;
                __uninitialized_copy(__position, __old_finish, this->_M_finish, __false_type());
                this->_M_finish += __elems_after;
                fill(__position, __old_finish, __x_copy);
            }
        }
        else
            _M_insert_overflow(__position, __x, __false_type(), __n);
    }
}
}

template void std::vector<CEngineLightingManager::CThemeChannel>::_M_fill_insert(CEngineLightingManager::CThemeChannel*,
                                                                                 unsigned int,
                                                                                 const CEngineLightingManager::CThemeChannel&);