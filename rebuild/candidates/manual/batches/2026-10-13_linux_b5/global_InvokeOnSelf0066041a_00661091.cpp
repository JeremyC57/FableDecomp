// InvokeOnSelf_0066041a(T*) -- __stdcall thunk: target->Method(target)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00661091, oracle length 18 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
// The manifest label for this body is propagated across byte-identical bodies and is NOT trusted;
// the class is modelled from the bytes only and the name below describes what the code does.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push [ebp+8] ; mov ecx,[ebp+8] ; call Method_0066041a ; pop ebp ; ret 4
// A __stdcall thunk from a translation unit built for size without global optimisation or frame
// pointer omission (`#pragma optimize("gsy",off)` + `("s",on)` reproduces it; plain /O2 emits a
// 13-byte tail jump and a __thiscall member keeps a `this` spill). The argument object is both the
// receiver and the parameter of a one-argument __thiscall method @ 0x0066041A. Five siblings
// (0x0066106D..B5) differ only in the callee.
#pragma optimize("gsy", off)
#pragma optimize("s", on)

struct CSelfTarget_0066041a {
    void Method_0066041a(CSelfTarget_0066041a* argument);   // 0x0066041A
};

void __stdcall InvokeOnSelf_0066041a(CSelfTarget_0066041a* target)
{
    target->Method_0066041a(target);
}
