#ifndef FABLE_ENGINE_CENGINEPRIMITIVETHEMECHANNELDESC_H
#define FABLE_ENGINE_CENGINEPRIMITIVETHEMECHANNELDESC_H
// New shared header (chunk 728, row 0x0074BC20): Ego_r.pdb CEnginePrimitiveThemeChannelDesc (ghidra_out/struct_layouts_egor.tsv),
// sizeof 4: ChannelIndex0 +0, ChannelIndex1 +1, ChannelBlendFactor +2, Pad +3. The inline default constructor zeroes the first three
// bytes in member order (the CEnginePrimitiveLine constructor 0x004C3090 stores +8 / +9 / +0xA = 0 and leaves Pad; the expanded
// copies in CTCCreatureGenerator::DrawDebugInfo 0x0074BF92.. do the same).
#include "rebuild_abi.h"
class CEnginePrimitiveThemeChannelDesc {
public:
    CEnginePrimitiveThemeChannelDesc() : ChannelIndex0(0), ChannelIndex1(0), ChannelBlendFactor(0) { }
#ifdef FABLE_RETAIL_CENGINEPRIMITIVETHEMECHANNELDESC_INDICES
    // Retail opt-in (chunk 782, row 0x007941D0 CTCUnholyPower ctor): FableWin.pdb ??0CEnginePrimitiveThemeChannelDesc@@QAE@EEE@Z, inline
    // in retail (the SpellExclusionChannel initialiser stores +0xB0 = 5, +0xB1 = 5, +0xB2 = 0xFF in member order and leaves Pad).
    CEnginePrimitiveThemeChannelDesc(unsigned char index0, unsigned char index1, unsigned char blend_factor)
        : ChannelIndex0(index0), ChannelIndex1(index1), ChannelBlendFactor(blend_factor) { }
#endif
    unsigned char ChannelIndex0;                // +0x00
    unsigned char ChannelIndex1;                // +0x01
    unsigned char ChannelBlendFactor;           // +0x02
    unsigned char Pad;                          // +0x03
};
FABLE_STATIC_ASSERT(sizeof(CEnginePrimitiveThemeChannelDesc) == 4);
#endif
