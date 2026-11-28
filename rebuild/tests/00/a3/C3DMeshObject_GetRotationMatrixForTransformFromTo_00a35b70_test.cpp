// 00a35b70 C3DMeshObject::GetRotationMatrixForTransformFromTo
// Leaf: ?GetRotationMatrixForTransformFromTo@C3DMeshObject@@SI?AVCMatrix3x4@@ABVCRightHandedSet@@0@Z
// Flags: /d2inlT150
// Inline-budget: all calls to Set, SetInverse and PostMultiply visible in the editor expand throughout the retail leaf.
#define FABLE_RETAIL_CARRYABLE_POSE_PASS29
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CQUATERNION_PARTICLE_UPDATE
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#define FABLE_RETAIL_CMATRIX3X4_PARTICLE_ADD_SET
#define FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_PASS23
#define FABLE_RETAIL_CMATRIX3X4_SETINVERSE_DONOR_PASS26
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_PREMULTIPLY
#define FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY_POSTMULTIPLY
#define FABLE_RETAIL_C3DVECTOR_CROSS_XYZ_GROUP
#include "rebuild_abi.h"
#include "engine/CRightHandedSet.h"
#include "engine/C3DMeshObject.h"

CMatrix3x4 FABLE_FASTCALL C3DMeshObject::GetRotationMatrixForTransformFromTo(
    const CRightHandedSet& transform_from_rhs, const CRightHandedSet& transform_to_rhs)
{
    CMatrix3x4 creature_carry_matrix;
    creature_carry_matrix.Set(transform_to_rhs, C3DVector(0.0f, 0.0f, 0.0f));
    CMatrix3x4 object_matrix;
    object_matrix.Set(transform_from_rhs.GetRight(), transform_from_rhs.GetUp(),
                      transform_from_rhs.GetForward(), C3DVector(0.0f, 0.0f, 0.0f));
    CMatrix3x4 transform_to;
    transform_to.SetInverse(object_matrix);
    transform_to.PostMultiply(creature_carry_matrix);
    return transform_to;
}
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

namespace {
void check(bool ok) { if (!ok) abort(); }
double absolute(double x) { return x<0.0?-x:x; }

void makeBasis(const double* up,const double* forward,bool to,double out[3][3])
{
    for(int k=0;k<3;++k) {
        int i=(k+1)%3, j=(k+2)%3;
        out[0][k]=forward[i]*up[j]-forward[j]*up[i];
        out[1][k]=to?forward[k]:up[k];
        out[2][k]=to?up[k]:forward[k];
    }
}
// Solve source * answer = destination with partial pivoting. This reference
// constructs no inverse and does not use the production cofactor formula.
bool solve(const double source[3][3],const double destination[3][3],double solution[3][3])
{
    double m[3][6]; int row,col;
    for(row=0;row<3;++row) for(col=0;col<3;++col) {
        m[row][col]=source[row][col];
        m[row][col+3]=destination[row][col];
    }
    for(col=0;col<3;++col) {
        int pivot=col;
        for(row=col+1;row<3;++row) if(absolute(m[row][col])>absolute(m[pivot][col]))pivot=row;
        if(m[pivot][col]==0.0)return false;
        for(int k=0;k<6;++k){double tmp=m[pivot][k]; m[pivot][k]=m[col][k]; m[col][k]=tmp;}
        double scale=m[col][col];
        for(int k=0;k<6;++k)m[col][k]/=scale;
        for(row=0;row<3;++row) if(row!=col) {
            double factor=m[row][col]; for(int k=0;k<6;++k)m[row][k]-=factor*m[col][k];
        }
    }
    for(row=0;row<3;++row)for(col=0;col<3;++col)solution[row][col]=m[row][col+3];
    return true;
}
unsigned int run(const double *su,const double *sf,const double *tu,const double *tf,bool alias)
{
    CRightHandedSet from, to;
    from.Init(C3DVector((float)su[0],(float)su[1],(float)su[2]),C3DVector((float)sf[0],(float)sf[1],(float)sf[2]));
    to.Init(C3DVector((float)tu[0],(float)tu[1],(float)tu[2]),C3DVector((float)tf[0],(float)tf[1],(float)tf[2]));
    const CMatrix3x4 m=C3DMeshObject::GetRotationMatrixForTransformFromTo(from,alias?from:to);
    const float value[4][3]={{m.E11,m.E12,m.E13},{m.E21,m.E22,m.E23},{m.E31,m.E32,m.E33},{m.E41,m.E42,m.E43}};
    double src[3][3], dst[3][3], expected[3][3]; makeBasis(su,sf,false,src); makeBasis(alias?su:tu,alias?sf:tf,true,dst);
    if(!solve(src,dst,expected)) {
        bool hasNonFinite=false;
        for(int r=0;r<3;++r) for(int c=0;c<3;++c) {
            float v=value[r][c]; if(v!=v || absolute(v)>3.4e38) hasNonFinite=true;
        }
        check(hasNonFinite);return 1;
    }
    for(int r=0;r<3;++r)for(int c=0;c<3;++c)
        check(absolute(value[r][c]-expected[r][c]) <= 0.00002*(1.0+absolute(expected[r][c])));
    check(value[3][0]==0.0f && value[3][1]==0.0f && value[3][2]==0.0f);
    return 1;
}
}
int main()
{
    const double axes[][3]={{1,0,0},{0,1,0},{0,0,1},{-1,0,0},{0,2,0},{1,1,0},{0,1,-2},{0,0,0}};
    unsigned int count=0;
    for(int a=0;a<8;++a)for(int b=0;b<8;++b) {
        count+=run(axes[a],axes[b],axes[0],axes[1],true);
        for(int c=0;c<8;++c)for(int d=0;d<8;++d)
            count+=run(axes[a],axes[b],axes[c],axes[d],false);
    }
    check(count==4160); puts("00a35b70_TEST PASS");return 0;
}