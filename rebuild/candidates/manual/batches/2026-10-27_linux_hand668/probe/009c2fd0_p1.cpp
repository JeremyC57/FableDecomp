// CRenderManager2D::~CRenderManager2D() -- implicit destructor
// Leaf: ??1CRenderManager2D@@UAE@XZ
#include "engine/CBaseClassNonCopyable.h"
#include "engine/CCountedPointer.h"
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
class CTVertexRHWColSpecTex1 { public: float X, Y, Z, RHW; unsigned long Colour, Specular; float U, V; };
class CShaderResource;
class CShaderDataBank { public:
    virtual ~CShaderDataBank();                 // 0x00A620D0
    unsigned char _body[0x128 - 4];
    CShaderDataBank();
};
class CVertexShaderData { public:
    virtual ~CVertexShaderData();
    void* PShader;                              // +0x04
    void* PDeclaration;                         // +0x08
    long  Handle;                               // +0x0c
    long  RefCount;                             // +0x10
};
class CShaderBase { public:
    CShaderBase() { }
protected:
    virtual ~CShaderBase() { }
};
class CVertexShader : public CShaderBase { public:
    CVertexShaderData* Data;                    // +0x04
    CVertexShader() : Data(0) { }
    virtual ~CVertexShader()
    {
        if (Data) {
            if (--Data->RefCount == 0)
                delete Data;
        }
    }
};
class CDisplayManager;
class CRenderStateManager : public CBaseClassNonCopyable { public:
};
class CRenderManagerCore : public CRenderStateManager { public:
    CRenderManagerCore(CDisplayManager& displayManager, unsigned long flags);
    virtual ~CRenderManagerCore();              // 0x00A0B060
    unsigned char _body[0x3d20 - 4];
};
class CRenderManager2D : public CRenderManagerCore { public:
    CShaderDataBank VertexShaderBank;           // +0x3d20
    CCountedPointer<CShaderResource> VertexShaderResource;   // +0x3e48
    CVertexShader VertexShader2D;               // +0x3e50
    std::vector<CTVertexRHWColSpecTex1> QuickDrawVertices;     // +0x3e58
    std::vector<CQuickDrawTriInfo> QuickDrawTriInfo;           // +0x3e64
    std::vector<CTVertexRHWColSpecTex1> QuickDrawAlphaVertices;   // +0x3e70
    std::vector<CQuickDrawTriInfo> QuickDrawAlphaTriInfo;      // +0x3e7c
    std::vector<CTVertexRHWColSpecTex1> QuickDraw2DVertices;   // +0x3e88
    std::vector<CQuickDrawTriInfo> QuickDraw2DTriInfo;         // +0x3e94
    bool AlternateRenderTargetClearEnabled;     // +0x3ea0
    CRenderManager2D(CDisplayManager& displayManager, unsigned long flags);
};
CRenderManager2D::CRenderManager2D(CDisplayManager& displayManager, unsigned long flags)
    : CRenderManagerCore(displayManager, flags)
{
}
