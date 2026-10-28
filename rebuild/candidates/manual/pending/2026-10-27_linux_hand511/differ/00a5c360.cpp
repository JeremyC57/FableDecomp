// NVectorMath::GFNormalise3DVectors(const C4DVector* src, unsigned long srcStride, C4DVector* dst, unsigned long dstStride, unsigned long count) -- normalise the xyz part of `count` 4D vectors into dst (w = 0): a hand SSE loop two vectors at a time when the CPU has SSE, else an x87 loop
// Grade: DIFFER(386v421) -- retail_asm candidate; the SSE block matches. Residue: (1) the prologue -- retail builds the aligned frame with a return-address copy (`sub esp,8 ; and esp,-16 ; add esp,4 ; push ebp ; mov ebp,[ebx+4] ; mov [esp+4],ebp ; mov ebp,esp ; sub esp,38h`), ours `push ecx ; and ; add ; push ebp ; mov ebp,esp ; sub esp,2Ch` (also with /Oy- and optimize("y",off)); (2) frame slots 0x10 lower in retail (epsilon -0x30, steps -4/-8); (3) the x87 fallback keeps only z in a frame local (retail) where ours spills y and z, and retail saves edi
// Retail-asm: 0x00A5C3B6..0x00A5C465 `mov eax,[dst] ; mov edx,[src] ; mov ecx,[count] ; push ecx ; shr ecx,1 ; ... movaps xmm0,[edx] ; mulps ; shufps 4Eh/11h ; addps ; sqrtps ; maxps xmm1,[epsilon] ; divps ; movaps [eax],xmm0 ; ... pop ecx ; and ecx,1` -- a hand SSE block (fixed registers, `push ecx`/`pop ecx` around the pair loop, the W words of the const source cleared with `mov dword [edx+0Ch],0`); VC7.1 C++ never emits SSE without intrinsics, and its xmmintrin spelling allocates the xmm registers itself. Same TU as the landed retail_asm GFClear4DVectors 0x00A5C300 (the SSE flag 0x013D2880).
// Leaf: ?GFNormalise3DVectors@NVectorMath@@YAXPBVC4DVector@@KPAV2@KK@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A5C360, 421 bytes, __cdecl, plain `ret`; the 16-byte aligned frame (`and esp,-16`,
//   ebx = the incoming esp, ebp = the aligned frame) comes from the aligned epsilon vector local.
// Identity: both PDBs name `?GFNormalise3DVectors@NVectorMath@@YAXPBVC4DVector@@KPAV2@KK@Z` (const C4DVector*, ulong,
//   C4DVector*, ulong, ulong; cdecl), matching the five stack arguments; callers 0x009876C9, 0x009876E8, 0x00989EDF
//   (CShaderRenderManager), 0x00BB251F.
// Globals: the SSE-available byte 0x013D2880, 1.0f 0x0122DED8, 0.0f 0x0122DEDC.
class C4DVector {
public:
    float X, Y, Z, W;
    C4DVector() { }
    C4DVector(float x, float y, float z, float w) : X(x), Y(y), Z(z), W(w) { }
    C4DVector(const C4DVector& other) : X(other.X), Y(other.Y), Z(other.Z), W(other.W) { }
    C4DVector& operator=(const C4DVector& other) { X = other.X; Y = other.Y; Z = other.Z; W = other.W; return *this; }
};
extern bool g_SSEAvailable;                                          // 0x013D2880
extern "C" double __cdecl sqrt(double);
#pragma intrinsic(sqrt)

namespace NVectorMath {
#pragma optimize("y", off)
void GFNormalise3DVectors(const C4DVector* src, unsigned long srcStride, C4DVector* dst, unsigned long dstStride,
                          unsigned long count)
{
    if (g_SSEAvailable) {
        __declspec(align(16)) float epsilon[4];
        epsilon[0] = 0.01f;
        epsilon[1] = 0.01f;
        epsilon[2] = 0.01f;
        epsilon[3] = 0.01f;
        unsigned long srcStep = srcStride * 2;
        unsigned long dstStep = dstStride * 2;
        __asm {
            mov     eax, dst
            mov     edx, src
            mov     ecx, count
            push    ecx
            shr     ecx, 1
            je      single
        pair:
            mov     dword ptr [edx + 0Ch], 0
            mov     dword ptr [edx + 1Ch], 0
            movaps  xmm0, [edx]
            movaps  xmm4, [edx + 10h]
            movaps  xmm1, xmm0
            movaps  xmm5, xmm4
            mulps   xmm1, xmm1
            mulps   xmm5, xmm5
            movaps  xmm2, xmm1
            movaps  xmm6, xmm5
            shufps  xmm2, xmm2, 4Eh
            shufps  xmm6, xmm6, 4Eh
            addps   xmm1, xmm2
            addps   xmm5, xmm6
            movaps  xmm2, xmm1
            movaps  xmm6, xmm5
            shufps  xmm2, xmm2, 11h
            shufps  xmm6, xmm6, 11h
            addps   xmm1, xmm2
            addps   xmm5, xmm6
            sqrtps  xmm1, xmm1
            sqrtps  xmm5, xmm5
            maxps   xmm1, epsilon
            maxps   xmm5, epsilon
            divps   xmm0, xmm1
            divps   xmm4, xmm5
            movaps  [eax], xmm0
            movaps  [eax + 10h], xmm4
            add     eax, dstStep
            add     edx, srcStep
            dec     ecx
            jne     pair
        single:
            pop     ecx
            and     ecx, 1
            je      done
            mov     dword ptr [edx + 0Ch], 0
            movaps  xmm0, [edx]
            movaps  xmm1, xmm0
            mulps   xmm1, xmm1
            movaps  xmm2, xmm1
            shufps  xmm2, xmm2, 4Eh
            addps   xmm1, xmm2
            movaps  xmm2, xmm1
            shufps  xmm2, xmm2, 11h
            addps   xmm1, xmm2
            sqrtps  xmm1, xmm1
            maxps   xmm1, epsilon
            divps   xmm0, xmm1
            movaps  [eax], xmm0
        done:
        }
    } else {
        for (unsigned long i = 0; i < count; ++i) {
            float x = src->X;
            float y = src->Y;
            float z = src->Z;
            float r = 1.0f / (float)sqrt(z * z + y * y + x * x);
            *dst = C4DVector(r * x, r * y, r * z, r * 0.0f);
            ++src;
            ++dst;
        }
    }
}
}   // namespace NVectorMath
