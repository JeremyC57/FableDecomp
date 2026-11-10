// CEngineInternalPrimitive2DAnimatedMesh::Initialise() -- fit the 2D animated mesh into its clip box: Scale = the smaller half box side, taken to view space at the layer's distance and divided by the mesh sphere radius; the world position from the box centre (screen XY at the layer distance to world), then mark it initialised
// Grade: DIFFER(374v374) -- on shared headers (1 grade): x87 order of the Scale block only. Retail loads the sphere radius before the min store, evaluates OneOverHalfWindowWidth*Scale*z/HomogenousViewScaleX left to right and (1/radius)*Scale; ours (through the inline ?TransformDistScreenToViewHorizontal@CEngineCamera@@QBEXMAAM@Z by reference) reassociates to (c0/d4)*z*Scale and Scale/radius, frame 0x10 v 0xc. The world-position half (TransformScreenXYLinearZToWorld, TransformPoint) matches.
// Leaf: ?Initialise@CEngineInternalPrimitive2DAnimatedMesh@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BCF4F0, 374 bytes, thiscall, plain `ret`.
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

extern CEnginePrimitiveRenderer2D* g_pEnginePrimitiveRenderer2D;   // 0x01436E7C
extern CEngineCamera* g_pEngineCamera;                             // 0x01436EA0

void CEngineInternalPrimitive2DAnimatedMesh::Initialise()
{
    float distance = g_pEnginePrimitiveRenderer2D->Layer2DToDistance(Layer);
    float radius = ObjectBoundingSphere.GetRadius();
    float halfWidth = (ClipBox.BRX - ClipBox.TLX) * 0.5f;
    float halfHeight = (ClipBox.BRY - ClipBox.TLY) * 0.5f;
    Scale = halfWidth < halfHeight ? halfWidth : halfHeight;
    g_pEngineCamera->TransformDistScreenToViewHorizontal(distance, Scale);
    Scale *= 1.0f / radius;
    WorldPos.StartPosition = C3DVector(ClipBox.TLX + halfWidth, ClipBox.TLY + halfHeight, distance);
    g_pEngineCamera->TransformScreenXYLinearZToWorld(WorldPos.StartPosition);
    WorldPos.EndPosition = WorldPos.StartPosition;
    Initialised = true;
}
