// CSampleScaler_00c6ac60::Scale(void* dest, unsigned short volume) -- for a non-zero packed volume, marks the block as scaled and runs the MMX scaler over 16 samples of the source buffer into dest
// Leaf: Scale
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C6AC60, length 39 bytes.
// Callees: the MMX sample scaler @ 0x00C6AA10 (`__cdecl` (dest, count, source, packed 16-bit volume); manifest
// "Math_ScaleVectorByPackedValue"). Callers 0x00C54F18 / 0x00C5C875 pass the volume zero-extended from a word.
// Label: the manifest's "CTCCarryable::OnKill" is propagated and rejected; the class is named by address.
// Shape: `test ax,ax` on the 16-bit argument, the whole dword pushed; the flag store at +8 is scheduled
// between the argument pushes; `__cdecl` cleanup `add esp,10h`.
void __cdecl ScaleSamplesByPackedVolume(void* dest, long count, const void* source, unsigned short volume);   // 0x00C6AA10

struct CSampleScaler_00c6ac60 {
    unsigned char _pad0[0x08];
    unsigned char m_bScaled;                        // +0x08
    unsigned char _pad9[0x21c - 0x09];
    const void*   m_pSource;                        // +0x21c
    void Scale(void* dest, unsigned short volume);
};

void CSampleScaler_00c6ac60::Scale(void* dest, unsigned short volume)
{
    if (volume) {
        m_bScaled = 1;
        ScaleSamplesByPackedVolume(dest, 0x10, m_pSource, volume);
    }
}