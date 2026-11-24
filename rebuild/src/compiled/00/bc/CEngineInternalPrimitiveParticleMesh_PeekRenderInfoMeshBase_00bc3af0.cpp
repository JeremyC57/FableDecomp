// Leaf: ?PeekRenderInfoMeshBase@CEngineInternalPrimitiveParticleMesh@@UBEPBVCEngineRenderInfoMeshBase@@XZ
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CEngineInternalPrimitiveParticleMesh.h under FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEMESH_CLASS with the PDB parameter / return types (was ?PeekRenderInfoMeshBase@CEngineInternalPrimitiveParticleMesh_Methods@@QAEPAVCEngineLocalLightArray@@XZ);
//   the retail RTTI vftable placement (ghidra_out/labels_rtti_port.tsv) names this PDB member here.
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEPARTICLEMESH_CLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#include "stlport/fable_map.h"
#include "engine/CEnginePrimitiveParticleMesh.h"
#include "engine/CEngineInternalPrimitiveParticleMesh.h"
const CEngineRenderInfoMeshBase* CEngineInternalPrimitiveParticleMesh::PeekRenderInfoMeshBase() const { return (const CEngineRenderInfoMeshBase*)&this->RenderInfo_Lights; }   // the header flattens the CEngineRenderInfoMeshBase member at +0x7C into its words
