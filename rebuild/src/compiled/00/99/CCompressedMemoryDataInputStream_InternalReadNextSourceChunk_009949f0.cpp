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
#define FABLE_RETAIL_CACOMPRESSOR
#define FABLE_RETAIL_CCOMPRESSORZLIB
#define FABLE_RETAIL_CCOMPRESSORZLIB_STREAM
#include "engine/CCompressorZlib.h"
#define FABLE_RETAIL_CAFILE_ERRORCALLBACK   // NF37: CAFile::m_errorCallback 0x013BC9FC (FableWin/Ego_r ?m_errorCallback@CAFile@@0P6IXXZA)
#include "engine/CAFile.h"
#include "engine/CCompressedMemoryDataInputStream.h"


template<class T> struct CScopedPointer {                       // scoped_ptr<T>: one owning pointer
    T* ptr;
    T* operator->() const { return ptr; }
};

#define FABLE_ASSERT(cond) do { if (!(cond)) { CAFile::DoErrorCallback(); } } while (0)

struct CCompressedMemoryDataInputStream_Retail {
    virtual void v00();
    unsigned char          _state[0x14];                        // +0x04..+0x17 (CDataStream)
    unsigned char*         Buffer;                              // +0x18
    long                   BufferSize;                          // +0x1c
    CScopedPointer<CCompressorZlib> Compressor;           // +0x20 (scoped_ptr: ptr)
    void InternalReadNextSourceChunk(unsigned char** chunkData, unsigned long* chunkSize);
};

FABLE_STATIC_ASSERT(sizeof(CCompressedMemoryDataInputStream_Retail) == sizeof(CCompressedMemoryDataInputStream));

void CCompressedMemoryDataInputStream_Retail::InternalReadNextSourceChunk(unsigned char** chunkData, unsigned long* chunkSize)
{
    bool endOfStream = false;
    unsigned long bytesProduced = Compressor->DecompressStream(Buffer, BufferSize, endOfStream);
    FABLE_ASSERT(bytesProduced > 0);
    *chunkData = Buffer;
    *chunkSize = bytesProduced;
}