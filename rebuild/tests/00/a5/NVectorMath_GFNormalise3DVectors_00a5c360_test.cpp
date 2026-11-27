#define FABLE_DUAL_PATH_TEST
// 00a5c360 NVectorMath::GFNormalise3DVectors
// Evidence: FableWin lib_vector_math.obj marks this five-argument overload as containing inline assembly.
// Leaf: ?GFNormalise3DVectors@NVectorMath@@YAXPBVC4DVector@@KPAV2@KK@Z
// Flags: /Zi
// Retail-asm-helper: GF_SSE_NORMALISE_3D_VECTORS
// Retail-asm: 0x00A5C3B6..0x00A5C46F, aligned pair loop and count-preserving odd tail.
// Inferred: /Zi retains the realigned return-address-copy prologue of this TU.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_VECTOR_NORMALISE_PASS29
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_MAGNITUDE_SQRTF
#include "engine/C4DVector.h"
#include "engine/NVectorMath.h"
#include "engine/NVectorMathSSE.h"
namespace NVectorMath {
void GFNormalise3DVectors(const C4DVector* src, unsigned long src_stride, C4DVector* dest, unsigned long dest_stride, unsigned long count)
{
    if (GEnableSIMD) {
        float md = 0.01f;
#if defined(_MSC_VER)
        __declspec(align(16))
#endif
        C4DVector min_divisor(md, md, md, md);
        unsigned long src_stride_2 = src_stride * 2;
        unsigned long dest_stride_2 = dest_stride * 2;
        GF_SSE_NORMALISE_3D_VECTORS(src, dest, count, src_stride_2, dest_stride_2, min_divisor)
    } else {
        for (unsigned long c1 = 0; c1 < count; ++c1) {
            C4DVector vect(src[c1].ToC3DVector(), 0.0f);
            float dist = GFRoot(GFSqr(vect.X) + GFSqr(vect.Y) + GFSqr(vect.Z) + GFSqr(vect.W));
            vect /= dist;
            dest[c1] = vect;
        }
    }
}
}
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <float.h>
static void require(bool condition, int line) {
    if (!condition) { printf("failed at %d\n",line);exit(1); }
}
#define VERIFY(c) require((c),__LINE__)
bool GEnableSIMD;
static unsigned int seed=1;
static float sample() { seed=seed*1664525U+1013904223U;return float(int(seed>>16)-32768)*0.0078125f; }
#if defined(_MSC_VER)
#define TEST_ALIGN __declspec(align(16))
#else
#define TEST_ALIGN
#endif
static unsigned int digest = 2166136261U;
static void record(const void* data, size_t size) {
    const unsigned char* p = static_cast<const unsigned char*>(data);
    for (size_t i=0;i<size;++i) { digest ^= p[i]; digest *= 16777619U; }
}
static void compare(unsigned long count,unsigned long src_stride,unsigned long dst_stride,bool inplace,int shift)
{
    TEST_ALIGN C4DVector native[40],portable[40],out[40],ref[40];
    for(unsigned long i=0;i<40;++i) {
        const float x=sample(); const float y=sample(); const float z=sample();
        native[i]=C4DVector(x,y,z,float(i+1));
        out[i]=C4DVector(19,23,29,31);
    }
    native[0]=C4DVector(0.003f,-0.004f,0,81);
    native[1]=C4DVector(3,4,0,82);
    native[2]=C4DVector(0,0,0,83);
    memcpy(portable,native,sizeof native);memcpy(ref,out,sizeof out);
    C4DVector* dest=inplace?native+shift:out;
    C4DVector* target=inplace?portable+shift:ref;
    GEnableSIMD=true;
    NVectorMath::GFNormalise3DVectors(native,src_stride,dest,dst_stride,count);
    fable_portable::Normalise3DVectors(portable,target,count,src_stride*2,dst_stride*2,0.01f);
    VERIFY(!memcmp(portable,native,sizeof native));VERIFY(!memcmp(out,ref,sizeof out));
    record(native,sizeof native); record(out,sizeof out);
    if(!inplace && count && src_stride && dst_stride) {
        VERIFY(fabs(out[0].X-0.3f)<0.000001 && fabs(out[0].Y+0.4f)<0.000001 && out[0].W==0);
        if(count>1) VERIFY(fabs(out[1].X-0.6f)<0.000001 && fabs(out[1].Y-0.8f)<0.000001);
    }
}
int main() {
    for(unsigned long n=0;n<=7;++n) for(unsigned long src=0;src<=48;src+=16)
      for(unsigned long dst=0;dst<=48;dst+=16) {
        compare(n,src,dst,false,0);compare(n,src,dst,true,0);compare(n,src,dst,true,1);
      }
    GEnableSIMD=false;
    C4DVector input[4]={C4DVector(3,4,0,5),C4DVector(0,0,2,9),C4DVector(0.003f,-0.004f,0,2),C4DVector(0,0,0,9)};
    C4DVector output[4]; NVectorMath::GFNormalise3DVectors(input,0,output,64,4);
    VERIFY(fabs(output[0].X-0.6f)<0.000001 && fabs(output[0].Y-0.8f)<0.000001);
    VERIFY(output[1].Z==1 && output[1].W==0 && input[0].W==5 && input[1].W==9);
    VERIFY(fabs(output[2].X-0.6f)<0.000001 && fabs(output[2].Y+0.8f)<0.000001);
    VERIFY(output[3].X!=output[3].X && output[3].W!=output[3].W);
    NVectorMath::GFNormalise3DVectors(0,0,0,0,0);
    VERIFY(digest==0xf3a939e1U);
    GEnableSIMD=true;
    TEST_ALIGN C4DVector special[4];
    TEST_ALIGN C4DVector converted[4];
    unsigned int nanBits=0x7fc00000U;
    float quietNaN; memcpy(&quietNaN,&nanBits,sizeof(quietNaN));
    special[0]=C4DVector(quietNaN,2.0f,0.0f,7.0f);
    special[1]=C4DVector(FLT_MAX,FLT_MAX,-FLT_MAX,1.0f);
    special[2]=C4DVector(0.01f,0.0f,-0.0f,1.0f);
    special[3]=C4DVector(FLT_MIN,-FLT_MIN,0.0f,1.0f);
    NVectorMath::GFNormalise3DVectors(special,16,converted,16,4);
    VERIFY(converted[0].X!=converted[0].X && converted[0].Y==200.0f);
    VERIFY(converted[1].X==0.0f && converted[1].Y==0.0f && converted[1].Z==0.0f);
    VERIFY(converted[2].X==1.0f && memcmp(&converted[2].Z,&special[2].Z,sizeof(float))==0);
    VERIFY(converted[3].X==FLT_MIN/0.01f && converted[3].Y== -FLT_MIN/0.01f);
    VERIFY(special[0].W==0 && special[1].W==0 && special[2].W==0 && special[3].W==0);
    puts("00a5c360_TEST PASS");return 0;
}