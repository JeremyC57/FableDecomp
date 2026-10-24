// Leaf: UpdateCombinedProjectionTransform
// Grade: DIFFER(1048v978) -- residue: retail ran two inline-asm SSE 4x4 matrix multiplies (~68 instructions each); the intrinsics version cannot reproduce them
// CShaderRenderManager::UpdateCombinedProjectionTransform() @ 0x00988A50, real span 978 bytes (catalog 975). This file is
//   the LANDED source unchanged (CShaderRenderManager_UpdateCombinedProjectionTransform_00988a50.cpp, 1048 bytes; its
//   .pre.cpp with optimize("y",off) gives 1030). Evidence for retail asm: (a) an ebp frame with and esp,-16 in an /Oy
//   body; (b) before each multiply the three matrix pointers are stored to [esp+14h..1Ch] and at once reloaded into the
//   fixed registers eax / edx / edi (asm operand loads); (c) the SSE body is unscheduled textbook asm -- load the four
//   left rows into xmm0..3, then per right row movaps xmm4,[eax+k] / movaps xmm5..7,xmm4 / shufps xmmN,xmmN,00/55/AA/FF /
//   mulps / addps 4+5, 6+7, 4+6 / movaps [edi+k],xmm4, where VC7.1 intrinsics interleave and use 2-operand shuffles
//   (see the landed build). The x87 fallback loops (SSE flag 0x013D2880 clear) are C++. 2 x ~68 asm instructions is far
//   over purity.py RETAIL_ASM_MAX = 8 -> owner decision (allow a retail_asm SSE matrix helper, or keep parked).
#include <xmmintrin.h>

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
        const __m128 left0 = _mm_load_ps(left.m[0]);
        const __m128 left1 = _mm_load_ps(left.m[1]);
        const __m128 left2 = _mm_load_ps(left.m[2]);
        const __m128 left3 = _mm_load_ps(left.m[3]);

#define FABLE_MULTIPLY_SSE_ROW(row_)                                             \
        {                                                                         \
            __m128 part0 = _mm_load_ps(right.m[row_]);                            \
            __m128 part1 = part0;                                                 \
            __m128 part2 = part0;                                                 \
            __m128 part3 = part0;                                                 \
            part0 = _mm_shuffle_ps(part0, part0, _MM_SHUFFLE(0, 0, 0, 0));        \
            part1 = _mm_shuffle_ps(part1, part1, _MM_SHUFFLE(1, 1, 1, 1));        \
            part2 = _mm_shuffle_ps(part2, part2, _MM_SHUFFLE(2, 2, 2, 2));        \
            part3 = _mm_shuffle_ps(part3, part3, _MM_SHUFFLE(3, 3, 3, 3));        \
            part0 = _mm_mul_ps(part0, left0);                                     \
            part1 = _mm_mul_ps(part1, left1);                                     \
            part2 = _mm_mul_ps(part2, left2);                                     \
            part3 = _mm_mul_ps(part3, left3);                                     \
            _mm_store_ps(                                                         \
                output.m[row_],                                                   \
                _mm_add_ps(_mm_add_ps(part0, part1), _mm_add_ps(part2, part3)));  \
        }
        FABLE_MULTIPLY_SSE_ROW(0);
        FABLE_MULTIPLY_SSE_ROW(1);
        FABLE_MULTIPLY_SSE_ROW(2);
        FABLE_MULTIPLY_SSE_ROW(3);
#undef FABLE_MULTIPLY_SSE_ROW
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
