// CEnginePrimitiveManagerRepeatedStaticMeshes::PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags) -- preload a repeated static mesh for a viewer position: pick the LOD from the screen-size ratio at that distance, fetch that LOD's mesh and start loading every sub-mesh material texture not yet resident; true when any load was started
// Leaf: PreloadPrimitive
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Layout: local models -- the mesh/material shapes of the landed 2DStaticMeshes Render2DPrimitive 0x00B9E950, the graphic-bank
//   frame loading of the landed GetVolumeTexture 0x009FF710 / BeginLoading 0x009FE620 (inline RequestLoad as in 0x00B63800).
// Shape: the LOD search stays top-tested (`cmp lod,3 ; jae` with a jmp back) only with a __forceinline early-return condition
//   (IsBelowLODCount); the inline RequestLoad is nested (Data, then Data->Frame) with the new-CFrameData path last; the distance clamp is a
//   `?:` max (je, not jp); lodScale is read between the dx/dy statements.
#include "fable_compiler.h"
void __cdecl operator delete(void* block);         // 0x00BFE9BC
#include <math.h>
class CEngineInternalPrimitiveBase;
class CEngineSubPrimitiveChain;
class CCharString { public:
    void* m_pData;
    const CCharString& operator=(const CCharString& other);   // 0x0099B7D0
};
class CRGBColour { public: unsigned char B, G, R, A; };

struct SCountBlock {
    long  m_references;
    void (__fastcall* m_pfnDestroy)(void* object);
    void* m_pObject;
};
class CTexture;
struct CCountedPointer_CTexture {
    CTexture*    m_pObject;
    SCountBlock* m_pCount;
    FABLE_FORCEINLINE ~CCountedPointer_CTexture()
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
class CGraphicFrameData { public:
    unsigned long            ReferenceCount;       // +0x00
    void*                    Data;                 // +0x04
    CCountedPointer_CTexture PTexture;             // +0x08
    unsigned char            _pad10[0xa];
    unsigned char            m_flags;              // +0x1a
    void Uninitialise();                           // 0x009FCBA0
    FABLE_FORCEINLINE ~CGraphicFrameData()
    {
        if (m_flags & 0x10)
            Uninitialise();
    }
};
class CGraphicFrame { public:
    CGraphicFrameData* FrameData;
    CGraphicFrame(const CGraphicFrame& other) : FrameData(other.FrameData) { if (FrameData) ++FrameData->ReferenceCount; }
    FABLE_FORCEINLINE ~CGraphicFrame()
    {
        if (--FrameData->ReferenceCount == 0)
            delete FrameData;
    }
    CTexture* GetTexture() const { return FrameData->PTexture.m_pObject; }
};

class CResource;
class CResourceList { public:
    unsigned char _pad00[4];
    CResource*    Tail;                            // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CResource*    Head;                            // +0x10
    unsigned char _pad14[0x2c - 0x14];
    unsigned long Stamp;                           // +0x2c
};
class CResource { public:
    virtual void v00();
    virtual void Destroy();                        // slot 1 (+4)
    long           m_references;                   // +0x04
    CResourceList* List;                           // +0x08
    unsigned char  _pad0C[0x18 - 0x0c];
    unsigned long  Stamp;                          // +0x18
private:
    void ListRemove();                             // 0x009FBFB0
public:
private:
    void ListInsert(CResource*a0, CResource*a1);   // 0x009FBF90
public:
    FABLE_FORCEINLINE void Touch()
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
class CGraphicInfoFrame;
struct CFrameCounter {
    unsigned char _pad[0x194];
    unsigned long FrameStamp;                      // +0x194
};
class CGraphicInfo { public:
    unsigned char  _pad[0x20];
    CFrameCounter* Counter;                        // +0x20
};
class CGraphicInfoFrame { public: class CFrameData; public:
    void*          _word00;
    unsigned short FrameNum;
    signed char    SmallMipmapLevel;
    CFrameData*    Data;                           // +0x08
    CGraphicInfo*  GraphicInfo;                    // +0x0c
private:
    bool InternalGetFrameForcedLoading(unsigned long flags) const;   // 0x009FF6B0 (Ego_r.pdb ?InternalGetFrameForcedLoading@CGraphicInfoFrame@@ABE_NK@Z, private)   // 0x009FF6B0
public:
};
class CGraphicInfoFrame::CFrameData : public CResource { public:
    CGraphicFrame Frame;                           // +0x1c
    CVolumeHolder VolumeHolder;                    // +0x20
    unsigned char _pad28[0x38 - 0x28];
    signed char   SmallestResidentMip;             // +0x38
    unsigned char _pad39;
    signed char   LowestRequestedMip;              // +0x3a
    unsigned char _pad3B;
    unsigned long LastFrameStamp;                  // +0x3c
    unsigned char _pad40[4];
    CFrameData(const CGraphicInfoFrame* frame);          // 0x009FE380
    void BeginLoading(unsigned long a, unsigned long b);   // 0x009FDE80
};   // Ego_r.pdb: nested in CGraphicInfoFrame (RTTI .?AVCFrameData@CGraphicInfoFrame@@), 0x44 bytes
typedef CGraphicInfoFrame::CFrameData CFrameData;   // the row's free code keeps the unqualified name

template <class T> class CIVCountedPointer { public:
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
    FABLE_FORCEINLINE bool RequestLoad(unsigned long mip, unsigned long flags)
    {
        if (Data.Data) {
            if (Data->Frame.FrameData) {
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
            Data->BeginLoading(mip, flags);
            return true;
        }
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
    FABLE_FORCEINLINE SBankGraphic* Get(unsigned long index) { return index < m_records.size() ? &m_records[index] : 0; }
    FABLE_FORCEINLINE CGraphicInfoFrameT* GetFrame(unsigned long index) { return Get(index) ? Get(index)->m_pFrame : 0; }
};



#include "engine/C3DVector.h"
struct CRefInfo {
    long  RefCount;                                // +0
    void (__fastcall* Destroy)(void*);             // +4
    void* Object;                                  // +8
};
class C3DMesh2;
struct CCountedMeshPointer {                       // CCountedPointer<const C3DMesh2>
    C3DMesh2* Object;
    CRefInfo* Info;
    CCountedMeshPointer() : Object(0), Info(0) { }
    ~CCountedMeshPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->Destroy(Info->Object);
                operator delete(Info);
            }
        }
    }
};
struct CMeshMaterial {                             // 0x30 bytes
    unsigned char _pad00[0x10];
    unsigned long TextureIndex;                    // +0x10
    unsigned char _pad14[0x30 - 0x14];
};
struct CSubMesh {                                  // 0xa0 bytes
    unsigned char _pad00[4];
    long          MaterialIndex;                   // +0x04
    unsigned char _pad08[0x98];
};
class C3DMesh2 { public:
    unsigned char  _pad00[0x8c];
    CMeshMaterial* Materials;                      // +0x8c
    unsigned long  SubMeshCount;                   // +0x90
    CSubMesh*      SubMeshes;                      // +0x94
};
struct CMeshResources {
    unsigned char _pad[0x3d4];
    CGraphicBank* GraphicBank;                     // +0x3d4
};
class C3DMeshInfo { public:
    void*           _vtbl;
    CMeshResources* Resources;                     // +0x04
    unsigned char   _pad08[0x44 - 0x08];
    float           LODDistance[4];                // +0x44
    void*           LODMesh[4];                    // +0x54
    bool GetMesh(CCountedMeshPointer& mesh, unsigned long lod, unsigned long flags) const;   // 0x00A25E70
};
struct CEngineRenderSettings {
    unsigned char _pad00[0x9c];
    float         LODScale;                        // +0x9c
    unsigned char _pada0[0xa8 - 0xa0];
    float         FieldOfViewScale;                // +0xa8
};
extern CEngineRenderSettings* g_pEngineRenderSettings;   // 0x01436E78
class CEngineInternalPrimitiveBase;
class CEngineInternalPrimitiveRepeatedStaticMesh { public:
    unsigned char _pad00[0x50];
    C3DMeshInfo*  MeshInfo;                        // +0x50
    unsigned char _pad54[0x70 - 0x54];
    C3DVector     Centre;                          // +0x70
    float         Radius;                          // +0x7c
};
FABLE_FORCEINLINE bool IsBelowLODCount(unsigned long lod)
{
    if (lod >= 3)
        return false;
    return true;
}

class CEnginePrimitiveManagerRepeatedStaticMeshes { public:
    virtual bool PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags);
};

bool CEnginePrimitiveManagerRepeatedStaticMeshes::PreloadPrimitive(CEngineInternalPrimitiveBase& primitive, const C3DVector& position, unsigned long flags)
{
    CEngineInternalPrimitiveRepeatedStaticMesh& mesh = (CEngineInternalPrimitiveRepeatedStaticMesh&)primitive;
    CEngineRenderSettings* settings = g_pEngineRenderSettings;
    float dx = mesh.Centre.X - position.X;
    float lodScale = settings->LODScale;
    float dy = mesh.Centre.Y - position.Y;
    C3DMeshInfo* info = mesh.MeshInfo;
    float dz = mesh.Centre.Z - position.Z;
    float radius = mesh.Radius;
    float distance = (float)sqrt(dx * dx + dy * dy + dz * dz) - radius;
    distance = distance > 0.0001f ? distance : 0.0001f;
    float ratio = distance / (radius * settings->FieldOfViewScale) * lodScale;
    unsigned long lod;
    for (lod = 0; IsBelowLODCount(lod); ++lod) {
        if (!info->LODMesh[lod] || info->LODDistance[lod] > ratio)
            break;
    }
    CCountedMeshPointer meshPointer;
    if (info->GetMesh(meshPointer, lod, 0x10)) {
        bool started = false;
        unsigned long count = meshPointer.Object->SubMeshCount;
        for (unsigned long i = 0; i < count; ++i) {
            CGraphicBank* bank = mesh.MeshInfo->Resources->GraphicBank;
            unsigned long texture = meshPointer.Object->Materials[meshPointer.Object->SubMeshes[i].MaterialIndex].TextureIndex;
            if (texture && bank->GetFrame(texture)->RequestLoad(0, 0))
                started = true;
        }
        return started;
    }
    return true;
}