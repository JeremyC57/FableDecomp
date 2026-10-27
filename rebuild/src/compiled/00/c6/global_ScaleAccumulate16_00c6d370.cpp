// ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain) -- libpk mixer (SSE2 path): accumulate `count` blocks of eight 16-bit samples into the 32-bit mix buffer, each sample scaled by a Q15 gain (pmulhw, sign-extended, >> 15)
// Leaf: ?ScaleAccumulate16@@
// Retail-asm: 0x00C6D3AC..0x00C6D3FB `mov ecx,[ebx+0Ch] ; mov eax,[ebx+8] ; mov edx,[ebx+10h] ; movdqa xmm7,[ebp-20h] ; movdqu xmm0,[eax] ; pmulhw xmm0,xmm7 ; pxor xmm1,xmm1 ; pxor xmm2,xmm2 ; punpcklwd xmm1,xmm0 ; punpckhwd xmm2,xmm0 ; psrad xmm1,0Fh ; psrad xmm2,0Fh ; paddd xmm1,[edx] ; paddd xmm2,[edx+10h] ; movdqa [edx],xmm1 ; movdqa [edx+10h],xmm2 ; add eax,10h ; add edx,20h ; dec ecx ; jne` -- a hand SSE2 kernel (20 instructions, the loop jne, `align 16` on the loop head: rasm1 family (f)) over the arguments and the aligned gain vector local; the emmintrin spelling allocates registers differently and keeps no fixed xmm7 (rasm1 probe). The eight word stores that fill the gain vector, the /GS cookie and the 16-byte frame alignment (`push ebx ; mov ebx,esp ; and esp,-16`) are C++ (an `__declspec(align(16))` local under /GS).
// Retail: Fable.exe (TLC, VC7.1, /GS) function @ 0x00C6D370, 156 bytes (+4 int3), __cdecl, plain `ret`; the arguments are read
//   through ebx (the saved caller esp) because esp is realigned for the local. Cookie 0x0139C8A8, __security_check_cookie
//   0x00BFE9F9. Twin on the MMX path: 0x00C6F0F0. The catalog name "Audio_ProcessSSEMultiply" is descriptive.
// Flags: /GS
void __cdecl ScaleAccumulate16(const short* src, unsigned long count, long* dst, short gain)
{
    __declspec(align(16)) short gains[8];
    gains[0] = gain;
    gains[1] = gain;
    gains[2] = gain;
    gains[3] = gain;
    gains[4] = gain;
    gains[5] = gain;
    gains[6] = gain;
    gains[7] = gain;
    __asm {
        mov     ecx, count
        mov     eax, src
        mov     edx, dst
        movdqa  xmm7, gains
        align   16
    next:
        movdqu  xmm0, [eax]
        pmulhw  xmm0, xmm7
        pxor    xmm1, xmm1
        pxor    xmm2, xmm2
        punpcklwd xmm1, xmm0
        punpckhwd xmm2, xmm0
        psrad   xmm1, 15
        psrad   xmm2, 15
        paddd   xmm1, [edx]
        paddd   xmm2, [edx + 10h]
        movdqa  [edx], xmm1
        movdqa  [edx + 10h], xmm2
        add     eax, 10h
        add     edx, 20h
        dec     ecx
        jne     next
    }
}