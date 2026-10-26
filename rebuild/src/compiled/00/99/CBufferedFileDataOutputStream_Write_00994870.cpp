// CBufferedFileDataOutputStream::Write(const void*, long) -- buffered write: oversize blocks go straight to the file, otherwise the bytes land in the write-back buffer (flushing first when the position falls outside it)
// Leaf: Write
// Retail: Fable.exe (TLC, VC7.1) @ 0x00994870, 192 bytes (`ret 8`).
// Callees: FlushBuffer @ 0x00994520 (thiscall, this already in ecx); CAFile virtuals Write (slot 4, `data, size, 0`)
//          and Seek (slot 5).
// Layout: engine/CBufferedFileDataOutputStream.h (Ego_r.pdb, 0x24): File +8, Buffer +0xc, BufferSize +0x10,
//          BufferDataSize +0x14, DataStartFilePtr +0x18, FileSize +0x1c, FilePos +0x20.
// Shape: BufferSize is read once (esi). The direct path seeks to FilePos, writes, advances FilePos, grows
//          FileSize and seeks to the file end. The buffered path takes the buffer offset `FilePos - DataStartFilePtr`
//          (recomputed after a flush), copies with the inline memcpy (rep movsd/movsb), then grows BufferDataSize and FileSize.
#include <string.h>
#include "engine/CBufferedFileDataOutputStream.h"

class CAFile { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void Write(const void* data, long size, long flags);   // slot 4
    virtual void Seek(long position);                              // slot 5
};

struct CBufferedFileDataOutputStream_Methods : CBufferedFileDataOutputStream {
    void FlushBuffer();                                             // 0x00994520
    void Write(const void* data, long size);
};

void CBufferedFileDataOutputStream_Methods::Write(const void* data, long size)
{
    long bufferSize = BufferSize;
    if (size > bufferSize) {
        FlushBuffer();
        File->Seek(FilePos);
        File->Write(data, size, 0);
        FilePos += size;
        if (FileSize < FilePos)
            FileSize = FilePos;
        File->Seek(FileSize);
        return;
    }
    long offset = FilePos - DataStartFilePtr;
    if (FilePos < DataStartFilePtr || offset + size > bufferSize || offset > BufferDataSize) {
        FlushBuffer();
        offset = FilePos - DataStartFilePtr;
    }
    memcpy((unsigned char*)Buffer + offset, data, size);
    FilePos += size;
    if (BufferDataSize < offset + size)
        BufferDataSize = offset + size;
    if (FileSize < FilePos)
        FileSize = FilePos;
}