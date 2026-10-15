// CScriptThing::func_008aae60() -- return the word at +0x7c
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008AAE60, length 4 bytes (16 with padding); no direct reference found.
// Part of the catalogue row 008aae20 (skipped ledger, chunk 901). The manifest class is propagated: the
// 0xc-byte CScriptThing has no +0x7c, so `this` is modelled locally (an object in the same TU as operator==).
// Shape: `mov eax,[ecx+7Ch] ; ret`.
struct CScriptThing_Owner_Retail {
    unsigned char _pad_0x00[0x38];
    long          m_word38;                    // +0x38 (0x008AAE70 returns this one)
    unsigned char _pad_0x3c[0x7c - 0x3c];
    long          m_word7C;                    // +0x7c
    long func_008aae60();
};

long CScriptThing_Owner_Retail::func_008aae60()
{
    return m_word7C;
}