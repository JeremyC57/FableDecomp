// CScaledByteOwner_00643a8e::ScaleByte(unsigned char value) -- convert a byte count to a whole number through the float scale at +0x2A0 (`(long)(value * scale)`)
// Leaf: ?ScaleByte@CScaledByteOwner_00643a8e
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00643A8E, length 28 bytes, thiscall `ret 4`; size-optimised TU (packed
// between 0x00643A7D -- `fld [ecx+2A0h] ; fmul [const] ; jmp __ftol2`, the same scale -- and 0x00643AAA).
// Callee: __ftol2 @ 0x00BFEA70 (the float -> long conversion).
// Shape: the byte is widened in its own argument slot (`movzx eax,[ebp+8] ; mov [ebp+8],eax ; fild dword [ebp+8]`),
// which needs the ebp frame the size TU keeps for a parameter whose address is taken by the int->float load; then
// `fmul [ecx+2A0h]` and the __ftol2 call.
// Identity: no call site or vtable slot references this body (xref.py finds none); the owning class (a float at +0x2A0,
// a byte at +0x2BC read by 0x00643A76) is not recovered, so the class and method names are descriptive. The catalog's
// `_Cons_val<allocator<pair<EHeroMorphType,CParticleMorphs::CEntry>>>` label is propagated.
#pragma optimize("s", on)
struct CScaledByteOwner_00643a8e {
    unsigned char _pad_0x00[0x2A0];
    float m_scale;                               // +0x2A0
    long ScaleByte(unsigned char value);
};

long CScaledByteOwner_00643a8e::ScaleByte(unsigned char value)
{
    return (long)(value * m_scale);
}