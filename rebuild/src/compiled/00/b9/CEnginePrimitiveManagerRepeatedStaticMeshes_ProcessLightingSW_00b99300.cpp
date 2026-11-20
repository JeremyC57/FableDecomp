// CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW(CEngineInternalPrimitiveRepeatedStaticMesh& primitive, const float* normals_x, const float* normals_y, const float* normals_z, const C4DVector* object_offsets, long count, bool shadowed) -- software-light a batch of repeated-mesh instances into the SIMD variable block (the SSE batch lighter when available, count rounded up to 4; else one lighting call per instance) and upload the results: shadowed meshes light the main light separately (the debug statics bb / aa can force 16 entries / a red main light) and upload it too
// Leaf: ?ProcessLightingSW@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXAAVCEngineInternalPrimitiveRepeatedStaticMesh@@PBM11PBVC4DVector@@J_N@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B99300, 871 bytes, `ret 1Ch` (no frame pointer, 0x20 locals).
// Identity: Ego_r.pdb / Ego_d.pdb / FableWin.pdb ?ProcessLightingSW@CEnginePrimitiveManagerRepeatedStaticMeshes@@AAEXAAVCEngineInternalPrimitiveRepeatedStaticMesh@@PBM11PBVC4DVector@@J_N@Z
//   (private; the shared header's FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_RENDERSUB declaration; caller = landed
//   RenderSubPrimitive 0x00B9AC30). Locals and parameter names = Ego_d.pdb (debug length 837): primitive, normals_x, normals_y,
//   normals_z, object_offsets, count, shadowed, aligned_count, use_simd; { obj { pos, normal } }; { n }; { obj { pos, normal } }.
//   Inline accessors (audit h3c2b): Ego_d emits CEngineInternalPrimitiveRepeatedStaticMesh::PeekLights (21 debug bytes) and
//   PeekRenderInfo (19) right after this function in Engine_Primitive_Manager_Repeated_Meshes.obj (first user), and each of the four
//   lighting lines (261 / 271 / 300 / 309) is exactly 1 byte shorter in Ego_d than this text compiled /Od with a direct member read
//   (`call` + `push eax` v `mov r,[p+0A4h]` + `push r`): the light array is `primitive.PeekLights()`. The constant uploads go through
//   the PUBLIC inline CShaderRenderManager::SetVertexShaderConstantsDirect (FableWin.pdb ?SetVertexShaderConstantsDirect@
//   CShaderRenderManager@@QAEXKPBXK@Z; Ego_d: 33 debug bytes, one line = the three arguments forwarded to the private SetVSConstants).
//   Retail keeps this TU's unreferenced COMDAT copies of all three: PeekRenderInfo 0x00B98BA0, PeekLights 0x00B98BB0 and
//   SetVertexShaderConstantsDirect 0x00B9C060 (`jmp 0x0098B930`); this row's object emits the same bytes for each.
// Shape: Ego_d.pdb's C13 line table (ref/00b99300/egod_lines.tsv, engine_primitive_manager_repeated_meshes.cpp 246..315): aligned_count
//   (12 bytes) then use_simd (9), the shadowed / SSE tests, per-instance pos (58) then normal (47) then the call, the two debug switches,
//   the main-light colour written with one Initialise call (51 bytes: four float pushes + call; a CRGBFloatColour temporary copy is the
//   prior's 935-byte form), the colour upload after the if/else (line 314). Line 289 (2 bytes) is the `n` loop's closing brace
//   (its back jump): the loop is braced.
// Callees: CEngineLightingManager::CalcSWLightingNoClipSeparateMainLightVector 0x00B44A60 / CalcSWLightingNoClipSeparateMainLight
//   0x00B45A40 / CalcSWLightingNoClipVector 0x00B44820 / CalcSWLightingNoClip 0x00B45CF0 on NEngineInternal::GPLightingManager
//   0x01436E9C; CShaderRenderManager::SetVSConstants 0x0098B930 (private; reached through the public inline
//   SetVertexShaderConstantsDirect) on GPShaderRenderManager 0x01436E14; registers = GPResourceManager 0x01436E98
//   PeekVSConstantLayoutRepeatedMesh() LightingResults (+0x3018) / MainLightLightingResults (+0x3024). Data: GEnableSIMD 0x013D2880;
//   the two debug switches are Ego_d.pdb's S_LDATA32 `aa` / `bb` (type int; the only two data statics of
//   Engine_Primitive_Manager_Repeated_Meshes.obj, section 4 offsets 582556 / 582560, adjacent like retail's 0x0143B954 / 0x0143B958,
//   which Fable.exe reads only here, 0x00B9944C / 0x00B99438, and never writes). TU-local statics, modelled `extern` under the PDB
//   name like 0x013D27B0 NumberOfPeopleCalculatingRoute. Which is which: VC7.1 lays file statics out in declaration order whatever
//   the use order (measured, audit probes s1..s4) and Ego_d has aa below bb, so aa = 0x0143B954 (the red main light) and bb =
//   0x0143B958 (the 16-entry upload).
// Overlays (today's headers + opt-ins): engine/CEngineLightingManager.h FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_VECTOR (the
//   two batch lighters, Ego_d parameter names); engine/CShaderRenderManager.h
//   FABLE_RETAIL_CSHADERRENDERMANAGER_SETVERTEXSHADERCONSTANTSDIRECT_INLINE (the wrapper's body);
//   engine/CEngineInternalPrimitiveRepeatedStaticMesh.h FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH_PEEKLIGHTS.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_RENDERSUB
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_RENDERSUB_NESTEDCLIP
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGEROCCLUSIONBASE_PREPARETEST
#define FABLE_RETAIL_CENGINESUBPRIMITIVEREPEATEDSTATICMESH_GETPARENT
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER_TESTS
#define FABLE_RETAIL_CENGINEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PEEKRENDERFLAGS
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_PEEKVSCONSTANTLAYOUTREPEATEDMESH
#define FABLE_RETAIL_CENGINEVSCONSTANTLAYOUTREPEATEDMESH
#define FABLE_RETAIL_CVERTEXSHADERCONSTANTLAYOUT
#define FABLE_RETAIL_CENGINESUBPRIMITIVEBASE_PARENTBASE
#define FABLE_RETAIL_CENGINERESOURCEMANAGER_GETSTIPLEFADETEXTURE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETTEXTURE_INLINE
#define FABLE_RETAIL_CGRAPHICINFOFRAME_TOUCHFRAME
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETBANK_DIRECT
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING
#define FABLE_RETAIL_CFRAMEDATA_ISLOADING_ISNULL
#define FABLE_RETAIL_CGRAPHICINFOFRAME_GETFRAME
#define FABLE_RETAIL_CGRAPHICDATABANK_PEEKGRAPHIC
#define FABLE_RETAIL_CRESOURCE_TOUCHRESOURCE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CGRAPHICFRAME
#define FABLE_RETAIL_CGRAPHICFRAME_GETTEXTURE
#define FABLE_RETAIL_CRENDERMANAGERCORE
#define FABLE_RETAIL_CRENDERMANAGERCORE_FORCEINLINE_HELPERS
#define FABLE_RETAIL_CRESOURCEBANK_CURRENTFRAME_DIRECT
#define FABLE_RETAIL_CMANAGEDTEXTURE_TOUCHTEXTURE_GET
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_GET
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWMODE_DIRECT
#define FABLE_RETAIL_C3DMESHINFO_PEEKTEXTUREBANK_DIRECT
#define FABLE_RETAIL_CRENDERSTATECAPTUREBLOCK_INLINE
#define FABLE_RETAIL_CRENDERSTATEMANAGER
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SELECTVERTEXBUFFER
#define FABLE_RETAIL_CSHADERRENDERMANAGER_INLINE_BLOCKDRAW
#define FABLE_RETAIL_CENGINERESOURCEMANAGER
#define FABLE_RETAIL_C3DMESH2_PRIMITIVES
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_C3DPRIMITIVE2_INDEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_VERTEXBUFFER
#define FABLE_RETAIL_C3DPRIMITIVE2_PARTICLEMESH
#define FABLE_RETAIL_C3DMESHMATERIAL_PARTICLEMESH
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CINDEXBUFFER
#define FABLE_RETAIL_CVERTEXBUFFERWIN32
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ISPERSISTENT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_ISOCCLUDED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE_SETUP_FORCEINLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_STATICBLOCKS_ORIGINAL_ACCESSORS
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_SUBPRIMITIVE
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGER_BLOCKONLOAD
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_DISTANCE
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERUTIL_MESHRENDER
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERERSHAREDDATA_RENDERSETTINGS
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_SET
#define FABLE_RETAIL_CENGINEPRIMITIVEOCCLUSIONMANAGER_OCCLUDED
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADD
#define FABLE_RETAIL_CENGINESUBPRIMITIVERENDERER_ADDTYPED
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_ISNULL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_COUNT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CASTTOBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CGRAPHICDATABANK_BANKFILE
#define FABLE_RETAIL_CMESHDATABANK_TEXTUREBANK
#define FABLE_RETAIL_CBANKFILE_ENTRYDATASIZE
#define FABLE_RETAIL_C3DMESHMATERIAL_FLAG_ACCESSORS
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_GETMESH
#define FABLE_RETAIL_C3DMESHINFO_ISLOADED
#define FABLE_RETAIL_C3DMESHINFO_DETAILLEVEL
#define FABLE_RETAIL_C3DMESHINFO_BEGINLOADINGTEXTURES
#define FABLE_RETAIL_C3DMESHINFO_LODERROR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_PEEKWINDOW
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_CENGINESUBPRIMITIVEMESHBASE_PUBLICMEMBERS
#define FABLE_RETAIL_CENGINECAMERADESC_STRUCT
#define FABLE_RETAIL_CSHADOWCLIPVOLUME_CLIPBOUNDINGBOX
#define FABLE_RETAIL_CSHADOWRENDERERSCENEDATA_COPY
#define FABLE_RETAIL_CENGINESHADOWRENDERER
#define FABLE_RETAIL_CENGINESHADOWRENDERER_MEMBERS
#define FABLE_RETAIL_CENGINESHADOWRENDERER_PEEKSCENEDATA
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_MEMBERS
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_SETFADED_SHADOWGUARD
#define FABLE_RETAIL_CVERTEXSHADER
#define FABLE_RETAIL_C4DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTINGNOCLIP
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_SEPARATE_MAIN
#define FABLE_RETAIL_CENGINELIGHTINGMANAGER_CALCSWLIGHTING_VECTOR
#define FABLE_RETAIL_CSHADERRENDERMANAGER_SETVERTEXSHADERCONSTANTSDIRECT_INLINE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEREPEATEDSTATICMESH_PEEKLIGHTS
#include "fable_compiler.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineRenderEnums.h"
#include "engine/EEngineOcclusionLayer.h"
#include "engine/CEnginePrimitiveRenderUtil.h"
#include "engine/CEnginePrimitiveManager.h"
#include "engine/CShadowClipVolume.h"
#include "engine/CShadowRendererSceneData.h"
#include "engine/CEngineInternalPrimitiveBase.h"
#include "engine/CEngineInternalPrimitiveOcclusionBase.h"
#include "engine/CEngineInternalPrimitiveRepeatedStaticMesh.h"
#include "engine/CEnginePrimitiveManagerRepeatedStaticMeshes.h"
#include "engine/CEnginePrimitiveRendererSharedData.h"
#include "engine/CEnginePrimitiveRenderer.h"
#include "engine/CEnginePrimitiveOcclusionInfo.h"
#include "engine/CEnginePrimitiveOcclusionManager.h"
#include "engine/CEngineSubPrimitiveRenderer.h"
#include "engine/CEngineSubPrimitiveBoundingBox.h"
#include "engine/CEngineSubPrimitiveRepeatedStaticMesh.h"
#include "engine/CEnginePrimitiveThemeChannelDesc.h"
#include "engine/C3DMeshInfo.h"
#include "engine/C3DMesh2.h"
#include "engine/CCountedPointer.h"
#include "engine/CEngineShadowRenderer.h"
#include "engine/CEngineLightingManager.h"
#include "engine/CRenderManager.h"
#include "engine/CShaderRenderManager.h"
#include "engine/CRenderStateCaptureBlock.h"
#include "engine/CScopedVertexShaderConstantLayout.h"
#include "engine/CEngineOcclusionManager.h"
#include "engine/CEngineResourceManager.h"
#include "engine/CEngineVSConstantLayoutRepeatedMesh.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CGraphicInfo.h"
#include "engine/CGraphicInfoFrame.h"
#include "engine/C3DPrimitive2.h"
namespace NEngineInternal { extern CShaderRenderManager* GPShaderRenderManager; }      // 0x01436E14
namespace NEngineInternal { extern CEngineResourceManager* GPResourceManager; }        // 0x01436E98
namespace NEngineInternal { extern CEngineLightingManager* GPLightingManager; }        // 0x01436E9C
extern bool GEnableSIMD;                                                               // 0x013D2880 (Ego_d.pdb ?GEnableSIMD@@3_NA)
extern int aa;                                                                         // 0x0143B954 (Ego_d.pdb S_LDATA32 int aa, this TU's static)
extern int bb;                                                                         // 0x0143B958 (Ego_d.pdb S_LDATA32 int bb, this TU's static)

void CEnginePrimitiveManagerRepeatedStaticMeshes::ProcessLightingSW(CEngineInternalPrimitiveRepeatedStaticMesh& primitive,
                                                                    const float* normals_x, const float* normals_y,
                                                                    const float* normals_z, const C4DVector* object_offsets,
                                                                    long count, bool shadowed)
{
    long aligned_count = (count + 3) & ~3;
    const bool use_simd = GEnableSIMD;
    if (shadowed)
    {
        if (use_simd)
            NEngineInternal::GPLightingManager->CalcSWLightingNoClipSeparateMainLightVector(SIMDVariableBlock->Colour,
                SIMDVariableBlock->MainLightColour, object_offsets, normals_x, normals_y, normals_z, aligned_count,
                primitive.PeekLights());
        else
        {
            for (long obj = 0; obj < count; ++obj)
            {
                C3DVector pos(object_offsets[obj].X, object_offsets[obj].Y, object_offsets[obj].Z);
                C3DVector normal(normals_x[obj], normals_y[obj], normals_z[obj]);
                NEngineInternal::GPLightingManager->CalcSWLightingNoClipSeparateMainLight(pos, normal, primitive.PeekLights(),
                    SIMDVariableBlock->MainLightColour[obj], SIMDVariableBlock->Colour[obj]);
            }
        }
        if (bb)
            count = 16;
        if (aa)
        {
            for (long n = 0; n < count; ++n)
            {
                SIMDVariableBlock->MainLightColour[n].Initialise(1.0f, 0.0f, 0.0f, 0.0f);
            }
        }
        NEngineInternal::GPShaderRenderManager->SetVertexShaderConstantsDirect(
            NEngineInternal::GPResourceManager->PeekVSConstantLayoutRepeatedMesh().MainLightLightingResults.Offset,
            SIMDVariableBlock->MainLightColour, count);
    }
    else
    {
        if (use_simd)
            NEngineInternal::GPLightingManager->CalcSWLightingNoClipVector(SIMDVariableBlock->Colour, object_offsets, normals_x,
                normals_y, normals_z, aligned_count, primitive.PeekLights());
        else
        {
            for (long obj = 0; obj < count; ++obj)
            {
                C3DVector pos(object_offsets[obj].X, object_offsets[obj].Y, object_offsets[obj].Z);
                C3DVector normal(normals_x[obj], normals_y[obj], normals_z[obj]);
                NEngineInternal::GPLightingManager->CalcSWLightingNoClip(pos, normal, primitive.PeekLights(),
                    SIMDVariableBlock->Colour[obj]);
            }
        }
    }
    NEngineInternal::GPShaderRenderManager->SetVertexShaderConstantsDirect(
        NEngineInternal::GPResourceManager->PeekVSConstantLayoutRepeatedMesh().LightingResults.Offset, SIMDVariableBlock->Colour, count);
}