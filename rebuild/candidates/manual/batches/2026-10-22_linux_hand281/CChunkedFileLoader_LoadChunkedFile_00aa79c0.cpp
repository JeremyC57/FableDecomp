// CChunkedFileLoader::LoadChunkedFile(CChunkedFile& chunk, CDataInputStream& stream) -- read a chunked file's header: the ">>>>"
//   file id and the loader's supported file type (CChunkedFileException on either mismatch), the version and the file comment,
//   then the root chunk's type (exception unless it matches the root chunk) and size, and hand the stream to the root chunk
// Leaf: ?LoadChunkedFile@CChunkedFileLoader@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA79C0, 683 bytes, ret 8; PDB ?LoadChunkedFile@CChunkedFileLoader@@IAEXAAVCChunkedFile@@
//   AAVCDataInputStream@@@Z. No EH frame (throws without /EHsc): CChunkedFileException is an empty class (throw info 0x01369548,
//   type descriptor .?AVCChunkedFileException@@), thrown through _CxxThrowException 0x00BFEB84 from the dead stream-arg slot.
// Callees: CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0 (inline Read, as the landed 0x00B16F60), CDataInputStream
//   vslot 6 (+0x18, returns CCharString), CCharString::operator= 0x0099EFB0 / dtor 0x0099EAE0, operator delete 0x00BFE9BC,
//   CCountedPointer<CChunkedFileSuperChunk> dtor 0x00A9A350, CChunkedFileSuperChunk::ReadFromFile vslot 7 (+0x1c, 0x00AA8830).
// Layout: CChunkedFile / CChunkedFileLoader as Ego_r.pdb (struct_layouts_egor.tsv: CChunkedFile 0x14 = RootChunk +4, Version
//   +0xc, FileComment +0x10; CChunkedFileLoader 0x10 = ReadUnknownChunkTypes +4, SupportedFileType char[5] +8); no engine header
//   exists for either. CDataInputStream / CChunkedFileSuperChunk members restated from engine/CDataInputStream.h and
//   engine/CChunkedFileChunk.h (ChunkType +4) because the inline Read and the virtual slots need declarations.
// Shape: the id compares are 4-byte intrinsic memcmp; version and root size read through an inline ReadULong so they land in
//   the dead stream-arg slot; the root-type compare uses a temporary counted-pointer copy with an inline release, the root
//   call a named copy (out-of-line dtor) whose chunk pointer is taken before the reference bump (no reload after `inc`).
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
extern "C" int __cdecl memcmp(const void*, const void*, unsigned int);
#pragma intrinsic(memcpy, memcmp)
void operator delete(void* p);                         // 0x00BFE9BC
struct CCharString {
    void* PStringData;
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    ~CCharString();                                    // 0x0099EAE0
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
struct CChunkedFileLoader;
struct CDataInputStream {
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5();
    virtual CCharString ReadString();                   // slot 6 (+0x18)
    void ReadWithSrcChunkOverflow(unsigned char* buffer, long size);   // 0x00993CA0
    static __forceinline bool IsValidPosition(__int64 p) { return p >= 0 && p <= 0x7fffffff; }
    __forceinline void Read(void* dst, long size) {
        __int64 np = (__int64)(StreamPos + size);
        if (!IsValidPosition(np)) return;
        if (SourceChunkBytesRemaining >= size) {
            memcpy(dst, CurrentSourcePtr, size); CurrentSourcePtr += size; SourceChunkBytesRemaining -= size; StreamPos += size;
        } else ReadWithSrcChunkOverflow((unsigned char*)dst, size);
    }
    __forceinline unsigned long ReadULong() { unsigned long value; Read(&value, 4); return value; }
};
struct CChunkedFileSuperChunk {
    char ChunkType[5];                                  // +0x04
    virtual void v0(); virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);   // slot 7 (+0x1c)
};
struct CRootPointer {
    CChunkedFileSuperChunk* Data;
    CCPPointerInfo*         Info;
    ~CRootPointer();                                   // 0x00A9A350
    CChunkedFileSuperChunk* operator->() const { return Data; }
};
struct CRootView {
    CChunkedFileSuperChunk* Data;
    CCPPointerInfo*         Info;
    __forceinline CRootView(const CRootPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CRootView() { if (Info) { if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } } }
    CChunkedFileSuperChunk* operator->() const { return Data; }
};
struct CChunkedFile {
    void*        _vf;                                   // +0x00
    CRootPointer RootChunk;                             // +0x04
    unsigned long Version;                              // +0x0c
    CCharString  FileComment;                           // +0x10
};
struct CChunkedFileException { CChunkedFileException() {} };
extern const char CHUNKED_FILE_ID[];                   // 0x0129E454 ">>>>"
static __forceinline bool SameID(const void* a, const void* b) { return memcmp(a, b, 4) == 0; }
struct CChunkedFileLoader {
    virtual void v0();
    long ReadUnknownChunkTypes;                         // +0x04
    char SupportedFileType[5];                          // +0x08
    void LoadChunkedFile(CChunkedFile& chunk, CDataInputStream& stream);
};

void CChunkedFileLoader::LoadChunkedFile(CChunkedFile& chunk, CDataInputStream& stream)
{
    char id[4] = { 0, 0, 0, 0 };
    stream.Read(id, 4);
    if (!SameID(id, CHUNKED_FILE_ID))
        throw CChunkedFileException();
    unsigned long fileType = 0;
    stream.Read(&fileType, 4);
    if (!SameID(&fileType, SupportedFileType))
        throw CChunkedFileException();
    chunk.Version = stream.ReadULong();
    CCharString comment = stream.ReadString();
    chunk.FileComment = comment;
    unsigned long rootType;
    stream.Read(&rootType, 4);
    if (!SameID(&rootType, CRootView(chunk.RootChunk)->ChunkType))
        throw CChunkedFileException();
    unsigned long rootSize = stream.ReadULong();
    CRootPointer root = chunk.RootChunk;
    CChunkedFileSuperChunk* rootChunk = root.Data;
    if (root.Info) ++root.Info->RefCount;
    rootChunk->ReadFromFile(stream, rootSize, *this);
}
