// C3DAnimFileLoader::LoadAnimFileCompressed(const void* data, unsigned long size, CHandleBasedMemoryPool& pool) -- load a 3DAF anim file from a (possibly LZO-compressed) memory image
// Leaf: LoadAnimFileCompressed
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A99510, 250 bytes (`ret 10h`: hidden result + 3 args).
// Ego_r.pdb: ?LoadAnimFileCompressed@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VC3DAnimFile@@@@PBXKAAVCHandleBasedMemoryPool@@@Z.
// Callees: CMemoryDataInputStream(void*, long) 0x00994930 / ~CDataStream 0x00993B10 (reached through the implicit CMemoryDataInputStream/CDataInputStream destructors),
// C3DAnimFileLoader::LoadAnimFile(CDataInputStream&) 0x00A99420, CTemporaryBuffer(size, pool, scratch) 0x00A98860 /
// ~CTemporaryBuffer 0x00A98910, lzo1x_decompress_asm_fast 0x00C069D0 (cdecl), operator delete 0x00BFE9BC;
// the shared scratch buffer global 0x013CB498, the 4-byte ">>>>" tag 0x0129E454.
// Shape: a ">>>>" tag (4-byte memcmp intrinsic: `mov eax,offset ; mov edx,[eax] ; cmp`) marks an uncompressed
// image read in place (`return LoadAnimFile(stream)` builds straight into the result). Otherwise the first dword is
// the decompressed size: a temporary buffer (size + 3 for LZO's overrun), decompress (the in/out length local lands
// in the dead `data` argument slot), read the stream, then `return file;` (no NRVO: merged inc/dec copy-out with
// the count block kept in esi), the stream and the buffer destroyed after the counted pointer.
// LoadAnimFile is the previous function of this TU (0x00A99420): its landed body (and the C3DAnimFile ctor it
// sits on, 0x00A98F40) is compiled above under auto_inline(off) so VC7.1 knows the result object does not escape
// and keeps the count block in a register for the copy-out, as retail does.
#include <string.h>

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
    SCountBlock(void (__fastcall* destroy)(void*), void* object) : m_references(1), m_pfnDestroy(destroy), m_pObject(object) {}
};

template<class T> class CCountedPointer { public:
    T* Object;                                      // +0
    SCountBlock* Count;                             // +4
    static void __fastcall DeleteData(void* object);   // 0x00A99FE0 (root chunk) / 0x00A9A000 (file)
    CCountedPointer(T* object) : Object(object), Count(object ? new SCountBlock(DeleteData, object) : 0) {}
    CCountedPointer(const CCountedPointer& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    ~CCountedPointer()
    {
        if (Count) {
            if (--Count->m_references == 0) {
                Count->m_pfnDestroy(Count->m_pObject);
                delete Count;
            }
        }
    }
    template<class U> CCountedPointer(const CCountedPointer<U>& other) : Object(other.Object), Count(other.Count) { if (Count) ++Count->m_references; }
    T& operator*() const { return *Object; }
};

class CDataStream { public:
    virtual ~CDataStream();                          // 0x00993B10 (CDataInputStream / CMemoryDataInputStream: implicit)
};
class CDataInputStream : public CDataStream { public:
};
class CMemoryDataInputStream : public CDataInputStream { public:
    CMemoryDataInputStream(const void* data, long size);   // 0x00994930
    long  position;                                  // +0x04
    long  sourceLength;                              // +0x08
    long  chunkBase;                                 // +0x0c
    long  chunkPosition;                             // +0x10
    long  chunkLength;                               // +0x14
    void* StreamSource;                              // +0x18
};

class CHandleBasedMemoryPool;
class CScratchBuffer;
extern CScratchBuffer g_ScratchBuffer;               // 0x013CB498

struct CTemporaryBuffer {                            // 0x18 bytes (Ego_r.pdb)
    CTemporaryBuffer(unsigned long size, CHandleBasedMemoryPool* pool, CScratchBuffer* scratch);   // 0x00A98860
    ~CTemporaryBuffer();                             // 0x00A98910
    virtual void v00();
    void*                   Data;                    // +0x04
    CHandleBasedMemoryPool* MemoryPool;              // +0x08
    long                    MemoryHandle;            // +0x0c
    CScratchBuffer*         ScratchBuffer;           // +0x10
    long                    AllocateType;            // +0x14
};

extern "C" int lzo1x_decompress_asm_fast(const unsigned char* src, unsigned long src_len,
                                     unsigned char* dst, unsigned long* dst_len, void* wrkmem);   // 0x00C069D0

class CChunkedFileChunk { public:
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4();
    virtual void v5(); virtual void v6(); virtual void v7(); virtual void v8();
    virtual CChunkedFileChunk* CreateNewInstance() const;   // slot 9 (+0x24)
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:};
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { public:};

class CChunkedFile { public:
    CChunkedFile(const CCountedPointer<CChunkedFileSuperChunk>& root);   // 0x00AA7DB0
    virtual ~CChunkedFile();
    unsigned char _body[0x10];                      // RootChunk, Version, FileComment (sizeof 0x14)
};
class C3DAnimFile : public CChunkedFile { public:
    C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root);   // 0x00A98F40
};

class CChunkedFileLoader { public:
    virtual ~CChunkedFileLoader();
    unsigned char _pad04[0x0c];
    CChunkedFileChunk* RootChunkPrototype;          // +0x10
    void LoadChunkedFile(CChunkedFile& file, CDataInputStream& in);   // 0x00AA79C0
};

struct C3DAnimFileLoader : CChunkedFileLoader {
    CCountedPointer<C3DAnimFile> LoadAnimFile(CDataInputStream& in);   // 0x00A99420
    CCountedPointer<C3DAnimFile> LoadAnimFileCompressed(const void* data, unsigned long size, CHandleBasedMemoryPool& pool);
};

#pragma auto_inline(off)
C3DAnimFile::C3DAnimFile(const CCountedPointer<C3DAnimFileRootChunk>& root)
    : CChunkedFile(root)
{
}

CCountedPointer<C3DAnimFile> C3DAnimFileLoader::LoadAnimFile(CDataInputStream& in)
{
    CCountedPointer<C3DAnimFileRootChunk> root((C3DAnimFileRootChunk*)RootChunkPrototype->CreateNewInstance());
    CCountedPointer<C3DAnimFile> file(new C3DAnimFile(root));
    LoadChunkedFile(*file, in);
    return file;
}
#pragma auto_inline(on)

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