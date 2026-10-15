// CScriptThing::func_008aae70() -- return the word at +0x38
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AAE70, length 4 bytes (16 with padding); no direct reference found.
// Part of the catalogue row 008aae20 (skipped ledger, chunk 901). The manifest class is propagated: the
// 0xc-byte CScriptThing has no +0x38, so `this` is modelled locally (see 0x008AAE60 for +0x7c).
// Shape: `mov eax,[ecx+38h] ; ret`.
struct CScriptThing_Owner_Retail {
    unsigned char _pad_0x00[0x38];
    long          m_word38;                    // +0x38
    unsigned char _pad_0x3c[0x7c - 0x3c];
    long          m_word7C;                    // +0x7c (0x008AAE60 returns this one)
    long func_008aae70();
};

long CScriptThing_Owner_Retail::func_008aae70()
{
    return m_word38;
}
