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
#ifdef FABLE_RETAIL_CENGINECAMERA_GAMUT
#include "engine/CEngineCameraDesc.h"
#include "engine/CPlane.h"
class CCamera;
#endif
class CEngineInit;
class CEngineCamera : public CEngineComponent {
public:
    CEngineCamera(bool);                       // 0x00B31700 ??0CEngineCamera@@QAE@_N@Z
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
#elif defined(FABLE_RETAIL_CENGINECAMERA_GAMUT)
    // Retail opt-in (chunk 843, row 0x00B2FD60 ?SetupGamut@CEngineCamera@@AAEXXZ): the own members with their Ego_r.pdb names and
    // offsets (struct_layouts_egor.tsv; the generated default below lists the same offsets), checked against retail: SetupGamut reads
    // CameraDesc.Window +0x14, CameraDesc.FarZPlane +0x70, WindowWidth / WindowHeight +0xB0 / +0xB4, OneOverHalfWindowWidth / Height
    // +0xC0 / +0xC4, WindowCenterX / Y +0xC8 / +0xCC and InverseViewTransform +0xE4..+0x110, and writes LookVector +0x1B4 and
    // FrustrumPlanes +0x1C0..+0x1FF; the landed ClipSphere 0x00B30480 reads CameraDesc.WorldPosition +0x54 and FrustrumPlanes.
    // TransformScreenXYLinearZToView / TransformViewToWorld: FableWin.pdb ?TransformScreenXYLinearZToView@CEngineCamera@@QBEXAAVC3DVector@@@Z
    // (0x02C9F970) and ?TransformViewToWorld@CEngineCamera@@QBEXAAVC3DVector@@@Z (0x02C9F9E0), debug copies only: no Ego_r.pdb or retail
    // out-of-line copy (SetupGamut expands both five times; __forceinline per the owner 2026-11-01 helper rule, plain inline leaves
    // TransformViewToWorld called five times). Bodies from SetupGamut's bytes. No size change.
    __forceinline void TransformScreenXYLinearZToView(C3DVector& point) const {
        const float z = point.Z;
        point.X = (point.X - WindowCenterX) * OneOverHalfWindowWidth * z;
        point.Y = (WindowCenterY - point.Y) * OneOverHalfWindowHeight * z;
        point.Z = z;
    }
    // TransformViewToWorld sums each row left to right (E1k*x + E2k*y + E3k*z + E4k): retail's origin point 0x00B2FDE3..0x00B2FE0B
    // adds E21*y and E11*x before the E31*0.0f term, which CMatrix3x4::TransformPoint's parenthesised retail rows cannot give
    // (the matrix through a local reference: naming InverseViewTransform in each term moves the x87 order, 213 -> 271 diff lines).
    __forceinline void TransformViewToWorld(C3DVector& point) const {
        const CMatrix3x4& m = InverseViewTransform;
        const float xx = point.X;
        const float yy = point.Y;
        const float zz = point.Z;
        point.X = m.E11 * xx + m.E21 * yy + m.E31 * zz + m.E41;
        point.Y = m.E12 * xx + m.E22 * yy + m.E32 * zz + m.E42;
        point.Z = m.E13 * xx + m.E23 * yy + m.E33 * zz + m.E43;
    }
private:
    void SetupGamut();                                                   // 0x00B2FD60 ?SetupGamut@CEngineCamera@@AAEXXZ
public:
    CCamera* GameCamera;                        // +0x0C
    const C2DBoxF* GameWindow;                  // +0x10
    CEngineCameraDesc CameraDesc;               // +0x14
    CMatrix3x4 ViewTransform;                   // +0x80
    float WindowWidth;                          // +0xB0
    float WindowHeight;                         // +0xB4
    float HalfWindowWidth;                      // +0xB8
    float HalfWindowHeight;                     // +0xBC
    float OneOverHalfWindowWidth;               // +0xC0
    float OneOverHalfWindowHeight;              // +0xC4
    float WindowCenterX;                        // +0xC8
    float WindowCenterY;                        // +0xCC
    float AspectRatio;                          // +0xD0
    float HomogenousViewScaleX;                 // +0xD4
    float HomogenousViewScaleY;                 // +0xD8
    float ZBufferFactor;                        // +0xDC
    float ZBufferOffset;                        // +0xE0
    CMatrix3x4 InverseViewTransform;            // +0xE4
    CMatrix3x4 NoScaleViewTransform;            // +0x114
    CMatrix3x4 InverseNoScaleViewTransform;     // +0x144
    unsigned char ProjectionTransform[0x40];    // +0x174 CMatrix4x4 (not modelled)
    C3DVector LookVector;                       // +0x1B4
    CPlane FrustrumPlanes[4];                   // +0x1C0
    long ConsoleOverrideViewPortWidth;          // +0x200
    long ConsoleOverrideViewPortHeight;         // +0x204
    unsigned char DebugScript[0xc];             // +0x208 CArray<CEngineCamera::CDebugScriptElement> (not modelled)
    long CurrentDebugScriptElement;             // +0x214
    bool DebugScriptRunning;                    // +0x218
    double DebugScriptElementStartTime;         // +0x220
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
