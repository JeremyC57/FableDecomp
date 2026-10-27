#include <cstdio>
// behaviour test of NVectorMath::GFFinishPartialSlerp (0x00A5C2B0): out = out * weight.X + in * weight.Y per quaternion, strided
#ifdef _MSC_VER
#define ALIGN16 __declspec(align(16))
#else
#define ALIGN16 __attribute__((aligned(16)))
#endif
class CQuaternion { public: float X, Y, Z, W; };
#ifdef _MSC_VER
#pragma optimize("y", off)
void GFFinishPartialSlerp(CQuaternion* out, unsigned long outStride, const CQuaternion* in, unsigned long inStride,
                          const CQuaternion* weights, unsigned long weightStride, unsigned long count)
{
    __asm {
        mov     eax, out
        mov     edx, in
        mov     esi, weights
        mov     ecx, count
        test    ecx, ecx
        je      done
    next:
        movaps  xmm0, [esi]
        movaps  xmm2, [eax]
        movaps  xmm3, [edx]
        add     esi, weightStride
        add     edx, inStride
        movaps  xmm1, xmm0
        shufps  xmm0, xmm0, 0
        shufps  xmm1, xmm1, 55h
        mulps   xmm2, xmm0
        mulps   xmm3, xmm1
        addps   xmm2, xmm3
        movaps  [eax], xmm2
        add     eax, outStride
        dec     ecx
        jne     next
    done:
    }
}
#else
void GFFinishPartialSlerp(CQuaternion* out, unsigned long outStride, const CQuaternion* in, unsigned long inStride,
                          const CQuaternion* weights, unsigned long weightStride, unsigned long count)
{
    for (unsigned long i = 0; i < count; ++i) {
        float a = weights->X, b = weights->Y;
        out->X = out->X * a + in->X * b; out->Y = out->Y * a + in->Y * b;
        out->Z = out->Z * a + in->Z * b; out->W = out->W * a + in->W * b;
        out = (CQuaternion*)((char*)out + outStride); in = (const CQuaternion*)((const char*)in + inStride);
        weights = (const CQuaternion*)((const char*)weights + weightStride);
    }
}
#endif
int main()
{
    ALIGN16 CQuaternion out[4];
    ALIGN16 CQuaternion in[4];
    ALIGN16 CQuaternion weights[4];
    for (int i = 0; i < 4; ++i) {
        out[i].X = 1.0f + i; out[i].Y = 2.0f; out[i].Z = -1.0f; out[i].W = 0.5f;
        in[i].X = 10.0f; in[i].Y = 20.0f + i; in[i].Z = 4.0f; in[i].W = 1.0f;
        weights[i].X = 0.25f; weights[i].Y = 0.5f * i; weights[i].Z = 99.0f; weights[i].W = 99.0f;
    }
    GFFinishPartialSlerp(out, 16, in, 16, weights, 16, 3);
    bool ok = true;
    for (int i = 0; i < 3; ++i) {
        float a = 0.25f, b = 0.5f * i;
        ok = ok && out[i].X == (1.0f + i) * a + 10.0f * b && out[i].Y == 2.0f * a + (20.0f + i) * b
                && out[i].Z == -1.0f * a + 4.0f * b && out[i].W == 0.5f * a + 1.0f * b;
    }
    ok = ok && out[3].X == 4.0f && out[3].Y == 2.0f;              // count 3: the fourth is untouched
    ALIGN16 CQuaternion keep = { 7.0f, 8.0f, 9.0f, 10.0f };
    GFFinishPartialSlerp(&keep, 16, in, 16, weights, 16, 0);      // count 0 does nothing
    ok = ok && keep.X == 7.0f && keep.W == 10.0f;
    std::printf(ok ? "00a5c2b0_TEST PASS\n" : "00a5c2b0_TEST FAIL\n");
    return ok ? 0 : 1;
}