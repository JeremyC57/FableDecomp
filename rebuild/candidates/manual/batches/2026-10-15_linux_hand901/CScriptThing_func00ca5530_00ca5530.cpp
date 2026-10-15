// CScriptThing::func_00ca5530() -- return the address of the sub-object at +0xa8
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5530, length 7 bytes (16 with padding); no direct reference found.
// Part of the catalogue row 00ca5510 (skipped ledger, chunk 901); the manifest class is propagated, so `this` is
// modelled locally (see 0x00CA5520 and 0x00CA5540 for the neighbouring members).
// Shape: `lea eax,[ecx+0A8h] ; ret`.
struct CLibrarySubObject_Retail {
    long m_first;
};

struct CLibraryObject_Retail {
    unsigned char           _pad_0x00[0xa8];
    CLibrarySubObject_Retail m_subA8;          // +0xa8
    CLibrarySubObject_Retail* func_00ca5530();
};

CLibrarySubObject_Retail* CLibraryObject_Retail::func_00ca5530()
{
    return &m_subA8;
}
