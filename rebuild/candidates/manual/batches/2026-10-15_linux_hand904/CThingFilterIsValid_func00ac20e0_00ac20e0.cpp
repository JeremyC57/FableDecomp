// CThingFilter_IsValid::func_00ac20e0(long index) -- the 16-bit element at `index` of the word array the object starts with, widened to a long
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC20E0, length 11 bytes (16 with padding); no direct reference. Part of
// the catalogue row 00ac20d0 (skipped ledger, chunk 904); the class label is propagated, so the owner is modelled
// locally (its first member is an array of unsigned shorts).
// Shape: `mov eax,[esp+4] ; movzx eax,word ptr [ecx+eax*2] ; ret 4`.
struct CWordArrayOwner_Retail {
    unsigned short m_words[1];                 // +0x00 (open-ended)
    long func_00ac20e0(long index) const;
};

long CWordArrayOwner_Retail::func_00ac20e0(long index) const
{
    return m_words[index];
}
