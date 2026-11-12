// NGraphicAppearance::CTCGraphicAppearance::IsResourcesLoaded(bool begin_loading) const -- whether every mesh the appearance's
// components use is loaded; optionally start loading the missing ones.
// Leaf: ?IsResourcesLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_N_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004BE3E0, 173 bytes, `ret 4`.
// Identity: Ego_r.pdb / FableWin.pdb ?IsResourcesLoaded@CTCGraphicAppearance@NGraphicAppearance@@QBE_N_N@Z (engine header HELPERNAMES).
// Callees: vector<CMeshInfo>::reserve 0x004C46C0, each component's virtual GetMeshesUsed (slot 2, +0x08) -- through
//   GetAllMeshes (landed 0x004BD420), expanded here --, CMeshDataBank::PeekMeshInfo 0x00A24260 on g_pMeshDataBank 0x013B8A04,
//   C3DMeshInfo::IsLoaded 0x00A23E40 / BeginLoading 0x00A256F0 -- through IsResourcesLoadedForMesh (Ego_r.pdb
//   ?IsResourcesLoadedForMesh@CTCGraphicAppearance@NGraphicAppearance@@IBE_NABVCMeshInfo@2@_N@Z; its out-of-line retail copy is
//   0x004BC090), expanded here --, free 0x00BFEA14 (the vector's storage).
// Real bodies in this TU: GetAllMeshes (landed 0x004BD420) and IsResourcesLoadedForMesh (0x004BC090, not yet landed: the same
//   bytes as that copy).
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPER_BODY
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HELPERNAMES
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMESHDATABANK_PEEKINFO
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#include "engine/ETCInterfaceType.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NGraphicAppearance_CAGraphicAppearanceComponent.h"
#include "engine/NGraphicAppearance_CMeshInfo.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
extern CMeshDataBank* g_pMeshDataBank;                                   // 0x013B8A04

namespace NGraphicAppearance {

inline void CTCGraphicAppearance::GetAllMeshes(std::vector<CMeshInfo>& meshes) const
{
    meshes.reserve(4);
    for (std::list<CAGraphicAppearanceComponent*>::const_iterator it = Components.begin(); it != Components.end(); ++it)
        (*it)->GetMeshesUsed(meshes);
}

inline bool CTCGraphicAppearance::IsResourcesLoadedForMesh(const CMeshInfo& mesh, bool begin_loading) const
{
    const C3DMeshInfo* mesh_info = g_pMeshDataBank->PeekMeshInfo(mesh.Index);
    bool loaded = mesh_info->IsLoaded();
    if (!loaded && begin_loading)
        mesh_info->BeginLoading((unsigned long)-1, false, false);
    return loaded;
}

bool CTCGraphicAppearance::IsResourcesLoaded(bool begin_loading) const
{
    std::vector<CMeshInfo> meshes;
    GetAllMeshes(meshes);
    bool loaded = true;
    for (std::vector<CMeshInfo>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
        if (!IsResourcesLoadedForMesh(*it, begin_loading))
            loaded = false;
    return loaded;
}

}