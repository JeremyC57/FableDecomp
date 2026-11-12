// CTCPhysicsStandard::SetPhysicsMeshBankIndex(long index) -- index 0 drops the physics mesh info (radius recomputed, zero offset,
//   no navigation layers); otherwise (re)fills the CThingPhysicsMeshInfo from the bank's physics mesh: the full render scale, the
//   bounding-box centre rotated into the body's orientation and scaled by RenderSizeX * 0.01f, the bounding sphere / circle radii,
//   then the offset, radius and navigation-layer flag; finally re-places the thing in its mapwho if it is in one.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00724630, 741 bytes, thiscall `ret 4`; vtable 0x01265F84 slot 85.
// Callees: operator delete 0x00BFE9BC, operator new 0x00BFEA1A, CTCPhysicsBase::CalculateRadius 0x006AFE20,
//   NGraphicAppearance::CTCGraphicAppearance::GetFullScale 0x004BBFF0, CTCBase::PeekWorld 0x00686A80,
//   CMeshDataBank::PeekPhysicsMeshInfo 0x00A24250, CPhysicsMeshInfo::BeginLoading 0x00A25BD0, CPhysicsMeshInfo::GetMesh 0x00A262B0 (x4),
//   C3DMeshObject::GetPositionMatrixFromRHSet 0x00A358F0, CVectorMap LowerBound 0x0040F020, CTCMapwho::RemoveFromMapwho 0x0071A730,
//   CTCPhysicsBase::GetMapwhoPosition 0x006AFCB0, CTCMapwho::PlaceInMapwho 0x0071A710. 0.5f 0x0122F59C, 0.01f 0x01230A5C.
// Leaf: ?SetPhysicsMeshBankIndex@CTCPhysicsStandard@@UAEXJ@Z
// Flags: /EHsc
// Shape: the zero branch frees the info with a plain `operator delete(PPhysicsMeshInfo)` call (no delete-expression): with the
//   delete-expression VC7.1 emits the same call but loads the bounding-box centre X pair Min first (0x007246C2); this spelling gives
//   retail's Max-first `fld [eax+0x90] ; add eax,0x84 ; fadd [eax]`. The offset is zeroed with C3DVector::Initialise (keeps the
//   `and byte [esi+0x85],0xef` read-modify-write after the three stores).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCPHYSICSBASE_CALCULATERADIUS
#define FABLE_RETAIL_CTCPHYSICSSTANDARD
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_OVERRIDES
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_FLAGS
#define FABLE_RETAIL_CTCPHYSICSSTANDARD_SETPHYSICSMESHBANKINDEX
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_FULLSCALE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_MESH_BANK
#define FABLE_RETAIL_CWORLD_PEEKMESHBANK
#define FABLE_RETAIL_CMESHDATABANK_PEEKPHYSICSMESHINFO
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_CPHYSICSMESH_BOUNDS
#define FABLE_RETAIL_CPHYSICSMESH_NAVIGATION
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_DTOR_DIRECT
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_C3DMESHOBJECT_POSITIONMATRIXFROMRHSET
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_C3DVECTOR
class CTCMapwho;
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPhysicsStandard.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/CTCMapwho.h"
#include "engine/CWorld.h"
#include "engine/CMeshDataBank.h"
#include "engine/CPhysicsMesh.h"
#include "engine/CIVCountedPointer.h"
#include "engine/CMatrix3x4.h"
#include "engine/C3DMeshObject.h"
#include "engine/CCircle.h"

class CThingPhysicsMeshInfo {                       // Ego_r CThingPhysicsMeshInfo, 0x1c bytes
public:
    unsigned short BankIndex;                       // +0x00
    float          RenderSizeX;                     // +0x04
    C3DVector      ToCentre;                        // +0x08
    float          BoundingRadius;                  // +0x14
    float          BoundingCircleRadius;            // +0x18
};
class CPhysicsMeshInfo {
public:
    void BeginLoading() const;                                       // 0x00A25BD0
    CIVCountedPointer<const CPhysicsMesh> GetMesh() const;           // 0x00A262B0
};

void CTCPhysicsStandard::SetPhysicsMeshBankIndex(long index)
{
    if (index == 0) {
        operator delete(PPhysicsMeshInfo);
        PPhysicsMeshInfo = 0;
        Radius = CalculateRadius();
        OffsetVector3D.Initialise(0.0f, 0.0f, 0.0f);
        NavigationLayers = false;
    } else {
        if (!PPhysicsMeshInfo)
            PPhysicsMeshInfo = new CThingPhysicsMeshInfo;
        PPhysicsMeshInfo->BankIndex = (unsigned short)index;
        PPhysicsMeshInfo->RenderSizeX = ParentThing->GetTCGraphicAppearance()->GetFullScale();
        const CMeshDataBank* bank = PeekWorld().PMainMeshBank_Data;
        const CPhysicsMeshInfo* meshInfo = bank->PeekPhysicsMeshInfo(index);
        meshInfo->BeginLoading();
        C3DVector centre = meshInfo->GetMesh()->GetBoundingBox().GetCenter();
        C3DMeshObject::GetPositionMatrixFromRHSet(RHSet).TransformVector(centre);
        PPhysicsMeshInfo->ToCentre = centre *= PPhysicsMeshInfo->RenderSizeX * 0.01f;
        PPhysicsMeshInfo->BoundingRadius = meshInfo->GetMesh()->GetBoundingSphere().Radius * PPhysicsMeshInfo->RenderSizeX * 0.01f;
        PPhysicsMeshInfo->BoundingCircleRadius = PPhysicsMeshInfo->RenderSizeX * meshInfo->GetMesh()->GetBoundingCircle().Radius * 0.01f;
        OffsetVector3D = PPhysicsMeshInfo->ToCentre;
        Radius = CalculateRadius();
        NavigationLayers = !meshInfo->GetMesh()->GetNavigationContainmentVolumes().empty();
    }
    CTCMapwho* mapwho;
    ParentThing->GetTC(TCI_MAPWHO, &mapwho);
    if (mapwho->IsInMap()) {
        mapwho->RemoveFromMapwho();
        mapwho->PlaceInMapwho(GetMapwhoPosition());
    }
}