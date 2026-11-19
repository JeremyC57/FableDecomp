#pragma optimize("s",on)
// COpinionTransientOffset::TransferBinaryIn(CMemoryDataInputStream& stream) @ 0x004793F8 (81 bytes): read the 0x18-byte record in one
// inlined stream read (64-bit position guard, chunk copy or the 0x00993CA0 overflow read).
// Leaf: ?TransferBinaryIn@COpinionTransientOffset@@QAEXAAVCMemoryDataInputStream@@@Z
// NF104 (q6539, 2026-11-19): respelled from TransferBinaryIn(CDataInputStream*) (local stream model) onto the PDB spelling:
//   Ego_r.pdb / FableWin.pdb name sets hold only ?TransferBinaryIn@COpinionTransientOffset@@QAEXAAVCMemoryDataInputStream@@@Z (17 hits in
//   ghidra_out; the stream is a reference, same bytes as a pointer); the landed binders Transfer<COpinionTransientOffset> 0x00685F80 and
//   its ILT thunk 0x00479369 bind it. The stream is the shared engine/CMemoryDataInputStream.h (its inline CDataInputStream::Read).
//   Layout: Ego_r.pdb COpinionTransientOffset is 0x18 bytes (Opinion, OffsetPerFrameRunIn, FramesToRunIn, FramesOfCappedPeak,
//   OffsetPerFrameRunOut, FramesToRunOut); no engine header, kept as the row's pre-existing byte model.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include <string.h>
#include "engine/CMemoryDataInputStream.h"

class COpinionTransientOffset
{ public:
    unsigned char Bytes[0x18];             // the 6-dword record this reader fills

    void TransferBinaryIn(CMemoryDataInputStream& stream);
};

void COpinionTransientOffset::TransferBinaryIn(CMemoryDataInputStream& stream)
{
    stream.Read(this, sizeof(*this));
}
