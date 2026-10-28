// CCompositeSurfaceCache::CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner) -- a resource-list entry for one composited surface: owner, empty current/cached morphs, null graphic frame and managed texture, zero bank index and last-used frame
// Leaf: ??0CCompositeSurfaceCache@@QAE@AAVCEngineSurfaceCompositionManager@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B2C400, 91 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): Ego_r.pdb ??0CCompositeSurfaceCache@@QAE@AAVCEngineSurfaceCompositionManager@@@Z; stores vtable
//   0x012A116C after CResource::CResource() 0x009FBEC0; the member at +0x50 stores vtable 0x0129C880
//   (RTTI .?AVCManagedTexture@@ : CBaseClass) after CBaseClass::CBaseClass() 0x0099A2D0.
// Layout (retail, 12-byte STLport vector, so each CEngineTextureMorph is 0x14 not the PDB 0x18): CResource 0x1C,
//   Owner +0x1C, CurrentMorph +0x20, CachedMorph +0x34, MipmapLevel +0x48 (left alone), GraphicFrame +0x4C,
//   ManagedTexture +0x50, BankFileIndex +0x58, LastUsedFrame +0x5C, TimeStamp +0x60 (left alone).
// Header debt: no engine header models CResource / CEngineTextureMorph / CManagedTexture with constructors.
class CBaseClass {
public:
    CBaseClass();                                            // 0x0099A2D0
    virtual ~CBaseClass();
};
class CResourceList;
class CResource {
public:
    CResource();                                             // 0x009FBEC0
    virtual ~CResource();
    unsigned char  _base[0x4];                               // +0x04 (CBaseClass-side word)
    CResourceList* ResourceList;                             // +0x08
    CResource*     PrevResource;                             // +0x0c
    CResource*     NextResource;                             // +0x10
    unsigned long  ResourceSize;                             // +0x14
    unsigned long  LastUsedFrame;                            // +0x18
};

namespace std {
template <class _Tp> class allocator { public: allocator() { } };
template <class _Value, class _Tp, class _MaybeReboundAlloc> class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
    _Value _M_data;
};
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(const _Alloc& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) { }
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    explicit vector(const _Alloc& __a = _Alloc()) : _Vector_base<_Tp, _Alloc>(__a) { }
};
}

class CCompositeBlendDesc;
class CEngineTextureMorph {
public:
    long                                 OriginalTextureIndex;   // +0x00
    long                                 MorphID;                // +0x04
    std::vector<CCompositeBlendDesc>     Blends;                 // +0x08
    CEngineTextureMorph() : OriginalTextureIndex(0), MorphID(0) { }
};
class CGraphicFrameData;
class CGraphicFrame {
public:
    CGraphicFrameData* FrameData;
    CGraphicFrame() : FrameData(0) { }
};
template <class T> class CIVCountedPointer {
public:
    T* Data;
    CIVCountedPointer() : Data(0) { }
};
class CManagedTextureData;
class CManagedTexture : public CBaseClass {
public:
    CManagedTexture() { }
    virtual ~CManagedTexture();
    CIVCountedPointer<CManagedTextureData> Data;             // +0x04
};

class CEngineSurfaceCompositionManager;
class CCompositeSurfaceCache : public CResource {
public:
    CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner);
    virtual ~CCompositeSurfaceCache();
    CEngineSurfaceCompositionManager* Owner;                 // +0x1c
    CEngineTextureMorph               CurrentMorph;          // +0x20
    CEngineTextureMorph               CachedMorph;           // +0x34
    unsigned long                     MipmapLevel;           // +0x48
    CGraphicFrame                     GraphicFrame;          // +0x4c
    CManagedTexture                   ManagedTexture;        // +0x50
    unsigned long                     BankFileIndex;         // +0x58
    unsigned long                     LastUsedFrame;         // +0x5c
    double                            TimeStamp;             // +0x60
};

CCompositeSurfaceCache::CCompositeSurfaceCache(CEngineSurfaceCompositionManager& owner)
    : Owner(&owner), BankFileIndex(0), LastUsedFrame(0)
{
}