// Leaf: ??$remove_copy_if@PAVC3DVector@@PAV1@V?$unary_negate@VCVectorFilter_IsWithinHeightRange@@@std@@@std@@YIPAVC3DVector@@PAV1@00V?$unary_negate@VCVectorFilter_IsWithinHeightRange@@@0@@Z
// Flags: /Gr
#define FABLE_RETAIL_CVECTORFILTER_ISWITHINHEIGHTRANGE_PASS30
#define FABLE_RETAIL_STLP_REMOVE_COPY_IF
#define FABLE_RETAIL_STLP_NOT1
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"
#include "engine/CVectorFilter_IsWithinHeightRange.h"
#include "stlport/fable_algorithm.h"

namespace std {
template C3DVector* FABLE_FASTCALL remove_copy_if<C3DVector*,C3DVector*,unary_negate<CVectorFilter_IsWithinHeightRange> >(
    C3DVector*,C3DVector*,C3DVector*,unary_negate<CVectorFilter_IsWithinHeightRange>);
}

#include <stdlib.h>
#include <stdio.h>
static void require(bool yes) { if(!yes) exit(1); }
static bool equals(const C3DVector& a,const C3DVector& b) { return a.X==b.X && a.Y==b.Y && a.Z==b.Z; }
int main() {
    C3DVector v[12], result[14], overlap[12], expected[12], mark(81,82,83);
    CVectorFilter_IsWithinHeightRange band(4.0f,1.5f);
    for(int n=0;n<=10;++n) for(int mask=0;mask<(1<<n);++mask) {
        int kept=0;
        for(int i=0;i<n;++i) {
            float z=(mask&(1<<i))?((i&1)?5.49f:2.51f):((i&1)?5.5f:2.5f);
            v[i]=C3DVector(i,-i,z); overlap[i]=v[i];
            if(mask&(1<<i)) expected[kept++]=v[i];
        }
        for(int i=0;i<14;++i) result[i]=mark;
        require(std::remove_copy_if(v,v+n,result+1,std::not1(band))==result+1+kept);
        require(std::remove_copy_if(overlap,overlap+n,overlap,std::not1(band))==overlap+kept);
        for(int i=0;i<kept;++i) require(equals(expected[i],result[i+1]) && equals(expected[i],overlap[i]));
        require(equals(result[0],mark));
        for(int i=1+kept;i<14;++i) require(equals(result[i],mark));
    }
    puts("0096ba30_TEST PASS"); return 0;
}
