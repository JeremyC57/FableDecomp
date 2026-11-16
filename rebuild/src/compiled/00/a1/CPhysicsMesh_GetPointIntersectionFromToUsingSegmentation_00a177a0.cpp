// 00a177a0 CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation
// Inline-budget: native00a17822..a17a45 expands HasCollidedWithTriangleAt; ordinary inline, T200/S500 and depth255 remain calls; force only this proven helper.
// Flags: none
// Leaf: ?GetPointIntersectionFromToUsingSegmentation@CPhysicsMesh@@ABE_NABVC3DVector@@0AAVCMeshIntersectionData@@@Z
// Retail:0x00a177a0,840bytes. Identity: both original PDBs and caller00a18798.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CPHYSICSMESH_INTERSECTION_PASS23
#define FABLE_RETAIL_CPLANE_NORMAL_CTOR_PASS23
#define FABLE_RETAIL_CPLANE_LINE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_HOST_BITS_PASS23
#define FABLE_RETAIL_C3DVECTOR_CROSS_MEMBER_X_GROUP
#define FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED
#define FABLE_RETAIL_C3DVECTOR_GETMAGNITUDEANDNORMALISE
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_XYZ
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_GROUPED_SUM
#define FABLE_RETAIL_C3DPRIMITIVE_CORNER_ACCESSORS
#include "engine/CPhysicsMesh.h"
#include "engine/CPlane.h"
// Original name and fastcall signature in both PDBs; native call00a17a34.
bool FABLE_FASTCALL GFIsPointInTriangle(const C3DVector&,const C3DVector&,
    const C3DVector&,const C3DVector&,const C3DVector&);

// FableWin01d9e1b0 and the original03199a42 call identify this real inline.
inline float FABLE_FASTCALL GFDotProduct(const C3DVector& a,const C3DVector& b)
{
    float result=a.X*b.X+a.Y*b.Y+a.Z*b.Z;
    return result;
}

FABLE_FORCEINLINE bool CPhysicsMesh::HasCollidedWithTriangleAt(const C3DTriangle2& face,
    const C3DVector& from,const C3DVector& direction,float length,
    C3DVector& hit,C3DVector& hitNormal) const
{
    C3DVector unused;
    const C3DVector& a=TotalPoints[face.GetA()];
    const C3DVector& b=TotalPoints[face.GetB()];
    const C3DVector& c=TotalPoints[face.GetC()];
    C3DVector normal=(b-a).Cross(c-a);
    normal.Normalise();
    float planeDistance=a.Dot(normal);
    CPlane plane(normal,planeDistance);
    float facing=GFDotProduct(normal,direction);
    if(facing>0.0f) {
        if(plane.GetIntersectionWithLine(from,direction,&hit)) {
            float along=direction.Dot(hit-from);
            if(along>-DELTA_NOTIONAL_ZERO && along<length+DELTA_NOTIONAL_ZERO) {
                if(GFIsPointInTriangle(a,b,c,-normal,hit)) {
                    hitNormal=normal;
                    return true;
                }
            }
        }
    }
    return false;
}

bool CPhysicsMesh::GetPointIntersectionFromToUsingSegmentation(const C3DVector& from,
    const C3DVector& to,CMeshIntersectionData& data) const
{
    std::vector<long>& faces=GetSingleInstanceCollisionSet();
    RootSegment->GetCollidedWithTriangles(this,0.0f,from,to,faces);
    std::vector<long>::const_iterator it;
    C3DVector hit;
    C3DVector normal;
    float nearest=1e10f;
    bool found=false;
    C3DVector direction=to-from;
    float length=direction.GetMagnitudeAndNormalise();
    for(it=faces.begin();it!=faces.end();++it) {
        const C3DTriangle2& face=TotalFaceIndices[*it];
        if(HasCollidedWithTriangleAt(face,from,direction,length,hit,normal)) {
            float distanceSquared=(hit-from).GetSquaredMagnitude();
            if(distanceSquared<nearest) {
                data.DistanceSquared=distanceSquared;
                data.IntersectionPosition=hit;
                data.Normal=normal;
                nearest=distanceSquared;
                found=true;
            }
        }
    }
    return found;
}