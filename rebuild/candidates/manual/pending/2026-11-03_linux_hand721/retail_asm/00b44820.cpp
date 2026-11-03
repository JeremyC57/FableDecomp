// CEngineLightingManager::CalcSWLightingNoClipVector(CRGBFloatColour* colours, const C4DVector* positions, const float* normalX, const float* normalY, const float* normalZ, long count, const CEngineLocalLightArray& lights) -- SSE software lighting of a vertex stream four at a time: global diffuse / backlight / ambient from the SIMD var block, then every local light's attenuated N.L term, colours written unclipped
// Grade: MATCH (572 bytes)
// Flags: none
// Leaf: ?CalcSWLightingNoClipVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
// Retail-asm: 0x00B44839..0x00B44A52 -- the whole vertex loop is Lionhead's hand-written SSE block: `movaps xmm0,[eax+esi]`,
//   `prefetcht0 [eax+esi+20h]`, `rsqrtps xmm1,xmm3` (0x00B449AF), `rcpps xmm3,xmm3` (0x00B449C4), `subps xmm1,xmm1` zeroing,
//   `shufps` transposes and an in-block `push ebx ; push edi` / `pop edi ; pop ebx` (0x00B448EA / 0x00B44A14) around the light
//   loop. VC7.1 has no SSE code generation or intrinsics path that emits prefetcht0 / rsqrtps / rcpps with this register
//   allocation and the asm-only `mov esi,0` (C++ zeroes with xor); this is the engine's __asm body, kept verbatim.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B44820, 572 bytes, ebp frame, `ret 1Ch`.
// Identity (proven): Ego_r.pdb / FableWin.pdb ?CalcSWLightingNoClipVector@CEngineLightingManager@@QAEXPAVCRGBFloatColour@@PBVC4DVector@@PBM22JABVCEngineLocalLightArray@@@Z
//   -- seven arguments (`ret 1Ch`), the colour array written at +0x40 per four vertices, three float streams stepped by 16 bytes,
//   the C4DVector stream by 64, the local light array {count, lights of 0x34 bytes} by reference; donor order puts it first of the
//   CalcSWLighting family, as retail 0x00B44820 before ...SeparateMainLightVector 0x00B44A60.
// C++ around the block: `count /= 4` (cdq / and edx,3 / sar 2 into the argument slot) and the `this` home at [ebp-4] that the
//   block's `mov eax,this` reads; the SIMD var block pointer is CEngineLightingManager::SIMDVarBlock (+0x4690,
//   engine/CEngineLightingManager.h).
#define CEngineLightingManager CEngineLightingManagerLayout
#include "engine/CEngineLightingManager.h"
#undef CEngineLightingManager
class CRGBFloatColour;
class C4DVector;
class CEngineLocalLightArray;
class CEngineLightingManager : public CEngineLightingManagerLayout {
public:
    void CalcSWLightingNoClipVector(CRGBFloatColour* colours, const C4DVector* positions, const float* normalX, const float* normalY,
                                    const float* normalZ, long count, const CEngineLocalLightArray& lights);
};

void CEngineLightingManager::CalcSWLightingNoClipVector(CRGBFloatColour* colours, const C4DVector* positions, const float* normalX,
                                                        const float* normalY, const float* normalZ, long count,
                                                        const CEngineLocalLightArray& lights)
{
    count /= 4;
    __asm {
        mov esi, 0
        mov eax, this
        mov edx, dword ptr [eax+4690h]
        mov ecx, normalZ
        mov edi, colours
        mov ebx, count
    VertexQuadLoop:
        mov eax, normalX
        movaps xmm0, xmmword ptr [eax+esi]
        prefetcht0 byte ptr [eax+esi+20h]
        mov eax, normalY
        movaps xmm1, xmmword ptr [eax+esi]
        prefetcht0 byte ptr [eax+esi+20h]
        mov eax, normalZ
        movaps xmm2, xmmword ptr [eax+esi]
        prefetcht0 byte ptr [eax+esi+20h]
        mulps xmm0, xmmword ptr [edx+20h]
        mulps xmm1, xmmword ptr [edx+30h]
        mulps xmm2, xmmword ptr [edx+40h]
        addps xmm0, xmm1
        addps xmm0, xmm2
        subps xmm1, xmm1
        subps xmm1, xmm0
        maxps xmm0, xmmword ptr [edx]
        maxps xmm1, xmmword ptr [edx]
        mulps xmm1, xmm1
        minps xmm1, xmmword ptr [edx+10h]
        movaps xmm2, xmmword ptr [edx+50h]
        movaps xmm3, xmmword ptr [edx+60h]
        movaps xmm4, xmmword ptr [edx+70h]
        mulps xmm2, xmm1
        mulps xmm3, xmm1
        mulps xmm4, xmm1
        movaps xmm5, xmmword ptr [edx+80h]
        movaps xmm6, xmmword ptr [edx+90h]
        movaps xmm7, xmmword ptr [edx+0A0h]
        mulps xmm5, xmm0
        mulps xmm6, xmm0
        mulps xmm7, xmm0
        addps xmm5, xmm2
        addps xmm6, xmm3
        addps xmm7, xmm4
        addps xmm5, xmmword ptr [edx+0B0h]
        addps xmm6, xmmword ptr [edx+0C0h]
        addps xmm7, xmmword ptr [edx+0D0h]
        push ebx
        push edi
        mov eax, lights
        mov ebx, dword ptr [eax]
        lea edi, [eax+4]
        or ebx, ebx
        je LocalLightsDone
        mov eax, positions
        movaps xmm0, xmmword ptr [eax+esi*4]
        shufps xmm0, xmmword ptr [eax+esi*4+10h], 44h
        movaps xmm1, xmmword ptr [eax+esi*4]
        shufps xmm1, xmmword ptr [eax+esi*4+10h], 0EEh
        movaps xmm2, xmmword ptr [eax+esi*4+20h]
        shufps xmm2, xmmword ptr [eax+esi*4+30h], 44h
        movaps xmm3, xmmword ptr [eax+esi*4+20h]
        shufps xmm3, xmmword ptr [eax+esi*4+30h], 0EEh
        movaps xmm4, xmm0
        shufps xmm4, xmm2, 88h
        shufps xmm0, xmm2, 0DDh
        shufps xmm1, xmm3, 88h
        movaps xmmword ptr [edx+0E0h], xmm4
        movaps xmmword ptr [edx+0F0h], xmm0
        movaps xmmword ptr [edx+100h], xmm1
    LocalLightLoop:
        movss xmm0, dword ptr [edi]
        movss xmm1, dword ptr [edi+4]
        movss xmm2, dword ptr [edi+8]
        shufps xmm0, xmm0, 0
        shufps xmm1, xmm1, 0
        shufps xmm2, xmm2, 0
        subps xmm0, xmmword ptr [edx+0E0h]
        subps xmm1, xmmword ptr [edx+0F0h]
        subps xmm2, xmmword ptr [edx+100h]
        movaps xmm3, xmm0
        mulps xmm3, xmm3
        movaps xmm4, xmm1
        mulps xmm4, xmm4
        addps xmm3, xmm4
        movaps xmm4, xmm2
        mulps xmm4, xmm4
        addps xmm3, xmm4
        mov eax, normalX
        mulps xmm0, xmmword ptr [eax+esi]
        mov eax, normalY
        mulps xmm1, xmmword ptr [eax+esi]
        mov eax, normalZ
        mulps xmm2, xmmword ptr [eax+esi]
        addps xmm0, xmm1
        addps xmm0, xmm2
        rsqrtps xmm1, xmm3
        mulps xmm0, xmm1
        maxps xmm0, xmmword ptr [edx]
        movss xmm1, dword ptr [edi+28h]
        shufps xmm1, xmm1, 0
        mulps xmm3, xmm1
        rcpps xmm3, xmm3
        movss xmm1, dword ptr [edi+2Ch]
        shufps xmm1, xmm1, 0
        addps xmm3, xmm1
        mulps xmm3, xmm0
        maxps xmm3, xmmword ptr [edx]
        minps xmm3, xmmword ptr [edx+10h]
        movss xmm0, dword ptr [edi+18h]
        movss xmm1, dword ptr [edi+1Ch]
        movss xmm2, dword ptr [edi+20h]
        shufps xmm0, xmm0, 0
        shufps xmm1, xmm1, 0
        shufps xmm2, xmm2, 0
        mulps xmm0, xmm3
        mulps xmm1, xmm3
        mulps xmm2, xmm3
        addps xmm5, xmm0
        addps xmm6, xmm1
        addps xmm7, xmm2
        add edi, 34h
        dec ebx
        jne LocalLightLoop
    LocalLightsDone:
        pop edi
        pop ebx
        movaps xmm0, xmm5
        shufps xmm0, xmm6, 44h
        shufps xmm5, xmm6, 0EEh
        movaps xmm2, xmm0
        shufps xmm2, xmm7, 8
        movaps xmmword ptr [edi], xmm2
        shufps xmm0, xmm7, 5Dh
        movaps xmmword ptr [edi+10h], xmm0
        movaps xmm2, xmm5
        shufps xmm2, xmm7, 0A8h
        movaps xmmword ptr [edi+20h], xmm2
        shufps xmm5, xmm7, 0FDh
        movaps xmmword ptr [edi+30h], xmm5
        add esi, 10h
        add edi, 40h
        dec ebx
        jne VertexQuadLoop
    }
}
