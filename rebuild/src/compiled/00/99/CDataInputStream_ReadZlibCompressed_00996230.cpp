// CDataInputStream::ReadZlibCompressed(void* data, long size) -- read a block written by WriteZlibCompressed: the compressed byte count, the compressed bytes into a scratch buffer, then zlib-inflate them into data (the assertion hook fires if fewer than size bytes come out)
// Leaf: ?ReadZlibCompressed@ (Ego_r.pdb ?ReadZlibCompressed@CDataInputStream@@UAEXPAXJ@Z, virtual; modelled through CDataInputStream_Methods)
// Retail: Fable.exe @ 0x00996230, 341 bytes, `ret 8`.
// Body: size <= 0 -> nothing. Read(&compressedSize, 4); CArray<unsigned char> buffer(compressedSize) (vector<unsigned char>(n)
// 0x00514E20); Read(buffer.GetAsCArray(), compressedSize); CCompressorZlib (CACompressor(GetWorkspaceSize()) 0x00A3AEA0 /
// 0x00A3A7D0, vtable 0x01245974, Init 0x00A3AA10); InternalDecompress(buffer.GetAsCArray(), compressedSize, data, size)
// (0x00A3AB00) != size -> g_pfnAssertionFailed (0x013BC9FC) if installed; ~CCompressorZlib 0x00A3ACC0; free 0x00BFEA14.
// The inline stream Read is the landed CMemoryDataInputStream::Read (64-bit position range check, chunk memcpy fast
// path, ReadWithSrcChunkOverflow 0x00993CA0).
// Layout: engine/CDataInputStream.h (StreamPos +4, CurrentSourcePtr +0xC, SourceChunkBytesRemaining +0x14).
// Shape: CByteArray / CCompressorZlib models of the landed CDataOutputStream::WriteZlibCompressed 0x009967E0, the
// FABLE_ASSERT hook of the landed CCompressedMemoryDataInputStream sources. The count comes from an inline ReadLong()
// with its own local (a caller local is homed in the dead `size` slot); Read starts with `if (size <= 0) return;` and
// tests `size <= SourceChunkBytesRemaining` (size on the left for `cmp ebp,[ebx+14h] ; jg`).
// Opt: /O2 (16-aligned entry).
#define FABLE_RETAIL_CAFILE_ERRORCALLBACK   // NF37: CAFile::m_errorCallback 0x013BC9FC (FableWin/Ego_r ?m_errorCallback@CAFile@@0P6IXXZA)
#include "engine/CAFile.h"
#include "fable_compiler.h"
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStream_Methods   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"

extern "C" void __cdecl free(void* block);                   // 0x00BFEA14
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

class CACompressor { public:                                        // landed 0x00A3AEA0 (vtable 0x0129B424)
    virtual ~CACompressor();
    unsigned char _body[0x1c - 4];
    CACompressor(long workspaceSize);                        // 0x00A3AEA0
};
class CCompressorZlib : public CACompressor { public:                      // vtable 0x01245974, retail 0x58 bytes
    unsigned char _zlib[0x58 - 0x1c];
    static long GetWorkspaceSize();                          // 0x00A3A7D0
    private: void Initialise(); public:                                             // 0x00A3AA10; NF299: PDB name, was Init
    unsigned long InternalDecompress(const void* source, unsigned long sourceSize, void* dest, unsigned long destSize);   // 0x00A3AB00
    virtual ~CCompressorZlib();                              // 0x00A3ACC0
    CCompressorZlib() : CACompressor(GetWorkspaceSize()) { Initialise(); }
};
// NF93 (q11297): the local CByteArray model replaced by engine/CArray.h: CArray<unsigned char> (FableWin ??0?$CArray@E@@QAE@I@Z 0x00514E20,
//   out of line; Ego_d types these buffers CArray<unsigned char>), the null-tested release, GetAsCArray (the start, or its own address).
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CARRAY_SIZE_CTOR_OUTOFLINE
#include "engine/CArray.h"

struct CDataInputStream_Methods : CDataInputStream {
    static FABLE_FORCEINLINE bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    FABLE_FORCEINLINE void Read(void* buffer, long size)
    {
        if (size <= 0)
            return;
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (size <= SourceChunkBytesRemaining) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    FABLE_FORCEINLINE long ReadLong() { long value; Read(&value, sizeof(value)); return value; }
    void ReadZlibCompressed(void* data, long size);
};

void CDataInputStream_Methods::ReadZlibCompressed(void* data, long size)
{
    if (size > 0) {
        long compressedSize = ReadLong();
        CArray<unsigned char> buffer(compressedSize);
        Read(buffer.GetAsCArray(), compressedSize);
        CCompressorZlib compressor;
        unsigned long written = compressor.InternalDecompress(buffer.GetAsCArray(), compressedSize, data, size);
        if (written != (unsigned long)size) {
            CAFile::DoErrorCallback();
        }
    }
}