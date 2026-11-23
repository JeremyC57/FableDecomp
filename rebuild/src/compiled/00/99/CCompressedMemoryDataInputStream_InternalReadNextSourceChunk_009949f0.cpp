// CCompressedMemoryDataInputStream::InternalReadNextSourceChunk(unsigned char**, unsigned long*) -- inflate the next chunk into the stream's buffer and hand back the buffer and the byte count (asserting that something was produced)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009949F0, length 69 bytes (+ int3 padding).
// Callees: CCompressorZlib::DecompressStream(void*, ulong, bool*) @ 0x00A3A840 on the owned compressor
// (Compressor_ptr at +0x20 of engine/CCompressedMemoryDataInputStream.h, a scoped_ptr), and the
// assertion hook: the function pointer at 0x013BC9FC is called when the assertion fails and the hook
// is installed. Buffer +0x18 / BufferSize +0x1c as the header; the end-of-stream flag is a 1-byte
// local homed in the high byte of the frame slot (`lea eax,[esp+0Bh]`) whose clear is scheduled after
// the pushes. The byte count is kept in edi across the hook call; `n > 0` is the unsigned compare
// (`test ; ja`). The manifest's doubled "CCompressedMemoryDataInputStream::CCompressedMemory
// DataInputStream::" prefix is a naming artefact; vtable 0x0129A758 slot 8 is this body.
// NF176 (2026-11-20, q5003): the CCompressorZlib_Calls stand-in is gone: the compressor is the shared engine/CCompressorZlib.h class
//   (FABLE_RETAIL_CACOMPRESSOR / _CCOMPRESSORZLIB / _STREAM), whose Ego_r.pdb / FableWin.pdb DecompressStream takes the end-of-stream flag
//   by reference: 0x00A3A840 binds ?DecompressStream@CCompressorZlib@@QAEKPAXKAA_N@Z (was ?DecompressStream@CCompressorZlib_Calls@@QAEKPAXKPA_N@Z).
// Leaf: ?InternalReadNextSourceChunk@CCompressedMemoryDataInputStream@@EAEXAAPBEAAJ@Z
// Signature port (NF320, 2026-11-23): defined on the real class of engine/CCompressedMemoryDataInputStream.h under
//   FABLE_RETAIL_CCOMPRESSEDMEMORYDATAINPUTSTREAM with the PDB parameter types (was
//   ?InternalReadNextSourceChunk@CCompressedMemoryDataInputStream_Retail@@QAEXPAPAEPAK@Z); the retail RTTI vftable placement
//   (ghidra_out/labels_rtti_port.tsv) names this PDB member here. The local _Retail view and its CScopedPointer stand-in are replaced by
//   the header's members (Buffer +0x18, BufferSize +0x1C, boost::scoped_ptr<CCompressorZlib> Compressor +0x20).
// Preamble: the landed precedent CCompressedMemoryDataInputStream_SetPosition_00994a80.cpp.
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CCOMPRESSEDMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_STREAM
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CAFILE_ERRORCALLBACK   // NF37: CAFile::m_errorCallback 0x013BC9FC (FableWin/Ego_r ?m_errorCallback@CAFile@@0P6IXXZA)
#include "engine/CCompressorZlib.h"
#include "engine/CCompressedMemoryDataInputStream.h"
#include "engine/CAFile.h"
#define FABLE_ASSERT(cond) do { if (!(cond)) { CAFile::DoErrorCallback(); } } while (0)
void CCompressedMemoryDataInputStream::InternalReadNextSourceChunk(const unsigned char*& chunkData, long& chunkSize)
{
    bool endOfStream = false;
    unsigned long bytesProduced = Compressor->DecompressStream(Buffer, BufferSize, endOfStream);
    FABLE_ASSERT(bytesProduced > 0);
    chunkData = Buffer;
    chunkSize = bytesProduced;
}
