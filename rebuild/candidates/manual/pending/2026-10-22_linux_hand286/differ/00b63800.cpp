// Grade: DIFFER(1022v1023) -- register/frame allocation only: retail keeps this in ebp and the zero (then the started flag) in ebx with the vector's last in ecx, and spills the load-loop index/bound to [esp+0x14]/[esp+0x18] (frame 0x18); ours keeps this in ebx, zero in ebp, last in eax, frame 0x14. Shapes (STLport push_back through a local copy of _M_finish, by-value element, inline frame RequestLoad) all match.
// CEngineSkyRenderer::LoadNewScene(const CEngineComponentLoadNewSceneParams& params) -- gather the graphic bank indices the sky needs (two each of the current environment's three sky texture lists, the sky def's three textures and every cloud layer's texture) and start loading every one not yet resident; true when any load was started
// Leaf: LoadNewScene
// Layout: engine/CEngineSkyRenderer.h (PSkyDef_Object +0x190, viewed as the local CSkyDefView); the frame-loading and bank models
//   of the landed GetVolumeTexture 0x009FF710 / BeginLoading 0x009FE620 / Line Update 0x00BC7F70; STLport vector<unsigned long>
//   (12 bytes, InsertGrow 0x00464ACE, storage released with free 0x00BFEA14).
#include <new>
#include <stdlib.h>
#include "engine/CEngineSkyRenderer.h"
void __cdecl operator delete(void* block);         // 0x00BFE9BC
struct CEngineInternalPrimitiveBase;
struct CEngineSubPrimitiveChain;
struct CCharString {
    void* m_pData;
    CCharString& operator=(const CCharString& other);   // 0x0099B7D0
};
struct CRGBColour { unsigned char B, G, R, A; };

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void* object);
    void* m_pObject;
};
struct CTexture;
struct CCountedPointer_CTexture {
    CTexture*    m_pObject;
    SCountBlock* m_pCount;
    __forceinline ~CCountedPointer_CTexture()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                operator delete(m_pCount);
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};
struct CGraphicFrameData {
    unsigned long            ReferenceCount;       // +0x00
    void*                    Data;                 // +0x04
    CCountedPointer_CTexture PTexture;             // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;              // +0x1a
    void Uninitialise();                           // 0x009FCBA0
    __forceinline ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
struct CGraphicFrame {
    CGraphicFrameData* FrameData;
    CGraphicFrame(const CGraphicFrame& other) : FrameData(other.FrameData) { if (FrameData) ++FrameData->ReferenceCount; }
    __forceinline ~CGraphicFrame()
    {
        if (--FrameData->ReferenceCount == 0)
            delete FrameData;
    }
    CTexture* GetTexture() const { return FrameData->PTexture.m_pObject; }
};

struct CResource;
struct CResourceList {
    unsigned char _pad00[4];
    CResource*    Tail;                            // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CResource*    Head;                            // +0x10
    unsigned char _pad14[0x2c - 0x14];
    unsigned long Stamp;                           // +0x2c
};
struct CResource {
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4)
    long           m_references;                   // +0x04
    CResourceList* List;                           // +0x08
    unsigned char  _pad0C[0x18 - 0x0c];
    unsigned long  Stamp;                          // +0x18
    void ListRemove();                             // 0x009FBFB0
    void ListInsert(CResource* before, CResource* after);   // 0x009FBF90
    __forceinline void Touch()
    {
        if (List && Stamp != List->Stamp) {
            ListRemove();
            ListInsert(List->Head, (CResource*)&List->Tail);
            Stamp = List->Stamp;
        }
    }
};
struct CVolumeHolder {
    void*      _word00;                            // +0x00
    CResource* Volume;                             // +0x04
};
struct CGraphicInfoFrame;
struct CFrameData : CResource {
    CGraphicFrame Frame;                           // +0x1c
    CVolumeHolder VolumeHolder;                    // +0x20
    unsigned char _pad28[0x38 - 0x28];
    signed char   SmallestResidentMip;             // +0x38
    unsigned char _pad39;
    signed char   LowestRequestedMip;              // +0x3a
    unsigned char _pad3B;
    unsigned long LastFrameStamp;                  // +0x3c
    unsigned char _pad40[4];
    CFrameData(CGraphicInfoFrame* frame);          // 0x009FE380
    void BeginLoading(unsigned long a, unsigned long b);   // 0x009FDE80
};
struct CFrameCounter {
    unsigned char _pad[0x194];
    unsigned long FrameStamp;                      // +0x194
};
struct CGraphicInfo {
    unsigned char  _pad[0x20];
    CFrameCounter* Counter;                        // +0x20
};
struct CGraphicInfoFrame {
    void*          _word00;
    unsigned short FrameNum;
    signed char    SmallMipmapLevel;
    CFrameData*    Data;                           // +0x08
    CGraphicInfo*  GraphicInfo;                    // +0x0c
    bool InternalGetFrameForcedLoading(long flags);   // 0x009FF6B0
};
template <class T> struct CIVCountedPointer {
    T* Data;
    void Reset()
    {
        T* p = Data;
        if (p) {
            if (--p->m_references == 0)
                p->Destroy();
            Data = 0;
        }
    }
    CIVCountedPointer& operator=(T* p) { Reset(); Data = p; return *this; }
    T* operator->() const { return Data; }
};
struct CGraphicInfoFrameT {
    void*                         _word00;
    unsigned short                FrameNum;
    signed char                   SmallMipmapLevel;
    CIVCountedPointer<CFrameData> Data;            // +0x08
    CGraphicInfo*                 GraphicInfo;     // +0x0c
    bool InternalGetFrameForcedLoading(long flags);   // 0x009FF6B0
    __forceinline bool RequestLoad(unsigned long mip, unsigned long flags)
    {
        if (Data.Data && Data->Frame.FrameData) {
            Data->LastFrameStamp = GraphicInfo->Counter->FrameStamp;
            CVolumeHolder& holder = Data->VolumeHolder;
            if (holder.Volume)
                holder.Volume->Touch();
            Data->Touch();
            if ((unsigned long)Data->LowestRequestedMip > mip)
                Data->LowestRequestedMip = (signed char)mip;
            if ((unsigned long)Data->SmallestResidentMip > mip)
                Data->BeginLoading(mip, flags);
            return false;
        }
        if (!Data.Data)
            Data = new CFrameData((CGraphicInfoFrame*)this);
        Data->BeginLoading(mip, flags);
        return true;
    }
};
struct SBankGraphic {
    unsigned char       _pad[0x28];
    CGraphicInfoFrameT* m_pFrame;                  // +0x28
};
template <class T> struct CRetailVector {
    T* m_pFirst;
    T* m_pLast;
    T* m_pEnd;
    T* begin() { return m_pFirst; }
    T* end() { return m_pLast; }
    unsigned long size() { return end() - begin(); }
    T& operator[](unsigned long n) { return *(begin() + n); }
};
struct CGraphicBank {
    unsigned char               _pad[0x1e0];
    CRetailVector<SBankGraphic> m_records;         // +0x1e0
    __forceinline SBankGraphic* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
    __forceinline CGraphicInfoFrameT* GetFrame(unsigned long index) { return Get(index) ? Get(index)->m_pFrame : 0; }
};
struct CEngineGlobal_01436e98 {
    unsigned char _pad[0x954];
    CGraphicBank* m_pGraphicBank;                  // +0x954
};
extern CEngineGlobal_01436e98* g_pEngine_01436e98;   // 0x01436E98


struct SScalarTag { SScalarTag() { } };
template <class T1, class T2> inline void _Construct(T1* p, const T2& value) { new (p) T1(value); }
struct CBankIndexVector {
    unsigned long* first;
    unsigned long* last;
    unsigned long* end;
    CBankIndexVector() : first(0), last(0), end(0) { }
    ~CBankIndexVector() { if (first) free(first); }
    void InsertGrow(unsigned long* where, const unsigned long& value, const SScalarTag& tag, unsigned long count, bool noTail);   // 0x00464ACE
    void push_back(unsigned long value)
    {
        unsigned long* p = last;
        if (p != end) {
            _Construct(p, value);
            last = p + 1;
        }
        else
            InsertGrow(last, value, SScalarTag(), 1, true);
    }
    unsigned long size() const { return last - first; }
    unsigned long& operator[](unsigned long n) { return first[n]; }
};
template <class T> struct CArrayView {
    T* First;
    T* Last;
    T* End;
};
struct SSkyTextureSettings {
    unsigned char     _pad000[0x1a8];
    CArrayView<long>  DayTextures;                 // +0x1a8
    unsigned char     _pad1b4[0x1c0 - 0x1b4];
    CArrayView<long>  DuskTextures;                // +0x1c0
    unsigned char     _pad1cc[0x1d8 - 0x1cc];
    CArrayView<long>  NightTextures;               // +0x1d8
};
struct SEnvironmentSettings {
    unsigned char        _pad00[0x54];
    SSkyTextureSettings* m_pSky;                   // +0x54
};
struct CEnvironment {
    virtual void v00();
    virtual SEnvironmentSettings* GetSettings();   // slot 1
};
struct CEnvironmentProvider {
    virtual void v00();
    virtual CEnvironment* GetCurrentEnvironment(); // slot 1
};
struct SEngineState_01436e8c {
    unsigned char         _pad[0x198];
    CEnvironmentProvider* m_pEnvironmentProvider;  // +0x198
};
extern SEngineState_01436e8c* g_pEngineState_01436e8c;   // 0x01436E8C

struct SCloudLayer {                               // 0x4c bytes
    unsigned char _pad00[0x48];
    long          Texture;                         // +0x48
};
struct CSkyDefView {
    unsigned char           _pad00[0x40];
    long                    SunTexture;            // +0x40
    unsigned char           _pad44[4];
    long                    MoonTexture;           // +0x48
    unsigned char           _pad4c[0x54 - 0x4c];
    long                    StarTexture;           // +0x54
    unsigned char           _pad58[0x74 - 0x58];
    CArrayView<SCloudLayer> CloudLayers;           // +0x74
};
struct CEngineComponentLoadNewSceneParams;
struct CEngineSkyRenderer_Methods : CEngineSkyRenderer {
    bool LoadNewScene(const CEngineComponentLoadNewSceneParams& params);
};

bool CEngineSkyRenderer_Methods::LoadNewScene(const CEngineComponentLoadNewSceneParams& params)
{
    SSkyTextureSettings* sky = g_pEngineState_01436e8c->m_pEnvironmentProvider->GetCurrentEnvironment()->GetSettings()->m_pSky;
    CBankIndexVector banks;
    for (unsigned long i = 0; i < 2; ++i) {
        banks.push_back(sky->DayTextures.First[i]);
        banks.push_back(sky->NightTextures.First[i]);
        banks.push_back(sky->DuskTextures.First[i]);
    }
    banks.push_back(((CSkyDefView*)PSkyDef_Object)->SunTexture);
    banks.push_back(((CSkyDefView*)PSkyDef_Object)->MoonTexture);
    banks.push_back(((CSkyDefView*)PSkyDef_Object)->StarTexture);
    long count = ((CSkyDefView*)PSkyDef_Object)->CloudLayers.Last - ((CSkyDefView*)PSkyDef_Object)->CloudLayers.First;
    for (long j = 0; j < count; ++j)
        banks.push_back(((CSkyDefView*)PSkyDef_Object)->CloudLayers.First[j].Texture);
    bool started = false;
    unsigned long total = banks.size();
    for (unsigned long k = 0; k < total; ++k) {
        unsigned long index = banks[k];
        if (index) {
            CGraphicInfoFrameT* frame = g_pEngine_01436e98->m_pGraphicBank->GetFrame(index);
            if (frame && frame->RequestLoad(0, 0))
                started = true;
        }
    }
    return started;
}
