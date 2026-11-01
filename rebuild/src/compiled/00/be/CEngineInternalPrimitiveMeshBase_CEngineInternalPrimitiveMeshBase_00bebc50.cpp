// CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase() -- clear the simulation info's simulations, release the MeshEffect and SimulationInfo intrusive pointers, then the CEngineInternalPrimitiveOcclusionBase base destructor
// Leaf: ??1CEngineInternalPrimitiveMeshBase@@UAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BEBC50, 77 bytes, thiscall, tail `jmp ~CEngineInternalPrimitiveOcclusionBase`.
// Identity (proven): stores vtable 0x012A41D4 -- RTTI .?AVCEngineInternalPrimitiveMeshBase@@ : CEngineInternalPrimitiveOcclusionBase
//   (rtti-vtables.tsv / rtti_bases.tsv), whose slot-0 `??_G` 0x00BA7FF0 calls this body; the other callers are the derived
//   classes' ??_G (0x00BA80F0, 0x00BA81B0, 0x00BA8210, 0x00BAA380) and implicit dtors (CEngineInternalPrimitiveAnimatedMesh
//   0x00BD9250 tail jump, landed binding ??1CEngineInternalPrimitiveMeshBase@@UAE@XZ). Ego_r.pdb (0x00AA3E50) and FableWin.pdb
//   (0x02EE37A0) spell ??1CEngineInternalPrimitiveMeshBase@@UAE@XZ.
// Callees: CEnginePrimitiveSimulationInfo::Clear 0x00B81E20 (?Clear@CEnginePrimitiveSimulationInfo@@QAEXXZ: Ego_r / FableWin
//   name; Ego_r places it right after the ctor ??0CEnginePrimitiveSimulationInfo 0x00A29F90 -> 0x00A29FB0, retail ctor 0x00B81E00
//   -> 0x00B81E20; this is its only caller), the pointees' slot-1 Delete through the vtable, and
//   CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase 0x00B9C660 (Ego_r ??1..UAE@XZ 0x00A542F0).
// Shape: user destructor `if (!SimulationInfo.IsNull()) SimulationInfo->Clear();` (the vtable store is scheduled between the
//   load and the branch); then the members in reverse order: MeshEffect (+0x68) and SimulationInfo (+0x64) run the inline
//   CIVCountedPointer<T>::Release (Object copied into a local, `dec [obj+4]`, Delete at zero, Object zeroed on the non-null
//   path; chunk 696 0x00A69E20). ThemeChannelDesc (+0x6C, four bytes) is trivially destroyed.
// Layout: Ego_r.pdb CEngineInternalPrimitiveMeshBase (0x70): base CEngineInternalPrimitiveOcclusionBase 0x50, MeshInfo +0x50,
//   ShadowMeshInfo +0x54, Scale +0x58, TintColour +0x5C, BulletTimeAffectsCloth +0x60, SimulationInfo +0x64, MeshEffect +0x68,
//   ThemeChannelDesc +0x6C. Local polymorphic models: engine/CEngineInternalPrimitiveBase.h, CEnginePersistentPrimitiveBase.h and
//   CEngineInternalPrimitiveOcclusionBase.h flatten the vptr into a data member (header debt); CIVCountedPointeeBase is the
//   shared retail model (FABLE_RETAIL_DEF_CLASS_BASE).
#include "rebuild_abi.h"
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CIVCountedPointeeBase.h"

class C3DMeshInfo;

template <class T> class CIVCountedPointer {
public:
    ~CIVCountedPointer() { Release(); }
    T* operator->() const { return static_cast<T*>(Object); }
    bool IsNull() const { return Object == 0; }
    void Release()
    {
        CIVCountedPointeeBase* object = Object;
        if (object) {
            if (object->Release() == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointeeBase* Object;                       // +0x00
};

class CEnginePrimitiveSimulationInfo : public CIVCountedPointeeBase {
public:
    void Clear();                                        // 0x00B81E20
    unsigned char Simulations[0xc];                      // +0x08 CArray<CCountedPointer<CEngineSimulationBase> > (retail 12 bytes)
};

class CEngineMeshEffect : public CIVCountedPointeeBase {
public:
    long MeshEffect;                                     // +0x08 EEngineMeshEffect
};

class CRGBColour {
public:
    unsigned char B, G, R, A;
};

class CEnginePrimitiveThemeChannelDesc {
public:
    unsigned char ChannelIndex0, ChannelIndex1, ChannelBlendFactor, Pad;
};

class CEnginePersistentPrimitiveBase : public CIVCountedPointeeBase {
public:
    long Type;                                           // +0x08
};

class CEngineInternalPrimitiveBase : public CEnginePersistentPrimitiveBase {
public:
    unsigned char _body[0x48 - 0xc];                     // +0x0C EnginePrimitiveFlags .. RefLayerMask (engine header)
};

class CEngineInternalPrimitiveOcclusionBase : public CEngineInternalPrimitiveBase {
public:
    virtual ~CEngineInternalPrimitiveOcclusionBase();    // 0x00B9C660
    CIVCountedPointer<CIVCountedPointeeBase> OcclusionInfo[2];   // +0x48 CIVCountedPointer<CEnginePrimitiveOcclusionInfo>[2]
};

class CEngineInternalPrimitiveMeshBase : public CEngineInternalPrimitiveOcclusionBase {   // vtable 0x012A41D4, 0x70 bytes
public:
    virtual ~CEngineInternalPrimitiveMeshBase();
    C3DMeshInfo* MeshInfo;                               // +0x50
    C3DMeshInfo* ShadowMeshInfo;                         // +0x54
    float Scale;                                         // +0x58
    CRGBColour TintColour;                               // +0x5C
    bool BulletTimeAffectsCloth;                         // +0x60
    CIVCountedPointer<CEnginePrimitiveSimulationInfo> SimulationInfo;   // +0x64
    CIVCountedPointer<CEngineMeshEffect> MeshEffect;     // +0x68
    CEnginePrimitiveThemeChannelDesc ThemeChannelDesc;   // +0x6C
};

CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase()
{
    if (!SimulationInfo.IsNull())
        SimulationInfo->Clear();
}