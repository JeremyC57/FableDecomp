// std::vector<CQuickDrawTriInfo, std::allocator<CQuickDrawTriInfo> >::~vector() -- destroy every 60-byte element (its texture, then
//   its string), then free the storage
// Leaf: ??1?$vector@VCQuickDrawTriInfo@@V?$allocator@VCQuickDrawTriInfo@@@std@@@std@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009C2F50, 58 bytes, thiscall.
// Identity (proven, NAME_FIXES_QUEUE): called by ~CRenderManager2D 0x009C2FD0 on its three vector<CQuickDrawTriInfo> members (+0x3E7C /
//   +0x3E88 / +0x3E94; Ego_r CRenderManager2D member types); body = STLport ~vector (_Destroy over [start, finish): ~CTexture ->
//   CTexture::Uninitialise at +0x18, ~CWideString at +0x14), then ~_Vector_base frees a non-null start.
// Callees: CTexture::Uninitialise 0x009F9F70, CWideString::~CWideString 0x0099B510, free 0x00BFEA14.
// Shape: the real STLport 4.6 text and the CQuickDrawTriInfo model (Ego_r member names) copied from the landed ~CRenderManager2D
//   0x009C2FD0 row (whose TU emits this COMDAT), explicitly instantiated for the proven element type.
// Naming wave 16: landed as the descriptive CRetailVector_SOcclusionEntry_009c2f50::~CRetailVector_SOcclusionEntry_009c2f50 (manifest
//   label CCombatWheel::ResetRings is propagated); re-authored on the proven STLport instantiation (name-only: the bytes are unchanged).
extern "C" void __cdecl free(void* block);      // 0x00BFEA14
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy_aux(__first, __last, __false_type()); }
template <class _Tp, class _Alloc>
class _Vector_base { public:
    typedef _Alloc allocator_type;
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    typedef _Alloc allocator_type;
    explicit vector(const allocator_type& __a = allocator_type()) : _Vector_base<_Tp, _Alloc>(__a) { }
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
class CWideString { public:
    void* PStringData;
    ~CWideString();                             // 0x0099B510
};
class CTexture { public:
    void*         PTexture;
    unsigned long Flags;
    void Uninitialise();                        // 0x009F9F70
    ~CTexture() { Uninitialise(); }
};
class C2DBoxF { public: float MinX, MinY, MaxX, MaxY; };
class CRGBColour { public: unsigned char B, G, R, A; };
class CFontBank;
class CQuickDrawTriInfo { public:
    unsigned char Line : 1;                     // +0x00
    unsigned char IsText : 1;
    C2DBoxF       Window;                       // +0x04
    CWideString   String;                       // +0x14
    CTexture      UploadedTexture;              // +0x18
    unsigned long Union20[2];                   // +0x20
    float         StringZ;                      // +0x28
    float         StringScale;                  // +0x2c
    unsigned long StringFlags;                  // +0x30
    CRGBColour    Colour;                       // +0x34
    CFontBank*    PFont;                        // +0x38
};
template class std::vector<CQuickDrawTriInfo>;
