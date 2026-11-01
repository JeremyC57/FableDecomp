// CDisplayManager::~CDisplayManager() -- uninitialise the shader render manager when in use, drop the render manager, uninitialise the back/depth buffer surfaces, release the D3D device, then the members in reverse order and ~CBaseClassNonCopyable
// Leaf: ??1CDisplayManager@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009C01C0, 268 bytes, thiscall, tail jump to ~CBaseClassNonCopyable 0x0099A300.
// Identity (proven): the body stores vtable 0x0129B3F8 first, whose RTTI COL names .?AVCDisplayManager@@ (rtti-vtables.tsv:
//   CDisplayManager 0x0129B3F8 slot 0 = 0x009C30B0); the sole caller 0x009C30B3 is that slot-0 scalar deleting destructor
//   (??_GCDisplayManager@@UAEPAXI@Z, FableWin.pdb). Ego_r.pdb + FableWin.pdb ??1CDisplayManager@@UAE@XZ (virtual, public).
//   The member walk matches the Ego_r CDisplayManager layout (UsingShaderRenderManager +0x1B1, ShaderResourcePath +0x258,
//   BusyResourceManager +0x2B0 ...). The catalog label "CInputManagerDX::~CInputManagerDX" is propagated and wrong.
// Layout: Ego_r.pdb CDisplayManager (0x2EC) members; retail equal up to DeviceResetCallbacks +0x264 (STLport list, 4 bytes vs
//   Dinkumware 12), then 8 lower: LastDeviceCreationParameters +0x268, VsyncCallbackTimer +0x294, BackBufferFormats +0x298,
//   MultisampleTypes +0x2A4 (CArray = STLport vector, 12 bytes), BusyResourceManager +0x2B0 (two 4-byte STLport lists +
//   three dwords: 0x14), Gamma +0x2C4.
// Callees: CShaderRenderManager::Uninitialise 0x0098A7A0 on g_ShaderRenderManager 0x013BC470, CSurface::Uninitialise 0x009F2E20,
//   CBusyResourceManager::~CBusyResourceManager 0x00A61B10, free 0x00BFEA14 (STLport allocator), ~CVsyncCallbackTimer 0x009F7330
//   + operator delete 0x00BFE9BC (boost::checked_delete), ~_List_base<CDeviceResetCallback*> 0x009C2220 (out of line: clear()
//   defined out of class as in STLport _list.c), ~CWideString 0x0099B510, CTexture::Uninitialise 0x009F9F70 (inline
//   ~CTexture), the four CSurface virtual dtors inline (vtable 0x0122F84C + Uninitialise), CRenderManager's virtual deleting
//   dtor (slot 0) through boost::scoped_ptr, ~CBaseClassNonCopyable 0x0099A300.
// Shape: user dtor body `if (UsingShaderRenderManager) g_ShaderRenderManager.Uninitialise(); PRenderManager.reset(); ...`;
//   boost 1.30 scoped_ptr::reset is `this_type(p).swap(*this)`, hence the pointer is cleared before the delete.
// Layout: engine/CSurface.h and engine/CTexture.h are generated flat layouts (a `__vftable` member, no destructors); CSurface
//   (virtual ~CSurface, Ego_r ??1CSurface@@UAE@XZ) and CTexture (non-virtual ??1CTexture@@QAE@XZ) are local models of the same
//   layout with their inline destructors (NOTES.md "Header debt"). No engine/CDisplayManager.h exists.
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
void __cdecl operator delete(void* block);                  // 0x00BFE9BC
#include "engine/CBaseClassNonCopyable.h"                   // ~CBaseClassNonCopyable 0x0099A300 (protected virtual)

namespace boost {
template <class T> inline void checked_delete(T* x)
{
    typedef char type_must_be_complete[sizeof(T) ? 1 : -1];
    (void) sizeof(type_must_be_complete);
    delete x;
}
template <class T> class scoped_ptr { public:
    typedef scoped_ptr<T> this_type;
    explicit scoped_ptr(T* p = 0) : ptr(p) { }
    ~scoped_ptr() { boost::checked_delete(ptr); }
    void reset(T* p = 0) { this_type(p).swap(*this); }
    void swap(scoped_ptr& b) { T* tmp = b.ptr; b.ptr = ptr; ptr = tmp; }
private:
    T* ptr;
};
} // namespace boost

namespace std {
// STLport 4.6 stl/_alloc.h, stl/_construct.h, stl/_vector.h, stl/_list.h + _list.c (the members this row needs)
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __false_type has_trivial_destructor; };
template <class _Tp> struct __type_traits<_Tp*> { typedef __true_type has_trivial_destructor; };
template <class _Tp> class allocator { public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc { public:
    _Value _M_data;
};
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _ForwardIterator, class _Tp>
inline void __destroy(_ForwardIterator __first, _ForwardIterator __last, _Tp*)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last) { __destroy(__first, __last, __first); }
template <class _Tp, class _Alloc>
class _Vector_base { public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
    ~_Vector_base() { if (_M_start != 0) _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start); }
};
template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> { public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
struct _List_node_base {
    _List_node_base* _M_next;
    _List_node_base* _M_prev;
};
template <class _Tp> struct _List_node : public _List_node_base {
    _Tp _M_data;
};
template <class _Tp, class _Alloc>
class _List_base { public:
    _STLP_alloc_proxy<_List_node_base*, _List_node<_Tp>, allocator<_List_node<_Tp> > > _M_node;
    ~_List_base() { clear(); _M_node.deallocate((_List_node<_Tp>*)_M_node._M_data, 1); }
    void clear();
};
template <class _Tp, class _Alloc>
void _List_base<_Tp, _Alloc>::clear()
{
    _List_node<_Tp>* __cur = (_List_node<_Tp>*)this->_M_node._M_data->_M_next;
    while (__cur != this->_M_node._M_data) {
        _List_node<_Tp>* __tmp = __cur;
        __cur = (_List_node<_Tp>*)__cur->_M_next;
        _Destroy(&__tmp->_M_data);
        this->_M_node.deallocate(__tmp, 1);
    }
    this->_M_node._M_data->_M_next = this->_M_node._M_data;
    this->_M_node._M_data->_M_prev = this->_M_node._M_data;
}
template <class _Tp, class _Alloc = allocator<_Tp> >
class list : protected _List_base<_Tp, _Alloc> { public:
};
} // namespace std
template <class T> class CArray : public std::vector<T> { };   // Fable's CArray over the STLport vector

struct IDirect3D9;
struct IDirect3DDevice9 {                                   // d3d9.h COM interface (IUnknown slots)
    virtual long __stdcall QueryInterface(const struct _GUID& riid, void** ppvObj) = 0;
    virtual unsigned long __stdcall AddRef() = 0;
    virtual unsigned long __stdcall Release() = 0;
};
struct IDirect3DSurface9;
struct IDirect3DTexture9;
struct HWND__;

class CShaderRenderManager { public:
    void Uninitialise();                                    // 0x0098A7A0
};
extern CShaderRenderManager g_ShaderRenderManager;          // 0x013BC470

class CSystemManager;
class CRenderManager { public:
    virtual ~CRenderManager();                              // slot 0 (deleting dtor called with 1)
};
class CVsyncCallbackTimer { public:
    ~CVsyncCallbackTimer();                                 // 0x009F7330
};
class CDeviceResetCallback;
class CWideStringData;
class CWideString { public:
    ~CWideString();                                         // 0x0099B510
    CWideStringData* PStringData;
};
class CSurface { public:                                    // vtable 0x0122F84C, 0x10 bytes (engine/CSurface.h layout)
    virtual ~CSurface() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F2E20
    IDirect3DSurface9* PD3DSurface;                         // +0x04
    unsigned long      AllocationSource;                    // +0x08
    void*              PAllocatedMemory;                    // +0x0C
};
class CTexture { public:                                    // 8 bytes (engine/CTexture.h layout)
    ~CTexture() { Uninitialise(); }
    void Uninitialise();                                    // 0x009F9F70
    IDirect3DTexture9* PD3DTexture;                         // +0x00
    unsigned long      ByteLength : 28;                     // +0x04
    unsigned long      AllocationSource : 4;
};
class C2DExtentsI { public: long X; long Y; };
class C2DBoxI { public: long TLX; long TLY; long BRX; long BRY; };
class C2DBoxF { public: float TLX; float TLY; float BRX; float BRY; };
class CPixelFormat { public: long FormatIndex; };
class CPresentationIntervalDesc { public:
    bool PresentImmediate;
    bool PresentImmediateIfSyncMissed;
    long VSyncCount;
};
enum ESurfaceMultisampleType { };
class CBusyResourceManager { public:                        // 0x14 bytes in retail
    struct CPendingQuery;
    ~CBusyResourceManager();                                // 0x00A61B10
    std::list<CPendingQuery> FreeQueoryObjects;             // +0x00
    std::list<CPendingQuery> PendingQueories;               // +0x04
    long PendingFrame;                                      // +0x08
    long FinishedFrame;                                     // +0x0C
    IDirect3DDevice9* D3DDevice;                            // +0x10
};

class CDisplayManager : public CBaseClassNonCopyable { public:   // vtable 0x0129B3F8
    struct CCreateDeviceParameters {                        // 0x2C bytes
        HWND__* WinHandle;
        C2DExtentsI BackBufferDimensions;
        long BackBufferBitDepth;
        long ZBufferBitDepth;
        long BackBufferCount;
        long RefreshRate;
        bool ExclusiveMode;
        CPresentationIntervalDesc PresentationInterval;
        ESurfaceMultisampleType MultisampleType;
    };
    CDisplayManager(CSystemManager& systemManager);
    virtual ~CDisplayManager();
    CSystemManager* SystemManager;                          // +0x04
    boost::scoped_ptr<CRenderManager> PRenderManager;       // +0x08
    CSurface RenderTargetSurface;                           // +0x0C
    CSurface RenderTargetDepthBuffer;                       // +0x1C
    CSurface BackBufferSurface;                             // +0x2C
    CSurface DepthBufferSurface;                            // +0x3C
    CTexture BackBufferCopyTexture;                         // +0x4C
    IDirect3D9* D3D;                                        // +0x54
    IDirect3DDevice9* D3DDevice;                            // +0x58
    unsigned char D3DDeviceCaps[0x130];                     // +0x5C _D3DCAPS9
    C2DExtentsI BackBufferDimensions;                       // +0x18C
    C2DExtentsI RenderTargetDimensions;                     // +0x194
    long NoBackBuffers;                                     // +0x19C
    bool ExclusiveMode;                                     // +0x1A0
    ESurfaceMultisampleType MultisampleType;                // +0x1A4
    CPixelFormat BackBufferFormat;                          // +0x1A8
    CPixelFormat DepthBufferFormat;                         // +0x1AC
    bool UsingRenderManager;                                // +0x1B0
    bool UsingShaderRenderManager;                          // +0x1B1
    HWND__* WindowHandle;                                   // +0x1B4
    unsigned long CurrentMode[4];                           // +0x1B8 _D3DDISPLAYMODE
    long RefreshRate;                                       // +0x1C8
    CPresentationIntervalDesc PresentationInterval;         // +0x1CC
    float PresentationIntervalTime;                         // +0x1D4
    bool ViewportOffscreen;                                 // +0x1D8
    unsigned long D3DViewport[6];                           // +0x1DC _D3DVIEWPORT9
    C2DBoxI ViewportBox;                                    // +0x1F4
    C2DBoxF ViewportBoxFloat;                               // +0x204
    bool Rendering;                                         // +0x214
    bool CanBeginRender;                                    // +0x215
    long CurrentScreenIndex;                                // +0x218
    unsigned long PresentParams[14];                        // +0x21C _D3DPRESENT_PARAMETERS_
    bool VirtualCoordsResolutionIndependant;                // +0x254
    bool DeviceLostFlag;                                    // +0x255
    CWideString ShaderResourcePath;                         // +0x258
    long FrameCounter;                                      // +0x25C
    unsigned long RenderManagerInitFlags;                   // +0x260
    std::list<CDeviceResetCallback*> DeviceResetCallbacks;  // +0x264
    CCreateDeviceParameters LastDeviceCreationParameters;   // +0x268
    boost::scoped_ptr<CVsyncCallbackTimer> VsyncCallbackTimer;   // +0x294
    CArray<CPixelFormat> BackBufferFormats;                 // +0x298
    CArray<ESurfaceMultisampleType> MultisampleTypes;       // +0x2A4
    CBusyResourceManager BusyResourceManager;               // +0x2B0
    float Gamma;                                            // +0x2C4
    bool m_bWantHiResScreenShot;                            // +0x2C8
    bool m_bTakingScreenShot;                               // +0x2C9
};

CDisplayManager::~CDisplayManager()
{
    if (UsingShaderRenderManager)
        g_ShaderRenderManager.Uninitialise();
    PRenderManager.reset();
    BackBufferSurface.Uninitialise();
    DepthBufferSurface.Uninitialise();
    if (D3DDevice)
        D3DDevice->Release();
}
FABLE_STATIC_ASSERT(sizeof(CDisplayManager) == 0x2CC);