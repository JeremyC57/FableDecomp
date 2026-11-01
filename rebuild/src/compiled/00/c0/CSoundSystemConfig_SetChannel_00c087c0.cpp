// CSoundSystemConfig::SetChannel(unsigned long channel, bool enable) -- set or clear the channel's bit in the record's two 16-bit mask words through SetMaskBit16 (channel, enable, the words, 0)
// Leaf: ?SetChannel@CSoundSystemConfig@@QAEXK_N@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C087C0, length 19 bytes.
// Name (naming wave 20, chunk 687): descriptive (libpk, no PDB) -- called once per set bit of CSoundManagerInit::ChannelMask by the landed
//   CSoundManagerLH ctor 0x00A3EF00 on the config record; was the propagated CMainGameComponent::BeginInputLoading over a BeginLoading stand-in.
// Callees: the landed ?SetMaskBit16@@YIXK_NPAG1@Z 0x00C0AEE0 (`__fastcall`: the channel in ecx, the flag in dl, the mask words and 0 on the stack, `ret 8`).
void __fastcall SetMaskBit16(unsigned long bit, bool set, unsigned short* words, unsigned short* touched);   // 0x00C0AEE0

class CSoundSystemConfig { public:
    unsigned short ChannelMask;                 // +0x00
    unsigned short ChannelTouched;              // +0x02
    void SetChannel(unsigned long channel, bool enable);
};

void CSoundSystemConfig::SetChannel(unsigned long channel, bool enable)
{
    SetMaskBit16(channel, enable, &ChannelMask, 0);
}
