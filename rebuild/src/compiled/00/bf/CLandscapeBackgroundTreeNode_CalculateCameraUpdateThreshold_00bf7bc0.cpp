// 00BF7BC0 CLandscapeBackgroundTreeNode::CalculateCameraUpdateThreshold
// Inline-budget: actual distance members expand at /d2inlT282;281 retains a call; plain/scoped depth255 retain calls.
// Leaf: ?CalculateCameraUpdateThreshold@CLandscapeBackgroundTreeNode@@AAEXJ@Z
// Flags: /d2inlT282
// Retail-asm-helper: GF_SSE_BOX_DISTANCE_SQUARED (shared C3DBoundingBoxSSE.h)
// Retail-asm: 0x00BF7BE6..0x00BF7C2E original packed-distance kernel (`movhps` / `maxps` / `minps`), homed pointers,
// MAXPS/MINPS/SUBPS/MULPS/SHUFPS/ADDPS/ADDSS without aligned frame.
// Per-site ladder: 30 plain kernels and 10 intrinsic kernels, all retained and
// documented in PROBES.md; best intrinsic595bytes vs546 with an aligned frame.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_PASS22_DISTANCE
#define FABLE_RETAIL_CMOVABLERESOURCE
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_BASE
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_SUB_NODE_BASE_PASS22_ISPATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_PATCH
#define FABLE_RETAIL_LANDSCAPE_BACKGROUND_TREE_NODE_PASS22
#define FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECAMERA_POSITION
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER
#define FABLE_RETAIL_CENGINELANDSCAPERENDERER_PASS22_LODBAND
#include <math.h>
#include "engine/C3DBoundingBoxSSE.h"
#include "engine/CLandscapeBackgroundTreeNode.h"
#include "engine/CEngineCamera.h"
#include "engine/CEngineLandscapeRenderer.h"
#pragma intrinsic(sqrt)
extern bool GEnableSIMD;
namespace NEngineInternal {
    extern CEngineCamera* GPCamera;
    extern CEngineLandscapeRenderer* GPLandscapeRenderer;
}
inline float C3DBoundingBox::CalcShortestDistanceToPointSquared(const C3DVector& point) const
{
    if(GEnableSIMD) {
        float result;
        GF_SSE_BOX_DISTANCE_SQUARED(this, point, result);
        return result;
    }
    C3DVector closest=point;
    if(closest.X<MinCoord.X) closest.X=MinCoord.X;
    else if(closest.X>MaxCoord.X) closest.X=MaxCoord.X;
    if(closest.Y<MinCoord.Y) closest.Y=MinCoord.Y;
    else if(closest.Y>MaxCoord.Y) closest.Y=MaxCoord.Y;
    if(closest.Z<MinCoord.Z) closest.Z=MinCoord.Z;
    else if(closest.Z>MaxCoord.Z) closest.Z=MaxCoord.Z;
    return (point-closest).GetSquaredMagnitude();
}
inline float C3DBoundingBox::CalcShortestDistanceToPoint(const C3DVector& point) const
{
    return (float)sqrt(CalcShortestDistanceToPointSquared(point));
}
void CLandscapeBackgroundTreeNode::CalculateCameraUpdateThreshold(long current_lod)
{
    float distance_from_camera=BoundingBox.CalcShortestDistanceToPoint(NEngineInternal::GPCamera->PeekCameraPos());
    float inc_band_distance=1.0e30f;
    float dec_band_distance=1.0e30f;
    CLandscapeBackgroundPatch* patch=GetPatch();
    if(patch) {
        long patch_band=patch->LODBand;
        if(patch_band<8) {
            long next_patch=patch_band+1;
            while(GetLodBandInfo(next_patch).OptimizedBandRemap==patch_band && next_patch<8)
                ++next_patch;
            if(next_patch<8)
                inc_band_distance=NEngineInternal::GPLandscapeRenderer->GetLODZBandZ(next_patch)-distance_from_camera;
        }
        if(patch_band>1)
            dec_band_distance=distance_from_camera-NEngineInternal::GPLandscapeRenderer->GetLODZBandZ(patch_band);
    } else {
        inc_band_distance=NEngineInternal::GPLandscapeRenderer->GetLODZBandZ(FirstNonSplitBand)-distance_from_camera;
    }
    CameraUpdateThreshold.Center=NEngineInternal::GPCamera->PeekCameraPos();
    CameraUpdateThreshold.Radius=inc_band_distance<dec_band_distance?inc_band_distance:dec_band_distance;
}