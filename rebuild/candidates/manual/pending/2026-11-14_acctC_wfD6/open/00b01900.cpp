// NParticleEngine::CPSCRenderMesh::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params) -- vtable slot 19: grow the emitter's bounding sphere over every live mesh particle and its trail
// Grade: DIFFER(3749v3777) -- shared-header port (successor 2), 19 structural hunks: frame 0x298 v 0x2B0, loop-head pad at 0x00B01C38, the x87 trail-end / matrix blocks 0x00B025A1..0x00B026A3 (TransformPoint term orders, the !CentredOnPos Set(-x,-y,z) block with fchs), GetInterpolatedLifetimer operand order 0x00B026FF, 0x00B02493 fdivr/fld use a qword constant in ours (sqrt / 1.0 double).
// Earlier lane Grade: DIFFER(3755v3777) on local stand-in models (frame, pad, Z rows, magnitude load order, matrix block).
// Leaf: ?AddToBoundingSphere@CPSCRenderMesh@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z
// Flags: /d2inlT1537 /d2inlS15
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B01900, 3777 bytes, thiscall, `ret 10h`, returns bool.
// Identity: FableWin.pdb ?AddToBoundingSphere@CPSCRenderMesh@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z;
//   slot 19 of vtable 0x0129F96C (CPSCBase's slot 19 is AddToBoundingSphere 0x00AF2A30; the CPSCLight twin 0x00B18AF0 is landed); `ret 10h`
//   matches the four stack arguments.
// Shape: unpack the start / end sizes and the trail width (GFRoundFloatNear), trail length = TrailLengthInteger * 0.001 (recorded positions
//   capped at 2, the rest scaled); the mesh's bounding sphere from GetMeshBank().PeekMeshInfo(BankIndex). Per active particle: the
//   interpolated position (system transform when the particles stay with the emitter); the trail end from GetRecordedPos and the trail
//   segment added as a sphere; the mesh size (RenderSizeParam via GetVectorParam, else the life lerp; ScaleParticles), its orientation
//   (GetInterpolatedOrientation -> ToRightHandedSet) placing the mesh sphere centre unless CentredOnPos; dead particles past MaxLife are
//   cleared. Nothing added: both primitive handles are cleared.
// Callees: C3DBoundingSphere::AddToBounds 0x00ADDD80, GetMeshBank 0x00AEAAB0, CMeshDataBank::PeekMeshInfo 0x00A24260,
//   CParticleSystem::GetSystemTransform 0x00AF2250 / ClearParticle 0x00AF5090, CParticleUpdateParameters::GetVectorParam 0x00B059F0,
//   CQuaternion::operator* 0x00A88B60.
// Attempts: b1..b19 (LOG.tsv); best b8/b16/b19 family. Tried: __forceinline particle helpers (needed), sequential C3DVector member
//   stores, a user C3DVector copy ctor (dl 704 -> 468), Z-row order x,y,z (fixes the first TransformPoint), `trail = trailEnd - pos`
//   assignment (fixes the temp copy), all six magnitude orders (load order unchanged: X first), axis statements before Set (b16),
//   CMatrix3x4::operator* with a zero translation and `pos +=` (b17/b18, worse).
// Next idea: model the mesh offset as CMatrix3x4::TransformVector + `pos +=` with per-member negation (the shape that fixed the matrix
//   block of PreparePrimitives 0x00B03CA0, p8), then retune the trail-end transforms.
// Helpers: GFFloatToLongNear (fistp in GFRoundFloatNear / GFFastArcCos / GFFastSin), GFFastFloatToLong (CParticle::IsActive,
//   0x00B01C4D..0x00B01C65), GFFloatToLongCeil (GetRecordedPos, 0x00B01DAA..0x00B01DD0 / 0x00B01F9D..0x00B01FBD).
// Retail-asm: 0x00B01956 `fistp dword ptr [ebp+54h]` (GFFloatToLongNear) and the bare dword fistp of GFFastFloatToLong /
//   GFFloatToLongCeil above: VC7.1 emits no bare dword fistp from C++ (proven-impossible list).
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMESHDATABANKENTRY
#define FABLE_RETAIL_CBANKFILE_ENTRYQUERY
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_ZYX_SUMS
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CQUATERNION_MUL
#define FABLE_RETAIL_CMESHDATABANK_BASES
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_SCOPED_PTR_RESET
#define FABLE_RETAIL_CMESHDATABANK_PEEKINFO
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE
#define FABLE_RETAIL_STLP_VECTOR_RESIZE_ONE_DECL
#define FABLE_RETAIL_CQUATERNION_SLERPFROMIDENTITY
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_ADDTOBOUNDS
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGBOX
#define FABLE_RETAIL_C3DMESHINFO_BOUNDINGSPHERE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLESYSTEM
#define FABLE_RETAIL_CPARTICLESYSTEM_UPDATE
#define FABLE_RETAIL_CPARTICLESYSTEM_SCALEVECTOR
#define FABLE_RETAIL_CPARTICLE_SPRITE
#define FABLE_RETAIL_CPARTICLE_INTERPOLATEDORIENTATION
#define FABLE_RETAIL_CRESOURCELIST
#include <math.h>
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CQuaternion.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DBoundingSphere.h"
#include "engine/GFFloatToLongNear.h"
#include "engine/GFFastFloatToLong.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFPackFloat.h"
extern float g_ArcCosTable[1026];       // 0x013CB530
extern float g_CosTable[1025];          // 0x013CD550
// The landed 0x00AA0090 texts of the inline helpers SetSlerpFromIdentity expands (GFFastArcCos / GFFastSin) and its own body.
FABLE_FORCEINLINE float __fastcall GFFastArcCos(float cosine)
{
    cosine = cosine * 512.0f + 512.0f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    float a = g_ArcCosTable[i];
    float b = g_ArcCosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
}
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    float a = g_CosTable[i];
    float b = g_CosTable[i + 1];
    return (1.0f - frac) * a + frac * b;
}
FABLE_FORCEINLINE void CQuaternion::SetSlerpFromIdentity(const CQuaternion& q, float t)
{
    float cosom = q.W;
    float sign = 1.0f;
    if (cosom < 0.0f) {
        sign = -1.0f;
        cosom = -cosom;
    }
    float scale0;
    float scale1;
    if (1.0f - cosom > 0.05f) {
        float omega = GFFastArcCos(cosom);
        float sinom = (float)sqrt(1.0f - cosom * cosom);
        scale0 = GFFastSin((1.0f - t) * omega) * (1.0f / sinom);
        scale1 = GFFastSin(omega * t) * (1.0f / sinom);
    }
    else {
        scale0 = 1.0f - t;
        scale1 = t;
    }
    scale1 *= sign;
    X = scale1 * q.X;
    Y = scale1 * q.Y;
    Z = scale1 * q.Z;
    W = scale1 * q.W + scale0;
}
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CMeshDataBank.h"
#include "engine/C3DMeshInfo.h"
#include "engine/CParticle.h"
#include "engine/CParticleSystem.h"
#include "engine/CPSCBase.h"
#include "engine/CParticleUpdateParameters.h"
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C
namespace NParticleEngine {
const CMeshDataBank& FABLE_FASTCALL GetMeshBank();      // 0x00AEAAB0
class CPSCRenderMesh : public CPSCBase {
public:
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    unsigned long Colours[6];                           // +0x14
    unsigned long BlendMode : 3;                        // +0x2c
    unsigned long TrailBlendMode : 3;
    unsigned long BlendOp : 3;
    unsigned long TrailBlendOp : 3;
    unsigned long FadeInEndInteger : 10;
    unsigned long FadeOutBeginInteger : 10;
    unsigned long TrailLengthInteger : 12;              // +0x30
    unsigned long FlickerMinAlphaInteger : 10;
    unsigned long FlickerMinSizeInteger : 10;
    unsigned long StartRenderSizeX : 11;                // +0x34
    unsigned long StartRenderSizeY : 11;
    unsigned long StartRenderSizeZ : 11;                // +0x38
    unsigned long CentredOnPos : 1;
    unsigned long AlphaFadeEnable : 1;
    unsigned long EndRenderSizeX : 11;
    unsigned long EndRenderSizeY : 11;                  // +0x3c
    unsigned long EndRenderSizeZ : 11;
    unsigned long SizeFadeEnable : 1;
    unsigned long FlickerEnable : 1;
    unsigned long TrailUseStartColour : 1;
    unsigned long TrailUseMidColour : 1;
    unsigned long TrailUseEndColour : 1;
    unsigned long UseRenderSizeParam : 1;
    unsigned long FlickerBias : 8;                      // +0x40
    unsigned long FlickerSpeed : 12;
    unsigned long AlphaFadeMinimum : 7;
    unsigned long UseStartColour : 1;
    unsigned long SizeFadeMinimum : 7;                  // +0x44
    unsigned long UseMidColour : 1;
    unsigned long TrailWidth : 10;
    unsigned long UseEndColour : 1;
    unsigned long RenderSizeParam;                      // +0x48
    CEnginePrimitiveHandle PrimHandle;                 // +0x4c
    long ChildPrimIndex;                                // +0x54
    CEnginePrimitiveHandle TrailPrimHandle;            // +0x58
    long ChildTrailPrimIndex;                           // +0x60
    virtual bool AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params);
};
bool CPSCRenderMesh::AddToBoundingSphere(C3DBoundingSphere& sphere, float timeOffset, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params)
{
    C3DVector startSize;
    startSize.X = GFUnpackFloat(StartRenderSizeX, 11, 0.0f, 20.0f, 2);
    startSize.Y = GFUnpackFloat(StartRenderSizeY, 11, 0.0f, 20.0f, 2);
    startSize.Z = GFUnpackFloat(StartRenderSizeZ, 11, 0.0f, 20.0f, 2);
    C3DVector endSize;
    endSize.X = GFUnpackFloat(EndRenderSizeX, 11, 0.0f, 20.0f, 2);
    endSize.Y = GFUnpackFloat(EndRenderSizeY, 11, 0.0f, 20.0f, 2);
    endSize.Z = GFUnpackFloat(EndRenderSizeZ, 11, 0.0f, 20.0f, 2);
    float trailWidth = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
    float trailLength = (float)TrailLengthInteger * 0.001f;
    bool longTrail;
    float recordedLength;
    if (trailLength > 2.0f) {
        recordedLength = 2.0f;
        longTrail = true;
    } else {
        recordedLength = trailLength;
        longTrail = false;
    }
    float trailScale = recordedLength * 0.5f;
    float meshScale = 0.0f;
    C3DBoundingSphere meshSphere;
    if (BankIndex) {
        meshSphere = GetMeshBank().PeekMeshInfo(BankIndex)->GetBoundingSphere();
        meshScale = 1.0f / meshSphere.Radius;
    }
    bool added = false;
    for (std::list<CParticle*>::iterator it = system.GetParticleList().begin(); it != system.GetParticleList().end(); ++it) {
        CParticle* particle = *it;
        if (particle->IsActive() && particle->IsActive(timeOffset)) {
            C3DVector pos = particle->GetInterpolatedPos(timeOffset);
            system.TransformPosIfLocal(pos);
            if (TrailBankIndex) {
                C3DVector trailEnd;
                if (longTrail) {
                    trailEnd = particle->GetRecordedPos(2.0f, timeOffset);
                    system.TransformPosIfLocal(trailEnd);
                    trailEnd = (trailEnd - pos) * trailScale + pos;
                } else {
                    trailEnd = particle->GetRecordedPos(trailLength, timeOffset);
                    system.TransformPosIfLocal(trailEnd);
                }
                C3DVector trail;
                trail = trailEnd - pos;
                float length = trail.GetMagnitude();
                if (trailWidth > length)
                    length = trailWidth;
                sphere.AddToBounds(trail * 0.5f + pos, length * 0.5f);
                added = true;
            }
            if (BankIndex) {
                C3DVector size;
                if (!UseRenderSizeParam || !params.GetVectorParam(size, RenderSizeParam)) {
                    float life = particle->GetInterpolatedLifetimer(timeOffset) / particle->GetMaxLife();
                    if (life < 0.0f)
                        life = 0.0f;
                    else {
                        float limit = 1.0f - DELTA_NOTIONAL_ZERO;
                        if (life > limit)
                            life = limit;
                    }
                    size.X = (endSize.X - startSize.X) * life + startSize.X;
                    size.Y = (endSize.Y - startSize.Y) * life + startSize.Y;
                    size.Z = (endSize.Z - startSize.Z) * life + startSize.Z;
                }
                system.ScaleParticleDimensions(size);
                if (!CentredOnPos) {
                    CQuaternion orientation = particle->GetInterpolatedOrientation(timeOffset);
                    CRightHandedSet rhs;
                    orientation.ToRightHandedSet(rhs);
                    C3DVector right = rhs.GetRight();
                    CMatrix3x4 matrix;
                    C3DVector xAxis = right * (size.X * meshScale);
                    C3DVector yAxis = rhs.GetForward() * (size.Y * meshScale);
                    C3DVector zAxis = rhs.GetUp() * (size.Z * meshScale);
                    matrix.Set(-xAxis, -yAxis, zAxis, pos);
                    matrix.TransformPoint(meshSphere.Center, pos);
                }
                sphere.AddToBounds(pos, size.GetMagnitude() * 0.5f);
                added = true;
            }
        } else if (particle->GetInterpolatedLifetimer(timeOffset) > particle->GetMaxLife()) {
            system.ClearParticle(it);
        }
    }
    if (!added) {
        PrimHandle.Clear();
        TrailPrimHandle.Clear();
    }
    return added;
}
}
