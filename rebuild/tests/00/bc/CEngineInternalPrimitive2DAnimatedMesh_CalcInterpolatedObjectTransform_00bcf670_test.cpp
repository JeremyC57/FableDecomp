// 00bcf670 CEngineInternalPrimitive2DAnimatedMesh::CalcInterpolatedObjectTransform
// Leaf: ?CalcInterpolatedObjectTransform@CEngineInternalPrimitive2DAnimatedMesh@@UBEXAAVCMatrix3x4@@M_N@Z
// Flags:
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_CONVERT
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEBASE_ATTACHED
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEOCCLUSIONBASE
#define FABLE_RETAIL_MESH_2D_OBJECT_MATRIX_PASS30
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_ORIGINAL_CACHE
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVEANIMATEDMESH_WORLDPOS
#define FABLE_RETAIL_CENGINEINTERNALPRIMITIVE2DANIMATEDMESH
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR
#define FABLE_RETAIL_C3DORIENTATIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DPOSITIONINTERPOLATOR_GETINTERPOLATED
#define FABLE_RETAIL_C3DBOUNDINGSPHERE
#define FABLE_RETAIL_C3DBOUNDINGSPHERE_CENTER_VALUE
#define FABLE_RETAIL_CRIGHTHANDEDSET_SET
#define FABLE_RETAIL_CRIGHTHANDEDSET_INTERPOLATE
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CARRAY
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
#include "engine/CMatrix3x4.h"
#include "engine/CEngineInternalPrimitive2DAnimatedMesh.h"


#include <cstdio>
#include <cstdlib>
static unsigned int stage;
static float weight, capturedScale;
static C3DVector capturedCentre, capturedPosition;
static CRightHandedSet capturedFrame;
static CMatrix3x4* capturedMatrix;
static void check(bool x) { if (!x) std::abort(); }
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
C3DPositionInterpolator::C3DPositionInterpolator() {}
C3DPositionInterpolator::~C3DPositionInterpolator() {}
C3DOrientationInterpolator::C3DOrientationInterpolator() {}
CRightHandedSet CRightHandedSet::Interpolate(const CRightHandedSet& other,float t) const {
    check(stage++==0 && weight==t);
    CRightHandedSet result;
    result.Set(GetUp()+other.GetUp()*t,GetForward()+other.GetForward()*t);
    return result;
}
void CRightHandedSet::MakePerpendicular() {
    check(stage++==1);
    SetUp(GetUp()+C3DVector(4,2,1));
}
void FABLE_FASTCALL CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix(CMatrix3x4& m,const C3DVector& offset,
    const C3DVector& position,const CRightHandedSet& axes,float scale) {
    check(stage++==2); capturedMatrix=&m; capturedScale=scale;
    capturedCentre=offset; capturedPosition=position; capturedFrame=axes;
}
// Exercise both caller branches with the original shared property types.
// The native differential test covers the complete member on its real layout.
static void evaluate(const C3DPositionInterpolator& WorldPos,const C3DOrientationInterpolator& Orientation,
                     const C3DBoundingSphere& ObjectBoundingSphere,float Scale,
                     CMatrix3x4& dest,float interp,bool use_scaling)
{
    C3DVector world_pos = WorldPos.GetInterpolated(interp);
    CRightHandedSet rhs = Orientation.GetInterpolated(interp);
    rhs.MakePerpendicular();
    CEngineInternalPrimitiveMeshBase::Calc2DObjectMatrix(dest, ObjectBoundingSphere.GetCenter(), world_pos, rhs, use_scaling ? Scale : 1.0f);
}

int main() {
    C3DPositionInterpolator pos;
    C3DOrientationInterpolator orientation;
    C3DBoundingSphere bounds;
    CMatrix3x4 matrix;
    pos.StartPosition=C3DVector(2,4,8); pos.EndPosition=C3DVector(-2,6,16);
    orientation.StartOrientation.Set(C3DVector(0,1,0), C3DVector(0,0,1));
    orientation.EndOrientation.Set(C3DVector(1,2,3),C3DVector(-1,0,2));
    bounds.Center=C3DVector(17,-11,2);
    for (int part=-4;part<=8;++part) for (int s=-4;s<=4;++s) for (int flag=0;flag<2;++flag) {
        weight=0.25f*part;stage=0;
        evaluate(pos,orientation,bounds,0.5f*s,matrix,weight,flag!=0);
        check(stage==3 && capturedMatrix==&matrix && capturedScale==(flag ? s*0.5f : 1.0f));
        check(capturedCentre.X==17 && capturedCentre.Y==-11 && capturedCentre.Z==2);
        check(capturedPosition.X==2-4*weight && capturedPosition.Y==4+2*weight && capturedPosition.Z==8+8*weight);
        const C3DVector& u=capturedFrame.GetUp();const C3DVector& f=capturedFrame.GetForward();
        check(u.X==4+weight && u.Y==3+2*weight && u.Z==1+3*weight);
        check(f.X==-weight && f.Y==0 && f.Z==1+2*weight);
    }
    std::puts("00bcf670_TEST PASS");return 0;
}
