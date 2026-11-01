// CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4&) -- this = (copy of this) * m: hand SSE 4x4 product when g_SSEAvailable, x87 row loop otherwise
// Grade: DIFFER(472v471) -- only the x87 fallback's faddp term order in columns 0/2/3 (VC7.1 canonicalises the 4-term sum; 30 diff lines); prologue, rep movsd copy, the whole SSE asm block and the loop control match. next: more expression forms of the fallback sum (the order follows the expression's address forms, not term order).
// Leaf: ?AlignedPreMultiplyTransposed@CMatrix4x4@@QAEXABV1@@Z
// Retail-asm: 0x00ADEFEA..0x00ADF0E7 `mov [esp+1Ch],eax ; mov [esp+18h],ecx ; mov [esp+14h],edx ; mov eax,[esp+1Ch] ; mov edx,[esp+18h] ; mov edi,[esp+14h] ; movaps xmm0,[edx] ... shufps xmm4,xmm4,0 ; shufps xmm5,xmm5,55h ; mulps xmm4,xmm0 ; addps xmm4,xmm5 ; movaps [edi],xmm4` -- the hand SSE kernel of the CMatrix4x4 Aligned* family (same shape as the landed retail_asm AlignedSetInterpolate 0x00A5CEE0): three C locals homed to frame words and reloaded into fixed eax/edx/edi, fixed xmm0..xmm7 with the broadcast shufps 0/55h/0AAh/0FFh. The store-then-reload of the pointers is the __asm-block signature; xmmintrin intrinsics keep the pointers in registers and pick their own xmm registers.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADEFC0, 471 bytes, __thiscall `ret 4`, `and esp,-16` frame (0x58 + esi/edi).
// Name: FableWin.pdb name set ?AlignedPreMultiplyTransposed@CMatrix4x4@@QAEXABV1@@Z (void, one const CMatrix4x4&, thiscall = `ret 4`
//   with this in ecx), next to AlignedSetMultiplyTransposed / AlignedSetMultiply in donor order; "Aligned" = movaps on this
//   and m plus the g_SSEAvailable 0x013D2880 branch (the landed Aligned* rows); "PreMultiplyTransposed": on the stored
//   (transposed) matrices it computes this = this * m. The only other one-matrix candidate, PostMultiply (FableWin, 0x130
//   debug bytes), has no SSE path. No caller in retail (like crc32_big, an unreferenced body kept by the link); it sits
//   after C3DBoundingBox::BuildFromPoints in that TU.
// Shape: `copy = *this` is the rep movsd (16 dwords) into the 16-aligned local; the SSE block reads copy rows as the
//   broadcast operand (eax), m rows as xmm0..xmm3 (edx), writes this (edi); the fallback is a row loop whose 4-column
//   inner loop VC7.1 unrolls (esi = 4 trip counter, eax/edx stepped 0x10). A function holding __asm is never inlined,
//   so the asm sits in this body (an inline AlignedSetMultiply helper stays a call: probe 1).
extern bool g_SSEAvailable;                        // 0x013D2880
class __declspec(align(16)) CMatrix4x4 {
public:
    float m[4][4];
    void AlignedPreMultiplyTransposed(const CMatrix4x4& other);
};

void CMatrix4x4::AlignedPreMultiplyTransposed(const CMatrix4x4& other)
{
    CMatrix4x4 copy = *this;
    if (g_SSEAvailable) {
        const CMatrix4x4* pa = &copy;
        const CMatrix4x4* pb = &other;
        CMatrix4x4* self = this;
        __asm {
            mov     eax, pa
            mov     edx, pb
            mov     edi, self
            movaps  xmm0, [edx]
            movaps  xmm1, [edx+10h]
            movaps  xmm2, [edx+20h]
            movaps  xmm3, [edx+30h]
            movaps  xmm4, [eax]
            movaps  xmm5, xmm4
            movaps  xmm6, xmm4
            movaps  xmm7, xmm4
            shufps  xmm4, xmm4, 0
            shufps  xmm5, xmm5, 55h
            shufps  xmm6, xmm6, 0AAh
            shufps  xmm7, xmm7, 0FFh
            mulps   xmm4, xmm0
            mulps   xmm5, xmm1
            mulps   xmm6, xmm2
            mulps   xmm7, xmm3
            addps   xmm4, xmm5
            addps   xmm6, xmm7
            addps   xmm4, xmm6
            movaps  [edi], xmm4
            movaps  xmm4, [eax+10h]
            movaps  xmm5, xmm4
            movaps  xmm6, xmm4
            movaps  xmm7, xmm4
            shufps  xmm4, xmm4, 0
            shufps  xmm5, xmm5, 55h
            shufps  xmm6, xmm6, 0AAh
            shufps  xmm7, xmm7, 0FFh
            mulps   xmm4, xmm0
            mulps   xmm5, xmm1
            mulps   xmm6, xmm2
            mulps   xmm7, xmm3
            addps   xmm4, xmm5
            addps   xmm6, xmm7
            addps   xmm4, xmm6
            movaps  [edi+10h], xmm4
            movaps  xmm4, [eax+20h]
            movaps  xmm5, xmm4
            movaps  xmm6, xmm4
            movaps  xmm7, xmm4
            shufps  xmm4, xmm4, 0
            shufps  xmm5, xmm5, 55h
            shufps  xmm6, xmm6, 0AAh
            shufps  xmm7, xmm7, 0FFh
            mulps   xmm4, xmm0
            mulps   xmm5, xmm1
            mulps   xmm6, xmm2
            mulps   xmm7, xmm3
            addps   xmm4, xmm5
            addps   xmm6, xmm7
            addps   xmm4, xmm6
            movaps  [edi+20h], xmm4
            movaps  xmm4, [eax+30h]
            movaps  xmm5, xmm4
            movaps  xmm6, xmm4
            movaps  xmm7, xmm4
            shufps  xmm4, xmm4, 0
            shufps  xmm5, xmm5, 55h
            shufps  xmm6, xmm6, 0AAh
            shufps  xmm7, xmm7, 0FFh
            mulps   xmm4, xmm0
            mulps   xmm5, xmm1
            mulps   xmm6, xmm2
            mulps   xmm7, xmm3
            addps   xmm4, xmm5
            addps   xmm6, xmm7
            addps   xmm4, xmm6
            movaps  [edi+30h], xmm4
        }
    } else {
        for (long row = 0; row < 4; ++row) {
            const float* a = copy.m[row];
            for (long column = 0; column < 4; ++column) {
                m[row][column] = a[0] * other.m[0][column] + a[1] * other.m[1][column]
                               + a[2] * other.m[2][column] + a[3] * other.m[3][column];
            }
        }
    }
}
