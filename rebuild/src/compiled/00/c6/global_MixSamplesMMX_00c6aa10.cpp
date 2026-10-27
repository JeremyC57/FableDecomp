// MixSamplesMMX(const unsigned char* source, long quadCount, long* accumulator, unsigned short volume) -- libpk mixer (MMX path): accumulate `count` blocks of eight 16-bit samples into the 32-bit mix buffer, each sample scaled by a Q15 gain (pmulhw, sign-extended, >> 15), two quadwords per iteration
// Leaf: ?MixSamplesMMX@@YAXPBEJPAJG@Z
// Retail-asm: 0x00C6AA3C..0x00C6AAAC `mov ecx,[ebx+0Ch] ; mov eax,[ebx+8] ; mov edx,[ebx+10h] ; movq mm7,[ebp-20h] ; movq mm0,[eax] ; pmulhw mm0,mm7 ; pxor mm1,mm1 ; pxor mm2,mm2 ; punpcklwd mm1,mm0 ; punpckhwd mm2,mm0 ; psrad mm1,0Fh ; psrad mm2,0Fh ; paddd mm1,[edx] ; paddd mm2,[edx+8] ; movq [edx],mm1 ; movq [edx+8],mm2 ; movq mm3,[eax+8] .. movq [edx+18h],mm5 ; add eax,10h ; add edx,20h ; dec ecx ; jne ; emms` -- the hand MMX kernel of the landed retail_asm twin ScaleAccumulate16MMX 0x00C6F0F0 (byte-identical; `align 16` on the loop head); mmintrin gives a different register allocation. The four word stores that fill the gain vector, the /GS cookie and the 16-byte frame alignment are C++ (an `__declspec(align(16))` local under /GS).
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1, /GS) @ 0x00c6aa10, 173 bytes, __cdecl, plain `ret`; arguments read through ebx (the saved
//   caller esp) because esp is realigned for the local. Cookie 0x0139C8A8, __security_check_cookie 0x00BFE9F9.
// Identity (descriptive): bound ?MixSamplesMMX@@YAXPBEJPAJG@Z by the landed CSoundMixer::MixBlocks 0x00C6ACC0, whose (source, count, accumulator, volume) order is the body's (eax is read, edx accumulated); the landed CSampleScaler_00c6ac60::Scale 0x00C6AC60 binds it as ?ScaleSamplesByPackedVolume@@YAXPAXJPBXG@Z with destination and source swapped -- a label correction.
void __cdecl MixSamplesMMX(const unsigned char* source, long quadCount, long* accumulator, unsigned short volume)
{
    __declspec(align(16)) unsigned short gains[4];
    gains[0] = volume;
    gains[1] = volume;
    gains[2] = volume;
    gains[3] = volume;
    __asm {
        mov     ecx, quadCount
        mov     eax, source
        mov     edx, accumulator
        movq    mm7, gains
        align   16
    next:
        movq    mm0, [eax]
        pmulhw  mm0, mm7
        pxor    mm1, mm1
        pxor    mm2, mm2
        punpcklwd mm1, mm0
        punpckhwd mm2, mm0
        psrad   mm1, 15
        psrad   mm2, 15
        paddd   mm1, [edx]
        paddd   mm2, [edx + 8]
        movq    [edx], mm1
        movq    [edx + 8], mm2
        movq    mm3, [eax + 8]
        pmulhw  mm3, mm7
        pxor    mm4, mm4
        pxor    mm5, mm5
        punpcklwd mm4, mm3
        punpckhwd mm5, mm3
        psrad   mm4, 15
        psrad   mm5, 15
        paddd   mm4, [edx + 10h]
        paddd   mm5, [edx + 18h]
        movq    [edx + 10h], mm4
        add     eax, 10h
        movq    [edx + 18h], mm5
        add     edx, 20h
        dec     ecx
        jne     next
        emms
    }
}