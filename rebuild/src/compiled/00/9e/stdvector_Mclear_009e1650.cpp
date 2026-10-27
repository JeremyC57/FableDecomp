// std::vector::_M_clear() -- (vector<CQuickDrawTriInfo >) destroy every element and free the block
// Leaf: ?_M_clear@?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@std@@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009E1650, 58 bytes, thiscall, plain `ret`.
// Identity (proven): called by vector<CQuickDrawTriInfo>::_M_insert_overflow 0x009E1750 (catalogued so); 0x3C-byte elements destroying CTexture UploadedTexture +0x18 (0x009F9F70, bound ??1CTexture@@QAE@XZ) then CWideString String +0x14 -- Ego_r.pdb CQuickDrawTriInfo.
// Callees: the element's member destructors 0x009F9F70 / 0x0099B510 (inline implicit element destructor), free 0x00BFEA14.
// Shape (family M081, representative 0x004AE040): STLport 4.6 `_M_clear()` (declared public as the existing binding spells it) -- `_Destroy(_M_start, _M_finish)` inline
//   (one aligned loop), then the allocator's `if (p) free(p)`; speed TU.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CWideString {                                       // 4 bytes (PStringData)
public:
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
class CTexture {                                          // 8 bytes
public:
    ~CTexture();                                          // 0x009F9F70
    unsigned long Members[2];
};
class CQuickDrawTriInfo {                                 // Ego_r.pdb, 0x3C bytes
public:
    unsigned long Flags;                                  // +0x0 (Line, IsText bit-fields)
    float Window[4];                                      // +0x4 C2DBoxF
    CWideString String;                                   // +0x14
    CTexture UploadedTexture;                             // +0x18
    unsigned long Members[7];                             // +0x20 .. +0x3c (union, StringZ, StringScale, StringFlags, Colour, PFont)
};
namespace std {
struct __false_type { };
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator> inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
};
template void vector<CQuickDrawTriInfo >::_M_clear();
}