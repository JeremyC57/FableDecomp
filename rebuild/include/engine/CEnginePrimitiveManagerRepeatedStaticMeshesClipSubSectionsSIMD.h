#ifdef FABLE_RETAIL_PASS22_REPEATED_MESH_CLIP_SIMD
#ifndef FABLE_ENGINE_REPEATED_MESH_CLIP_SIMD_HELPER_H
#define FABLE_ENGINE_REPEATED_MESH_CLIP_SIMD_HELPER_H
#include <string.h>
// Retail-asm-helper: CEnginePrimitiveManagerRepeatedStaticMeshes::ClipSubSectionsSIMD
// Retail-asm: 0x00B98E5B..0x00B99026 original hand-SSE four-lane kernel (movaps/mulps/addps/subps/movmskps and three prefetcht0).
// Original PDB module915 has reject_mask/sub_clip_flags/pass_mask; opaque block reads/writes the two stack locals.
// Original S_FRAMEPROC flags0x28208 has HasInlineAssembly(0x8) set; six actual PDB labels are retained below.
// Native has an ordinary unaligned12-byte EBP frame, unlike VC71 intrinsic __m128 locals' dynamic16-byte frame.
//30full-context C++ site spellings emit no packed SIMD/prefetch;10intrinsic spellings752..1885 fail exact664body.
// Original destructive stage ANDs are preserved: earlier pass nibbles are discarded, not silently repaired.
// Portable arithmetic: IEEE binary32, round-to-nearest, gradual underflow, masked exceptions, ordered products/sums.
// Explicit first-operand NaN propagation preserves SIGN because movmskps feeds culling, not only NaN payloads.
// Dual-path:7680fullrecursivecases x4backends and196971binary32arithmeticcases x4backends agree bit-for-bit; no approximate operations.
#ifdef FABLE_PORTABLE

namespace NEnginePrimitiveManagerRepeatedStaticMeshes { namespace NPortableClipArithmetic {
inline unsigned int Bits(float value){unsigned int bits;memcpy(&bits,&value,4);return bits;}
inline float Value(unsigned int bits){float value;memcpy(&value,&bits,4);return value;}
inline bool IsNaN(unsigned int bits){return (bits&0x7fffffffU)>0x7f800000U;}
// SSE add/sub/mul propagate the first NaN operand (quieted), then the second.
// A new invalid-operation NaN has the x86 default negative quiet-NaN sign.
inline float Add(float first,float second){unsigned int a=Bits(first),b=Bits(second);if(IsNaN(a))return Value(a|0x00400000U);if(IsNaN(b))return Value(b|0x00400000U);float result=first+second;unsigned int bits=Bits(result);return Value(IsNaN(bits)?0xffc00000U:bits);}
inline float Subtract(float first,float second){unsigned int a=Bits(first),b=Bits(second);if(IsNaN(a))return Value(a|0x00400000U);if(IsNaN(b))return Value(b|0x00400000U);float result=first-second;unsigned int bits=Bits(result);return Value(IsNaN(bits)?0xffc00000U:bits);}
inline float Multiply(float first,float second){unsigned int a=Bits(first),b=Bits(second);if(IsNaN(a))return Value(a|0x00400000U);if(IsNaN(b))return Value(b|0x00400000U);float result=first*second;unsigned int bits=Bits(result);return Value(IsNaN(bits)?0xffc00000U:bits);}
} }

#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_CLIPSUBSECTIONSSIMD \
    do { \
        float dx[4],dy[4],dz[4],distance_sq[4],plane_dot[4]; \
        CSIMDVariableBlock* vars=SIMDVariableBlock; \
        for(int lane=0;lane<4;++lane){dx[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(element->BoundingSphereCenterX[lane],vars->CameraPosX[lane]);dy[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(element->BoundingSphereCenterY[lane],vars->CameraPosY[lane]);dz[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(element->BoundingSphereCenterZ[lane],vars->CameraPosZ[lane]);} \
        if((sub_clip_flags&=0xf0UL)==0){ \
            for(int lane=0;lane<4;++lane){distance_sq[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dx[lane],dx[lane]),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dy[lane],dy[lane])),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dz[lane],dz[lane]));float outer=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(vars->FadeDistance[lane],element->BoundingSphereRadius[lane]);reject_mask|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(outer,outer),distance_sq[lane]))>>31)<<lane;} \
            if(reject_mask==15)break; \
            unsigned long range_pass=0; \
            for(int lane=0;lane<4;++lane){float inner=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(vars->FadeDistance[lane],element->BoundingSphereRadius[lane]);range_pass|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(distance_sq[lane],NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(inner,inner)))>>31)<<lane;} \
            sub_clip_flags|=range_pass<<4; \
        } \
        if((sub_clip_flags&=0xf00UL)==0){ \
            for(int lane=0;lane<4;++lane){plane_dot[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dx[lane],vars->CameraClipPlaneLeftX[lane]),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dy[lane],vars->CameraClipPlaneLeftY[lane])),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dz[lane],vars->CameraClipPlaneLeftZ[lane]));reject_mask|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(element->BoundingSphereRadius[lane],plane_dot[lane]))>>31)<<lane;} \
            if(reject_mask==15)break; \
            unsigned long plane_pass=0; \
            for(int lane=0;lane<4;++lane)plane_pass|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(plane_dot[lane],element->BoundingSphereRadius[lane]))>>31)<<lane; \
            sub_clip_flags|=plane_pass<<8; \
        } \
        if((sub_clip_flags&=0xf000UL)==0){ \
            for(int lane=0;lane<4;++lane){plane_dot[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dx[lane],vars->CameraClipPlaneRightX[lane]),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dy[lane],vars->CameraClipPlaneRightY[lane])),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dz[lane],vars->CameraClipPlaneRightZ[lane]));reject_mask|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(element->BoundingSphereRadius[lane],plane_dot[lane]))>>31)<<lane;} \
            if(reject_mask==15)break; \
            unsigned long plane_pass=0; \
            for(int lane=0;lane<4;++lane)plane_pass|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(plane_dot[lane],element->BoundingSphereRadius[lane]))>>31)<<lane; \
            sub_clip_flags|=plane_pass<<12; \
        } \
        if((sub_clip_flags&=0xf0000UL)==0){ \
            for(int lane=0;lane<4;++lane){plane_dot[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dx[lane],vars->CameraClipPlaneTopX[lane]),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dy[lane],vars->CameraClipPlaneTopY[lane])),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dz[lane],vars->CameraClipPlaneTopZ[lane]));reject_mask|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(element->BoundingSphereRadius[lane],plane_dot[lane]))>>31)<<lane;} \
            if(reject_mask==15)break; \
            unsigned long plane_pass=0; \
            for(int lane=0;lane<4;++lane)plane_pass|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(plane_dot[lane],element->BoundingSphereRadius[lane]))>>31)<<lane; \
            sub_clip_flags|=plane_pass<<16; \
        } \
        if((sub_clip_flags&=0xf00000UL)==0){ \
            for(int lane=0;lane<4;++lane){plane_dot[lane]=NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dx[lane],vars->CameraClipPlaneBottomX[lane]),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dy[lane],vars->CameraClipPlaneBottomY[lane])),NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Multiply(dz[lane],vars->CameraClipPlaneBottomZ[lane]));reject_mask|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Subtract(element->BoundingSphereRadius[lane],plane_dot[lane]))>>31)<<lane;} \
            if(reject_mask==15)break; \
            unsigned long plane_pass=0; \
            for(int lane=0;lane<4;++lane)plane_pass|=(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Bits(NEnginePrimitiveManagerRepeatedStaticMeshes::NPortableClipArithmetic::Add(plane_dot[lane],element->BoundingSphereRadius[lane]))>>31)<<lane; \
            sub_clip_flags|=plane_pass<<20; \
        } \
    }while(false);
#else
#define FABLE_RETAIL_CENGINEPRIMITIVEMANAGERREPEATEDSTATICMESHES_CLIPSUBSECTIONSSIMD \
{ \
__asm mov esi, element \
__asm mov eax, this \
__asm mov edi, dword ptr [eax + 0x90] \
__asm mov ebx, reject_mask \
__asm mov ecx, clip_flags \
__asm movaps xmm0, xmmword ptr [esi] \
__asm movaps xmm1, xmmword ptr [esi + 0x10] \
__asm movaps xmm2, xmmword ptr [esi + 0x20] \
__asm subps xmm0, xmmword ptr [edi + 0xb00] \
__asm subps xmm1, xmmword ptr [edi + 0xb10] \
__asm subps xmm2, xmmword ptr [edi + 0xb20] \
__asm prefetcht0 byte ptr [esi + 0x50] \
__asm prefetcht0 byte ptr [esi + 0x70] \
__asm prefetcht0 byte ptr [esi + 0x90] \
__asm and ecx, 0xf0 \
__asm jne skip_far_check \
__asm movaps xmm3, xmm0 \
__asm movaps xmm4, xmm1 \
__asm movaps xmm5, xmm2 \
__asm mulps xmm3, xmm0 \
__asm mulps xmm4, xmm1 \
__asm mulps xmm5, xmm2 \
__asm addps xmm3, xmm4 \
__asm addps xmm3, xmm5 \
__asm movaps xmm4, xmmword ptr [edi + 0xbf0] \
__asm addps xmm4, xmmword ptr [esi + 0x30] \
__asm mulps xmm4, xmm4 \
__asm subps xmm4, xmm3 \
__asm movmskps eax, xmm4 \
__asm or ebx, eax \
__asm cmp ebx, 0xf \
__asm je reject_all_objects \
__asm movaps xmm4, xmmword ptr [edi + 0xbf0] \
__asm subps xmm4, xmmword ptr [esi + 0x30] \
__asm mulps xmm4, xmm4 \
__asm subps xmm3, xmm4 \
__asm movmskps eax, xmm3 \
__asm shl eax, 4 \
__asm or ecx, eax \
__asm skip_far_check: \
__asm and ecx, 0xf00 \
__asm jne skip_left_clip \
__asm movaps xmm3, xmm0 \
__asm movaps xmm4, xmm1 \
__asm movaps xmm5, xmm2 \
__asm mulps xmm3, xmmword ptr [edi + 0xb30] \
__asm mulps xmm4, xmmword ptr [edi + 0xb40] \
__asm mulps xmm5, xmmword ptr [edi + 0xb50] \
__asm addps xmm3, xmm4 \
__asm addps xmm3, xmm5 \
__asm movaps xmm4, xmmword ptr [esi + 0x30] \
__asm subps xmm4, xmm3 \
__asm movmskps eax, xmm4 \
__asm or ebx, eax \
__asm cmp ebx, 0xf \
__asm je reject_all_objects \
__asm addps xmm3, xmmword ptr [esi + 0x30] \
__asm movmskps eax, xmm3 \
__asm shl eax, 8 \
__asm or ecx, eax \
__asm skip_left_clip: \
__asm and ecx, 0xf000 \
__asm jne skip_right_clip \
__asm movaps xmm3, xmm0 \
__asm movaps xmm4, xmm1 \
__asm movaps xmm5, xmm2 \
__asm mulps xmm3, xmmword ptr [edi + 0xb60] \
__asm mulps xmm4, xmmword ptr [edi + 0xb70] \
__asm mulps xmm5, xmmword ptr [edi + 0xb80] \
__asm addps xmm3, xmm4 \
__asm addps xmm3, xmm5 \
__asm movaps xmm4, xmmword ptr [esi + 0x30] \
__asm subps xmm4, xmm3 \
__asm movmskps eax, xmm4 \
__asm or ebx, eax \
__asm cmp ebx, 0xf \
__asm je reject_all_objects \
__asm addps xmm3, xmmword ptr [esi + 0x30] \
__asm movmskps eax, xmm3 \
__asm shl eax, 0xc \
__asm or ecx, eax \
__asm skip_right_clip: \
__asm and ecx, 0xf0000 \
__asm jne skip_top_clip \
__asm movaps xmm3, xmm0 \
__asm movaps xmm4, xmm1 \
__asm movaps xmm5, xmm2 \
__asm mulps xmm3, xmmword ptr [edi + 0xb90] \
__asm mulps xmm4, xmmword ptr [edi + 0xba0] \
__asm mulps xmm5, xmmword ptr [edi + 0xbb0] \
__asm addps xmm3, xmm4 \
__asm addps xmm3, xmm5 \
__asm movaps xmm4, xmmword ptr [esi + 0x30] \
__asm subps xmm4, xmm3 \
__asm movmskps eax, xmm4 \
__asm or ebx, eax \
__asm cmp ebx, 0xf \
__asm je reject_all_objects \
__asm addps xmm3, xmmword ptr [esi + 0x30] \
__asm movmskps eax, xmm3 \
__asm shl eax, 0x10 \
__asm or ecx, eax \
__asm skip_top_clip: \
__asm and ecx, 0xf00000 \
__asm jne skip_bottom_clip \
__asm movaps xmm3, xmm0 \
__asm movaps xmm4, xmm1 \
__asm movaps xmm5, xmm2 \
__asm mulps xmm3, xmmword ptr [edi + 0xbc0] \
__asm mulps xmm4, xmmword ptr [edi + 0xbd0] \
__asm mulps xmm5, xmmword ptr [edi + 0xbe0] \
__asm addps xmm3, xmm4 \
__asm addps xmm3, xmm5 \
__asm movaps xmm4, xmmword ptr [esi + 0x30] \
__asm subps xmm4, xmm3 \
__asm movmskps eax, xmm4 \
__asm or ebx, eax \
__asm cmp ebx, 0xf \
__asm je reject_all_objects \
__asm addps xmm3, xmmword ptr [esi + 0x30] \
__asm movmskps eax, xmm3 \
__asm shl eax, 0x14 \
__asm or ecx, eax \
__asm skip_bottom_clip: \
__asm mov sub_clip_flags, ecx \
__asm reject_all_objects: \
__asm mov reject_mask, ebx \
}
#endif
#endif
#endif
