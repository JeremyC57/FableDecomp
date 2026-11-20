// NParticleEngine::CPSCRenderMesh::Initialise(CIEngine*, CParticleSystem&, const CParticleUpdateParameters<K,K>&, CEnginePrimitiveHandle&) -- create the mesh child primitive (type 0x1c, identity transform) and the trail line-group child primitive (type 0x1a) once
// Leaf: ?Initialise@CPSCRenderMesh@NParticleEngine@@UAE_NPAVCIEngine@@AAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AFF0A0, 266 bytes (+ int3 padding), `ret 10h`; CPSCRenderMesh vtable 0x0129F96C slot 28.
// Name: Ego_r.pdb 0x00AC0350 / FableWin.pdb 0x02F30B60 ?Initialise@CPSCRenderMesh@NParticleEngine@@UAE_NPAVCIEngine@@AAVCParticleSystem@2@
//   ABV?$CParticleUpdateParameters@KK@2@AAVCEnginePrimitiveHandle@@@Z (the CPSCBase virtual, engine/CPSCBase.h). The catalog's
//   "CPSCRenderSprite::Initialise" is a propagated name; the vtable slot and the offsets are CPSCRenderMesh's (the sprite one is 0x00AF9310).
// Callees: CEnginePrimitiveParticleMeshGroup(unsigned long, const CMatrix3x4&, EEnginePrimitiveBlendMode, EEnginePrimitiveBlendOp)
//   0x00AFEAD0 (Ego_r.pdb / FableWin.pdb), CIEngine::AddChildPrimitive (vtable slot 26, +0x68; engine/CIEngine.h).
// Layout: the 0x00B03CA0 CPSCRenderMesh model (Ego_r.pdb; header-less class on the shared CPSCBase), BankIndex +0x0c, TrailBankIndex
//   +0x10, the render bit-field word +0x2c, ChildPrimIndex +0x54, ChildTrailPrimIndex +0x60; the two create records are the
//   header-less CEnginePrimitiveParticleMeshGroup (ctor out of line) / CEnginePrimitiveParticleLineGroup (ctor inline) of 0x00B03CA0.
// Shape: the mesh record is a temporary built by its out-of-line ctor inside the create call (engine vtable fetched first),
//   from an identity matrix in its own block (top-down zero fill, then the diagonal); the trail record is a block-local temporary.
// NF192 (q4795): ported onto the shared engine/CIEngine.h / CPSCBase.h / CMatrix3x4.h / CEnginePrimitive.h (was a row-local
//   SPrimitiveCreateInfo / SMeshCreateInfo / STrailCreateInfo family, a placeholder CIEngine v00..v25, CMatrix3x4 and CPSCBase models);
//   0x00AFEAD0 now binds ??0CEnginePrimitiveParticleMeshGroup@@QAE@KABVCMatrix3x4@@W4EEnginePrimitiveBlendMode@@W4EEnginePrimitiveBlendOp@@@Z.
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
#define FABLE_RETAIL_CMATRIX3X4_SETIDENTITY
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
#include "engine/C3DVector.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CParticleSystem.h"
#include "engine/CPSCBase.h"
#include "engine/CParticleUpdateParameters.h"
#include "engine/CRGBColour.h"
#include "engine/CEnginePrimitive.h"
#include "engine/CIEngine.h"
#include "engine/CEngineRenderEnums.h"
// No shared header (as in the landed 0x00B03CA0): the two particle-group create records.
class CEnginePrimitiveParticleMeshGroup : public CEnginePrimitive { public:
    unsigned long BankIndex;
    CMatrix3x4 WorldMat;
    EEnginePrimitiveBlendMode BlendMode;
    EEnginePrimitiveBlendOp BlendOp;
    CEnginePrimitiveParticleMeshGroup(unsigned long bank_index, const CMatrix3x4& world_mat, EEnginePrimitiveBlendMode blend_mode, EEnginePrimitiveBlendOp blend_op);   // 0x00AFEAD0
};
class CEnginePrimitiveParticleLineGroup : public CEnginePrimitive { public:
    unsigned long BankIndex;                            // +0x0c
    unsigned short Flags;                               // +0x10
    EEnginePrimitiveBlendMode BlendMode;                // +0x14
    EEnginePrimitiveBlendOp BlendOp;                    // +0x18
    CEnginePrimitiveParticleLineGroup(unsigned long bank_index, unsigned short flags, EEnginePrimitiveBlendMode blend_mode, EEnginePrimitiveBlendOp blend_op)
        : CEnginePrimitive((EEnginePrimitiveType)0x1a, 0, (EEngineInterpolationSet)0), BankIndex(bank_index), Flags(flags), BlendMode(blend_mode), BlendOp(blend_op) { }
};
namespace NParticleEngine {
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
    virtual bool Initialise(CIEngine* engine, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& parent);
};

bool CPSCRenderMesh::Initialise(CIEngine* engine, CParticleSystem& system, const CParticleUpdateParameters<unsigned long, unsigned long>& params, CEnginePrimitiveHandle& parent)
{
    if (engine) {
        if (BankIndex && ChildPrimIndex == -1) {
            CMatrix3x4 identity;
            identity.SetIdentity();
            EEnginePrimitiveBlendMode blendMode = (EEnginePrimitiveBlendMode)BlendMode;
            EEnginePrimitiveBlendOp blendOp = (EEnginePrimitiveBlendOp)BlendOp;
            ChildPrimIndex = engine->AddChildPrimitive(parent,
                CEnginePrimitiveParticleMeshGroup(BankIndex, identity, blendMode, blendOp), 0);
        }
        if (TrailBankIndex && ChildTrailPrimIndex == -1) {
            ChildTrailPrimIndex = engine->AddChildPrimitive(parent,
                CEnginePrimitiveParticleLineGroup(TrailBankIndex, 0, (EEnginePrimitiveBlendMode)TrailBlendMode, (EEnginePrimitiveBlendOp)TrailBlendOp), 0);
        }
        Initialised = true;
    }
    return Initialised;
}
}
