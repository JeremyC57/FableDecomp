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
#ifdef FABLE_RETAIL_SCENEGRID_VISIBILITY_PASS27
 // Ego_r and FableWin names, retail 0x00B308E0.
 bool ClipWorldSpaceBoundingBox(const class C3DBoundingBox& box, float max_draw_dist, bool& all_inside) const;
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_SETGAMEWINDOW
 // NF172 opt-in (row 0x00B2FC00): Ego_r.pdb 0x009D0510 / FableWin.pdb 0x02DC15E0 ?SetGameWindow@CEngineCamera@@QAEXPBVC2DBoxF@@@Z
 // (stores the argument in GameWindow +0x10; CEngine::SetWindow 0x00B23B80 tail-jumps here on the engine camera). Declaration only.
 void SetGameWindow(const class C2DBoxF* window);                      // 0x00B2FC00
#endif
#if defined(FABLE_RETAIL_CENGINECAMERA_CLIPSPHERE)
 // Retail opt-in (chunk 2028, row 0x00B8DF50 CEngineOcclusionManager::FrameEnd): FableWin.pdb ?ClipSphere@CEngineCamera@@QBE_NABVC3DVector@@M@Z
 // (debug copy only: no Ego_r.pdb or Fable.exe out-of-line copy; reloc_bindings binds only the 3-argument 0x00B30480), expanded by the landed
 // CEngine::ClipSphere 0x00B24150 and by FrameEnd 0x00B8E0F6..0x00B8E22B: the sphere against the near / far planes along the look
 // vector, then the four side planes in the order 2, 3, 0, 1 (the order of the out-of-line 3-argument ClipSphere 0x00B30480).
 // FABLE_FORCEINLINE per the owner's 2026-11-01 helper rule (no out-of-line copy anywhere in retail; plain inline is called).
 FABLE_FORCEINLINE bool ClipSphere(const C3DVector& centre,float radius)const{
  C3DVector d=centre-CameraDesc.WorldPosition;
  float depth=d.Dot(LookVector);
  if(depth+radius<CameraDesc.NearZPlane||depth-radius>CameraDesc.FarZPlane||
     d.Dot(FrustrumPlanes[2].Normal)>radius||d.Dot(FrustrumPlanes[3].Normal)>radius||
     d.Dot(FrustrumPlanes[0].Normal)>radius||d.Dot(FrustrumPlanes[1].Normal)>radius)
   return false;
  return true;}
 // FableWin.pdb ?TransformWorldToView@CEngineCamera@@QBEXABVC3DVector@@AAV2@@Z (debug only; FrameEnd 0x00B8E2CB..0x00B8E2EA reads the
 // ViewTransform Z row +0x88 / +0x94 / +0xA0 / +0xAC in place) and ?PeekNonHomogenousViewToViewScaleX / Y@CEngineCamera@@QBEMXZ (debug only;
 // HomogenousViewScaleX +0xD4 / Y +0xD8 read in place at 0x00B8E337 / 0x00B8E323: the factor that takes the unscaled, "non-homogenous"
 // view space of NoScaleViewTransform to the scaled ViewTransform space).
 const CMatrix3x4& PeekViewTransform()const{return ViewTransform;}
 FABLE_FORCEINLINE void TransformWorldToView(const C3DVector& world,C3DVector& view)const{ViewTransform.TransformPoint(world,view);}
 float PeekNonHomogenousViewToViewScaleX()const{return HomogenousViewScaleX;}
 float PeekNonHomogenousViewToViewScaleY()const{return HomogenousViewScaleY;}
#elif defined(FABLE_RETAIL_CENGINEPRIMITIVEATTACHMENTMANAGER_UPDATE)
 bool ClipSphere(const C3DVector&,float)const;
#endif
 virtual unsigned long PeekSceneFilterFlags()const;
 const C3DVector& PeekCameraPos()const{return CameraDesc.WorldPosition;}
#ifdef FABLE_RETAIL_RADIAL_WORLD_TRACK_PASS29
 float PeekNearZ()const{return CameraDesc.NearZPlane;}
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_PEEKWINDOW
 // Opt-in (bwf2, row 0x00BA2350): FableWin.pdb ?PeekWindow@CEngineCamera@@QBEABVC2DBoxF@@XZ (9 debug instructions), inline.
 // Also wfA5, row 0x00B9AC30: inline in release, CameraDesc.Window +0x14 read in place (0x00B9B350).
 const C2DBoxF& PeekWindow()const{return CameraDesc.Window;}
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_ZSPRITE_ACCESSORS
 // Overlay opt-in (row 0x00BAA870 RenderFromSortList): FableWin.pdb inline accessors (no Ego_r.pdb or Fable.exe copy; the row reads
 // the members in place): ?PeekCameraState@CEngineCamera@@QAEABUCEngineCameraDesc@@XZ, ?PeekHFOV@CEngineCamera@@QBEMXZ (below, shared
 // with FABLE_RETAIL_CENGINECAMERA_LOOKFOV),
 // ?PeekNonHomogenousViewToViewScaleX/Y@CEngineCamera@@QBEMXZ, ?PeekInvZToZBufferFactor/Offset@CEngineCamera@@QBEMXZ (first used by the
 // row: FableWin COMDATs after 0x02ED3E30), ?PeekNonHomogenousViewTransform / ?PeekInverseNonHomogenousViewTransform@CEngineCamera@@QBEABVCMatrix3x4@@XZ.
 const CEngineCameraDesc& PeekCameraState(){return CameraDesc;}
 float PeekNonHomogenousViewToViewScaleX()const{return HomogenousViewScaleX;}
 float PeekNonHomogenousViewToViewScaleY()const{return HomogenousViewScaleY;}
 float PeekInvZToZBufferFactor()const{return ZBufferFactor;}
 float PeekInvZToZBufferOffset()const{return ZBufferOffset;}
 const CMatrix3x4& PeekNonHomogenousViewTransform()const{return NoScaleViewTransform;}
 const CMatrix3x4& PeekInverseNonHomogenousViewTransform()const{return InverseNoScaleViewTransform;}
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_LOOKFOV
 // Opt-in (wfE6, row 0x00B70A90 CEngineWaterRenderer::RenderEnvironmentMap): FableWin.pdb ?PeekLookVector@CEngineCamera@@QBEABVC3DVector@@XZ
 // (9 debug instructions), ?PeekHFOV@CEngineCamera@@QBEMXZ (9i; below, shared with FABLE_RETAIL_CENGINECAMERA_ZSPRITE_ACCESSORS) and
 // ?PeekVFOV@CEngineCamera@@QBEMXZ (22i), inline in retail: 0x00B710FE
 // reads LookVector +0x1B4..+0x1BC, 0x00B7112B HorizontalFOV +0x60, 0x00B71161..0x00B71191 `Use2DFOV +0x68 ? VerticalFOV +0x64 :
 // fld 1.0 ; fdiv AspectRatio +0xD0 ; fmul HorizontalFOV`.
 const C3DVector& PeekLookVector()const{return LookVector;}
 float PeekVFOV()const{return CameraDesc.Use2DFOV?CameraDesc.VerticalFOV:CameraDesc.HorizontalFOV*(1.0f/AspectRatio);}
#endif
#if defined(FABLE_RETAIL_CENGINECAMERA_ZSPRITE_ACCESSORS) || defined(FABLE_RETAIL_CENGINECAMERA_LOOKFOV)
 float PeekHFOV()const{return CameraDesc.HorizontalFOV;}
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_SHADOWFRUSTRUM_PASS22
 // Original FableWin02d76ff0 member; identical owner/signature in the complete TPI.
 const CPlane& PeekFrustrumPlane(long plane)const{return FrustrumPlanes[plane];}
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_CLIPBOX
 // Retail opt-in (chunk 2037, CEngineWeatherRenderer::ClipBoxToCamera 0x00B51F60 in the RenderRain / RenderSnow TU): Ego_r.pdb
 // ?ClipBox@CEngineCamera@@QBE_NABVCMatrix3x4@@@Z, retail 0x00B30610 (called at 0x00B52066 with the box matrix, `test al,al`).
 bool ClipBox(const CMatrix3x4& box)const;
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_CAMERASTATE
 // Retail opt-in (chunk 2008, row 0x00B665E0 CEngineSkyRenderer::GetEnvironmentMap): FableWin.pdb
 // ?PeekCameraState@CEngineCamera@@QAEABUCEngineCameraDesc@@XZ (0x02D416A0, debug only: inline in release, CameraDesc +0x14 copied in
 // place, 0x00B66619), Ego_r.pdb ?SetCameraState@CEngineCamera@@QAEXABUCEngineCameraDesc@@_N1@Z (retail 0x00B30B50) and
 // ?RefreshD3DTransforms@CEngineCamera@@QAEXXZ (retail 0x00B2FC50). Needs FABLE_RETAIL_CENGINECAMERADESC_STRUCT. No layout change.
 const CEngineCameraDesc& PeekCameraState(){return CameraDesc;}
 void SetCameraState(const CEngineCameraDesc& desc,bool,bool);         // 0x00B30B50
 void RefreshD3DTransforms();                                          // 0x00B2FC50
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_NONHOMOGENOUS
 // Retail opt-in (chunk 2008, row 0x00B54890 CEngineWeatherRenderer::RenderRain): FableWin.pdb
 // ?PeekNonHomogenousViewTransform@CEngineCamera@@QBEABVCMatrix3x4@@XZ (0x02CEE5D0) and ?PeekInverseNonHomogenousViewTransform@..@@QBEABVCMatrix3x4@@XZ
 // (0x02D3C6F0), debug only: inline in release (RenderRain reads +0x114 / +0x144, Ego_r NoScaleViewTransform / InverseNoScaleViewTransform,
 // in place, 0x00B549C3 / 0x00B54A2D). No layout change.
 const CMatrix3x4& PeekNonHomogenousViewTransform()const{return NoScaleViewTransform;}
 const CMatrix3x4& PeekInverseNonHomogenousViewTransform()const{return InverseNoScaleViewTransform;}
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_SCREENTOHOMOGENOUS
 // agentNF41 opt-in (row 0x00B63530 CEngineSkyRenderer::RenderScreenSpaceSprite): FableWin.pdb
 // ?TransformScreenTo2DHomogenousSpace@CEngineCamera@@QAEXAAVC2DVector@@@Z, out of line in retail at 0x00B2FB70 (catalog label; `ret 4`).
 void TransformScreenTo2DHomogenousSpace(C2DVector& point);
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_VIEWTOSCREEN
 // Opt-in (chunk 2048, RenderSunFlare 0x00B64550): FableWin.pdb ?TransformWorldToView@CEngineCamera@@QBEXAAVC3DVector@@@Z and
 // ?TransformViewToScreen@CEngineCamera@@QBEXAAVC3DVector@@@Z (debug COMDATs listed right after RenderSunFlare = first used there; expanded
 // in retail: ViewTransform +0x80, HalfWindowWidth / Height +0xB8 / +0xBC, WindowCenterX / Y +0xC8 / +0xCC).
 FABLE_FORCEINLINE void TransformWorldToView(C3DVector& point)const{ViewTransform.TransformPoint(point);}
 FABLE_FORCEINLINE void TransformViewToScreen(C3DVector& point)const{
  const float one_over_z=1.0f/point.Z;
  point.X=WindowCenterX+one_over_z*HalfWindowWidth*point.X;
  point.Y=WindowCenterY-one_over_z*HalfWindowHeight*point.Y;
 }
#endif
#ifdef FABLE_RETAIL_RADIAL_CAMERA_PROJECTION_PASS29
 // FableWin 02c9f690 stores all three coordinates; native both calls expand.
 // Inline-budget: The 580-byte native tracker expands both calls completely.
 FABLE_FORCEINLINE
 void TransformViewToScreen(const C3DVector& from,C3DVector& to) const {
     const float invz = 1.0f / from.Z;
     to.X = from.X * invz * HalfWindowWidth + WindowCenterX;
     to.Y = WindowCenterY - from.Y * invz * HalfWindowHeight;
     to.Z = ZBufferFactor * invz + ZBufferOffset;
 }
#endif
#ifdef FABLE_RETAIL_PROJECT_TEXT_PASS27
 // Raw FableWin 02C9F690 stores reciprocal depth transformed by Factor and Offset.
 void TransformViewToScreen(const C3DVector& from,C3DVector& to)const {
  const float invz=1.0f/from.Z;
  to.X=WindowCenterX+from.X*invz*HalfWindowWidth;
  to.Y=WindowCenterY-from.Y*invz*HalfWindowHeight;
  to.Z=ZBufferFactor*invz+ZBufferOffset;
 }
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_VIEWTOSCREEN_TWO_ARGS
 // Retail opt-in (Codex pass 24): Ego_d Engine.obj 119- and 35-byte two-argument methods,
 // FableWin 0x02C9F690/0x02C9F730; 0x00B242D0 expands the two view transformations.
 void TransformViewToScreen(const C3DVector& from,C3DVector& to)const{
  const float invz=1.0f/from.Z;
  to.X=from.X*HalfWindowWidth*invz+WindowCenterX;
  to.Y=WindowCenterY-invz*HalfWindowHeight*from.Y;
  to.Z=from.Z;
 }
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
#ifdef FABLE_RETAIL_CENGINECAMERA_DISTH_NAMED
 // Retail opt-in (wfA4, row 0x00BE0EB0): the same text as _MESH2D_ORIGINAL's (product named, then the divide) on the FORCEINLINE branch.
 void TransformDistScreenToViewHorizontal(float z,float& distance)const{const float scaled=OneOverHalfWindowWidth*distance*z;distance=scaled/HomogenousViewScaleX;}
#else
 void TransformDistScreenToViewHorizontal(float z,float& distance)const{distance=OneOverHalfWindowWidth*distance*z/HomogenousViewScaleX;}
#endif
#endif
#endif
private:
#ifdef FABLE_RETAIL_CENGINECAMERA_DEBUGSCRIPT_PASS26
 // FableWin 0x02DC0580 and the Ego_d engine_camera.obj procedure record.
 void ProcessDebugScript();
#ifdef FABLE_RETAIL_CENGINECAMERA_SETSTATE_CONTEXT_PASS26
 // Both donor PDBs identify SetupGamut. The native setter calls it at 00B3110F.
 void SetupGamut();
#endif
#endif
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
#ifdef FABLE_RETAIL_CENGINECAMERA_DEBUGSCRIPT_PASS26
 // FableWin CArray<CDebugScriptElement>::operator[] mangles the nested element as a struct.
 struct CDebugScriptElement {C3DVector WorldPos,LookVector;float FOV,TimeToNextElement;};
#else
 class CDebugScriptElement {public:C3DVector WorldPos,LookVector;float FOV,TimeToNextElement;};
#endif
 CArray<CDebugScriptElement> DebugScript;
 long CurrentDebugScriptElement;
 bool DebugScriptRunning;
 double DebugScriptElementStartTime;
};
FABLE_STATIC_ASSERT_AT(116, sizeof(CEngineCamera)==552);
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
#ifdef FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM_CAMERASTATE
#include "engine/CEngineCameraDesc.h"
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
#ifdef FABLE_RETAIL_CENGINECAMERA_SETGAMECAMERA_PROCESS_DECL
    // Opt-in (NF321, 2026-11-23, row 0x00B23B50 CEngine::SetCamera): Ego_r.pdb / FableWin.pdb ?SetGameCamera@CEngineCamera@@QAEXPBVCCamera@@@Z
    // (retail 0x00B2FBF0) and ?Process@CEngineCamera@@QAEX_N@Z (landed 0x00B314E0), the two calls of the donor FableWin CEngine::SetCamera,
    // made on the camera component pointer 0x01436EA0. Declarations only.
    void SetGameCamera(const class CCamera* camera);                     // 0x00B2FBF0
    void Process(bool force);                                            // 0x00B314E0
#endif
#ifdef FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM
    // Chunk 764 (row 0x00B451D0 CEngineLightingManager::CalculateFogTransformMatrix): Ego_r.pdb NoScaleViewTransform (CMatrix3x4) at +0x114,
    // read in place by retail (fmul [eax+114h] .. [eax+140h] off the camera pointer 0x01436EA0). No size change.
private:
#ifdef FABLE_RETAIL_CENGINECAMERA_NOSCALEVIEWTRANSFORM_CAMERASTATE
    // Nested opt-in (wfE5, row 0x00BC8A60 CEnginePrimitiveManagerLines::RenderLine): Ego_r.pdb CameraDesc (CEngineCameraDesc, 0x6c) at +0x14
    // and FableWin.pdb ?PeekCameraState@CEngineCamera@@QAEABUCEngineCameraDesc@@XZ (debug only, inline in release): retail 0x00BC8BEE reads
    // NearZPlane at +0x6c (= +0x14 + 0x58) off the camera pointer 0x01436EA0. No size change.
    unsigned char _pad_0xc[0x8];                // +0xc GameCamera, GameWindow (not modelled)
    CEngineCameraDesc CameraDesc;               // +0x14
    unsigned char _pad_0x80[0x94];              // +0x80 ViewTransform .. InverseViewTransform (not modelled)
public:
    const CEngineCameraDesc& PeekCameraState() { return CameraDesc; }
#else
    unsigned char _pad_0xc[0x108];              // +0xc GameCamera .. InverseViewTransform (not modelled)
#endif
public:
    CMatrix3x4 NoScaleViewTransform;            // +0x114
#ifdef FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM
    // Nested opt-in (chunk 2059, row 0x00BC25F0): FableWin.pdb ?PeekNonHomogenousViewTransform@CEngineCamera@@QBEABVCMatrix3x4@@XZ (19b/9i),
    // inline in retail (0x00BC2807 copies +0x114 into a local with `rep movsd`).
    const CMatrix3x4& PeekNonHomogenousViewTransform() const { return NoScaleViewTransform; }
#endif
private:
#ifdef FABLE_RETAIL_CENGINECAMERA_PEEKINVERSENONHOMOGENOUSVIEWTRANSFORM
    // Nested opt-in (row 0x00BBF4A0): Ego_r.pdb InverseNoScaleViewTransform (CMatrix3x4) at +0x144 and FableWin.pdb
    // ?PeekInverseNonHomogenousViewTransform@CEngineCamera@@QBEABVCMatrix3x4@@XZ, inline in retail (0x00BBFF0F reads +0x144 .. +0x158).
public:
    CMatrix3x4 InverseNoScaleViewTransform;     // +0x144
    const CMatrix3x4& PeekInverseNonHomogenousViewTransform() const { return InverseNoScaleViewTransform; }
#ifdef FABLE_RETAIL_CENGINECAMERA_NONHOMOGENOUSVIEW_TRANSFORMS
    // Nested opt-in (wfE5 14th author, row 0x00BC8A60 CEnginePrimitiveManagerLines::RenderLine): FableWin.pdb
    // ?TransformWorldToNonHomogenousView@CEngineCamera@@QBEXAAVC3DVector@@@Z (0x02EC0730, 31b/12i) and
    // ?TransformNonHomogenousViewToWorld@CEngineCamera@@QBEXAAVC3DVector@@@Z (0x02EC0700, 31b/12i): debug COMDATs placed right after RenderLine
    // (first used there), one-call wrappers of TransformPoint on NoScaleViewTransform (+0x114) / InverseNoScaleViewTransform (+0x144); /Od of
    // these bodies = FableWin byte counts. No Ego_r.pdb / retail out-of-line copy: expanded in place (0x00BC8ABF.., 0x00BC8E28..).
    // Needs FABLE_RETAIL_CMATRIX3X4 and FABLE_RETAIL_CENGINECAMERA_PEEKNONHOMOGENOUSVIEWTRANSFORM. No layout change.
    FABLE_FORCEINLINE void TransformWorldToNonHomogenousView(C3DVector& point) const { NoScaleViewTransform.TransformPoint(point); }
    FABLE_FORCEINLINE void TransformNonHomogenousViewToWorld(C3DVector& point) const { InverseNoScaleViewTransform.TransformPoint(point); }
#endif
private:
    unsigned char _pad_0x174[0xb4];             // +0x174 .. (not modelled)
#else
    unsigned char _pad_0x144[0xe4];             // +0x144 InverseNoScaleViewTransform .. (not modelled)
#endif
#else
private:
    unsigned char _pad_0xc[0x21c];              // +0xc members not modelled yet (Ego_r.pdb names; retail STLport sizes)
#endif
};
FABLE_STATIC_ASSERT_AT(176, sizeof(CEngineCamera) == 0x228);
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

FABLE_STATIC_ASSERT_AT(231, sizeof(CEngineCamera) == 0x228);
FABLE_STATIC_ASSERT_AT(232, offsetof(CEngineCamera, __vftable) == 0x0);
FABLE_STATIC_ASSERT_AT(233, offsetof(CEngineCamera, GameCamera) == 0xc);
FABLE_STATIC_ASSERT_AT(234, offsetof(CEngineCamera, GameWindow) == 0x10);
FABLE_STATIC_ASSERT_AT(235, offsetof(CEngineCamera, CameraDesc) == 0x14);
FABLE_STATIC_ASSERT_AT(236, offsetof(CEngineCamera, ViewTransform) == 0x80);
FABLE_STATIC_ASSERT_AT(237, offsetof(CEngineCamera, WindowWidth) == 0xb0);
FABLE_STATIC_ASSERT_AT(238, offsetof(CEngineCamera, WindowHeight) == 0xb4);
FABLE_STATIC_ASSERT_AT(239, offsetof(CEngineCamera, HalfWindowWidth) == 0xb8);
FABLE_STATIC_ASSERT_AT(240, offsetof(CEngineCamera, HalfWindowHeight) == 0xbc);
FABLE_STATIC_ASSERT_AT(241, offsetof(CEngineCamera, OneOverHalfWindowWidth) == 0xc0);
FABLE_STATIC_ASSERT_AT(242, offsetof(CEngineCamera, OneOverHalfWindowHeight) == 0xc4);
FABLE_STATIC_ASSERT_AT(243, offsetof(CEngineCamera, WindowCenterX) == 0xc8);
FABLE_STATIC_ASSERT_AT(244, offsetof(CEngineCamera, WindowCenterY) == 0xcc);
FABLE_STATIC_ASSERT_AT(245, offsetof(CEngineCamera, AspectRatio) == 0xd0);
FABLE_STATIC_ASSERT_AT(246, offsetof(CEngineCamera, HomogenousViewScaleX) == 0xd4);
FABLE_STATIC_ASSERT_AT(247, offsetof(CEngineCamera, HomogenousViewScaleY) == 0xd8);
FABLE_STATIC_ASSERT_AT(248, offsetof(CEngineCamera, ZBufferFactor) == 0xdc);
FABLE_STATIC_ASSERT_AT(249, offsetof(CEngineCamera, ZBufferOffset) == 0xe0);
FABLE_STATIC_ASSERT_AT(250, offsetof(CEngineCamera, InverseViewTransform) == 0xe4);
FABLE_STATIC_ASSERT_AT(251, offsetof(CEngineCamera, NoScaleViewTransform) == 0x114);
FABLE_STATIC_ASSERT_AT(252, offsetof(CEngineCamera, InverseNoScaleViewTransform) == 0x144);
FABLE_STATIC_ASSERT_AT(253, offsetof(CEngineCamera, ProjectionTransform) == 0x174);
FABLE_STATIC_ASSERT_AT(254, offsetof(CEngineCamera, LookVector_X) == 0x1b4);
FABLE_STATIC_ASSERT_AT(255, offsetof(CEngineCamera, LookVector_Y) == 0x1b8);
FABLE_STATIC_ASSERT_AT(256, offsetof(CEngineCamera, LookVector_Z) == 0x1bc);
FABLE_STATIC_ASSERT_AT(257, offsetof(CEngineCamera, FrustrumPlanes) == 0x1c0);
FABLE_STATIC_ASSERT_AT(258, offsetof(CEngineCamera, ConsoleOverrideViewPortWidth) == 0x200);
FABLE_STATIC_ASSERT_AT(259, offsetof(CEngineCamera, ConsoleOverrideViewPortHeight) == 0x204);
FABLE_STATIC_ASSERT_AT(260, offsetof(CEngineCamera, DebugScript) == 0x208);
FABLE_STATIC_ASSERT_AT(261, offsetof(CEngineCamera, CurrentDebugScriptElement) == 0x214);
FABLE_STATIC_ASSERT_AT(262, offsetof(CEngineCamera, DebugScriptRunning) == 0x218);
FABLE_STATIC_ASSERT_AT(263, offsetof(CEngineCamera, DebugScriptElementStartTime) == 0x220);

#endif // FABLE_RETAIL_CENGINECAMERA
#endif // FABLE_ENGINE_CENGINECAMERA_H

#endif
