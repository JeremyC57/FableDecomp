#pragma optimize("s",on)
// CDataInputStream::CDataInputStream() -- empty input stream: position, size, source pointer and chunk counters all zero
// Leaf: ??0CDataInputStream@@QAE@XZ
// Retail: Fable.exe @ 0x00993B20, 35 bytes; Ego_r.pdb ??0CDataInputStream@@QAE@XZ. Calls the out-of-line base ctor
// CBaseClassNonCopyable::CBaseClassNonCopyable 0x0099A2F0 (Ego_r.pdb / FableWin.pdb ??0CBaseClassNonCopyable@@IAE@XZ, protected), zeroes
// +0x04..+0x14, then stores the CDataInputStream vptr 0x0129A6B0 (the intermediate CDataStream store is dead and dropped).
// Layout: engine/CDataInputStream.h (FABLE_RETAIL_CDATAINPUTSTREAM: CBaseClassNonCopyable <- CDataStream <- CDataInputStream, Ego_r.pdb
// data members +0x04..+0x14, 24 bytes). Naming NF203 (q11902): ported off the N_0099a2f0::CBaseClass stand-in onto the shared headers.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#include "engine/CDataInputStream.h"

CDataInputStream::CDataInputStream()
    : StreamPos(0), StreamSize(0), CurrentSourcePtr(0), SourceChunkStreamPos(0), SourceChunkBytesRemaining(0)
{
}
