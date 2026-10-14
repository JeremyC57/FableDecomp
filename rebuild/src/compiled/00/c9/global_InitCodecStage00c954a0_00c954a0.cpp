// InitCodecStage_00c954a0(CCodecStage_00c954a0* stage, SCodecSource_00c954a0* source) -- libpk stage set-up: clear the 0x2c-byte stage, bind it to its source (copying the source's first two words into +4/+8 and +0), then build the two derived tables from the source's code buffer and from the source itself; returns 0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C954A0, length 74 bytes (the oracle row carries 6 bytes of int3 padding).
// Callees: BuildCodeTable @ 0x00C94FD0 (`__fastcall`: ecx = source->m_pCodes, edx = source->m_count, stack 0; manifest "Bitstream_ProcessCodes"),
// BuildSynthesisTable @ 0x00C951B0 (`__fastcall`: ecx = source, edx = source->m_count, stack 0; manifest "Waveform_Synthesize"). Shape: the libpk
// module is compiled `__fastcall` (/Gr): the stage arrives in ecx (ebx), the source in edx (esi); `rep stosd` is the 11-word memset; the
// source pointer is stored first, then the three copied words in source order (+4, +8, +0), then the two table results (+0x14, then +0x10).
// The manifest label "CEntry::CEntry" is propagated (no `this` return); names are descriptive.
#include <string.h>

struct SCodecSource_00c954a0 {
    long  m_mode;                                  // +0x00
    long  m_count;                                 // +0x04
    void* m_pCodes;                                // +0x08
};

void* __fastcall BuildCodeTable_00c94fd0(void* codes, long count, long flags);                       // 0x00C94FD0
void* __fastcall BuildSynthesisTable_00c951b0(SCodecSource_00c954a0* source, long count, long flags); // 0x00C951B0

struct CCodecStage_00c954a0 {                      // 0x2c bytes
    long                   m_mode;                 // +0x00
    long                   m_count;                // +0x04
    long                   m_remaining;            // +0x08
    SCodecSource_00c954a0* m_pSource;              // +0x0c
    void*                  m_pSynthesis;           // +0x10
    void*                  m_pCodeTable;           // +0x14
    long                   m_18, m_1c, m_20, m_24, m_28;
};

long __fastcall InitCodecStage_00c954a0(CCodecStage_00c954a0* stage, SCodecSource_00c954a0* source)
{
    memset(stage, 0, sizeof(*stage));
    stage->m_pSource = source;
    stage->m_count = source->m_count;
    stage->m_remaining = source->m_count;
    stage->m_mode = source->m_mode;
    stage->m_pCodeTable = BuildCodeTable_00c94fd0(source->m_pCodes, source->m_count, 0);
    stage->m_pSynthesis = BuildSynthesisTable_00c951b0(source, source->m_count, 0);
    return 0;
}