// C3DAnimFile::C3DAnimFile() -- build the file around a new C3DAnimFileRootChunk, stamp the copyright comment and version 100
// Leaf: ??0C3DAnimFile@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98E60, length 224 bytes. Vtable 0x0129E034 (rtti-vtables.tsv).
// Callees: operator new @ 0x00BFEA1A (the 0x34-byte root chunk, the 0x0C-byte count block), ??0CChunkedFileSuperChunk@@IAE@PBD@Z @ 0x00AA8360
//   (the root chunk's inline constructor, "ANRT", then ??_7C3DAnimFileRootChunk 0x0129DFF0), ??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z
//   @ 0x00AA7DB0, the counted pointer's __fastcall DeleteData @ 0x00A99FD0, operator delete @ 0x00BFE9BC, CCharString(const char*, long)
//   @ 0x0099EBF0, CCharString::operator= @ 0x0099EFB0 (the inline SetFileComment), ~CCharString @ 0x0099EAE0.
// Name: Ego_r.pdb 0x00B97FC0 (lib_3d_anim_file.obj index 13) and FableWin.pdb 0x03204B00 ??0C3DAnimFile@@QAE@XZ.
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned by NF422): ONE translation unit, retail lib_3d_anim_file.obj, for
//   0x00A98E60 (this constructor, the owner: the lowest member address), 0x00A98F40 (the private root-chunk constructor),
//   0x00A99420 (C3DAnimFileLoader::LoadAnimFile), 0x00A99510 (C3DAnimFileLoader::LoadAnimFileCompressed) and 0x00A9A730 (the
//   compiler-generated ??_GC3DAnimFile@@UAEPAXI@Z, emitted with the vtable the constructors store). Ego_r.pdb lists all five in that .obj
//   (indexes 13 / 14 / 22 / 23 / 36) and so does FableWin.pdb. The thin rows include this file. Bodies in retail address order.
// Shape: the loaders see the root-chunk constructor's body (retail: same TU), so VC7.1 keeps the root pointer's count block in ebp across
//   the call (0x00A994DF `test ebp,ebp`); LoadAnimFileCompressed sees LoadAnimFile's, so it keeps the result's count block in esi.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILE_SETTERS
#define FABLE_RETAIL_CCHUNKEDFILELOADER_LOADCHUNKEDFILE
#define FABLE_RETAIL_C3DANIMFILE_IMPLICIT_DESTRUCTOR
#define FABLE_RETAIL_C3DANIMFILE_LOADER_FRIEND
#define FABLE_RETAIL_C3DANIMFILEROOTCHUNK_INLINE_CTOR
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include <string.h>
#include "engine/C3DAnimFile.h"
#include "engine/C3DAnimFileRootChunk.h"
#include "engine/C3DAnimFileLoader.h"
#include "engine/CMemoryDataInputStream.h"

class CHandleBasedMemoryPool;
class CScratchBuffer;
extern CScratchBuffer g_ScratchBuffer;               // 0x013CB498

struct CTemporaryBuffer {                            // 0x18 bytes (Ego_r.pdb)
    CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch);   // 0x00A98860
    virtual ~CTemporaryBuffer();                     // 0x00A98910, slot 0 (NF374: Ego_r + FableWin ??1CTemporaryBuffer@@UAE@XZ)
    void*                   Data;                    // +0x04
    CHandleBasedMemoryPool* MemoryPool;              // +0x08
    long                    MemoryHandle;            // +0x0c
    CScratchBuffer*         ScratchBuffer;           // +0x10
    long                    AllocateType;            // +0x14
};

extern "C" int lzo1x_decompress_asm_fast(const unsigned char* src, unsigned long src_len,
                                     unsigned char* dst, unsigned long* dst_len, void* wrkmem);   // 0x00C069D0

// 0x00A98E60
C3DAnimFile::C3DAnimFile() : CChunkedFile(CCountedPointer<CChunkedFileSuperChunk>(new C3DAnimFileRootChunk))
{
    {
        CCharString comment("Copyright Big Blue Box Studios Ltd.");
        SetFileComment(comment);
    }
    SetVersion(100);
}

// 0x00A98F40
C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root)
    : CChunkedFile(root)
{
}

// 0x00A99420
CCountedPointer<C3DAnimFile> C3DAnimFileLoader::LoadAnimFile(CDataInputStream& in)
{
    CCountedPointer<C3DAnimFileRootChunk> root((C3DAnimFileRootChunk*)RootChunkType->CreateNewInstance());
    CCountedPointer<C3DAnimFile> file(new C3DAnimFile(root));
    LoadChunkedFile(*file, in);
    return file;
}

// 0x00A99510
CCountedPointer<C3DAnimFile> C3DAnimFileLoader::LoadAnimFileCompressed(const void* data, unsigned long size, CHandleBasedMemoryPool& pool)
{
    if (memcmp(data, ">>>>", 4) == 0) {
        CMemoryDataInputStream stream(data, size);
        return LoadAnimFile(stream);
    }
    unsigned long decompressedSize = *(const unsigned long*)data;
    CTemporaryBuffer buffer(decompressedSize + 3, &pool, &g_ScratchBuffer);
    unsigned char* dst = (unsigned char*)buffer.Data;
    unsigned long outLength = decompressedSize;
    lzo1x_decompress_asm_fast((const unsigned char*)data + 4, size - 4, dst, &outLength, 0);
    CMemoryDataInputStream stream(dst, decompressedSize);
    CCountedPointer<C3DAnimFile> file = LoadAnimFile(stream);
    return file;
}
