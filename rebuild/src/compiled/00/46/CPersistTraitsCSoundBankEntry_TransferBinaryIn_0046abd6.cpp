// CPersistTraits<CSoundBankEntry>::TransferBinaryIn(CDataInputStream*, CSoundBankEntry*) -- read the ID, the three def strings and the DVD flag
// Retail: Fable.exe (TLC, VC7.1) @ 0x0046ABD6, 210 bytes, `ret 8` (__fastcall, the empty traits `this` is unused).
// Callees: CDataInputStream::ReadWithSrcChunkOverflow @ 0x00993CA0 (the inline Read's slow path, x2),
//   CDefString::TransferBinaryIn(CDataInputStream*) @ 0x0044FC00 (manifest "CPersistTraits<float>"; landed callers name
//   it CDefString::TransferBinaryIn) on Filename, SymbolFilename and LipSyncBankHandle.
// Layout: engine/CDataInputStream.h; PDB CSoundBankEntry (0x14 bytes): ID +0, Filename +4, SymbolFilename +8,
//   LipSyncBankHandle +0xC (CDefString), DVD +0x10. No engine header for the entry; local model.
// Shape: inline 4-byte read of the ID (into the dead stream-argument slot) and inline 1-byte read of the flag (its top
//   byte, [ebp+0Bh]) through a non-bool temporary, stored as `!= 0` (setne). Size-optimised TU:
//   #pragma optimize("s", on) (ebp frame).
#pragma optimize("s", on)
#define FABLE_CDATAINPUTSTREAM_FRIEND struct CDataInputStreamReader   // reaches the private CDataInputStream::ReadWithSrcChunkOverflow (0x00993CA0)
#include "engine/CDataInputStream.h"
extern "C" void* __cdecl memcpy(void* dest, const void* src, unsigned int bytes);
#pragma intrinsic(memcpy)

struct CDataInputStreamReader : CDataInputStream {
    static __forceinline bool IsValidPosition(__int64 position) { return position >= 0 && position <= 0x7fffffff; }
    __forceinline void Read(void* buffer, long size)
    {
        __int64 newPosition = (__int64)(StreamPos + size);
        if (!IsValidPosition(newPosition))
            return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(buffer, CurrentSourcePtr, size);
            CurrentSourcePtr = (unsigned char*)CurrentSourcePtr + size;
            SourceChunkBytesRemaining -= size;
            StreamPos += size;
        }
        else
            ReadWithSrcChunkOverflow((unsigned char*)buffer, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, sizeof(value)); return value; }
    __forceinline unsigned char ReadByte() { unsigned char value; Read(&value, sizeof(value)); return value; }
};

class CDefString { public:
    long Index;
    void TransferBinaryIn(CDataInputStream* stream);    // 0x0044FC00
};

class CSoundBankEntry { public:
    unsigned long ID;                   // +0x00
    CDefString    Filename;             // +0x04
    CDefString    SymbolFilename;       // +0x08
    CDefString    LipSyncBankHandle;    // +0x0C
    bool          DVD;                  // +0x10
};

template<class T> struct CPersistTraits;
template<> struct CPersistTraits<CSoundBankEntry> {
    void TransferBinaryIn(CDataInputStream* stream, CSoundBankEntry* value);
};

void CPersistTraits<CSoundBankEntry>::TransferBinaryIn(CDataInputStream* stream, CSoundBankEntry* value)
{
    CDataInputStreamReader* in = (CDataInputStreamReader*)stream;
    value->ID = in->ReadULong();
    value->Filename.TransferBinaryIn(stream);
    value->SymbolFilename.TransferBinaryIn(stream);
    value->LipSyncBankHandle.TransferBinaryIn(stream);
    value->DVD = in->ReadByte() != 0;
}