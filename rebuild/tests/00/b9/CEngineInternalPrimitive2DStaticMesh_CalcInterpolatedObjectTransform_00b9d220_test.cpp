// 00b9d220 CEngineInternalPrimitive2DStaticMesh::CalcInterpolatedObjectTransform
// Leaf: ?CalcInterpolatedObjectTransform@CEngineInternalPrimitive2DStaticMesh@@UAEXM_N@Z
// Flags:
// Inline-budget: native 00B9D220 expands both matrix multiplies and their SetMultiply helper. This uses the same shared inline forms as the verified 00BEB6C0 transform.
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_ENUM_CTOR
#define FABLE_RETAIL_CMATRIX3X4_SETTRANSLATEONLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#define FABLE_RETAIL_STATIC_2D_TRANSFORM_PASS30
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DSTATICMESH
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVESTATICMESH
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_C4DVECTOR
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineInternalPrimitive2DStaticMesh.h"

void CEngineInternalPrimitive2DStaticMesh::CalcInterpolatedObjectTransform(float interp, bool use_scaling)
{
    float scale = use_scaling ? Scale : 1.0f;
    CMatrix3x4 trans_centre_to_o(MATRIX34_INIT_IDENTITY);
    trans_centre_to_o.SetTranslateOnly(-BoundingSphere.GetCenter());
    CMatrix3x4 trans_to_pos(MATRIX34_INIT_IDENTITY);
    trans_to_pos.SetTranslateOnly(WorldPos);
    CRightHandedSet rhs = Orientation.GetInterpolated(interp);
    C3DVector forward = rhs.GetForward() * scale;
    C3DVector right = rhs.GetRight() * scale;
    C3DVector up = rhs.GetUp() * scale;
    ObjectTransform.Set(-right.X, -right.Y, -right.Z,
                        -forward.X, -forward.Y, -forward.Z,
                        up.X, up.Y, up.Z, 0.0f, 0.0f, 0.0f);
    ObjectTransform.PreMultiply(trans_centre_to_o);
    ObjectTransform.PostMultiply(trans_to_pos);
}

#include <cstdlib>
#include <cstdio>
CEngineInternalPrimitive2DStaticMesh::CEngineInternalPrimitive2DStaticMesh() {}
CEngineInternalPrimitive2DStaticMesh::~CEngineInternalPrimitive2DStaticMesh() {}

static float requestedWeight;
static unsigned int interpolationCalls;
static void check(bool ok) { if (!ok) std::abort(); }
CRightHandedSet CRightHandedSet::Interpolate(const CRightHandedSet& other,float t) const
{
    check(requestedWeight==t);
    ++interpolationCalls;
    CRightHandedSet result;
    result.Set(GetUp() + (other.GetUp() - GetUp()) * t,
               GetForward() + (other.GetForward() - GetForward()) * t);
    return result;
}
CEngineInternalPrimitiveBase::CEngineInternalPrimitiveBase(long type,unsigned long)
    : CEnginePersistentPrimitiveBase(type) {}

// These unrelated virtual boundaries must never be called during interpolation.
bool CEngineInternalPrimitiveBase::WasRendered() const { std::abort(); }
bool CEnginePersistentPrimitiveBase::WasRendered() const { std::abort(); }
void CEngineInternalPrimitiveBase::NotifyReleased(CEnginePrimitiveHandle const &) { std::abort(); }
void CEnginePersistentPrimitiveBase::NotifyReleased(CEnginePrimitiveHandle const &) { std::abort(); }
C3DOrientationInterpolator::C3DOrientationInterpolator() {}
CBaseClass::CBaseClass() {}
CEngineFadeDistance::CEngineFadeDistance() {}
CEnginePersistentPrimitiveBase::CEnginePersistentPrimitiveBase(long type) : Type(type) {}
CEngineSceneGridPos::CEngineSceneGridPos() {}
CBaseClass::~CBaseClass() {}
CEngineInternalPrimitiveBase::~CEngineInternalPrimitiveBase() {}
CEngineInternalPrimitiveMeshBase::~CEngineInternalPrimitiveMeshBase() {}
CEngineInternalPrimitiveOcclusionBase::~CEngineInternalPrimitiveOcclusionBase() {}
CEngineInternalPrimitiveStaticMesh::~CEngineInternalPrimitiveStaticMesh() {}
CEnginePersistentPrimitiveBase::~CEnginePersistentPrimitiveBase() {}
bool CEngineInternalPrimitive2DStaticMesh::GetBoundingBoxTransform(CMatrix3x4 &) { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::GetBoundingBoxWorldSpace(C3DBoundingBox &) { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::GetBoundingSphereWorldSpace(C3DBoundingSphere &) { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::GetObjectTransform(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::GetTransformFrameEnd(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::GetTransformFrameStart(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::GetWorldPosition(C3DVector &) { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::IsInsideFrustum(CEngineCamera const &,bool) { std::abort(); }
bool CEngineInternalPrimitive2DStaticMesh::SetObjectTransform(CMatrix3x4 const &) { std::abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingBoxTransform(CMatrix3x4 &) { std::abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingBoxWorldSpace(C3DBoundingBox &) { std::abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingSphereRadiusWorldSpace(float &) { std::abort(); }
bool CEngineInternalPrimitiveBase::GetBoundingSphereWorldSpace(C3DBoundingSphere &) { std::abort(); }
bool CEngineInternalPrimitiveBase::GetMeshEffect(CIVCountedPointer<CEngineMeshEffect> &) const { std::abort(); }
bool CEngineInternalPrimitiveBase::GetObjectTransform(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitiveBase::GetWorldPosition(C3DVector &) { std::abort(); }
bool CEngineInternalPrimitiveBase::IsInsideFrustum(CEngineCamera const &,bool) { std::abort(); }
bool CEngineInternalPrimitiveBase::SetObjectTransform(CMatrix3x4 const &) { std::abort(); }
bool CEngineInternalPrimitiveMeshBase::GetTransformFrameEnd(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitiveMeshBase::GetTransformFrameStart(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::GetBoundingBoxTransform(CMatrix3x4 &) { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::GetBoundingBoxWorldSpace(C3DBoundingBox &) { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::GetBoundingSphereWorldSpace(C3DBoundingSphere &) { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::GetObjectTransform(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::GetTransformFrameEnd(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::GetTransformFrameStart(CMatrix3x4 &) const { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::GetWorldPosition(C3DVector &) { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::IsInsideFrustum(CEngineCamera const &,bool) { std::abort(); }
bool CEngineInternalPrimitiveStaticMesh::SetObjectTransform(CMatrix3x4 const &) { std::abort(); }
CEngineFadeDistance CEngineInternalPrimitiveBase::GetFadeDistance() const { std::abort(); }
CEngineInternalPrimitiveMorphInfo * CEngineInternalPrimitiveMeshBase::GetMorphInfo() { std::abort(); }
CEngineInternalPrimitiveOcclusionBase * CEngineInternalPrimitiveBase::GetAsOcclusionPrimitive() { std::abort(); }
CEnginePersistentPrimitiveBase * CEngineInternalPrimitiveBase::GetChildPrimitive(unsigned long) { std::abort(); }
CEngineRenderInfoMeshBase const * CEngineInternalPrimitiveStaticMesh::PeekRenderInfoMeshBase() const { std::abort(); }
unsigned long CEngineInternalPrimitiveBase::AddChildPrimitive(CEnginePrimitive const &,unsigned long) { std::abort(); }
void CEngineInternalPrimitive2DStaticMesh::Update(CEnginePrimitive const &,unsigned long) { std::abort(); }
void CEngineInternalPrimitiveBase::ClearAllChildPrimitives() { std::abort(); }
void CEngineInternalPrimitiveBase::ClearChildPrimitive(unsigned long) { std::abort(); }
void CEngineInternalPrimitiveBase::CollectStats(std::map<unsigned long,unsigned long,std::less<unsigned long>,std::allocator<std::pair<unsigned long const,unsigned long> > > &,EEnginePrimitiveStatIndex,unsigned long) const { std::abort(); }
void CEngineInternalPrimitiveBase::GetChildPrimitiveHandle(unsigned long,CEnginePrimitiveHandle &) { std::abort(); }
void CEngineInternalPrimitiveBase::PostRender() { std::abort(); }
void CEngineInternalPrimitiveBase::SetAnimation(CEnginePrimitiveAnimation const &) { std::abort(); }
void CEngineInternalPrimitiveBase::SetChildPrimitive(unsigned long,CEnginePrimitive const &,unsigned long) { std::abort(); }
void CEngineInternalPrimitiveBase::SetChildPrimitive(unsigned long,CEnginePrimitiveHandle &,CEnginePrimitive const &,unsigned long) { std::abort(); }
void CEngineInternalPrimitiveBase::SetMeshEffect(CIVCountedPointer<CEngineMeshEffect> const &) { std::abort(); }
void CEngineInternalPrimitiveBase::SetPosition(CEnginePrimitivePosition const &) { std::abort(); }
void CEngineInternalPrimitiveBase::Update(CEnginePrimitive const &,unsigned long) { std::abort(); }
void CEngineInternalPrimitiveStaticMesh::SetPosition(CEnginePrimitivePosition const &) { std::abort(); }
void CEngineInternalPrimitiveStaticMesh::Update(CEnginePrimitive const &,unsigned long) { std::abort(); }
void CIVCountedPointeeBase::Delete() const { std::abort(); }
int main()
{
    const float axes[][3]={{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,-2,0},{1,-1,2},{0,0,0}};
    CEngineInternalPrimitive2DStaticMesh mesh;
    for (unsigned int i=0; i<7; ++i) for (unsigned int j=0; j<7; ++j) {
        mesh.Orientation.StartOrientation.Set(C3DVector(axes[i][0],axes[i][1],axes[i][2]),
                                             C3DVector(axes[j][0],axes[j][1],axes[j][2]));
        mesh.Orientation.EndOrientation.Set(C3DVector(0,1,2),C3DVector(1,-1,0));
        mesh.BoundingSphere.Center=C3DVector(2,-1,3);
        mesh.WorldPos=C3DVector(1,4,-2);
        for (int part=-2; part<=6; ++part) for (int s=-2; s<=3; ++s) for (int flag=0; flag<2; ++flag) {
            mesh.Scale=0.5f*s;
            requestedWeight=0.25f*part;
            interpolationCalls=0;
            mesh.CalcInterpolatedObjectTransform(requestedWeight,flag!=0);
            check(interpolationCalls==1);
            const CMatrix3x4& m=mesh.ObjectTransform;
            const float actual[]={m.E11,m.E12,m.E13,m.E21,m.E22,m.E23,m.E31,m.E32,m.E33,m.E41,m.E42,m.E43};
            const float world[]={1,4,-2}, u1[]={0,1,2}, f1[]={1,-1,0};
            float u[3],f[3];
            for (int k=0;k<3;++k) {
                u[k]=axes[i][k]+(u1[k]-axes[i][k])*requestedWeight;
                f[k]=axes[j][k]+(f1[k]-axes[j][k])*requestedWeight;
            }
            float scale=flag ? mesh.Scale:1.0f;
            for (int k=0;k<3;++k) {
                const int b=(k+1)%3,d=(k+2)%3;
                const float right=(f[b]*u[d]-f[d]*u[b])*scale,forward=f[k]*scale,up=u[k]*scale;
                check(actual[k]==-right && actual[k+3]==-forward && actual[k+6]==up);
                check(actual[k+9]==world[k]+2*right-forward-3*up);
            }
            check(mesh.WorldPos.X==1 && mesh.Scale==0.5f*s && mesh.BoundingSphere.Center.Z==3);
        }
    }
    std::puts("00b9d220_TEST PASS");
    return 0;
}