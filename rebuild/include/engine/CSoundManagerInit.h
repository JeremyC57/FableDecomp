#ifndef FABLE_ENGINE_CSOUNDMANAGERINIT_H
#define FABLE_ENGINE_CSOUNDMANAGERINIT_H
#include "rebuild_abi.h"
// New header (B24C4, row 0x00402510 GFMain): CSoundManagerInit, the 0x20-byte sound settings block embedded in the native
// CSystemManagerInit at +0x38 (absent from Ego_r.pdb, whose CSystemManagerInit is 0xA0 bytes: the PC sound module is in no PDB).
// The type name is the landed spelling of the leaves ??0CASoundManager@@QAE@ABUCSoundManagerInit@@_N@Z (0x00991910) and
// ??0CSoundManagerLH@@QAE@ABUCSoundManagerInit@@_N@Z (0x00A3EF00); CSystemManager::Initialise 0x009A6610 (landed) passes
// &init->SoundManagerInit to the CSoundManagerLH constructor. Member names are descriptive (no PDB covers them):
//   +0x00 MaxEmitters: copied to CASoundManager +0x18, the limit CASoundManager::CanAllocateEmitter 0x009915C0 compares the
//         emitter count with (`call [eax+28h] ; cmp eax,[esi+18h] ; sbb`); CSystemManagerInit() 0x00403B10 stores 0x80.
//   +0x04 (0x40), +0x18 (0): copied to CASoundManager +0x14 / not read by the two constructors; no reader found (named by offset).
//   +0x08 / +0x0C: floats (0x00403B10 stores 64.0f / 2.0f, GFMain 0x00403010.. stores 32.0f / 5.0f), copied to CASoundManager +4 / +8.
//   +0x10 Scale: float (1.0f); CASoundManager keeps it and its reciprocal (`fdiv [edi+10h]`, 0x0099193D).
//   +0x14 ChannelMask: one bit per output channel CSoundManagerLH enables (0x00A3EF00, its landed spelling); GFMain stores 0x100.
//   +0x1C ResourceMemory: the CResourceList budget (CResourceList::SetMaximumMemory, 0x00991910; its landed spelling); 0x40000 / 0x10000.
struct CSoundManagerInit {
    long          MaxEmitters;                  // +0x00
    long          Setting04;                    // +0x04
    float         Setting08;                    // +0x08
    float         Setting0C;                    // +0x0c
    float         Scale;                        // +0x10
    unsigned long ChannelMask;                  // +0x14
    long          Setting18;                    // +0x18
    unsigned long ResourceMemory;               // +0x1c
};
FABLE_STATIC_ASSERT(sizeof(CSoundManagerInit) == 0x20);
#endif
