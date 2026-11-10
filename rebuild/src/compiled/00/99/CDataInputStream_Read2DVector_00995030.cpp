// CDataInputStream::ReadC2DVector() -- read X then Y and return the 2D vector by value
// Retail: Fable.exe (TLC, VC7.1) @ 0x00995030, 188 bytes (+ int3 padding), `ret 4` (__fastcall on the stream, hidden result pointer).
// Label: the manifest's "CPersistTraits<CFloatRange>::TransferBinaryIn" is propagated; the landed callers
//   CPersistContext::Transfer<C2DVector> 0x00800B90 and CActionInputControl::TransferBinaryIn 0x00410120 call it as
//   CMemoryDataInputStream::ReadC2DVector (by-value C2DVector through the hidden pointer, `this` = the stream).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (x2, the slow path of the inlined Read).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: two inlined Read calls into two float frame locals, then the result is built from them (X via fld/fstp, Y
//   through a GPR) and its address returned. Speed-optimised (default) TU, the same as the other 0x0099xxxx readers.
// Leaf: ?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ
// Name fix (agentNF5 2026-11-10): Ego_r.pdb / FableWin.pdb ?Read2DVector@CDataInputStream@@QAE?AVC2DVector@@XZ (the landed caller
//   GFSerialiseVectorBinaryIn<C2DVector> 0x00475DFA binds it at 0x00995030), was the derived stand-in
//   CDataInputStreamReader::ReadC2DVector; now the shared retail CDataInputStream (FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR).
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_READ2DVECTOR
#include "engine/CDataInputStream.h"
#include "engine/C2DVector.h"
C2DVector CDataInputStream::Read2DVector()
{
    float x = ReadFloat();
    float y = ReadFloat();
    return C2DVector(x, y);
}
