// std::vector::_M_clear() -- (vector<std::pair<long, CMusicManager::CRegisteredMusicEntry> >) destroy every element and free the block
// Leaf: ?_M_clear@?$vector@U?$pair@JVCRegisteredMusicEntry@CMusicManager@@@std@@V?$allocator@U?$pair@JVCRegisteredMusicEntry@CMusicManager@@@std@@@2@@std@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006BEFE0, 58 bytes, thiscall, plain `ret`.
// Identity (proven): called by vector<pair<long, CMusicManager::CRegisteredMusicEntry> >::_M_insert_overflow 0x006BF1A0 (catalogued so) and bound ?_M_clear@?$vector@U?$pair@JVCRegisteredMusicEntry@CMusicManager@@@std@@@std@@QAEXXZ; 24-byte pairs, CWideStrings at +8/+4.
// Callees: the element's member destructors 0x0099B510 / 0x0099B510 (inline implicit element destructor), free 0x00BFEA14.
// Shape (family M081, representative 0x004AE040): STLport 4.6 `_M_clear()` (declared public as the existing binding spells it) -- `_Destroy(_M_start, _M_finish)` inline
//   (one aligned loop), then the allocator's `if (p) free(p)`; speed TU.
extern "C" void __cdecl free(void* block);                 // 0x00BFEA14
class CWideString {                                       // 4 bytes (PStringData)
public:
    ~CWideString();                                       // 0x0099B510
    void* PStringData;
};
class CMusicManager {
public:
    class CRegisteredMusicEntry {                         // Ego_r.pdb, 20 bytes
    public:
        CWideString SrcFileName;                          // +0x0
        CWideString DstFileName;                          // +0x4
        float Gain;                                       // +0x8
        float Offset;                                     // +0xc
        bool CacheToHD;                                   // +0x10
    };
};
namespace std { template <class _T1, class _T2> struct pair { _T1 first; _T2 second; }; }
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
template void vector<std::pair<long, CMusicManager::CRegisteredMusicEntry> >::_M_clear();
}