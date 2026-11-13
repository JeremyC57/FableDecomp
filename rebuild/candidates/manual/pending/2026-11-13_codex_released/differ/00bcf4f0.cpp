// 00bcf4f0 CEngineInternalPrimitive2DAnimatedMesh::Initialise
// Grade: DIFFER(374v374) -- one adjacent independent load/store pair swapped at tail: retail loads StartPosition.Z before EndPosition.Y store; ours stores Y before loading Z.101/101instructions,99.01%strict,2hunks.
// Retail: 00bcf4f0,374 bytes.
// Inline-budget: /d2inlT144 -- original camera/TransformPoint graph expanded; T143 leaves a call.
// Flags: /d2inlT144
// Leaf: ?Initialise@CEngineInternalPrimitive2DAnimatedMesh@@QAEXXZ
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_BASE
#define FABLE_RETAIL_C3DANIMATIONBLENDSTATE_TYPED_DATA
#define FABLE_RETAIL_CENGINECAMERA_MESH2D_ORIGINAL
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
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DANIMATEDMESH
#include "engine/CEngineCamera.h"
#include "engine/CEnginePrimitiveRenderer2D.h"
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"

namespace NEngineInternal {
extern CEnginePrimitiveRenderer2D* GPPrimitiveRenderer2D;
extern CEngineCamera* GPCamera;
}
void CEngineInternalPrimitive2DAnimatedMesh::Initialise()
{
    const float view_z=NEngineInternal::GPPrimitiveRenderer2D->Layer2DToDistance(Layer);
    const float half_clip_box_width=ClipBox.GetWidth()*0.5f;
    const float half_clip_box_height=ClipBox.GetHeight()*0.5f;
    const float half_clip_square_width=half_clip_box_width<half_clip_box_height?half_clip_box_width:half_clip_box_height;
    const float one_on_bsphere_radius=1.0f/ObjectBoundingSphere.GetRadius();
    Scale=half_clip_square_width;
    NEngineInternal::GPCamera->TransformDistScreenToViewHorizontal(view_z,Scale);
    Scale*=one_on_bsphere_radius;
    WorldPos.StartPosition=C3DVector(ClipBox.TLX+half_clip_box_width,ClipBox.TLY+half_clip_box_height,view_z);
    NEngineInternal::GPCamera->TransformScreenXYLinearZToWorld(WorldPos.StartPosition);
    C3DVector& end_position=WorldPos.EndPosition;
    end_position=WorldPos.StartPosition;
    Initialised=true;
}
