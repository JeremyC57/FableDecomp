// std::vector::resize(size_type __new_size, const _Tp& __x) -- (vector<CFadeInFadeOutBase >) STLport resize: erase the surplus [begin() + n, end()) when shrinking, else insert n - size() copies of x at the end
// Leaf: ?resize@?$vector@VCFadeInFadeOutBase@@V?$allocator@VCFadeInFadeOutBase@@@std@@@std@@QAEXIABVCFadeInFadeOutBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00594CEE, 72 bytes (size-optimised TU), thiscall, `ret 8`.
// Identity (proven): std::vector<CFadeInFadeOutBase >::resize(size_type, const CFadeInFadeOutBase&); evidence: its only caller is resize(n) 0x00594DF4, which default-constructs T through 0x00436560 (the bindings' ??0CFadeInFadeOutBase@@QAE@XZ: Active, Closing, Opening, OpenTimer, CloseTimer zeroed and ToColour = 0xFF000000 at +0x18, the Ego_r.pdb CFadeInFadeOutBase layout); that resize(n) is called by CTCInventory::CTCInventory 0x00590D32 (it stores vtable 0x01250DE4, RTTI .?AVCTCInventory@@) on this+0x160, 12 bytes after the vector<long> resized at +0x154 -- Ego_r.pdb ContextSensitiveFadeTimes (+0x188, vector<CFadeInFadeOutBase>) right after ContextSensitiveItems (+0x178)
// Label: the manifest's "push_back" is a behavioural / propagated label; this row is vector<CFadeInFadeOutBase >::resize(n, x) (family J069: five 72-byte bodies, each its own element).
// Callees: erase(iterator, iterator) 0x0059485B, _M_fill_insert(iterator, size_type, const CFadeInFadeOutBase&) 0x00594A73 (both out of line, thiscall).
// Shape: STLport 4.6 vector.h `resize` verbatim with insert(pos, n, x) inlined to _M_fill_insert; size() is the exact
//   pointer difference (`idiv 0x1C`, the divisor loaded by push/pop under /Os) and is recomputed on the insert path.
// Element: CFadeInFadeOutBase (Ego_r.pdb, 28 bytes)
#pragma optimize("s", on)
namespace std {
template <class _Tp> class allocator { };
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {          // STLport, 12 bytes
public:
    typedef _Tp* pointer;
    typedef _Tp* iterator;
    typedef unsigned int size_type;
    pointer _M_start;
    pointer _M_finish;
    _STLP_alloc_proxy<pointer, _Alloc> _M_end_of_storage;
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    iterator erase(iterator __first, iterator __last);                          // 0x0059485B
    void _M_fill_insert(iterator __pos, size_type __n, const _Tp& __x);         // 0x00594A73
    void insert(iterator __pos, size_type __n, const _Tp& __x) { _M_fill_insert(__pos, __n, __x); }
    void resize(size_type __new_size, const _Tp& __x)
    {
        if (__new_size < size())
            erase(begin() + __new_size, end());
        else
            insert(end(), __new_size - size(), __x);
    }
};
}   // namespace std
class CRGBColour {                                        // Ego_r.pdb 4 bytes (opaque here)
public:
    unsigned char _body[4];
};
class CFadeInFadeOutBase {                               // Ego_r.pdb 28 bytes
public:
    bool       Active;                                   // +0x00
    float      FadeInTime;                               // +0x04
    float      FadeOutTime;                              // +0x08
    bool       Closing;                                  // +0x0C
    bool       Opening;                                  // +0x0D
    float      OpenTimer;                                // +0x10
    float      CloseTimer;                               // +0x14
    CRGBColour ToColour;                                 // +0x18
};
namespace std {
template void vector<CFadeInFadeOutBase >::resize(unsigned int, const CFadeInFadeOutBase&);
}