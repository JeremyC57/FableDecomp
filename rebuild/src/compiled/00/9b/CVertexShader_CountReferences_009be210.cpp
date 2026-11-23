// Leaf: ?CountReferences@CVertexShader@@EAEKXZ
// Switch route (NF318, 2026-11-23): defined on the real class of engine/CVertexShader.h (class-model switch FABLE_RETAIL_CVERTEXSHADER; was ?CountReferences@CVertexShader_Methods@@QAEKXZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member at this address. Bytes unchanged.
// Preamble: the define / include block of the landed precedent CEnginePrimitiveManagerAnimatedMeshesBase_CEnginePrimitiveManagerAnimatedMeshesBase_00bcff10.cpp.
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CDISPLAYMANAGER_PIXELSHADERVERSION
#include "engine/Direct3D9Types.h"
#include "engine/CEnginePrimitiveManagerAnimatedMeshesBase.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CShaderResource.h"
#include "engine/CDisplayManager.h"
#include "engine/CVertexShader.h"  // retyped onto the PDB layout; byte parity re-verified
unsigned long CVertexShader::CountReferences() { return Data->ReferenceCount; }   // CVertexShaderData::ReferenceCount +0x10
