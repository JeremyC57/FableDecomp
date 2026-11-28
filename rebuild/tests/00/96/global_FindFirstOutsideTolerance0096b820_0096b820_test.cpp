// Leaf: ??$__find_if@PAVC3DVector@@V?$unary_negate@VCVectorFilter_IsWithinHeightRange@@@std@@@std@@YIPAVC3DVector@@PAV1@0V?$unary_negate@VCVectorFilter_IsWithinHeightRange@@@0@ABUrandom_access_iterator_tag@0@@Z
// Flags: /Gr
#define FABLE_RETAIL_CVECTORFILTER_ISWITHINHEIGHTRANGE_PASS30
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_STLP_FIND_IF_INPUT
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS
#define FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS_BODY
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorFilter_IsWithinHeightRange.h"
#include "stlport/fable_algorithm.h"

namespace std {
template C3DVector* FABLE_FASTCALL __find_if<C3DVector*,unary_negate<CVectorFilter_IsWithinHeightRange> >(
    C3DVector*,C3DVector*,unary_negate<CVectorFilter_IsWithinHeightRange>,const random_access_iterator_tag&);
}

#include <stdlib.h>
#include <stdio.h>
static void require(bool yes) { if(!yes) exit(1); }
int main() {
    C3DVector v[12]; std::random_access_iterator_tag tag;
    for (int n=0;n<=12;++n) for (int outside=0;outside<=n;++outside) for (int k=0;k<4;++k) {
        CVectorFilter_IsWithinHeightRange band(4.0f,1.5f);
        for(int i=0;i<n;++i) { float z=(i&1)?2.51f:5.49f;
            if (i==outside) z=k==0?2.5f:k==1?5.5f:k==2?2.0f:7.0f;
            v[i]=C3DVector((float)i, -100.0f, z); }
        require(std::__find_if(v,v+n,std::not1(band),tag)==v+outside);
    }
    CVectorFilter_IsWithinHeightRange negative(4.0f,-1.0f); v[0]=C3DVector(0,0,4);
    require(std::__find_if(v,v+1,std::not1(negative),tag)==v);
    puts("0096b820_TEST PASS"); return 0;
}
