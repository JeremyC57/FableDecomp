#ifndef FABLE_ENGINE_NGRAPHICAPPEARANCE_CMESHINFO_H
#define FABLE_ENGINE_NGRAPHICAPPEARANCE_CMESHINFO_H
#include "rebuild_abi.h"
// Original complete public12-byte mesh entry, both PDBs and native helper iterator stride12.
namespace NGraphicAppearance { class CMeshInfo { public: long Index;float SizeX;unsigned char PrimaryMesh:1; }; }
FABLE_STATIC_ASSERT(sizeof(NGraphicAppearance::CMeshInfo)==12);
// Original global comparator classes, both PDBs; the real sort COMDATs call them.
class CSortPrimaryMeshesFirst { public: bool operator()(const NGraphicAppearance::CMeshInfo& lhs,const NGraphicAppearance::CMeshInfo& rhs) { return lhs.PrimaryMesh; } };
class CSortPrimaryMeshesLast { public: bool operator()(const NGraphicAppearance::CMeshInfo& lhs,const NGraphicAppearance::CMeshInfo& rhs) { return !lhs.PrimaryMesh; } };
#endif
