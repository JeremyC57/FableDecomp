// CAudioFormat::GetLoopEnd() -- the loop end of the sample (+8), or the sample's length (+0) when no loop end is set
// Leaf: ?GetLoopEnd@CAudioFormat@@QAEKXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C2D910, 10 bytes: mov eax,[ecx+8] ; test eax,eax ; jne done ; mov eax,[ecx] ; done: ret
// Identity (descriptive): a CAudioFormat accessor (the landed descriptive class of 0x00C2D790..0x00C2D970: length +0,
//   loop start +4, codec +0xE, rate +0x10). Its callers 0x00C2739A and 0x00C351E0 subtract the loop start (getter
//   0x00C2D900) from its result to get the loop's length, so the +8 word is the loop END, defaulting to the length.
//   The catalog's "CTCWeapon::SetAiming" is a propagated game label (no PDB covers libpk).
// Shape: the ternary over the member gives one load, the test and the fallback load.
struct CAudioFormat {                           // the landed 0x00C2D800 / 0x00C2D970 model
    unsigned long  m_length;                    // +0x00
    unsigned long  m_loopStart;                 // +0x04
    unsigned long  m_loopEnd;                   // +0x08 (0 = the end of the sample; the landed rows call it m_loopLength)
    unsigned short m_channels;                  // +0x0C
    unsigned short m_codec;                     // +0x0E
    unsigned long  m_sampleRate;                // +0x10
    unsigned long GetLoopEnd();
};

unsigned long CAudioFormat::GetLoopEnd()
{
    return m_loopEnd ? m_loopEnd : m_length;
}