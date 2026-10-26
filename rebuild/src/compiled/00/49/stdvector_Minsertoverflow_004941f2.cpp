// std::vector::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend) -- (vector<CSubtitleRenderer::CEmbeddedHighlight >) STLport growth insert: allocate size + max(size, fill_len), copy the head, construct fill_len copies of x, copy the tail unless atend, release the old block, adopt the new one
// Flags: /Gr
// Leaf: ?_M_insert_overflow@?$vector@VCEmbeddedHighlight@CSubtitleRenderer@@V?$allocator@VCEmbeddedHighlight@CSubtitleRenderer@@@std@@@std@@IAEXPAVCEmbeddedHighlight@CSubtitleRenderer@@ABV34@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004941F2, 176 bytes (size-optimised TU with a frame pointer), thiscall, `ret 14h`.
// Identity (proven): std::vector<CSubtitleRenderer::CEmbeddedHighlight >::_M_insert_overflow (the non-POD overload); evidence: its fill_len == 1 constructor 0x00493617 copy-constructs exactly that layout (CWideString copy 0x0099B720 at +0, dwords +4 / +8 / +0x0C, the byte +0x10); its only caller is the push_back 0x004942C3, called by CSubtitleRenderer::ProcessEmbeddedHighlightTags 0x00491AD2 on the highlight vector (+0xC4, whose erase agent951 landed as vector<CSubtitleRenderer::CEmbeddedHighlight>::erase 0x004940AC)
// Label: the manifest's "CEmbeddedHighlight_Load" is a propagated / behavioural label (family J071: five 176-byte growth inserts, each its own element).
// Callees: allocator::allocate(n, const void* = 0) 0x00493503 (out of line on _M_end_of_storage), __uninitialized_copy(first, last,
//   result, const __false_type&) 0x004939BF (__fastcall, head and tail), the element copy constructor 0x00493617 (behind the placement
//   null test, fill_len == 1), __uninitialized_fill_n(first, n, x, const __false_type&) 0x004939EA (__fastcall), _M_clear() 0x004940E3.
// Shape: STLport 4.6 _vector.c `_M_insert_overflow` (non-POD, no EH) verbatim: `(max)` through its references, old size and then
//   new_finish in the frame. The constructor-less __false_type tags get no store and share the padding of the `atend` slot
//   ([ebp+1Bh]) because this TU compiles both callee bodies (STLport's loops, instantiated above) ahead of the growth insert.
// Element: CSubtitleRenderer::CEmbeddedHighlight (Ego_r.pdb 0x14 bytes)
#pragma optimize("y", off)
#pragma optimize("s", on)
inline void* operator new(unsigned int, void* __p) { return __p; }
namespace std {
struct __false_type { };
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n, const void* = 0);    // 0x00493503
};
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>                    // 0x004939BF for this element (compiled ahead of the growth insert)
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>               // 0x004939EA for this element (compiled ahead of the growth insert)
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
protected:
    void _M_clear();                                     // 0x004940E3
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend = false);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}   // namespace std
class CWideString {                                      // Ego_r.pdb 4 bytes
public:
    void* PStringData;                                   // +0x0 CWideStringData*
};
class C2DVector {                                        // Ego_r.pdb 8 bytes
public:
    float X;
    float Y;
};
class CSubtitleRenderer {
public:
    class CEmbeddedHighlight {                           // Ego_r.pdb 0x14 bytes
    public:
        CWideString Text;                                // +0x00
        long        CharIndex;                           // +0x04
        C2DVector   Offset;                              // +0x08
        bool        Split;                               // +0x10
        CEmbeddedHighlight(const CEmbeddedHighlight& other);   // 0x00493617 (implicit; emitted out of line in retail)
    };
};
namespace std {
template CSubtitleRenderer::CEmbeddedHighlight* __uninitialized_copy<CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*>(CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*, CSubtitleRenderer::CEmbeddedHighlight*, const __false_type&);
template CSubtitleRenderer::CEmbeddedHighlight* __uninitialized_fill_n<CSubtitleRenderer::CEmbeddedHighlight*, unsigned int, CSubtitleRenderer::CEmbeddedHighlight >(CSubtitleRenderer::CEmbeddedHighlight*, unsigned int, const CSubtitleRenderer::CEmbeddedHighlight&, const __false_type&);
template void vector<CSubtitleRenderer::CEmbeddedHighlight >::_M_insert_overflow(CSubtitleRenderer::CEmbeddedHighlight*, const CSubtitleRenderer::CEmbeddedHighlight&, const __false_type&, unsigned int, bool);
}