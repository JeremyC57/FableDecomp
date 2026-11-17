// CEngineInternalPrimitive2DStaticMesh::Initialise() -- place a 2D static mesh: the clip box centre at the layer's view distance taken from screen to world space through the engine camera, and a scale that fits the bounding sphere to the smaller half extent of the clip box at that distance
// Leaf: ?Initialise@CEngineInternalPrimitive2DStaticMesh@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B9E120, 365 bytes, plain `ret`.
// Identity: Ego_r.pdb 0x00A55450 / FableWin.pdb 0x02E98120 (the leaf above). Ego_d 2D static mesh TU: const float locals view_z,
//   half_clip_box_width, half_clip_box_height, half_clip_square_width -- the names of the landed twin
//   CEngineInternalPrimitive2DAnimatedMesh::Initialise 0x00BCF4F0, whose statements this row repeats in its own order (WorldPos placed
//   and transformed first, then Scale; Initialised set before the radius divide).
// Class: overlay opt-in FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH (Ego_d LF_CLASS 0x6611E on the retail
//   CEngineInternalPrimitiveStaticMesh). Camera helpers: the shared FABLE_FORCEINLINE TransformScreenXYLinearZToWorld chain and the
//   named-product TransformDistScreenToViewHorizontal (FABLE_RETAIL_CENGINECAMERA_DISTH_NAMED); no inline-control flag needed (the
//   twin's _MESH2D_ORIGINAL plain-inline texts match here only with /d2inlT144).
// Callees / globals: CEnginePrimitiveRenderer2D::Layer2DToDistance 0x00B4A6B0 on NEngineInternal::GPPrimitiveRenderer2D 0x01436E7C,
//   NEngineInternal::GPCamera 0x01436EA0 (OneOverHalfWindowWidth +0xC0 .. InverseViewTransform +0xE4), 0.5f 0x0122F59C.
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_CENGINECAMERA_DISTH_NAMED
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_CPLANE_LINE
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINECAMERA_SCREENTOWORLD
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D
#define FABLE_RETAIL_CENGINEPRIMITIVERENDERER2D_LAYER2DTODISTANCE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"

namespace NEngineInternal {
extern CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D;
extern CEngineCamera* GPCamera;
}
void CEngineInternalPrimitive2DStaticMesh::Initialise()
{
    const float view_z=NEngineInternal::GPPrimitiveRenderer2D->Layer2DToDistance(Layer);
    const float half_clip_box_width=ClipBox.GetWidth()*0.5f;
    const float half_clip_box_height=ClipBox.GetHeight()*0.5f;
    const float half_clip_square_width=half_clip_box_width<half_clip_box_height?half_clip_box_width:half_clip_box_height;
    WorldPos=C3DVector(ClipBox.TLX+half_clip_box_width,ClipBox.TLY+half_clip_box_height,view_z);
    NEngineInternal::GPCamera->TransformScreenXYLinearZToWorld(WorldPos);
    Scale=half_clip_square_width;
    NEngineInternal::GPCamera->TransformDistScreenToViewHorizontal(view_z,Scale);
    Initialised=true;
    Scale/=BoundingSphere.GetRadius();
}
