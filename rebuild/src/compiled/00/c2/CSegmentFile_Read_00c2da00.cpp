// CSegmentFile::Read(void* buffer, int size) -- read from the current segment: forwards to the segment file's buffered range reader
// (first member, +0x00), CBufferedRangeReader::Read 0x00C3A9E0; VC7.1 tail-calls it (`jmp rel32`).
// Leaf: ?Read@CSegmentFile@@QAEHPAXH@Z
// Naming (NF173, 2026-11-20): q5110: re-authored from the extern "C" `Forward` stub onto the shared libpk/CSegmentFile.h
//   (FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23 declares Read). Descriptive libpk names (owner 2026-11-12: library code gets light naming;
//   the header's NAMING_EVIDENCE): every landed caller (0x00C16E10, 0x00C16F00, 0x00C3BB50, 0x00C3BCB0) binds this spelling, and the
//   forward target binds as ?Read@CBufferedRangeReader@@QAEJPAXJ@Z (reloc_bindings, 0x00C2DA20). The catalog label
//   NScript::CV_MurderTwistScript::Read is a propagated label.
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C2DA00: jmp 0x00C3A9E0.
#define FABLE_RETAIL_BANK_LOAD_WRAPPER_PASS23
#include "libpk/CBankProcessor.h"
#include "libpk/CSegmentFile.h"

int CSegmentFile::Read(void* buffer, int size)
{
    return Reader.Read(buffer, size);
}
