// C3DMeshFile::C3DMeshFile() -- build the file around a new C3DMeshFileRootChunk, stamp the copyright comment and version 100
// Leaf: ??0C3DMeshFile@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A9AD80, length 224 bytes. Vtable 0x0129E0D8 (rtti-vtables.tsv).
// Callees: operator new @ 0x00BFEA1A (the 0x2C-byte root chunk, the 0x0C-byte count block), ??0CChunkedFileSuperChunk@@IAE@PBD@Z @ 0x00AA8360
//   (the root chunk's inline constructor, "3DRT", then ??_7C3DMeshFileRootChunk 0x0129E090), ??0CChunkedFile@@QAE@ABV?$CCountedPointer@VCChunkedFileSuperChunk@@@@@Z
//   @ 0x00AA7DB0, the counted pointer's __fastcall DeleteData @ 0x00A99FD0, operator delete @ 0x00BFE9BC, CCharString(const char*, long)
//   @ 0x0099EBF0, CCharString::operator= @ 0x0099EFB0 (the inline SetFileComment), ~CCharString @ 0x0099EAE0.
// Name: Ego_r.pdb 0x00C0BE80 (lib_3d_mesh_file.obj index 9) and FableWin.pdb ??0C3DMeshFile@@QAE@XZ.
// Option S unit (owner ruling 2026-11-24; NF419 b7, re-owned by NF422): ONE translation unit, retail lib_3d_mesh_file.obj, for
//   0x00A9AD80 (this constructor, the owner: the lowest member address), 0x00A9AE60 (the private root-chunk constructor),
//   0x00A9B640 (C3DMeshFileLoader::LoadMeshFile), 0x00A9B740 (C3DMeshFileLoader::LoadMeshFileCompressed) and 0x00A9D2A0 (the
//   compiler-generated ??_GC3DMeshFile@@UAEPAXI@Z, emitted with the vtable the constructors store). Ego_r.pdb lists all five in that .obj
//   (indexes 9 / 10 / 22 / 23 / 43) and so does FableWin.pdb. The thin rows include this file. Bodies in retail address order.
// Shape: the loaders see the root-chunk constructor's body (retail: same TU), so VC7.1 keeps the root pointer's count block in ebp across
//   the call; LoadMeshFileCompressed sees LoadMeshFile's, so it keeps the result's count block in a register for the copy-out.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHUNKEDFILECHUNK
#define FABLE_RETAIL_CCHUNKEDFILESUPERCHUNK
#define FABLE_RETAIL_CCHUNKEDFILE_SETTERS
#define FABLE_RETAIL_CCHUNKEDFILELOADER_LOADCHUNKEDFILE
#define FABLE_RETAIL_C3DMESHFILE_ROOT_CTOR
#define FABLE_RETAIL_C3DMESHFILE_LOADER_FRIEND
#define FABLE_RETAIL_C3DMESHFILELOADER_LOAD
#define FABLE_RETAIL_CDATAINPUTSTREAM
#define FABLE_RETAIL_CMEMORYDATAINPUTSTREAM
#include <string.h>
#include "engine/C3DMeshFile.h"
#include "engine/C3DMeshFileRootChunk.h"
#include "engine/C3DMeshFileLoader.h"
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

// 0x00A9AD80
C3DMeshFile::C3DMeshFile() : CChunkedFile(CCountedPointer<CChunkedFileSuperChunk>(new C3DMeshFileRootChunk))
{
    {
        CCharString comment("Copyright Big Blue Box Studios Ltd.");
        SetFileComment(comment);
    }
    SetVersion(100);
}

// 0x00A9AE60
C3DMeshFile::C3DMeshFile(const CCountedPointer<C3DMeshFileRootChunk>& root)
    : CChunkedFile(root)
{
}

// 0x00A9B640
CCountedPointer<C3DMeshFile> C3DMeshFileLoader::LoadMeshFile(CDataInputStream& in)
{
    CCountedPointer<C3DMeshFileRootChunk> root((C3DMeshFileRootChunk*)RootChunkType->CreateNewInstance());
    CCountedPointer<C3DMeshFile> file(new C3DMeshFile(root));
    unsigned long position = in.GetPosition();
    in.SetPosition(position);
    LoadChunkedFile(*file, in);
    return file;
}

// 0x00A9B740
CCountedPointer<C3DMeshFile> C3DMeshFileLoader::LoadMeshFileCompressed(const void* data, unsigned long size, CHandleBasedMemoryPool& pool)
{
    if (memcmp(data, ">>>>", 4) == 0) {
        CMemoryDataInputStream stream(data, size);
        return LoadMeshFile(stream);
    }
    unsigned long decompressedSize = *(const unsigned long*)data;
    CTemporaryBuffer buffer(decompressedSize + 3, &pool, &g_ScratchBuffer);
    unsigned char* dst = (unsigned char*)buffer.Data;
    unsigned long outLength = decompressedSize;
    lzo1x_decompress_asm_fast((const unsigned char*)data + 4, size - 4, dst, &outLength, 0);
    CMemoryDataInputStream stream(dst, decompressedSize);
    CCountedPointer<C3DMeshFile> file = LoadMeshFile(stream);
    return file;
}
