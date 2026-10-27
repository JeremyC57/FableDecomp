// GFFloatToLongFloor(float value) -- floor(value) as a long through the engine's inline-assembly FPU floor helper: round (value - 0.5) to nearest, then step up one when (result + 1) equals value exactly
// Leaf: GFFloatToLongFloor
// Retail-asm: 0x0048D954..0x0048D96E `fld [ebp+8] ; fsub [ebp-0Ch] ; fistp dword ptr [ebp-4] ; fild [ebp-4] ; fadd [ebp-8] ; fstp [ebp-10h]` on named memory locals (0.5f / 1.0f stored as frame constants, a bare dword fistp into a named long): VC7.1 emits no bare dword fistp from C++ (`(long)x` calls __ftol2, /QIfist gives `fistp qword`, /arch:SSE2 gives cvttss2si -- rasm1 probe family (a)), so the block is the original's `__asm`, including its fix-up (`mov eax,[ebp+8] ; cmp eax,[ebp-10h] ; jne ; inc dword ptr [ebp-4]`: a memory increment VC7.1 would hoist into a register from C++) -- rasm1 family (b), 10 instructions with one jne.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0048D940, 58 bytes, `?GFFloatToLongFloor@@YIJM@Z` (__fastcall, the float on the
// stack, `ret 4`); no callers in the image (the callers inline the helper, e.g. 0x00A52650). The standalone twin of the
// ceiling helper landed at 0x004FA9A0. The inline asm on frame locals forces the ebp frame.
#pragma optimize("y", off)
long __fastcall GFFloatToLongFloor(float value)
{
    long  result;
    float one;
    float half;
    float next;
    half = 0.5f;
    one = 1.0f;
    __asm {
        fld   value
        fsub  half
        fistp result
        fild  result
        fadd  one
        fstp  next
        mov   eax, value
        cmp   eax, next
        jne   done
        inc   result
    done:
    }
    return result;
}