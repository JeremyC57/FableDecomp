// CCompositeSurfaceCache::~CCompositeSurfaceCache() -- composite surface cache destructor: release the managed texture, then the members (the managed texture, the shared graphic frame, the cached and current morphs' blend vectors) and ~CResource
// Leaf: ??1CCompositeSurfaceCache@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2B910, 157 bytes, thiscall, `jmp` tail to ~CResource.
// Identity (proven): the first store is vtable 0x012A116C, RTTI .?AVCCompositeSurfaceCache@@ (CCompositeSurfaceCache :
//   CManagedTextureOwner : CResource : CIVCountedPointeeBase, ghidra_out/rtti_bases.tsv); its slot 0 is the scalar deleting
//   destructor 0x00B2EE00, this body's caller (the other two calls sit in CEngineSurfaceCompositionManager::
//   UpdateTextureMorphCache 0x00B2D9F0); Ego_r.pdb ??1CCompositeSurfaceCache@@UAE@XZ. The catalog label
//   CFrameData::~CFrameData is propagated from the byte-similar CGraphicInfoFrame::CFrameData dtor 0x009FE6C0 and wrong.
// Layout: Ego_r.pdb CCompositeSurfaceCache at retail STLport sizes: Owner +0x1c, CurrentMorph +0x20 / CachedMorph +0x34
//   (CEngineTextureMorph {OriginalTextureIndex, MorphID, vector<CCompositeBlendDesc> Blends +8}: 0x14 bytes), MipmapLevel +0x48,
//   GraphicFrame +0x4c, ManagedTexture +0x50, BankFileIndex +0x58, LastUsedFrame +0x5c, TimeStamp +0x60.
// Callees: CManagedTexture::Release 0x00A69AD0 (Ego_r ?Release@CManagedTexture@@QAE_NXZ), CManagedTexture::~CManagedTexture
//   0x00A69E20 (Ego_r ??1CManagedTexture@@UAE@XZ, called directly on the member), CGraphicFrameData::Uninitialise 0x009FCBA0,
//   operator delete 0x00BFE9BC (count block, frame data), free 0x00BFEA14 (STLport vector storage), ~CResource 0x009FC550;
//   the texture count block's DeleteFunc (indirect).
// Shape: user dtor `ManagedTexture.Release();`; the members' dtors follow in reverse order: the inline ~CGraphicFrame (the
//   UnInitialise body over a named `data` local -- it picks retail's esi: shared-count release, the inline user
//   ~CGraphicFrameData = Uninitialise when IsInitialised then the CCountedPointer<CTexture> release, operator delete; a
//   `delete data` calls ??_GCGraphicFrameData instead, probe 1) and the STLport ~_Vector_base frees of the trivially destructible blend vectors; the implicit
//   ~CManagedTextureOwner stores nothing (engine/CResource.h retail opt-in).
#include "fable_compiler.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CResource.h"                            // : CIVCountedPointeeBase; ~CResource 0x009FC550
#include "engine/CBaseClass.h"
#include "engine/CCountedPointer.h"                      // CCountedPointer<CTexture> (count block delete 0x00BFE9BC)
void __cdecl operator delete(void* block);               // 0x00BFE9BC
extern "C" void __cdecl free(void* block);               // 0x00BFEA14
namespace std {
struct __true_type { };
struct __false_type { };
template <class _Tp> struct __type_traits { typedef __true_type has_trivial_destructor; };
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator __first, _ForwardIterator __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        _Destroy(&*__first);
}
template <class _ForwardIterator>
inline void __destroy_aux(_ForwardIterator, _ForwardIterator, const __true_type&) { }
template <class _Tp> inline void _Destroy(_Tp* __first, _Tp* __last)
{
    typedef typename __type_traits<_Tp>::has_trivial_destructor _Trivial_destructor;
    __destroy_aux(__first, __last, _Trivial_destructor());
}
template <class _Tp> class allocator {
public:
    allocator() { }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    _Tp* _M_start;                                              // +0x00
    _Tp* _M_finish;                                             // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;     // +0x08
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    ~vector() { _Destroy(this->_M_start, this->_M_finish); }
};
}
class CTexture;
class CManagedTextureData;
class CEngineSurfaceCompositionManager;
template <class T> class CIVCountedPointer { public:
    ~CIVCountedPointer();
    T* Data;
};
class CManagedTexture : public CBaseClass {              // RTTI CManagedTexture : CBaseClass; vtable 0x0129C880
public:
    virtual ~CManagedTexture();                          // 0x00A69E20
    bool Release();                                      // 0x00A69AD0
    CIVCountedPointer<CManagedTextureData> Data;         // +0x04
};
class CGraphicFrameData {                                // Ego_r.pdb, 27 bytes
public:
    ~CGraphicFrameData()
    {
        if (IsInitialised)
            Uninitialise();
    }
    void Uninitialise();                                 // 0x009FCBA0
    unsigned long             ReferenceCount;            // +0x00
    void*                     Data;                      // +0x04
    CCountedPointer<CTexture> PTexture;                  // +0x08
    unsigned short            TLX;                       // +0x10
    unsigned short            TLY;                       // +0x12
    unsigned short            Width;                     // +0x14
    unsigned short            Height;                    // +0x16
    unsigned short            Pitch;                     // +0x18
    unsigned char             Alpha : 1;                 // +0x1a
    unsigned char             BooleanAlpha : 1;
    unsigned char             UsesSurface : 1;
    unsigned char             SectionOfTPage : 1;
    unsigned char             IsInitialised : 1;
    unsigned char             ScreenGraphic : 1;
};
class CGraphicFrame {                                    // Ego_r.pdb, 4 bytes (engine/CGraphicFrame.h layout, no methods there)
    // ~CGraphicFrame: no out-of-line copy in Ego_r / FableWin (no ??1CGraphicFrame) and every retail use is expanded, incl.
    // ??_GCGraphicFrame 0x00B2ED60; plain `inline` (with or without the UnInitialise call, `delete data` or the explicit
    // dtor + operator delete) leaves a call here (probes 3-5, 7, 9-11), so it is __forceinline like the other Lionhead
    // header release helpers (EXTRA_RULES owner decision 2026-11-01).
public:
    FABLE_FORCEINLINE ~CGraphicFrame()
    {
        CGraphicFrameData* data = FrameData;
        if (data) {
            if (--data->ReferenceCount == 0) {
                data->~CGraphicFrameData();
                operator delete(data);
            }
            FrameData = 0;
        }
    }
    void UnInitialise();                                 // 0x00A00B90 (Ego_r ?UnInitialise@CGraphicFrame@@QAEXXZ)
    CGraphicFrameData* FrameData;                        // +0x00
};
enum ECompositeBlendType { };
class CRGBColour { public: unsigned long Colour; };       // 4 bytes (members not touched here)
class CCompositeBlendDesc {                              // Ego_r.pdb, 16 bytes, trivially destructible
public:
    unsigned long       TextureIndex;                    // +0x00
    ECompositeBlendType BlendType;                       // +0x04
    CRGBColour          DiffuseColour;                   // +0x08
    CRGBColour          SpecularColour;                  // +0x0c
};
class CEngineTextureMorph {                              // Ego_r.pdb (0x14 bytes at STLport sizes)
public:
    long                             OriginalTextureIndex;   // +0x00
    long                             MorphID;                // +0x04
    std::vector<CCompositeBlendDesc> Blends;                 // +0x08
};
class CManagedTextureOwner : public CResource {          // vtable 0x0129C848; implicit dtor
public:
    virtual void ReleaseTexture() = 0;                   // slot 10
};
class CCompositeSurfaceCache : public CManagedTextureOwner {   // vtable 0x012A116C
public:
    virtual ~CCompositeSurfaceCache();
    virtual void ReleaseTexture();                       // slot 10 0x00B2B120
    CEngineSurfaceCompositionManager* Owner;             // +0x1c
    CEngineTextureMorph               CurrentMorph;      // +0x20
    CEngineTextureMorph               CachedMorph;       // +0x34
    unsigned long                     MipmapLevel;       // +0x48
    CGraphicFrame                     GraphicFrame;      // +0x4c
    CManagedTexture                   ManagedTexture;    // +0x50
    unsigned long                     BankFileIndex;     // +0x58
    unsigned long                     LastUsedFrame;     // +0x5c
    double                            TimeStamp;         // +0x60
};
CCompositeSurfaceCache::~CCompositeSurfaceCache()
{
    ManagedTexture.Release();
}