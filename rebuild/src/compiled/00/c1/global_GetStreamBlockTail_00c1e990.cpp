// GetStreamBlockTail(CStreamBlockReader* stream, long position, long size) -- the padding that rounds the range
//   [position, position + size) up to the end of its last stream block
// Leaf: ?GetStreamBlockTail@@YIJPAVCStreamBlockReader@@JJ@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C1E990, 44 bytes (`ret 4`). __fastcall: stream in ecx, position in edx, size on
//   the stack.
// Identity (descriptive; libpk has no PDB): libpk/CStreamBlockReader.h declares it under this name and signature; the landed
//   caller 0x00C1E9F0 passes (reader, time, extra) the same way. The old "CombatSequence_AlignDuration" was a propagated label.
// Callees: GetStreamBlockOffset 0x00C1E970 (edx passed straight through), the reader's slot 2 GetBlockSize.
// Shape: used = offset + size lands in esi (named local), the block size in ecx; `lea eax,[ecx+esi-1] ; div ; imul ; sub`
//   is (used + block - 1) / block * block - used on unsigned values.
#include "libpk/CStreamBlockReader.h"

long __fastcall GetStreamBlockTail(CStreamBlockReader* stream, long position, long size)
{
    unsigned long used = GetStreamBlockOffset(stream, position) + size;
    unsigned long block = stream->GetBlockSize();
    return (used + block - 1) / block * block - used;
}
