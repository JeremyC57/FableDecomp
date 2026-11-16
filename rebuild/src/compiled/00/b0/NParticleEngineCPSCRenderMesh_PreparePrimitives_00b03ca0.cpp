// NParticleEngine::CPSCRenderMesh::PreparePrimitives(const float, const CEnginePrimitivePrepareInfo&, CParticleSystem&, const CParticleUpdateParameters<unsigned long, unsigned long>&, CEnginePrimitiveHandle&) -- per-frame mesh / trail primitives of a mesh particle system
// Grade: RELOCATION_MATCH (wfE8 10th agent).
// Inline-budget: scoped `#pragma inline_depth(0)` round `C3DVector right = rhs.GetRight();` -- retail calls CRightHandedSet::GetRight 0x0049CE00
//   out of line at this one site (0x00B049BB) and expands it elsewhere in the TU (sibling 0x00B01900); /d2inlT / /d2inlS sweeps cannot split it
//   (T>=1250 expand it, T<=1200 change resize / SetPrimitiveInfo decisions). The same scoped pragma round both `CRGBColour colours[3]` /
//   `trailColours[3]` declarations gives retail's `vector constructor iterator` 0x004038C0 over the out-of-line CRGBColour() 0x004048E0
//   (0x00B040EF / 0x00B042AC) with no header change.
// Same-TU callees (lesson wfB5): retail calls the header-inline ctors / accessors below out of line; their bodies (the landed rows' texts, as
//   the landed twin 0x00AFCC30 defines its ctors) must be visible so VC7.1 knows they do not capture the locals passed by reference:
//   CEnginePrimitiveParticleMesh ctor 0x00B058E0, CEnginePrimitiveParticleLine ctor 0x00AFE220, CEnginePrimitiveParticleMeshGroup ctor
//   0x00AFEAD0, CParticleSystem::ScaleParticleDimensions(float&) 0x00AF8F80, GetUnscaledWorldTransform 0x00B05840 / GetParticleWorldTransform
//   0x00B05980. With them trailStart = pos is copy-propagated into the first TransformPosIfLocal (0x00B05289 reads pos) and worldTransform
//   shares its frame slot with the CentredOnPos matrix (0x00B04A3C): frame 0x3C0 as retail.
// Header opt-ins: FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES + FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER (existing pair, landed
//   0x00A9BBE0; FITTED for this TU: with the default pair the first SetPrimitiveInfo's handle releases keep `mov edx,[ecx+4]; dec edx; mov
//   eax,edx` where retail has `dec [ecx+4]`, 0x00B04F14 / 0x00B04F38); FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM (existing).
// FITTED: `const C3DVector& scale = system.GetScale();` (CentredOnPos block) and the named `noiseLookup` (Fable Anniversary PDB local
//   noise_lookup) -- two ordinary named locals; with `timeSeconds` (Anniversary local time_seconds: retail calls GetInterpolatedLifetimerInSeconds
//   0x00AF85E0 and pops the result, 0x00B0456B) they set the x87 order of the first trail GetRecordedPos interpolation (0x00B050A8).
// Leaf: ?PreparePrimitives@CPSCRenderMesh@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Flags: /d2inlT1537 /d2inlS15
// Retail: Fable.exe (TLC, VC7.1) @ 0x00B03CA0, 6468 bytes, thiscall, ebp frame (`lea ebp,[esp-64h]`), `ret 14h`.
// Identity: slot 13 of vtable 0x0129F96C (FableWin.pdb ?PreparePrimitives@CPSCRenderMesh@NParticleEngine@@UAEXMABVCEnginePrimitivePrepareInfo@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z).
// Same TU as the matched CPSCRenderMesh::AddToBoundingSphere 0x00B01900 (wfD6): its class model, helpers, opt-ins and flags; idioms of the
//   landed twin CPSCRenderSprite::PreparePrimitives 0x00AFCC30.
// Retail-asm: 0x00B03D26 `fistp dword ptr [ebp+4Ch]` (GFFloatToLongNear) and 0x00B05042 `fistp dword ptr [ebp+4Ch]` (GFFloatToLongCeil):
//   VC7.1 emits no bare dword fistp from C++ (proven-impossible list).
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
#define FABLE_RETAIL_C3DVECTOR_SCALE_NAMED_RESULT
#define FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT
#define FABLE_RETAIL_GFUNPACKFLOAT_GROUP_PRODUCT
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR_SOURCE_DEST
#define FABLE_RETAIL_CQUATERNION_TORIGHTHANDEDSET_FLOAT
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
#define FABLE_RETAIL_CPARTICLESYSTEM_PARTICLEWORLDTRANSFORM
#define FABLE_RETAIL_CPARTICLESYSTEM_GETSCALE
#define FABLE_RETAIL_CPARTICLE_LIFETIMERINSECONDS_DECL
#define FABLE_RETAIL_C3DVECTOR_SUBTRACT_ASSIGN
#define FABLE_RETAIL_CENGINEPRIMITIVEPARTICLEMESH_CTOR
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_ASSIGN
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_RELEASE_DELETES
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_MEMBER
#define FABLE_RETAIL_CPARTICLESYSTEM_PEEKEMITTERTRANSFORM
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
extern float GFastCosTable[1025];       // 0x013CD550 (the PDB name engine/GFFastCos.h binds; one address, one name)
// GFFastArcCos / GFFastSin in the one-expression text of the landed rows 0x00AA3560 / 0x00AA3C50 / 0x00A4D920 / 0x00AA3390 (0x00AA0090 names a/b; that text keeps both table values in registers here, retail folds them); SetSlerpFromIdentity body as landed.
FABLE_FORCEINLINE float __fastcall GFFastArcCos(float cosine)
{
    cosine = (cosine + 1.0f) * 512.5f;
    long i = GFFloatToLongNear(cosine - 0.5f);
    float frac = cosine - (float)i;
    return ((1.0f - frac) * g_ArcCosTable[i] + frac * g_ArcCosTable[i + 1]) * 0.15915494f;
}
FABLE_FORCEINLINE float __fastcall GFFastSin(float angle)
{
    angle = angle * 1024.0f;
    long i = GFFloatToLongNear(angle - 0.5f);
    float frac = angle - (float)i;
    i -= 256;
    i &= 0x3ff;
    return (1.0f - frac) * GFastCosTable[i] + frac * GFastCosTable[i + 1];
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
FABLE_FORCEINLINE void CMatrix3x4::TransformVector(const C3DVector& pSource,C3DVector& destination) const
{
    destination.X=pSource.X*E11+pSource.Y*E21+pSource.Z*E31;
    destination.Y=pSource.X*E12+pSource.Y*E22+pSource.Z*E32;
    destination.Z=pSource.X*E13+pSource.Y*E23+pSource.Z*E33;
}
namespace NParticleEngine {
// Same TU (header inlines emitted out of line after the row: 0x00B05840 / 0x00B05980, retail calls 0x00B05980): the landed rows' texts,
// visible to this row.
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline void CParticleSystem::GetUnscaledWorldTransform(CMatrix3x4& result) const
{
    if (StayWithEmitter) {
        result = PeekEmitterTransform();
    } else {
        result.E12 = result.E13 = result.E21 = result.E23 = result.E31 = result.E32 =
            result.E41 = result.E42 = result.E43 = 0.0f;
        result.E11 = result.E22 = result.E33 = 1.0f;
    }
}
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline void CParticleSystem::GetParticleWorldTransform(CMatrix3x4& result) const
{
    GetUnscaledWorldTransform(result);
    if (ScaleParticles) {
        result.E11 *= Scale.X;
        result.E22 *= Scale.Y;
        result.E33 *= Scale.Z;
    }
}
// Retail calls 0x00AF8F80 (CParticleSystem::ScaleParticleDimensions(float&), the twin's text).
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline void CParticleSystem::ScaleParticleDimensions(float& dim)const{
    if(ScaleParticles){float scale=((Scale.X>Scale.Y?Scale.X:Scale.Y)>Scale.Z?(Scale.X>Scale.Y?Scale.X:Scale.Y):Scale.Z);dim*=scale;}
}
// Same TU (template COMDAT, retail calls it out of line at 0x00B059F0): the landed row's text, visible to this row.
template<class K,class V>
#ifdef _MSC_VER
__declspec(noinline)
#endif
bool CParticleUpdateParameters<K,V>::GetVectorParam(C3DVector& vector,const K& key)const
{
    if (key) {
        typename CVectorMap<K,C3DVector,CParticleUpdateCCompareVector<K> >::const_iterator it = VectorParams.Find(key);
        if (it != VectorParams.end()) {
            vector = it->second;
            return true;
        }
    }
    return false;
}
}
extern const float DELTA_NOTIONAL_ZERO;                 // 0x0129BA3C
#include "engine/CParticleEmitterManager.h"
#include "engine/CEnginePrimitiveParticleMesh.h"
#include "engine/CEnginePrimitiveParticleLine.h"
// Same TU (header inline, retail calls 0x00AFE220 out of line): the landed text (as the twin 0x00AFCC30 defines it), visible to this row.
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline CEnginePrimitiveParticleLine::CEnginePrimitiveParticleLine(unsigned __int64 uid,const C3DVector& start,
    const C3DVector& end,const CRGBColour& colour,float width,EEngineInterpolationSet interp_set)
    :CEnginePrimitive((EEnginePrimitiveType)0x15,uid,interp_set),Start(start),End(end),Colour(colour),Width(width){}
// Same TU (header inline, emitted out of line at 0x00B058E0 after the row): the landed row's text, visible to this row.
inline __declspec(noinline) CEnginePrimitiveParticleMesh::CEnginePrimitiveParticleMesh(unsigned __int64 uid, const C3DVector& world_pos, const CRightHandedSet& rh_set,
                                                                  const C3DVector& size, const CRGBColour& colour, EEngineInterpolationSet interp_set)
    : CEnginePrimitive((EEnginePrimitiveType)0x17, uid, interp_set)
    , WorldPos(world_pos)
    , Size(size)
    , RightHandedSet(rh_set)
    , Colour(colour)
{
}
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/GFFastCos.h"
#include "engine/CEngineRenderEnums.h"
CRGBColour GFGetInterpolatedColour(CRGBColour,CRGBColour,unsigned long);   // 0x00A44250
// No shared header: CEnginePrimitiveParticleMeshGroup (ctor 0x00AFEAD0, out of line) and CEnginePrimitiveParticleLineGroup (type 0x1A,
// ctor inline: FableWin ??0CEnginePrimitiveParticleLineGroup@@QAE@KGW4EEnginePrimitiveBlendMode@@W4EEnginePrimitiveBlendOp@@@Z).
class CEnginePrimitiveParticleMeshGroup : public CEnginePrimitive { public:
    unsigned long BankIndex;
    CMatrix3x4 WorldMat;
    EEnginePrimitiveBlendMode BlendMode;
    EEnginePrimitiveBlendOp BlendOp;
    CEnginePrimitiveParticleMeshGroup(unsigned long bank_index, const CMatrix3x4& world_mat, EEnginePrimitiveBlendMode blend_mode, EEnginePrimitiveBlendOp blend_op);   // 0x00AFEAD0
};
// Retail calls 0x00AFEAD0 out of line: the landed row's text, visible to this row.
#ifdef _MSC_VER
__declspec(noinline)
#endif
inline CEnginePrimitiveParticleMeshGroup::CEnginePrimitiveParticleMeshGroup(unsigned long bank_index, const CMatrix3x4& world_mat,
    EEnginePrimitiveBlendMode blend_mode, EEnginePrimitiveBlendOp blend_op)
    : CEnginePrimitive((EEnginePrimitiveType)0x1c, 0, (EEngineInterpolationSet)0), BankIndex(bank_index), WorldMat(world_mat), BlendMode(blend_mode), BlendOp(blend_op) { }
class CEnginePrimitiveParticleLineGroup : public CEnginePrimitive { public:
    unsigned long BankIndex;                            // +0x0c
    unsigned short Flags;                               // +0x10
    EEnginePrimitiveBlendMode BlendMode;                // +0x14
    EEnginePrimitiveBlendOp BlendOp;                    // +0x18
    CEnginePrimitiveParticleLineGroup(unsigned long bank_index, unsigned short flags, EEnginePrimitiveBlendMode blend_mode, EEnginePrimitiveBlendOp blend_op)
        : CEnginePrimitive((EEnginePrimitiveType)0x1a, 0, (EEngineInterpolationSet)0), BankIndex(bank_index), Flags(flags), BlendMode(blend_mode), BlendOp(blend_op) { }
};
namespace NParticleEngine {
const CMeshDataBank& FABLE_FASTCALL GetMeshBank();      // 0x00AEAAB0
long FABLE_FASTCALL GetTickRate();                      // 0x00AEA9F0
float FABLE_FASTCALL GetOneOverTickRate();              // 0x00AEAA00
CParticleEmitterManager& FABLE_FASTCALL GetParticleEmitterManager();   // 0x00AEAAF0
inline float FABLE_FASTCALL GFSmoothFadeValue(float value){return 0.5f-GFFastCos(value*0.5f)*0.5f;}
class CPSCRenderMesh : public CPSCBase {
public:
    long BankIndex;                                     // +0x0c
    long TrailBankIndex;                                // +0x10
    CRGBColour TrailStartColour;                        // +0x14
    CRGBColour TrailMidColour;                          // +0x18
    CRGBColour TrailEndColour;                          // +0x1c
    CRGBColour StartColour;                             // +0x20
    CRGBColour MidColour;                               // +0x24
    CRGBColour EndColour;                               // +0x28
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
    virtual void PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                   const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group);
};
void CPSCRenderMesh::PreparePrimitives(const float interpolate, const CEnginePrimitivePrepareInfo& info, CParticleSystem& system,
                                       const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& system_group)
{
    if (BankIndex > 0 && GetMeshBank().PeekMesh(BankIndex)->GetDataType() == 4) {
        const float startSizeX = GFUnpackFloat(StartRenderSizeX, 11, 0.0f, 20.0f, 2);
        const float startSizeY = GFUnpackFloat(StartRenderSizeY, 11, 0.0f, 20.0f, 2);
        const float startSizeZ = GFUnpackFloat(StartRenderSizeZ, 11, 0.0f, 20.0f, 2);
        const float endSizeX = GFUnpackFloat(EndRenderSizeX, 11, 0.0f, 20.0f, 2);
        const float endSizeY = GFUnpackFloat(EndRenderSizeY, 11, 0.0f, 20.0f, 2);
        const float endSizeZ = GFUnpackFloat(EndRenderSizeZ, 11, 0.0f, 20.0f, 2);
        const float alphaFadeMinimum = GFUnpackFloat(AlphaFadeMinimum, 7, 0.0f, 1.0f, 2);
        const float sizeFadeMinimum = GFUnpackFloat(SizeFadeMinimum, 7, 0.0f, 1.0f, 2);
        const float flickerSpeed = GFUnpackFloat(FlickerSpeed, 12, 0.0f, 30.0f, 2);
        const float flickerBias = GFUnpackFloat(FlickerBias, 8, -1.0f, 1.0f, 2);
        const float trailWidth = GFUnpackFloat(TrailWidth, 10, 0.0f, 10.0f, 2);
        const float tickRate = (float)GetTickRate();
        const float fadeInEnd = FadeInEndInteger * 0.001f;
        const float fadeOutBegin = FadeOutBeginInteger * 0.001f;
        const float fadeAlpha = system.GetFadeAlpha();
        const float flickerRange = (float)fabs(flickerBias) + 0.5f;
        EEnginePrimitiveBlendMode blendMode = (EEnginePrimitiveBlendMode)BlendMode;
        EEnginePrimitiveBlendOp blendOp = (EEnginePrimitiveBlendOp)BlendOp;
        CRGBColour colour;
#pragma inline_depth(0)
        CRGBColour colours[3];
#pragma inline_depth()
        long trailPrimIndex = 0;
        if (BankIndex) {
            CMatrix3x4 worldTransform;
            system.GetParticleWorldTransform(worldTransform);
            info.Engine->SetChildPrimitive(system_group, ChildPrimIndex, PrimHandle,
                CEnginePrimitiveParticleMeshGroup(BankIndex, worldTransform, blendMode, blendOp), 0);
            trailPrimIndex = 1;
            colours[0] = UseStartColour ? StartColour : UseMidColour ? MidColour : UseEndColour ? EndColour : CRGBColour(255, 255, 255, 255);
            colours[2] = UseEndColour ? EndColour : UseMidColour ? MidColour : UseStartColour ? StartColour : CRGBColour(255, 255, 255, 255);
            colours[1] = UseMidColour ? MidColour : GFGetInterpolatedColour(colours[0], colours[2], 128);
            colours[0].A = (unsigned char)(colours[0].A * fadeAlpha);
            colours[1].A = (unsigned char)(colours[1].A * fadeAlpha);
            colours[2].A = (unsigned char)(colours[2].A * fadeAlpha);
        }
        EEnginePrimitiveBlendMode trailBlendMode = (EEnginePrimitiveBlendMode)TrailBlendMode;
        EEnginePrimitiveBlendOp trailBlendOp = (EEnginePrimitiveBlendOp)TrailBlendOp;
#pragma inline_depth(0)
        CRGBColour trailColours[3];
#pragma inline_depth()
        if (TrailBankIndex) {
            trailColours[0] = TrailUseStartColour ? TrailStartColour : TrailUseMidColour ? TrailMidColour : TrailUseEndColour ? TrailEndColour : CRGBColour(255, 255, 255, 255);
            trailColours[2] = TrailUseEndColour ? TrailEndColour : TrailUseMidColour ? TrailMidColour : TrailUseStartColour ? TrailStartColour : CRGBColour(255, 255, 255, 255);
            trailColours[1] = TrailUseMidColour ? TrailMidColour : GFGetInterpolatedColour(trailColours[0], trailColours[2], 128);
            info.Engine->SetChildPrimitive(system_group, ChildTrailPrimIndex, TrailPrimHandle,
                CEnginePrimitiveParticleLineGroup(TrailBankIndex, 0, trailBlendMode, trailBlendOp), 0);
        }
        for (CParticleSystem::ParticleIterator it = system.GetParticleList().begin(); it != system.GetParticleList().end(); ++it) {
            CParticle* particle = *it;
            float lifetimer = particle->GetInterpolatedLifetimer(interpolate);
            if (particle->IsActive() && particle->GetMaxLife() != 0) {
                float lifeSeconds = GetOneOverTickRate() * lifetimer;
                float life = lifetimer / particle->GetMaxLife();
                if (life < 0.0f)
                    life = 0.0f;
                else if (life > 1.0f)
                    life = 1.0f;
                float sizeLife = life;
                if (sizeLife < 0.0f)
                    sizeLife = 0.0f;
                else {
                    float limit = 1.0f - DELTA_NOTIONAL_ZERO;
                    if (sizeLife > limit)
                        sizeLife = limit;
                }
                unsigned long colourPos = GFFloatToLongNear(life * 511.0f);
                unsigned long colourIndex = colourPos >> 8;
                unsigned long colourFrac = colourPos & 0xff;
                colour = GFGetInterpolatedColour(colours[colourIndex], colours[colourIndex + 1], colourFrac);
                float timeSeconds = particle->GetInterpolatedLifetimerInSeconds(interpolate);   // unused (retail calls 0x00AF85E0 and pops the result)
                CQuaternion orientation = particle->GetInterpolatedOrientation(interpolate);
                CRightHandedSet rhs;
                orientation.ToRightHandedSet(rhs);
                const C3DBoundingSphere meshSphere = GetMeshBank().PeekMeshInfo(BankIndex)->GetBoundingSphere();
                float meshScale = 1.0f / (meshSphere.Radius * 0.01f);
                C3DVector size;
                if (!UseRenderSizeParam || !params.GetVectorParam(size, RenderSizeParam)) {
                    size.X = ((endSizeX - startSizeX) * sizeLife + startSizeX) * meshScale;
                    size.Y = ((endSizeY - startSizeY) * sizeLife + startSizeY) * meshScale;
                    size.Z = ((endSizeZ - startSizeZ) * sizeLife + startSizeZ) * meshScale;
                }
                C3DVector pos = particle->GetInterpolatedPos(interpolate);
                if (CentredOnPos) {
                    C3DVector forward = rhs.GetForward();
                    C3DVector up = rhs.GetUp();
#pragma inline_depth(0)
                    C3DVector right = rhs.GetRight();
#pragma inline_depth()
                    const C3DVector& scale = system.GetScale();
                    right *= size.X * scale.X * 0.01f;
                    forward *= size.Y * scale.Y * 0.01f;
                    up *= size.Z * scale.Z * 0.01f;
                    CMatrix3x4 matrix;
                    matrix.E11 = -right.X; matrix.E12 = -right.Y; matrix.E13 = -right.Z;
                    matrix.E21 = -forward.X; matrix.E22 = -forward.Y; matrix.E23 = -forward.Z;
                    matrix.E31 = up.X; matrix.E32 = up.Y; matrix.E33 = up.Z;
                    C3DVector offset;
                    matrix.TransformVector(meshSphere.Center, offset);
                    pos -= offset;
                }
                float alpha = system.GetGlobalAlpha();
                float sizeScale = 1.0f;
                if (life < fadeInEnd)
                    alpha *= GFSmoothFadeValue(life / fadeInEnd);
                if (life > fadeOutBegin)
                    alpha *= GFSmoothFadeValue((1.0f - life) / (1.0f - fadeOutBegin));
                float alphaScale = 1.0f;
                if (alpha < 1.0f) {
                    if (AlphaFadeEnable)
                        alphaScale = (1.0f - alphaFadeMinimum) * alpha + alphaFadeMinimum;
                    if (SizeFadeEnable)
                        sizeScale = (1.0f - sizeFadeMinimum) * alpha + sizeFadeMinimum;
                }
                if (FlickerEnable) {
                    const NFractal::CPerlinNoise& noise = GetParticleEmitterManager().PeekPerlinNoise();
                    C2DVector noiseLookup(lifeSeconds * flickerSpeed + particle->GetFlickerTimeOffset(), particle->GetFlickerIndex());
                    float flicker = noise.GetGaussian2DNoise(noiseLookup) * flickerRange + flickerBias + 0.5f;
                    if (flicker < 0.0f)
                        flicker = 0.0f;
                    else if (flicker > 1.0f)
                        flicker = 1.0f;
                    float minAlpha = FlickerMinAlphaInteger * 0.001f;
                    alphaScale *= (1.0f - minAlpha) * flicker + minAlpha;
                    float minSize = FlickerMinSizeInteger * meshScale * 0.001f;
                    sizeScale *= (1.0f - minSize) * flicker + minSize;
                }
                size *= sizeScale;
                colour.A = (unsigned char)GFFloatToLongNear(colour.A * alphaScale);
                if (colour.A > 0) {
                    long id;
                    if (particle->GetPrimID(0, 0, id))
                        info.Engine->SetChildPrimitive(PrimHandle, id,
                            CEnginePrimitiveParticleMesh(0, pos, rhs, size, colour, (EEngineInterpolationSet)0), 0);
                    else {
                        id = info.Engine->AddChildPrimitive(PrimHandle,
                            CEnginePrimitiveParticleMesh(0, pos, rhs, size, colour, (EEngineInterpolationSet)0), 0);
                        particle->SetPrimitiveInfo(0, 0, PrimHandle, id);
                    }
                    if (TrailBankIndex) {
                        colour = GFGetInterpolatedColour(trailColours[colourIndex], trailColours[colourIndex + 1], colourFrac);
                        float trailLength = TrailLengthInteger * 0.001f;
                        C3DVector trailStart = pos;
                        C3DVector trailEnd;
                        if (trailLength > 2.0f) {
                            float trailScale = trailLength * 0.5f;
                            trailEnd = particle->GetRecordedPos(2.0f, interpolate);
                            trailEnd = (trailEnd - pos) * trailScale + pos;
                        } else
                            trailEnd = particle->GetRecordedPos(trailLength, interpolate);
                        system.TransformPosIfLocal(trailStart);
                        system.TransformPosIfLocal(trailEnd);
                        float width = sizeScale * meshScale * trailWidth;
                        system.ScaleParticleDimensions(width);
                        if (particle->GetPrimID(trailPrimIndex, 0, id))
                            info.Engine->SetChildPrimitive(TrailPrimHandle, id,
                                CEnginePrimitiveParticleLine(0, trailStart, trailEnd, colour, width, (EEngineInterpolationSet)0), 0);
                        else {
                            id = info.Engine->AddChildPrimitive(TrailPrimHandle,
                                CEnginePrimitiveParticleLine(0, trailStart, trailEnd, colour, width, (EEngineInterpolationSet)0), 0);
                            particle->SetPrimitiveInfo(trailPrimIndex, 0, TrailPrimHandle, id);
                        }
                    }
                }
            } else if (lifetimer > particle->GetMaxLife())
                system.ClearParticle(it);
        }
    } else
        ClearAllPrimitives(info.Engine, system_group);
}
}