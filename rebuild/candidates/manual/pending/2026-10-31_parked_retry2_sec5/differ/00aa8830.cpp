// Flags: /GS
// Grade: DIFFER(913v889) -- dropped the dead zero-init of the type tag (267 diff lines v 296); left: zero cached in
//   ebp/edi, chunk size not homed in the dead stream slot
extern "C" void* __cdecl memcpy(void*, const void*, unsigned int);
extern "C" void* __cdecl memset(void*, int, unsigned int);
extern "C" void* __cdecl malloc(unsigned int);
extern "C" void __cdecl free(void*);
#pragma intrinsic(memcpy, memset)
inline void* operator new(unsigned int, void* p) { return p; }
struct __false_type { __false_type() {} };
struct CChunkedFileLoader {
    virtual void v0();
    long ReadUnknownChunkTypes;                         // +0x04
    char SupportedFileType[5];                          // +0x08
};
struct CDataInputStream {
    unsigned long StreamPos;                            // +0x04
    long StreamSize;                                    // +0x08
    const unsigned char* CurrentSourcePtr;              // +0x0c
    unsigned long SourceChunkStreamPos;                 // +0x10
    long SourceChunkBytesRemaining;                     // +0x14
    virtual void v0();
    virtual void Seek(unsigned long position);          // slot 1 (+0x04)
    virtual unsigned long GetPosition();                // slot 2 (+0x08)
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
struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                            // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();                   // 0x0099A300
};
struct CChunkedFileSuperChunk;
struct CChunkedFileChunk : CBaseClassNonCopyable {      // vtable 0x0129E474
    char                    ChunkType[5];               // +0x04
    CChunkedFileSuperChunk* Parent;                     // +0x0c
    CChunkedFileChunk(const char* type) : Parent(0) { memset(ChunkType, 0, 5); memcpy(ChunkType, type, 4); }
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);   // slot 7 (+0x1c)
    virtual void v8();
    virtual CChunkedFileChunk* Clone() const;           // slot 9 (+0x24)
};
struct CChunkedFileRawDataChunk : CChunkedFileChunk {   // vtable 0x0129E4B8
    unsigned char* Data;                                // +0x10
    unsigned long  ByteSize;                            // +0x14
    CChunkedFileRawDataChunk(const char* type) : CChunkedFileChunk(type), Data(0), ByteSize(0) {}
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
    CCPPointerInfo(void (__fastcall* deleteFunc)(void*), void* data) : RefCount(1), DeleteFunc(deleteFunc), Data(data) {}
};
struct CChunkPointer {
    CChunkedFileChunk* Data;
    CCPPointerInfo*    Info;
    static void __fastcall DeleteObject(void* data);    // 0x00A99FF0
    __forceinline explicit CChunkPointer(CChunkedFileChunk* data) : Data(data), Info(data ? new CCPPointerInfo(&DeleteObject, data) : 0) {}
    __forceinline CChunkPointer(const CChunkPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    __forceinline ~CChunkPointer()
    {
        if (Info) { if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
void __fastcall _Destroy(CChunkPointer* first, CChunkPointer* last, const __false_type&);   // 0x00AA9140
struct CChunkArray {
    CChunkPointer* _M_start;                            // +0x00
    CChunkPointer* _M_finish;                           // +0x04
    CChunkPointer* _M_end_of_storage;                   // +0x08
    void _M_insert_overflow(CChunkPointer* pos, const CChunkPointer& x, const __false_type&, unsigned int fillLen, bool atEnd);   // 0x00AA94E0
    void push_back(const CChunkPointer& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CChunkPointer(x); ++_M_finish; }
        else _M_insert_overflow(_M_finish, x, __false_type(), 1, true);
    }
    __forceinline void push_back_grow(const CChunkPointer& x)
    {
        if (_M_finish != _M_end_of_storage) { new (_M_finish) CChunkPointer(x); ++_M_finish; return; }
        CChunkPointer* pos = _M_finish;
        unsigned int fillLen = 1;
        unsigned int oldSize = pos - _M_start;
        unsigned int len = oldSize + (oldSize < fillLen ? fillLen : oldSize);
        CChunkPointer* newStart = len != 0 ? (CChunkPointer*)malloc(len * sizeof(CChunkPointer)) : 0;
        CChunkPointer* newFinish = newStart;
        for (CChunkPointer* p = _M_start; p != pos; ++p, ++newFinish)
            new (newFinish) CChunkPointer(*p);
        new (newFinish) CChunkPointer(x);
        ++newFinish;
        _Destroy(_M_start, _M_finish, __false_type());
        if (_M_start) free(_M_start);
        _M_start = newStart;
        _M_finish = newFinish;
        _M_end_of_storage = newStart + len;
    }
};
struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CChunkArray   SubChunks;                            // +0x10
    unsigned char _pad1c[4];
    unsigned char RegisteredSubChunkTypes[0x10];        // +0x20
    unsigned long CurrentChunkIndex;                    // +0x30
    CChunkedFileChunk* FindRegisteredChunk(const char* type);   // 0x00AA7950
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);
};

void CChunkedFileSuperChunk::ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader)
{
    for (unsigned long pos = 0; pos < size; ) {
        char type[5];
        stream.Read(type, 4);
        type[4] = 0;
        unsigned long chunkSize = stream.ReadULong();
        pos += 8;
        unsigned long start = stream.GetPosition();
        CChunkedFileChunk* registered = FindRegisteredChunk(type);
        if (registered) {
            CChunkedFileChunk* chunk = registered->Clone();
            CChunkPointer pointer(chunk);
            chunk->ReadFromFile(stream, chunkSize, loader);
            stream.GetPosition();
            chunk->Parent = this;
            SubChunks.push_back_grow(pointer);
        } else if (loader.ReadUnknownChunkTypes) {
            CChunkedFileRawDataChunk* raw = new CChunkedFileRawDataChunk(type);
            CChunkPointer pointer(raw);
            raw->ReadFromFile(stream, chunkSize, loader);
            raw->Parent = this;
            SubChunks.push_back(pointer);
        }
        stream.Seek(start + chunkSize);
        pos += chunkSize;
    }
}
