// ZeroBlock128MMX_00c700d0(void* dst) -- libpk mixer helper: zero 128 bytes at dst with sixteen MMX quadword stores, then emms
// Leaf: ?ZeroBlock128MMX_00c700d0@@YAXPAX@Z
// Retail-asm: 0x00C700D5..0x00C70117 `pxor mm0,mm0 ; movq qword ptr [edi],mm0 .. movq [edi+78h],mm0 ; emms` -- a hand MMX store block on a fixed edi loaded from the argument (`mov edi,[esp+8]`, the asm's `mov edi,dst`; `push edi` saves exactly the register the block names): the VC7.1 RTM include set has no mmintrin.h (GOTCHAS 0xA44250); it sits right after the landed MMX 128-byte copy 0x00C70040 (rasm1 family (f)), whose shape it shares. The frame is compiler-emitted.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C700D0, 75 bytes (+ int3 padding), __cdecl (one stack argument, plain `ret`).
// Callers: none found (the only "reference" xref.py reports is a coincidental dword inside a _PDATA word table at
//   0x01455850); the effect classes inline the same block (0x00C701E0, 0x00C6F300).
// Name: descriptive with the address suffix (libpk, no PDB/RTTI; the manifest label `_gapscan::sub_00c700d0` is a
//   placeholder).
void __cdecl ZeroBlock128MMX_00c700d0(void* dst)
{
    __asm {
        mov     edi, dst
        pxor    mm0, mm0
        movq    [edi], mm0
        movq    [edi + 8h], mm0
        movq    [edi + 10h], mm0
        movq    [edi + 18h], mm0
        movq    [edi + 20h], mm0
        movq    [edi + 28h], mm0
        movq    [edi + 30h], mm0
        movq    [edi + 38h], mm0
        movq    [edi + 40h], mm0
        movq    [edi + 48h], mm0
        movq    [edi + 50h], mm0
        movq    [edi + 58h], mm0
        movq    [edi + 60h], mm0
        movq    [edi + 68h], mm0
        movq    [edi + 70h], mm0
        movq    [edi + 78h], mm0
        emms
    }
}