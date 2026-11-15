#ifndef FABLE_ENGINE_CREPLACEABLEMESHDEF_H
#define FABLE_ENGINE_CREPLACEABLEMESHDEF_H
#ifdef FABLE_RETAIL_CREPLACEABLEMESHDEF_PASS22
#include "engine/CSubDefClassBase.h"
#include "engine/CEngineGraphic.h"
#include "engine/CArray.h"
namespace NGraphicAppearance { class CReplaceableMeshes {public:CArray<CEngineGraphic> Vector;}; }
class CReplaceableMeshDef:public CSubDefClassBase {public:NGraphicAppearance::CReplaceableMeshes Meshes;};
FABLE_STATIC_ASSERT(offsetof(CReplaceableMeshDef,Meshes)==0x28);
FABLE_STATIC_ASSERT(sizeof(CReplaceableMeshDef)==0x34);
#endif
#endif
// Retail opt-in (Codex pass22): both original TPI types; native def Meshes.Vector at28,retail12-byte CArray.
