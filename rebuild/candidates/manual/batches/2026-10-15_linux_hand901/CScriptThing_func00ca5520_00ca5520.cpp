// CScriptThing::func_00ca5520() -- return the byte at +0xa1 widened to a long
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CA5520, length 8 bytes (16 with padding); no direct reference found.
// Part of the catalogue row 00ca5510 (skipped ledger, chunk 901); the manifest class is propagated (an object in
// the 0x00CAxxxx library region with a byte at +0xa1, a sub-object at +0xa8 and a long at +0xf0), so `this` is
// modelled locally.
// Shape: `movzx eax,byte ptr [ecx+0A1h] ; ret`.
struct CLibraryObject_Retail {
    unsigned char _pad_0x00[0xa1];
    unsigned char m_byteA1;                    // +0xa1
    long func_00ca5520();
};

long CLibraryObject_Retail::func_00ca5520()
{
    return m_byteA1;
}
