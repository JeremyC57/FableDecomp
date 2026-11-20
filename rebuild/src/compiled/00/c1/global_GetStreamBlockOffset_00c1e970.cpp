// GetStreamBlockOffset(CStreamBlockReader* stream, unsigned long position) -- the offset of `position` within its stream block
// Leaf: ?GetStreamBlockOffset@@YIKPAVCStreamBlockReader@@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1E970, `__fastcall` (stream in ecx, position in edx): the reader's slot 2
//   GetBlockSize (`call [eax+8]`), then `div ; imul ; sub` = position - position / block * block.
// Identity (descriptive; libpk has no PDB): libpk/CStreamBlockReader.h declares it under this name (the bank's +4 reader:
//   Read slot 1, GetBlockSize slot 2); its callers are GetStreamBlockTail 0x00C1E990 and the stream position helper
//   0x00C1E9C0, and PlayStreamed uses it for its leading pad. The old "CCombatSequenceBase_IsBoastSequence" was a propagated
//   combat label.
#include "libpk/CStreamBlockReader.h"

unsigned long __fastcall GetStreamBlockOffset(CStreamBlockReader* stream, unsigned long position)
{
    unsigned long block = stream->GetBlockSize();
    return position - (position / block) * block;
}
