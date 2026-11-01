// CDspOps::FilterGain4(float (*dst)[128], float (*src)[128], float* const* states, const SGainParams* const* params, unsigned int count) -- SIMD twin of four CDspGain two-pole filters: gathers one sample of each of the four 128-float source blocks, runs the shared recurrence (y = x + (1-c1-c2) + c1*y1 + c2*y2, out = (t0*y + t1*y1 + t2*y2 - (t0+t1+t2)) * master) four wide and scatters the result into the four destination blocks, keeping the two history samples per channel in `states`
// Grade: DIFFER(681v681) -- x87 operand order of the per-channel coefficient sums (w2 = 1-(c1+c2), w6 = -(t0+t1+t2)) in unrolled copies 1..3 of the fill; the aligned frame, the fill stores, the whole SSE kernel and the write-back are byte-equal
// Leaf: ?FilterGain4@CDspOps@@QAEXPAY0IA@M0PBQAMPBQBUSGainParams@@I@Z
// Retail-asm: 0x00C89050 `shufps xmm0,xmm6,44h` (the four-stride gather built from `movss`/`unpcklps`), 0x00C89001 `subps xmm0,xmm0`
//   -- VC7.1 builds _mm_set_ps through four movss stores and a movaps pair and never zeroes a register with subps, so the
//   kernel 0x00C88FEA..0x00C890BB (fixed esi/edi/ecx/edx, 0x200-byte block strides, `shufps 39h` scatter) is retail inline asm
//   in a C shell: the C prologue fills the 10 aligned float[4] rows (coefficients, histories, master gains) and the C epilogue
//   writes the two history rows back through `states`.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C88E60, 681 bytes, thiscall (`this` unused) `ret 14h`, aligned ebx/ebp frame
//   (`push ebx ; mov ebx,esp ; push ecx ; and esp,-16 ; add esp,4 ; push ebp ; mov ebp,esp ; sub esp,0B8h`), [ebp-4] = the C
//   pointer the asm loads its base from. Caller: CReverbStage94::Process 0x00C75CE0 (landed; binds this symbol and passes
//   m_taps[i].m_gain.m_gain as states[i]). Names: libpk reverb, no PDB/RTTI -- descriptive (the landed CDspOps model).
//   Constant 1.0f 0x0122DED8. The params block fields are read at +8, +0xC (recurrence), +0x10..+0x18 (taps), +0 (master):
//   SGainParams::m_current[1..2], m_target[0..2], m_master in the landed descriptive layout.
struct SGainParams {
    float m_master;                        // +0x00
    float m_current[3];                    // +0x04
    float m_target[3];                     // +0x10
};
struct CDspOps {
    void FilterGain4(float (*dst)[128], float (*src)[128], float* const* states, const SGainParams* const* params, unsigned int count);
};
void CDspOps::FilterGain4(float (*dst)[128], float (*src)[128], float* const* states, const SGainParams* const* params, unsigned int count)
{
    __declspec(align(16)) float w[10][4];
    float* p = &w[0][0];
    for (unsigned int k = 0; k < 4; ++k) {
        const SGainParams* g = params[k];
        w[0][k] = g->m_current[1];
        w[1][k] = g->m_current[2];
        w[2][k] = 1.0f - (g->m_current[1] + g->m_current[2]);
        w[3][k] = g->m_target[0];
        w[4][k] = g->m_target[1];
        w[5][k] = g->m_target[2];
        w[6][k] = -(g->m_target[0] + g->m_target[1] + g->m_target[2]);
        w[7][k] = states[k][0];
        w[8][k] = states[k][1];
        w[9][k] = g->m_master;
    }
#ifdef _MSC_VER
    __asm {
        mov     esi, src
        mov     edi, dst
        mov     ecx, count
        mov     edx, p
        movaps  xmm1, xmmword ptr [edx+70h]
        movaps  xmm2, xmmword ptr [edx+80h]
        subps   xmm0, xmm0
        subps   xmm5, xmm5
        subps   xmm6, xmm6
    next:
        movaps  xmm3, xmmword ptr [edx+10h]
        movaps  xmm4, xmmword ptr [edx+50h]
        mulps   xmm3, xmm2
        mulps   xmm4, xmm2
        movaps  xmm5, xmmword ptr [edx]
        movaps  xmm6, xmmword ptr [edx+40h]
        mulps   xmm5, xmm1
        mulps   xmm6, xmm1
        addps   xmm3, xmm5
        addps   xmm4, xmm6
        movss   xmm0, dword ptr [esi]
        movss   xmm5, dword ptr [esi+200h]
        unpcklps xmm0, xmm5
        movss   xmm6, dword ptr [esi+400h]
        movss   xmm5, dword ptr [esi+600h]
        add     esi, 4
        unpcklps xmm6, xmm5
        shufps  xmm0, xmm6, 44h
        movaps  xmm6, xmmword ptr [edx+20h]
        addps   xmm0, xmm6
        addps   xmm0, xmm3
        movaps  xmm5, xmmword ptr [edx+30h]
        mulps   xmm5, xmm0
        addps   xmm5, xmm4
        movaps  xmm4, xmmword ptr [edx+60h]
        addps   xmm5, xmm4
        movaps  xmm2, xmm1
        movaps  xmm1, xmm0
        movaps  xmm4, xmmword ptr [edx+90h]
        mulps   xmm5, xmm4
        movss   dword ptr [edi], xmm5
        shufps  xmm5, xmm5, 39h
        movss   dword ptr [edi+200h], xmm5
        shufps  xmm5, xmm5, 39h
        movss   dword ptr [edi+400h], xmm5
        shufps  xmm5, xmm5, 39h
        movss   dword ptr [edi+600h], xmm5
        add     edi, 4
        dec     ecx
        jne     next
        movaps  xmmword ptr [edx+70h], xmm1
        movaps  xmmword ptr [edx+80h], xmm2
    }
#endif
    for (unsigned int k = 0; k < 4; ++k) {
        states[k][0] = w[7][k];
        states[k][1] = w[8][k];
    }
}
