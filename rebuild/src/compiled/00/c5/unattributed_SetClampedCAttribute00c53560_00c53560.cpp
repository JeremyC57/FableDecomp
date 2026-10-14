// CAttribute_00c53560::SetClamped(float) -- store the value clamped to [0.0f, 3.163f] at +0x24 and set flag 0x1000
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C53560, oracle length 95 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  fld [value] ; fcomp [lo] ; fnstsw ax ; test ah,5 ; jp B ; mov eax,[ecx] ; fld [lo] ; or eax,0x1000 ; fstp [ecx+0x24] ; mov [ecx],eax ; ret 4 ; B: fld [value] ; fcomp [hi] ; fnstsw ax ; test ah,41h ; jne C ; mov eax,[ecx] ; fld [hi] ; or eax,0x1000 ; fstp [ecx+0x24] ; mov [ecx],eax ; ret 4 ; C: mov eax,[ecx] ; fld [value] ; or eax,0x1000 ; fstp [ecx+0x24] ; mov [ecx],eax ; ret 4
// Clamped attribute setter: the value is clamped to [0.0f, 3.163f] with one nested ternary (so each
// branch's result is an FPU load of the chosen operand, and VC duplicates the store + flag tail
// into all three branches), stored at +0x24, and the "set" bit 0x1000 is or'd into the flags
// dword at +0 (load / or / store interleaved with the FPU copy). The bounds are float literals
// the compiler pools (the retail loads them from .rdata). The manifest label is propagated;
// the attribute class is named by address. One of 9 bodies of this shape.
struct CAttribute_00c53560 {
    unsigned long m_flags;                 // +0
    unsigned char _pad[0x24 - 4];
    float m_value;                         // +0x24
    void SetClamped_CAttribute_00c53560(float value);
};

void CAttribute_00c53560::SetClamped_CAttribute_00c53560(float value)
{
    m_value = value < 0.0f ? 0.0f : (value > 3.163f ? 3.163f : value);
    m_flags |= 0x1000;
}