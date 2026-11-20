// CParticleProgram::OptimiseConstraints() -- merge runs of same-typed constraint records in the program's constraint stream: each record is (type, count, data size, data); consecutive records of one type other than 0 and 1 are combined into a single record whose count and size are the sums and whose data is the concatenation; the merged stream then replaces the original
// Leaf: OptimiseConstraints
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ADFDC0, 647 bytes (+ int3 padding), plain `ret`.
// Callees: CMemoryDataInputStream(const CMemoryDataOutputStream&) 0x00995EE0 / ~ 0x00993B10 (label "~CChunkedFileChunk"),
//   CMemoryDataOutputStream() 0x00996AF0 / ~ 0x00996B30 (label "~CCompressedMemoryDataInputStream") / Reset 0x00996B60 /
//   Write 0x00996CC0 (the slot-4 virtual, called directly on the locals and through the vtable on the member) / GetSize
//   0x00994B40 / PeekData 0x00995E70, CDataOutputStream::WriteULONG 0x00993EA0, CDataInputStream::ReadWithSrcChunkOverflow
//   0x00993CA0 (slow path of the inline Read), CDataInputStream::ReadToStream(CDataOutputStream&, long) 0x009964E0.
// Layout: Constraints (CMemoryDataOutputStream, 0x18 bytes) at +0x114 as in the landed CParticleProgram::WriteToFile
//   0x00AE2230 model; CDataInputStream is the landed 0x00AF4660 model (inline Read with the 64-bit position range check,
//   StreamPos +0x04, StreamSize +0x08, CurrentSourcePtr +0x0c, SourceChunkBytesRemaining +0x14).
// NF188 (2026-11-20, q4251): the row-local CDataInputStream / CMemoryDataInputStream models (a user-declared destructor that
//   bound a CMemoryDataInputStream dtor spelling at 0x00993B10) are replaced by the shared engine/CDataInputStream.h /
//   engine/CMemoryDataInputStream.h classes, whose compiler-generated destructor calls ~CDataStream 0x00993B10 directly
//   (??1CDataStream@@UAE@XZ, Ego_r.pdb 0x00AFBBB0 + FableWin.pdb 0x03080440; engine/CDataInputStream.h retry11a note).
#include "fable_compiler.h"
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
#pragma intrinsic(memcpy)
class CDataOutputStream {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void Write(const void* data, long length);                 // slot 4 (+0x10)
    void WriteULONG(unsigned long value);                              // 0x00993EA0
};
class CMemoryDataOutputStream : public CDataOutputStream {
public:
    unsigned long GetLength() const;   // 0x00994B40 (Ego_r.pdb ?GetLength@CMemoryDataOutputStream@@QBEKXZ)
private:
public:
    unsigned char _body04[0x14];
    CMemoryDataOutputStream();                                         // 0x00996AF0
    virtual ~CMemoryDataOutputStream();                                        // 0x00996B30
    virtual void Write(const void* data, long length);                 // 0x00996CC0
    void Reset();                                                      // 0x00996B60
    void const* PeekData() const;                                                  // 0x00995E70
};
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#define FABLE_RETAIL_CDATAINPUTSTREAM_TEXT
#define FABLE_RETAIL_CDATAINPUTSTREAM_COPYTO
#define FABLE_RETAIL_WAD_ADD_FILE_PASS24
#include "engine/CMemoryDataInputStream.h"

class CParticleProgram {
public:
    unsigned char           _body00[0x114];
    CMemoryDataOutputStream Constraints;                               // +0x114
    void OptimiseConstraints();
};

void CParticleProgram::OptimiseConstraints()
{
    CMemoryDataOutputStream& constraints = Constraints;
    CMemoryDataInputStream in(constraints);
    CMemoryDataOutputStream merged;
    long lastType = -1;
    long totalCount = 0;
    long totalSize = 0;
    CMemoryDataOutputStream data;
    while (!in.IsEOF()) {
        long type = in.ReadULONG();
        long count = in.ReadULONG();
        long size = in.ReadULONG();
        if (type == lastType && type != 0 && type != 1) {
            totalCount += count;
            totalSize += size;
        } else {
            if (totalCount != 0) {
                merged.WriteULONG(lastType);
                merged.WriteULONG(totalCount);
                merged.WriteULONG(totalSize);
                merged.Write(((void*)data.PeekData()), ((CMemoryDataOutputStream*)&data)->GetLength());
            }
            lastType = type;
            totalCount = count;
            totalSize = size;
            data.Reset();
        }
        in.CopyTo(data, size);
    }
    if (totalCount != 0) {
        merged.WriteULONG(lastType);
        merged.WriteULONG(totalCount);
        merged.WriteULONG(totalSize);
        merged.Write(((void*)data.PeekData()), ((CMemoryDataOutputStream*)&data)->GetLength());
    }
    constraints.Reset();
    constraints.Write(((void*)merged.PeekData()), ((CMemoryDataOutputStream*)&merged)->GetLength());
}