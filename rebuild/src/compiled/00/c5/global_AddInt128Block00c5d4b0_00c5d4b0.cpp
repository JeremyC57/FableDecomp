// AddInt128Block_00c5d4b0(const long* src, long* dst) -- libpk mixer: add 128 packed 32-bit integers from src into dst with SSE2 (eight iterations of four aligned 16-byte paddd)
// Leaf: ?AddInt128Block_00c5d4b0@@
// Retail-asm: 0x00C5D4B3..0x00C5D500 `mov ecx,8 ; mov eax,[ebp+8] ; mov edx,[ebp+0Ch] ; movdqa xmm0,[eax] .. movdqa xmm3,[eax+30h] ; paddd xmm0,[edx] .. paddd xmm3,[edx+30h] ; movdqa [edx],xmm0 .. movdqa [edx+30h],xmm3 ; add eax,40h ; add edx,40h ; dec ecx ; jne` -- a hand SSE2 accumulate loop (19 instructions, the loop jne, an `align 16` on the loop head: rasm1 family (f)): fixed eax/edx/ecx, memory-operand paddd, no spill; the emmintrin spelling (rasm1 probe) allocates registers and the loop counter differently and gives no bare `push ebp ; mov ebp,esp` frame without locals. The whole function is the block; the frame is compiler-emitted.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C5D4B0, 86 bytes, __stdcall (`ret 8`); referenced from the kernel table at 0x012B5DF4.
//   The __cdecl twin is 0x00C5D370 (the other __stdcall copy: 0x00C5D3F0); named per address because each copy is a separate retail function.
//   The catalog name "IntBlock128_AddInPlace_c5d4b0" is descriptive.
void __stdcall AddInt128Block_00c5d4b0(const long* src, long* dst)
{
    __asm {
        mov     ecx, 8
        mov     eax, src
        mov     edx, dst
        align   16                        // the loop head is 16-byte aligned (the `mov edi,edi` pad at 0x00C5D37E)
    next:
        movdqa  xmm0, [eax]
        movdqa  xmm1, [eax + 10h]
        movdqa  xmm2, [eax + 20h]
        movdqa  xmm3, [eax + 30h]
        paddd   xmm0, [edx]
        paddd   xmm1, [edx + 10h]
        paddd   xmm2, [edx + 20h]
        paddd   xmm3, [edx + 30h]
        movdqa  [edx], xmm0
        movdqa  [edx + 10h], xmm1
        movdqa  [edx + 20h], xmm2
        movdqa  [edx + 30h], xmm3
        add     eax, 40h
        add     edx, 40h
        dec     ecx
        jne     next
    }
}