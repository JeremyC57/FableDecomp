// Leaf: ??$__copy@PAVC3DVector@@PAV1@H@std@@YIPAVC3DVector@@PAV1@00ABUrandom_access_iterator_tag@0@PAH@Z
// Flags: /Gr
#define FABLE_RETAIL_C3DVECTOR
#include "engine/C3DVector.h"
#include "stlport/fable_containers.h"

namespace std {
template C3DVector* FABLE_FASTCALL __copy<C3DVector*,C3DVector*,int>(
    C3DVector*,C3DVector*,C3DVector*,const random_access_iterator_tag&,int*);
}

#include <stdlib.h>
#include <stdio.h>
static void require(bool yes) { if(!yes) exit(1); }
static bool equals(const C3DVector& a,const C3DVector& b) { return a.X==b.X && a.Y==b.Y && a.Z==b.Z; }
int main() {
    C3DVector src[16], out[16], inplace[16];
    const C3DVector mark(91,92,93);
    for (int n=0;n<=12;++n) {
        for (int i=0;i<16;++i) { src[i]=C3DVector(i,-i,i*2+1); inplace[i]=src[i]; out[i]=mark; }
        std::random_access_iterator_tag tag;
        require(std::__copy(src+1,src+1+n,out+2,tag,(int*)0)==out+2+n);
        for(int i=0;i<16;++i) require(equals(out[i],i>=2&&i<2+n?src[i-1]:mark));
        require(std::__copy(inplace+1,inplace+1+n,inplace,tag,(int*)0)==inplace+n);
        for(int i=0;i<16;++i) require(equals(inplace[i],src[i<n?i+1:i]));
        require(std::__copy(src+13,src+13-n,out,tag,(int*)0)==out);
    }
    puts("0074ca20_TEST PASS"); return 0;
}
