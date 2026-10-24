// Grade: DIFFER(996v978) on the real span -- retail_asm PENDING OWNER DECISION (two 68-instruction SSE blocks > RETAIL_ASM_MAX 8): both SSE blocks now reproduce retail byte-for-byte as __asm (pointer homes [esp+14h..1Ch] reloaded into eax/edx/edi); residue = (1) `this` spilled to [esp+1Ch]/kept in ebx where retail keeps esi, (2) the x87 fallback loops: VC7.1 reassociates the four-term sums (retail order per column of the first loop is 0,2,3,1 / 3,1,2,0 / 3,1,2,0 / 2,0,3,1 against the natural 0,1,2,3 of the second), and a `row < 4` loop over the stack-local operand gets fully unrolled -- two source orders tried (landed r*l form, natural l*r form), both scrambled differently.
// CMatrix4x4::AlignedSetMultiplyTransposed is the inlined helper (FableWin.pdb ?AlignedSetMultiplyTransposed@CMatrix4x4@@QAEXABV1@0@Z).
// CShaderRenderManager::UpdateCombinedProjectionTransform() @ 0x00988A50, real span 978 bytes (catalog 975). This file is
//   the LANDED source unchanged (CShaderRenderManager_UpdateCombinedProjectionTransform_00988a50.cpp, 1048 bytes; its
//   .pre.cpp with optimize("y",off) gives 1030). Evidence for retail asm: (a) an ebp frame with and esp,-16 in an /Oy
//   body; (b) before each multiply the three matrix pointers are stored to [esp+14h..1Ch] and at once reloaded into the
//   fixed registers eax / edx / edi (asm operand loads); (c) the SSE body is unscheduled textbook asm -- load the four
//   left rows into xmm0..3, then per right row movaps xmm4,[eax+k] / movaps xmm5..7,xmm4 / shufps xmmN,xmmN,00/55/AA/FF /
//   mulps / addps 4+5, 6+7, 4+6 / movaps [edi+k],xmm4, where VC7.1 intrinsics interleave and use 2-operand shuffles
//   (see the landed build). The x87 fallback loops (SSE flag 0x013D2880 clear) are C++. 2 x ~68 asm instructions is far
//   over purity.py RETAIL_ASM_MAX = 8 -> owner decision (allow a retail_asm SSE matrix helper, or keep parked).
// Leaf: UpdateCombinedProjectionTransform

typedef unsigned char fable_u8;
typedef unsigned long fable_u32;

extern fable_u8 DAT_013d2880;

struct IDirect3DDevice9ProjectionView
{
    void** vtable;
};

typedef long (__stdcall *SetVertexShaderConstantFFn)(
    IDirect3DDevice9ProjectionView* device,
    unsigned int startRegister,
    const float* constantData,
    unsigned int vector4Count);

struct CShaderConstantRangeProjectionView
{
    void* vtable;
    long offset04;
    fable_u32 count08;
};

struct CVertexShaderConstantLayoutProjectionView
{
    fable_u8 beforeCombinedProjectionMatrix[0x74];
    CShaderConstantRangeProjectionView combinedProjectionMatrix74;
};

__declspec(align(16)) struct CMatrix4x4ProjectionView
{
    float m[4][4];
};

static __forceinline void MultiplyPretransposed(
    CMatrix4x4ProjectionView& output,
    const CMatrix4x4ProjectionView& left,
    const CMatrix4x4ProjectionView& right)
{
    if (DAT_013d2880 != 0)
    {
        const CMatrix4x4ProjectionView* a = &right;
        const CMatrix4x4ProjectionView* b = &left;
        CMatrix4x4ProjectionView* o = &output;
        // Retail-asm: 0x00988A88..0x00988B79 and 0x00988C4A..0x00988D3B (two expansions): the three matrix pointers are
        // stored to [esp+14h..1Ch] and reloaded into the fixed registers eax/edx/edi, then an unscheduled textbook
        // SSE 4x4 multiply (xmm0..3 = left rows; per right row: movaps/3x movaps/4x shufps/4x mulps/3x addps/movaps).
        // VC7.1 intrinsics interleave and use 2-operand shuffles; no compiled C++ reproduces this block.
        __asm {
            mov eax, a
            mov edx, b
            mov edi, o
            movaps xmm0, [edx]
            movaps xmm1, [edx+16]
            movaps xmm2, [edx+32]
            movaps xmm3, [edx+48]
            movaps xmm4, [eax]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xAA
            shufps xmm7, xmm7, 0xFF
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi], xmm4
            movaps xmm4, [eax+16]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xAA
            shufps xmm7, xmm7, 0xFF
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi+16], xmm4
            movaps xmm4, [eax+32]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xAA
            shufps xmm7, xmm7, 0xFF
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi+32], xmm4
            movaps xmm4, [eax+48]
            movaps xmm5, xmm4
            movaps xmm6, xmm4
            movaps xmm7, xmm4
            shufps xmm4, xmm4, 0x00
            shufps xmm5, xmm5, 0x55
            shufps xmm6, xmm6, 0xAA
            shufps xmm7, xmm7, 0xFF
            mulps xmm4, xmm0
            mulps xmm5, xmm1
            mulps xmm6, xmm2
            mulps xmm7, xmm3
            addps xmm4, xmm5
            addps xmm6, xmm7
            addps xmm4, xmm6
            movaps [edi+48], xmm4
        }
    }
    else
    {
        for (int row = 0; row != 4; ++row)
        {
            output.m[row][0] =
                right.m[row][0] * left.m[0][0] +
                right.m[row][1] * left.m[1][0] +
                right.m[row][2] * left.m[2][0] +
                right.m[row][3] * left.m[3][0];
            output.m[row][1] =
                right.m[row][0] * left.m[0][1] +
                right.m[row][1] * left.m[1][1] +
                right.m[row][2] * left.m[2][1] +
                right.m[row][3] * left.m[3][1];
            output.m[row][2] =
                right.m[row][0] * left.m[0][2] +
                right.m[row][1] * left.m[1][2] +
                right.m[row][2] * left.m[2][2] +
                right.m[row][3] * left.m[3][2];
            output.m[row][3] =
                right.m[row][0] * left.m[0][3] +
                right.m[row][1] * left.m[1][3] +
                right.m[row][2] * left.m[2][3] +
                right.m[row][3] * left.m[3][3];
        }
    }
}

class CShaderRenderManagerProjectionView
{
private:
    void UpdateCombinedProjectionTransform();

    fable_u8 beforeCurrentVSConstantLayout[0x04];
    CVertexShaderConstantLayoutProjectionView* currentVSConstantLayout04;
    fable_u8 throughRenderManager008[0x1CC];
    IDirect3DDevice9ProjectionView* d3dDevice1D4;
    void* currentVertexShader1D8;
    void* currentVertexDeclaration1DC;
    fable_u8 currentPixelShader1E0[0x08];
    bool worldMatrixIsIdentity1E8;
    fable_u8 alignWorldMatrix1E9[0x07];
    CMatrix4x4ProjectionView worldMatrix1F0;
    CMatrix4x4ProjectionView viewMatrix230;
    CMatrix4x4ProjectionView projectionMatrix270;
    CMatrix4x4ProjectionView viewProjectionMatrix2B0;
    CMatrix4x4ProjectionView combinedProjectionMatrix2F0;
    fable_u8 throughUpdateFlags330[0xA8];
    fable_u32 updateFlags3D8;
};

void CShaderRenderManagerProjectionView::UpdateCombinedProjectionTransform()
{
    CMatrix4x4ProjectionView worldView;
    MultiplyPretransposed(worldView, worldMatrix1F0, viewMatrix230);
    MultiplyPretransposed(
        combinedProjectionMatrix2F0,
        worldView,
        projectionMatrix270);

    SetVertexShaderConstantFFn setVertexShaderConstantF =
        reinterpret_cast<SetVertexShaderConstantFFn>(
            d3dDevice1D4->vtable[0x178 / 4]);
    setVertexShaderConstantF(
        d3dDevice1D4,
        currentVSConstantLayout04
            ->combinedProjectionMatrix74.offset04,
        &combinedProjectionMatrix2F0.m[0][0],
        4);
    updateFlags3D8 &= ~0x80UL;
}
