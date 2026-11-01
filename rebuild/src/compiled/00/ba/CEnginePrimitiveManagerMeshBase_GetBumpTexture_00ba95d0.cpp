// CEnginePrimitiveManagerMeshBase::GetBumpTexture(const C3DMeshMaterial* material, CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> >* surfaces, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank, bool force) -- the texture of a material's bump map frame at the wanted mipmap: the bank's blank bump texture when the material has no bump map, a composed surface texture when the composite surface map yields one, else the per-frame GetFrame body of CGraphicInfoFrame (touch the resource LRU, begin loading, force the load when the renderer blocks on loads or `force` is set) returning the frame's texture, or the frame's blank texture while it is still loading
// Leaf: ?GetBumpTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@PAV?$CVectorMap@JVCCompositeSurfaceInfo@@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@@KAA_NPBVCGraphicDataBank@@_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BA95D0, 597 bytes, static __fastcall (material ecx, surfaces edx; mipmap, preloading, bank, force
//   on the stack; `ret 10h`); `preloading` is not read. Speed TU (no pragma).
// Identity (proven): Ego_r.pdb / FableWin.pdb ?GetBumpTexture@CEnginePrimitiveManagerMeshBase@@SIPBVCTexture@@PBVC3DMeshMaterial@@PAV?$CVectorMap@JVCCompositeSurfaceInfo@@V?$CKeyPairCompareLess@JVCCompositeSurfaceInfo@@@@@@KAA_NPBVCGraphicDataBank@@_N@Z
//   (Ego_r donor order GetDiffuseTexture 00a5fff0 / GetBumpTexture 00a60220 / GetReflectionTexture 00a60440 / Get2DAlphaMaskTexture
//   00a60640 = retail 0x00BA9360 / 0x00BA95D0 / 0x00BA9830 / 0x00BA9A70); the body reads C3DMeshMaterial::BumpMapBankIndex (+0x14)
//   and passes `bump = true` to GetComposedTexture; the six callers 0x00BA495F.. 0x00BD4A91 are the mesh renderers. Catalog
//   "_global::GetBumpTexture" is right but for the class.
// Callees: CGraphicDataBank::GetBlankTexture(ETextureType) 0x009FD150 (type 4), CEnginePrimitiveManagerMeshBase::GetComposedTexture
//   0x00BEBED0 (private static __fastcall: long& index, surfaces, mipmap, bump, bank), CResource::ListRemove 0x009FBFB0 / ListInsert
//   0x009FBF90, CFrameData::BeginLoading 0x009FDE80, CFrameData::CFrameData(const CGraphicInfoFrame*) 0x009FE380 (operator new
//   0x00BFEA1A, 0x44 bytes), CGraphicInfoFrame::InternalGetFrameForcedLoading 0x009FF6B0, CGraphicInfoFrame::GetBlankTexture
//   0x009FD3F0; global 0x01436E80 (g_pPrimitiveRenderer_01436e80, BlockOnLoad +4).
// Shape: as the parked GetReflectionTexture 0x00BA9830 model (2026-10-27_linux_hand683: STLport-style vector member for the double
//   size computation), `long index` address-taken for GetComposedTexture (so the index lives in a stack home and is reloaded after
//   the call), `if (surfaces) { t = GetComposedTexture(..); if (t) return t; }`, `flags = 0; if (BlockOnLoad || force) flags = 0x11;`, the frame body inlined
//   per row because its frame-less result differs between the four siblings.
class CTexture;
enum ETextureType { TEXTURE_TYPE_0 = 0, TEXTURE_TYPE_BUMP = 4 };   // 4: the bank's blank bump texture (descriptive enumerator)
void __cdecl operator delete(void* block);             // 0x00BFE9BC

class CResourceList;
class CResource { public:                                     // Ego_r.pdb, 0x1C bytes
    virtual void v00();
    virtual void Release();                            // slot 1: the CIVCountedPointer release on the last reference
    unsigned long  RefCount;                           // +0x04 (CIVCountedPointer count)
    CResourceList* ResourceList;                       // +0x08
    CResource*     PrevResource;                       // +0x0c
    CResource*     NextResource;                       // +0x10
    unsigned long  ResourceSize;                       // +0x14
    unsigned long  LastUsedFrame;                      // +0x18
private:
    void ListRemove();                                 // 0x009FBFB0
public:
private:
    void ListInsert(CResource*a0, CResource*a1); // 0x009FBF90
public:
    __forceinline void Touch();
};
class CResourceList { public:                                 // Ego_r.pdb, 0x3C bytes
    unsigned long _pad00;
    CResource     Head;                                // +0x04
    unsigned long ResourceCount;                       // +0x20
    long          AllocatedMemory;                     // +0x24
    long          MaximumMemory;                       // +0x28
    unsigned long CurrentFrame;                        // +0x2c
};
__forceinline void CResource::Touch()
{
    if (ResourceList && LastUsedFrame != ResourceList->CurrentFrame) {
        ListRemove();
        ListInsert(ResourceList->Head.PrevResource, &ResourceList->Head);
        LastUsedFrame = ResourceList->CurrentFrame;
    }
}

struct CCPInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
};
struct CTexturePointer {                               // CCountedPointer<CTexture>
    CTexture* Data;
    CCPInfo* Info;
    __forceinline ~CTexturePointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CGraphicFrameData { public:                             // Ego_r.pdb, 27 bytes
    unsigned long   ReferenceCount;                    // +0x00
    void*           Data;                              // +0x04
    CTexturePointer PTexture;                          // +0x08
    unsigned short  TLX, TLY, Width, Height, Pitch;    // +0x10
    unsigned char   Alpha : 1;                         // +0x1a
    unsigned char   BooleanAlpha : 1;
    unsigned char   UsesSurface : 1;
    unsigned char   SectionOfTPage : 1;
    unsigned char   IsInitialised : 1;
    unsigned char   ScreenGraphic : 1;
    void Uninitialise();                               // 0x009FCBA0
    __forceinline ~CGraphicFrameData() { if (IsInitialised) Uninitialise(); }
    __forceinline void DecRef()
    {
        if (--ReferenceCount == 0) {
            this->~CGraphicFrameData();
            operator delete(this);
        }
    }
};
class CGraphicFrame { public:                                 // intrusive pointer to CGraphicFrameData
    CGraphicFrameData* FrameData;
    __forceinline CGraphicFrame& operator=(const CGraphicFrame& other)
    {
        if (FrameData)
            FrameData->DecRef();
        FrameData = other.FrameData;
        if (FrameData)
            ++FrameData->ReferenceCount;
        return *this;
    }
};
class CManagedTexture { public:                               // Ego_r.pdb, 8 bytes
    unsigned long _pad00;
    CResource*    Data;                                // +0x04 CIVCountedPointer<CManagedTextureData>
    __forceinline void Touch() { if (Data) Data->Touch(); }
};
class CGraphicInfoFrame;
struct CFrameData : CResource {                        // Ego_r.pdb, 0x44 bytes
    CGraphicFrame   Frame;                             // +0x1c
    CManagedTexture ManagedTexture;                    // +0x20
    CManagedTexture LoadingManagedTexture;             // +0x28
    void*           FileData[2];                       // +0x30 CCountedPointer<CBankFileAsyncData>
    signed char     CurrentMipmapLevel;                // +0x38
    signed char     LoadingMipmapLevel;                // +0x39
    signed char     TouchedMipmapLevel;                // +0x3a
    unsigned long   TouchedFrame;                      // +0x3c
    CGraphicInfoFrame* GraphicInfoFrame;               // +0x40
    CFrameData(CGraphicInfoFrame* frame);              // 0x009FE380
    void BeginLoading(unsigned long mipmapLevel, unsigned long flags);   // 0x009FDE80
    __forceinline void DecRef() { if (--RefCount == 0) Release(); }
};
class CGraphicDataBank { public:
    unsigned char _pad00[0x194];
    unsigned long CurrentFrame;                        // +0x194
};
struct CFrameDataPointer {                             // CIVCountedPointer<CFrameData>
    CFrameData* P;
    __forceinline CFrameDataPointer& operator=(CFrameData* p)
    {
        if (P) {
            P->DecRef();
            P = 0;
        }
        P = p;
        return *this;
    }
    CFrameData* operator->() const { return P; }
    operator bool() const { return P != 0; }
};
class CGraphicInfo;
class CGraphicInfoFrame { public:                             // Ego_r.pdb, 16 bytes
    unsigned long     _pad00;
    unsigned short    FrameNum;                        // +0x04
    signed char       SmallMipmapLevel;                // +0x06
    CFrameDataPointer Data;                            // +0x08
    CGraphicInfo*     GraphicInfo;                     // +0x0c
private:
    bool InternalGetFrameForcedLoading(unsigned long a0) const;   // 0x009FF6B0
public:
    __forceinline const CTexture* GetTexture(unsigned long mipmapLevel, unsigned long flags);   // per-row inline body (the frame-less result differs between the four siblings)
    const CTexture* GetBlankTexture() const;                      // 0x009FD3F0 ?GetBlankTexture@CGraphicInfoFrame@@QBEPBVCTexture@@XZ
};

class CGraphicInfo { public:                                  // Ego_r.pdb, 0x2c bytes
    unsigned char      _pad00[0x1c];                   // +0x00 CGraphicHeader Header
    ETextureType       TextureType;                    // +0x1c
    CGraphicDataBank*  Bank;                           // +0x20
    long               Index;                          // +0x24
    CGraphicInfoFrame* Frames;                         // +0x28
};
template<class T> struct SVector {                  // STLport std::vector<T> (start, finish, end of storage)
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    __forceinline unsigned long size() const { return (unsigned long)(_M_finish - _M_start); }
    __forceinline const T& operator[](unsigned long n) const { return *(_M_start + n); }
};
class CGraphicDataBankFull : public CGraphicDataBank { public:   // the bank's graphic list (std::vector<CGraphicInfo>) and blank textures
    unsigned char      _pad01[0x1e0 - 0x198];
    SVector<CGraphicInfo> GraphicList;                 // +0x1e0
    unsigned char      _pad02[0x2b4 - 0x1ec];
    const CTexture* GetBlankTexture(ETextureType type) const;   // 0x009FD150 ?GetBlankTexture@CGraphicDataBank@@QBEPBVCTexture@@W4ETextureType@1@@Z
    __forceinline unsigned long GetNoGraphics() const { return GraphicList.size(); }
    __forceinline const CGraphicInfo* GetGraphicInfo(unsigned long index) const { return index < GraphicList.size() ? &GraphicList[index] : 0; }
};
class C3DMeshMaterial { public:                              // Ego_r.pdb, 0x30 bytes
    void*          __vftable;
    long           MaterialIndex;                      // +0x04
    void*          Name;                               // +0x08 CCharString
    long           DecalID;                            // +0x0c
    unsigned long  DiffuseMapBankIndex;                // +0x10
    unsigned long  BumpMapBankIndex;                   // +0x14
    unsigned long  ReflectionMapBankIndex;             // +0x18
};
struct CEnginePrimitiveRendererSharedData { void* _pad00; bool BlockOnLoad; };   // +0x04
extern CEnginePrimitiveRendererSharedData* g_pPrimitiveRenderer_01436e80;       // 0x01436E80


class CCompositeSurfaceInfo;
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class C> class CVectorMap;                 // engine CVectorMap (0x10 bytes), opaque here
typedef CVectorMap<long, CCompositeSurfaceInfo, CKeyPairCompareLess<long, CCompositeSurfaceInfo> > CCompositeSurfaceMap;

struct CEnginePrimitiveManagerMeshBase {
    static const CTexture* __fastcall GetBumpTexture(const C3DMeshMaterial* material, CCompositeSurfaceMap* surfaces, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank, bool force);
private:
    static const CTexture* __fastcall GetComposedTexture(long& index, CCompositeSurfaceMap& surfaces, unsigned long mipmap, bool bump, const CGraphicDataBank* bank);   // 0x00BEBED0
};

__forceinline const CTexture* CGraphicInfoFrame::GetTexture(unsigned long mipmapLevel, unsigned long flags)
{
    if (Data) {
        if (Data->Frame.FrameData) {
            if (!(flags & 8)) {
                unsigned long frame = GraphicInfo->Bank->CurrentFrame;
                if (Data->TouchedFrame != frame || mipmapLevel <= (unsigned long)Data->TouchedMipmapLevel) {
                    Data->TouchedFrame = frame;
                    Data->ManagedTexture.Touch();
                    if (mipmapLevel <= (unsigned long)Data->CurrentMipmapLevel)
                        Data->Touch();
                    if ((unsigned long)Data->TouchedMipmapLevel > mipmapLevel)
                        Data->TouchedMipmapLevel = (signed char)mipmapLevel;
                }
            }
            if (mipmapLevel < (unsigned long)Data->CurrentMipmapLevel) {
                Data->BeginLoading(mipmapLevel, flags);
                if (!(flags & 0x10) || !Data->FileData[0])
                    return Data->Frame.FrameData->PTexture.Data;
            } else {
                return Data->Frame.FrameData->PTexture.Data;
            }
        }
        Data->BeginLoading(mipmapLevel, flags);
    } else {
        if (flags & 4)
            return GetBlankTexture();
        Data = new CFrameData(this);
        Data->BeginLoading(mipmapLevel, flags);
    }
    if (!(flags & 1) || !InternalGetFrameForcedLoading(flags))
        return GetBlankTexture();
    return Data->Frame.FrameData->PTexture.Data;
}

const CTexture* __fastcall CEnginePrimitiveManagerMeshBase::GetBumpTexture(const C3DMeshMaterial* material, CCompositeSurfaceMap* surfaces, unsigned long mipmap, bool& preloading, const CGraphicDataBank* bank, bool force)
{
    const CGraphicDataBankFull* fullBank = static_cast<const CGraphicDataBankFull*>(bank);
    long index = material->BumpMapBankIndex;
    if (index == 0)
        return fullBank->GetBlankTexture(TEXTURE_TYPE_BUMP);
    if (surfaces) {
        const CTexture* composed = GetComposedTexture(index, *surfaces, mipmap, true, bank);
        if (composed)
            return composed;
    }
    CGraphicInfoFrame* frame = fullBank->GetGraphicInfo(index) ? fullBank->GetGraphicInfo(index)->Frames : 0;
    unsigned long flags = 0;
    if (g_pPrimitiveRenderer_01436e80->BlockOnLoad || force)
        flags = 0x11;
    return frame->GetTexture(mipmap, flags);
}