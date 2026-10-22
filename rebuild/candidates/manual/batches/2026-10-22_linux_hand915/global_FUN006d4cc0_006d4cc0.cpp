// _global::FUN_006d4cc0() -- read the one-bit flag (bit 0) of the byte at this+0x1C9
// Retail: Fable.exe (TLC, VC7.1) @ 0x006D4CC0, 9 bytes, `ret`. Unreferenced out-of-line copy of an inline
//   one-bit bitfield getter (`mov al,[ecx+1C9h] ; and al,1`): an unsigned char bitfield read, so no `setne`.
//   The owning class is not identified (the row sits among CThing / CIntelligentPointer<CThing> helpers);
//   modelled locally with the flag byte at +0x1C9.
struct CFlagOwner_006d4cc0 {
    unsigned char _pad[0x1C9];
    unsigned char m_flag : 1;                    // +0x1C9 bit 0
    unsigned char FUN_006d4cc0();
};

unsigned char CFlagOwner_006d4cc0::FUN_006d4cc0()
{
    return m_flag;
}
