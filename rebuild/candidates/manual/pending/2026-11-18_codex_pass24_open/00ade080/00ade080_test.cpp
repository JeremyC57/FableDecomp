#ifndef _MSC_VER
#include <vector>
#define FABLE_STLPORT_CONTAINERS_H
#define __fastcall
#endif
#define FABLE_RETAIL_PASS23_HOST_TEST_ABI
#define FABLE_RETAIL_C3DBOUNDINGBOX_SETCENTERDIAG
#include <stdio.h>
#include <stdlib.h>
// 00ade080 C3DBoundingBox::InitialiseFromOBB
// Leaf: ?InitialiseFromOBB@C3DBoundingBox@@QAEXABV1@ABVCMatrix3x4@@@Z
// Flags: none
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DBOUNDINGBOX
#define FABLE_RETAIL_C3DBOUNDINGBOX_OBB
#define FABLE_RETAIL_CMATRIX4X4_ALIGNEDTRANSFORMTRANSPOSEDASSUMEW
#define FABLE_RETAIL_CMATRIX4X4_INITIALISETRANSPOSED
#define FABLE_RETAIL_CMATRIX3X4
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMPOINT_LTR
#define FABLE_RETAIL_CMATRIX3X4_TRANSFORMVECTOR
#include "engine/C3DBoundingBox.h"
#include "engine/CMatrix3x4.h"
#include "engine/CMatrix4x4.h"
#include <xmmintrin.h>
#ifdef _MSC_VER
#define FABLE_OBB_ALIGN16 __declspec(align(16))
#else
#define FABLE_OBB_ALIGN16 __attribute__((aligned(16)))
#endif
extern bool GEnableSIMD;   // 0x013D2880

void C3DBoundingBox::InitialiseFromOBB(const C3DBoundingBox& src, const CMatrix3x4& transform)
{
    if (GEnableSIMD) {
        FABLE_OBB_ALIGN16 C4DVector center(src.GetCenter(), 0.0f);
        C3DVector half = (src.MaxCoord - src.MinCoord) * 0.5f;
        FABLE_OBB_ALIGN16 C4DVector diag[4] = {
            C4DVector(half.X, half.Y, half.Z, 0.0f),
            C4DVector(half.X, -half.Y, half.Z, 0.0f),
            C4DVector(-half.X, half.Y, half.Z, 0.0f),
            C4DVector(-half.X, -half.Y, half.Z, 0.0f) };
        FABLE_OBB_ALIGN16 CMatrix4x4 sse_transform;
        const CMatrix3x4& t = transform;
        sse_transform.InitialiseTransposed(t);
        sse_transform.TransformTransposedAssumeW(center, center, 1.0f);
        sse_transform.AlignedTransformTransposedAssumeW(diag, diag, 4, 0.0f);
        FABLE_OBB_ALIGN16 C4DVector min_coord;
        FABLE_OBB_ALIGN16 C4DVector max_coord;
        const C4DVector* diag_ptr = diag;
        const __m128 c = _mm_load_ps(&center.X);
        __m128 d = _mm_load_ps(&diag_ptr[0].X);
        __m128 plus = _mm_add_ps(c,d);
        __m128 minus = _mm_sub_ps(c,d);
        __m128 lower = _mm_min_ps(plus,minus);
        __m128 upper = _mm_max_ps(plus,minus);
        for (int i = 1; i < 4; ++i) {
            d = _mm_load_ps(&diag_ptr[i].X);
            plus = _mm_add_ps(c,d);
            minus = _mm_sub_ps(c,d);
            lower = _mm_min_ps(lower,plus);
            upper = _mm_max_ps(upper,plus);
            lower = _mm_min_ps(lower,minus);
            upper = _mm_max_ps(upper,minus);
        }
        _mm_store_ps(&min_coord.X, lower);
        _mm_store_ps(&max_coord.X, upper);
        MinCoord = C3DVector(min_coord.X, min_coord.Y, min_coord.Z);
        MaxCoord = C3DVector(max_coord.X, max_coord.Y, max_coord.Z);
        return;
    }
    C3DVector diag1 = (src.MaxCoord - src.MinCoord) * 0.5f;
    C3DVector diag2(diag1.X, -diag1.Y, diag1.Z);
    C3DVector diag3(-diag1.X, diag1.Y, diag1.Z);
    C3DVector diag4(-diag1.X, -diag1.Y, diag1.Z);
    transform.TransformVector(diag1);
    transform.TransformVector(diag2);
    transform.TransformVector(diag3);
    transform.TransformVector(diag4);
    C3DVector center = src.GetCenter();
    transform.TransformPoint(center);

    MaxCoord = center + diag1;
    MinCoord = MaxCoord;
    MinCoord.LimitUpper(center - diag1);
    MaxCoord.LimitLower(center - diag1);
    MinCoord.LimitUpper(center + diag2);
    MaxCoord.LimitLower(center + diag2);
    MinCoord.LimitUpper(center - diag2);
    MaxCoord.LimitLower(center - diag2);
    MinCoord.LimitUpper(center + diag3);
    MaxCoord.LimitLower(center + diag3);
    MinCoord.LimitUpper(center - diag3);
    MaxCoord.LimitLower(center - diag3);
    MinCoord.LimitUpper(center + diag4);
    MaxCoord.LimitLower(center + diag4);
    MinCoord.LimitUpper(center - diag4);
    MaxCoord.LimitLower(center - diag4);
}

bool GEnableSIMD=false;
static unsigned long aligned_calls=0;
void CMatrix4x4::AlignedTransformTransposedAssumeW(const C4DVector* src,C4DVector* dest,unsigned long count,float w) const {
    if(count!=4 || w!=0.0f)abort();
    ++aligned_calls;for(unsigned long i=0;i<count;++i)TransformTransposedAssumeW(src[i],dest[i],w);
}
static bool same(const C3DVector& a,const C3DVector& b) {return a.X==b.X && a.Y==b.Y && a.Z==b.Z;}
int main(){
    for(int mode=0;mode<2;++mode)for(int kind=0;kind<6;++kind)for(int box=0;box<4;++box){
        GEnableSIMD=mode!=0;
        C3DBoundingBox src,out;
        C3DVector center(3.0f,-2.0f,7.0f), half(box?2.0f:0.0f,box>=2?1.0f:0.0f,box>=3?4.0f:0.0f);
        src.SetCenterDiag(center,half);
        CMatrix3x4 m;
        m.E11=kind==1?0.0f:kind==2?-2.0f:1.0f; m.E21=kind==1?-1.0f:kind==3?2.0f:0.0f;m.E31=kind==4?0.5f:0.0f;m.E41=kind?10.0f:0.0f;
        m.E12=kind==1?1.0f:0.0f; m.E22=kind==1?0.0f:kind==5?0.5f:1.0f;m.E32=kind==4?-1.0f:0.0f;m.E42=kind?-4.0f:0.0f;
        m.E13=kind==4?-1.0f:0.0f; m.E23=kind==4?2.0f:0.0f;m.E33=kind==3?0.0f:1.0f;m.E43=kind?8.0f:0.0f;
        C3DVector low,high;
        for(int index=0;index<8;++index){
            const float x=center.X+((index&1)?half.X:-half.X);
            const float y=center.Y+((index&2)?half.Y:-half.Y);
            const float z=center.Z+((index&4)?half.Z:-half.Z);
            C3DVector p(x*m.E11+y*m.E21+z*m.E31+m.E41,x*m.E12+y*m.E22+z*m.E32+m.E42,x*m.E13+y*m.E23+z*m.E33+m.E43);
            if(!index){low=p;high=p;}else{
                if(p.X<low.X)low.X=p.X;if(p.Y<low.Y)low.Y=p.Y;if(p.Z<low.Z)low.Z=p.Z;
                if(p.X>high.X)high.X=p.X;if(p.Y>high.Y)high.Y=p.Y;if(p.Z>high.Z)high.Z=p.Z;
            }
        }
        unsigned long before=aligned_calls;
        out.InitialiseFromOBB(src,m);if(!same(out.PeekMinCoord(),low)||!same(out.PeekMaxCoord(),high)||aligned_calls!=before+(unsigned int)mode)abort();
        src.InitialiseFromOBB(src,m);if(!same(src.PeekMinCoord(),low)||!same(src.PeekMaxCoord(),high))abort();
    }
    puts("00ade080_TEST PASS");return 0;
}
