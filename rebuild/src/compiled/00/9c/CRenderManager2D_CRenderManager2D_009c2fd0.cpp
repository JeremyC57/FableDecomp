// CRenderManager2D::~CRenderManager2D() -- compiler-generated destructor: the three quick-draw tri-info vectors (out-of-line ~vector) and vertex vectors (freed inline) in reverse order, the 2-D vertex shader (release its shared data), the vertex-shader resource counted pointer, the vertex-shader bank, then ~CRenderManagerCore
// Leaf: ??1CRenderManager2D@@UAE@XZ
// Layout: engine/CTexture.h (flat 8-byte record without the inline ~CTexture / Uninitialise members) and engine/CShaderDataBank.h
//   (flat, vptr as data, 0x13C) cannot carry the destructors this body runs, so both are modelled locally.
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C2FD0, 217 bytes, thiscall, tail `jmp ??1CRenderManagerCore@@UAE@XZ` (0x00A0B060).
// Identity: called by two slot-0 scalar deleting destructors -- 0x009E16E0 (vtable 0x0129B9EC, RTTI CRenderManager2D) and
//   0x009C3100 (vtable 0x0129B400, RTTI CRenderManager : CRenderManager2D, whose own implicit destructor adds nothing) -- so it is
//   the CRenderManager2D destructor (Ego_r.pdb / FableWin.pdb ??1CRenderManager2D@@UAE@XZ). No CRenderManager2D vtable store:
//   the destructor is compiler-generated. It ends in ~CRenderManagerCore 0x00A0B060 (stores the CRenderManagerCore vtable
//   0x0129C9A4; RTTI CRenderManager2D : CRenderManagerCore). The catalog's "CEnginePrimitiveManagerOcclusionBase::~..." is
//   propagated (RTTI CEnginePrimitiveManagerOcclusionBase derives from CEnginePrimitiveManager, unrelated).
// Callees: std::vector<CQuickDrawTriInfo>::~vector 0x009C2F50 (x3, out of line; landed under a descriptive
//   CRetailVector_SOcclusionEntry name), free 0x00BFEA14, operator delete 0x00BFE9BC, ~CShaderDataBank (??1CShaderDataBank@@UAE@XZ,
//   Ego_r / FableWin) 0x00A620D0, ??1CRenderManagerCore@@UAE@XZ 0x00A0B060; vtables CVertexShader 0x0129B3D0, CShaderBase 0x0129A030.
// Layout: Ego_r.pdb CRenderManager2D: VertexShaderBank +0x3D20 (CRenderManagerCore 0x3D20 in both), then retail (CShaderDataBank
//   0x128 here, donor 0x1A8): VertexShaderResource (CCountedPointer<CShaderResource>) +0x3E48, VertexShader2D (CVertexShader) +0x3E50,
//   QuickDrawVertices +0x3E58, QuickDrawTriInfo +0x3E64, QuickDrawAlphaVertices +0x3E70, QuickDrawAlphaTriInfo +0x3E7C,
//   QuickDraw2DVertices +0x3E88, QuickDraw2DTriInfo +0x3E94 (STLport vectors, 12 bytes), AlternateRenderTargetClearEnabled +0x3EA0.
//   CQuickDrawTriInfo (0x3C) = Ego_r: String (CWideString) +0x14, UploadedTexture (CTexture) +0x18.
// Shape: no user destructor. The real STLport 4.6 vector text lets VC7.1 make retail's per-type choice by itself: ~vector of the
//   trivially destroyed vertex type expands to `if (_M_start) free`, ~vector<CQuickDrawTriInfo> (element dtor = ~CTexture's
//   Uninitialise + ~CWideString) stays an out-of-line call. ~CVertexShader is the landed ??_GCVertexShader 0x009C1AE0 body (local
//   copy of Data, `dec [data+10h]`, virtual deleting call with 1), then the CShaderBase vtable store (FableWin ??1CShaderBase@@MAE@XZ,
//   protected virtual, empty), then engine/CCountedPointer.h's release with immediate zero stores.
// TU: the CRenderManager2D constructor (Ego_r ??0CRenderManager2D@@QAE@AAVCDisplayManager@@ABVCWideString@@K@Z) is defined with
//   an empty body only to emit the vtable, the ??_G and this implicit ??1 (not graded; retail's body is not reconstructed).
// Header debt: engine/CRenderManagerCore.h, CShaderDataBank.h (0x13C STLport-sized vs retail 0x128 here) and CVertexShader.h are
//   flat records (vptr as data), so the polymorphic chain is modelled locally.
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
class CVertexShaderData {
    friend class CVertexShader;
    virtual ~CVertexShaderData();               // private virtual (Ego_r / FableWin ??1CVertexShaderData@@EAE@XZ)
public:
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
        CVertexShaderData* data = Data;
        if (data) {
            if (--data->RefCount == 0)
                delete data;
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
    CRenderManager2D(CDisplayManager& displayManager, const CWideString& name, unsigned long flags);
};
CRenderManager2D::CRenderManager2D(CDisplayManager& displayManager, const CWideString& name, unsigned long flags)
    : CRenderManagerCore(displayManager, flags)
{
}