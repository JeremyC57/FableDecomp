#if defined(FABLE_RETAIL_CENGINECAMERA_POSITION)
#ifndef FABLE_ENGINE_CENGINECAMERA_H
#define FABLE_ENGINE_CENGINECAMERA_H
#include "engine/CEngineComponent.h"
#include "engine/CEngineCameraDesc.h"
#include "engine/CMatrix4x4.h"
#include "engine/CPlane.h"
#include "engine/CArray.h"
class CCamera;
class CEngineCamera:public CEngineComponent {
public:
 CEngineCamera(bool);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_UPDATE
 bool ClipSphere(const C3DVector&,float)const;
#endif
 virtual unsigned long PeekSceneFilterFlags()const;
 const C3DVector& PeekCameraPos()const{return CameraDesc.WorldPosition;}
#ifdef FABLE_RETAIL_CENGINECAMERA_ZSPRITE_ACCESSORS
 // Overlay opt-in (row 0x00BAA870 RenderFromSortList): FableWin.pdb inline accessors (no Ego_r.pdb or Fable.exe copy; the row reads
 // the members in place): ?PeekCameraState@CEngineCamera@@QAEABUCEngineCameraDesc@@XZ, ?PeekHFOV@CEngineCamera@@QBEMXZ,
 // ?PeekNonHomogenousViewToViewScaleX/Y@CEngineCamera@@QBEMXZ, ?PeekInvZToZBufferFactor/Offset@CEngineCamera@@QBEMXZ (first used by the
 // row: FableWin COMDATs after 0x02ED3E30), ?PeekNonHomogenousViewTransform / ?PeekInverseNonHomogenousViewTransform@CEngineCamera@@QBEABVCMatrix3x4@@XZ.
 const CEngineCameraDesc& PeekCameraState(){return CameraDesc;}
 float PeekHFOV()const{return CameraDesc.HorizontalFOV;}
 float PeekNonHomogenousViewToViewScaleX()const{return HomogenousViewScaleX;}
 float PeekNonHomogenousViewToViewScaleY()const{return HomogenousViewScaleY;}
 float PeekInvZToZBufferFactor()const{return ZBufferFactor;}
 float PeekInvZToZBufferOffset()const{return ZBufferOffset;}
 const CMatrix3x4& PeekNonHomogenousViewTransform()const{return NoScaleViewTransform;}
 const CMatrix3x4& PeekInverseNonHomogenousViewTransform()const{return InverseNoScaleViewTransform;}
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_SCREENTOWORLD
 // Opt-in (chunk 1020, row 0x00BCF4F0 CEngineInternalPrimitive2DAnimatedMesh::Initialise): FableWin.pdb
 // ?TransformScreenXYLinearZToView@CEngineCamera@@QBEXAAVC3DVector@@@Z, ?TransformViewToWorld@CEngineCamera@@QBEXAAVC3DVector@@@Z and
 // ?TransformScreenXYLinearZToWorld@CEngineCamera@@QBEXAAVC3DVector@@@Z (debug-only COMDATs: expanded in release, 0x00BCF5A5..0x00BCF65A).
#ifdef FABLE_RETAIL_CENGINECAMERA_MESH2D_ORIGINAL
 // Original donor02e79bb0 and native00bcf54d: depth times input, then projection scale/divisor.
 void TransformDistScreenToViewHorizontal(float z,float& distance)const{const float scaled=OneOverHalfWindowWidth*distance*z;distance=scaled/HomogenousViewScaleX;}
 void TransformScreenXYLinearZToView(C3DVector& point)const{
  point.X=(point.X-WindowCenterX)*OneOverHalfWindowWidth*point.Z;
  point.Y=(WindowCenterY-point.Y)*OneOverHalfWindowHeight*point.Z;}
 void TransformViewToWorld(C3DVector& point)const{InverseViewTransform.TransformPoint(point);}
 void TransformScreenXYLinearZToWorld(C3DVector& point)const{TransformScreenXYLinearZToView(point);TransformViewToWorld(point);}
#else
 FABLE_FORCEINLINE void TransformScreenXYLinearZToView(C3DVector& point)const{
  point.X=(point.X-WindowCenterX)*OneOverHalfWindowWidth*point.Z;
  point.Y=(WindowCenterY-point.Y)*OneOverHalfWindowHeight*point.Z;}
 FABLE_FORCEINLINE void TransformViewToWorld(C3DVector& point)const{InverseViewTransform.TransformPoint(point);}
 FABLE_FORCEINLINE void TransformScreenXYLinearZToWorld(C3DVector& point)const{TransformScreenXYLinearZToView(point);TransformViewToWorld(point);}
 // ?TransformDistScreenToViewHorizontal@CEngineCamera@@QBEXMAAM@Z (Ego_r / FableWin; expanded in release): a horizontal screen distance
 // to view space at depth z, in place through the reference (0x00BCF54D..0x00BCF566: `fld [cam+0C0h] ; fmul [dist] ; fmul z ;
 // fdiv [cam+0D4h] ; fstp [dist]`).
 void TransformDistScreenToViewHorizontal(float z,float& distance)const{distance=OneOverHalfWindowWidth*distance*z/HomogenousViewScaleX;}
#endif
#endif
private:
 const CCamera* GameCamera;
 const C2DBoxF* GameWindow;
 CEngineCameraDesc CameraDesc;
 CMatrix3x4 ViewTransform;
 float WindowWidth,WindowHeight,HalfWindowWidth,HalfWindowHeight;
 float OneOverHalfWindowWidth,OneOverHalfWindowHeight,WindowCenterX,WindowCenterY;
 float AspectRatio,HomogenousViewScaleX,HomogenousViewScaleY,ZBufferFactor,ZBufferOffset;
 CMatrix3x4 InverseViewTransform,NoScaleViewTransform,InverseNoScaleViewTransform;
 CMatrix4x4 ProjectionTransform;
 C3DVector LookVector;
 CPlane FrustrumPlanes[4];
 long ConsoleOverrideViewPortWidth,ConsoleOverrideViewPortHeight;
 class CDebugScriptElement {public:C3DVector WorldPos,LookVector;float FOV,TimeToNextElement;};
 CArray<CDebugScriptElement> DebugScript;
 long CurrentDebugScriptElement;
 bool DebugScriptRunning;
 double DebugScriptElementStartTime;
};
FABLE_STATIC_ASSERT(sizeof(CEngineCamera)==552);
#endif
#else
#ifndef FABLE_ENGINE_CENGINECAMERA_H
#define FABLE_ENGINE_CENGINECAMERA_H
#ifdef FABLE_RETAIL_CENGINECAMERA
// Retail branch (chunk 731, 2026-11-03): RTTI .?AVCEngineCamera@@ (vtable 0x012A12A4) : CEngineComponent (+0x0); sizeof 0x228 from
// CEngine::Initialise 0x00B26360 (`new` of 0x228 bytes before the constructor call). Constructor ??0CEngineCamera@@QAE@_N@Z 0x00B31700 and
// the PeekSceneFilterFlags override (?PeekSceneFilterFlags@CEngineCamera@@UBEKXZ): Ego_r.pdb / FableWin.pdb. Own members are
// not modelled yet (one opaque block; the generated Ego_r model below lists their names).
#ifndef FABLE_RETAIL_CENGINECOMPONENT
#define FABLE_RETAIL_CENGINECOMPONENT
#endif
#include "rebuild_abi.h"
#include "engine/CEngineComponent.h"
#ifdef FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
#include "engine/CMatrix3x4.h"
#endif
class CEngineInit;
class CEngineCamera : public CEngineComponent {
public:
    CEngineCamera(bool);
#ifdef FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_UPDATE
 bool ClipSphere(const C3DVector&,float)const;
#endif                       // 0x00B31700 ??0CEngineCamera@@QAE@_N@Z
    virtual unsigned long PeekSceneFilterFlags() const;
#ifdef FABLE_RETAIL_CENGINECAMERA_REFRESHD3DTRANSFORMS
    // Retail opt-in (chunk 792, row 0x00B5A3F0 CEngineScreenEffectColourFilter::ApplyColourFilter): Ego_r.pdb / FableWin.pdb
    // ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ (landed 0x00B2FC50), called on the camera component pointer 0x01436EA0. Declaration only.
    void RefreshD3DTransforms();                                         // 0x00B2FC50
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
    // Chunk 764 (row 0x00B451D0 CEngineLightingManager::CalculateFogTransformMatrix): Ego_r.pdb NoScaleViewTransform (CMatrix3x4) at +0x114,
    // read in place by retail (fmul [eax+114h] .. [eax+140h] off the camera pointer 0x01436EA0). No size change.
private:
    unsigned char _pad_0xc[0x108];              // +0xc GameCamera .. InverseViewTransform (not modelled)
public:
    CMatrix3x4 NoScaleViewTransform;            // +0x114
private:
    unsigned char _pad_0x144[0xe4];             // +0x144 InverseNoScaleViewTransform .. (not modelled)
#else
private:
    unsigned char _pad_0xc[0x21c];              // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
#endif
};
FABLE_STATIC_ASSERT(sizeof(CEngineCamera) == 0x228);
#else // generated flattened Ego_r layout (the landed default, unchanged)
// GENERATED by tools/decomp_pipeline/gen_class_headers.py -- do not edit by hand.
// Donor: Ego_r.pdb (ghidra_out/struct_layouts_egor.tsv) sizeof CEngineCamera = 0x228.
// Container layout: retail STLport 4.6 (list 4, vector/map/set/string/CArray 0xc, deque 0x28, CVectorMap 0x10, bases from Fable.exe RTTI): sizeof CEngineCamera = 0x228.
// Retail evidence: 0 trusted landed function(s); reconciliation clean.
// Members are original Lionhead names; `_pad_*` are unnamed bytes; opaque
// `unsigned char X[N]` members carry their real (composite/template) type in a comment.
#include <stddef.h>
#include "rebuild_abi.h"

class C2DBoxF;
class CCamera;

#pragma pack(push, 1)
class CEngineCamera {
public:
    void*         __vftable;                                 // +0x00 vptr, or first dword of a flattened base subobject
    unsigned char _pad_0x04[0x4];                            // +0x04
    unsigned char _pad_0x08[0x4];                            // +0x08
    CCamera*      GameCamera;                                // +0x0c
    C2DBoxF*      GameWindow;                                // +0x10
    unsigned char CameraDesc[0x6c];                          // +0x14 CEngineCameraDesc
    unsigned char ViewTransform[0x30];                       // +0x80 CMatrix3x4
    float         WindowWidth;                               // +0xb0
    float         WindowHeight;                              // +0xb4
    float         HalfWindowWidth;                           // +0xb8
    float         HalfWindowHeight;                          // +0xbc
    float         OneOverHalfWindowWidth;                    // +0xc0
    float         OneOverHalfWindowHeight;                   // +0xc4
    float         WindowCenterX;                             // +0xc8
    float         WindowCenterY;                             // +0xcc
    float         AspectRatio;                               // +0xd0
    float         HomogenousViewScaleX;                      // +0xd4
    float         HomogenousViewScaleY;                      // +0xd8
    float         ZBufferFactor;                             // +0xdc
    float         ZBufferOffset;                             // +0xe0
    unsigned char InverseViewTransform[0x30];                // +0xe4 CMatrix3x4
    unsigned char NoScaleViewTransform[0x30];                // +0x114 CMatrix3x4
    unsigned char InverseNoScaleViewTransform[0x30];         // +0x144 CMatrix3x4
    unsigned char ProjectionTransform[0x40];                 // +0x174 CMatrix4x4
    float         LookVector_X;                              // +0x1b4 C3DVector::X
    float         LookVector_Y;                              // +0x1b8 C3DVector::Y
    float         LookVector_Z;                              // +0x1bc C3DVector::Z
    unsigned char FrustrumPlanes[0x40];                      // +0x1c0 CPlane[4]
    long          ConsoleOverrideViewPortWidth;              // +0x200
    long          ConsoleOverrideViewPortHeight;             // +0x204
    unsigned char DebugScript[0xc];                          // +0x208 CArray<CEngineCamera::CDebugScriptElement>
    long          CurrentDebugScriptElement;                 // +0x214
    bool          DebugScriptRunning;                        // +0x218
    unsigned char _pad_0x219[0x7];                           // +0x219
    double        DebugScriptElementStartTime;               // +0x220
};
#pragma pack(pop)

FABLE_STATIC_ASSERT(sizeof(CEngineCamera) == 0x228);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, __vftable) == 0x0);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, GameCamera) == 0xc);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, GameWindow) == 0x10);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, CameraDesc) == 0x14);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, ViewTransform) == 0x80);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, WindowWidth) == 0xb0);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, WindowHeight) == 0xb4);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, HalfWindowWidth) == 0xb8);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, HalfWindowHeight) == 0xbc);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, OneOverHalfWindowWidth) == 0xc0);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, OneOverHalfWindowHeight) == 0xc4);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, WindowCenterX) == 0xc8);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, WindowCenterY) == 0xcc);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, AspectRatio) == 0xd0);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, HomogenousViewScaleX) == 0xd4);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, HomogenousViewScaleY) == 0xd8);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, ZBufferFactor) == 0xdc);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, ZBufferOffset) == 0xe0);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, InverseViewTransform) == 0xe4);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, NoScaleViewTransform) == 0x114);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, InverseNoScaleViewTransform) == 0x144);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, ProjectionTransform) == 0x174);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, LookVector_X) == 0x1b4);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, LookVector_Y) == 0x1b8);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, LookVector_Z) == 0x1bc);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, FrustrumPlanes) == 0x1c0);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, ConsoleOverrideViewPortWidth) == 0x200);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, ConsoleOverrideViewPortHeight) == 0x204);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, DebugScript) == 0x208);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, CurrentDebugScriptElement) == 0x214);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, DebugScriptRunning) == 0x218);
FABLE_STATIC_ASSERT(offsetof(CEngineCamera, DebugScriptElementStartTime) == 0x220);

#endif // FABLE_RETAIL_CENGINECAMERA
#endif // FABLE_ENGINE_CENGINECAMERA_H

#endif
