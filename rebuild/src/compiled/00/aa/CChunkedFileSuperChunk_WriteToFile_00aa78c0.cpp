// CChunkedFileSuperChunk::WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const -- writes every sub-chunk as [fourcc][u32 size][body], back-patching each size after the body is written
// Leaf: WriteToFile
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AA78C0, 131 bytes of code (virtual slot 8, `ret 8`).
// Callees: the stream's virtuals Write (slot 4), GetPosition (slot 2), SetPosition (slot 1); CDataOutputStream::WriteLong
//          @ 0x00993EA0; each sub-chunk's virtual WriteToFile (slot 8).
// Shape: a size placeholder (0) follows the four-character code; the body start and end positions are kept in
//          ebx/ebp, the size is patched at start-4 and the stream returns to the end. `this` lives in the `push ecx`
//          frame word and the array end is re-read through it every iteration (the virtual calls may change it).
// Retail layout: SubChunks at +0x10 as a 12-byte STLport vector (the PDB header's 16-byte CArray does not fit: the
//          second array starts at +0x1c in retail), so `this` is modelled locally.
struct CDataOutputStream {
    virtual void v00();
    virtual void SetPosition(long position);        // slot 1
    virtual long GetPosition();                     // slot 2
    virtual void v03();
    virtual void Write(const void* data, long size);// slot 4
    void WriteLong(long value);                     // 0x00993EA0 (Write(&value, 4); manifest "WriteCBYTE" is propagated)
};
struct CDataInputStream;
struct CChunkedFileSaver;
struct CChunkedFileLoader;
struct CChunkedFileSuperChunk;

struct CChunkedFileChunk {                          // retail layout = engine/CChunkedFileChunk.h (+ the vtable it cannot declare)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual long IsUnknownType() const;             // slot 4 (CChunkedFileRawDataChunk @ 0x00AA7740: `mov eax,1`)
    virtual void v05(); virtual void v06();
    virtual void ReadFromFile(CDataInputStream& stream, unsigned long size, CChunkedFileLoader& loader);   // slot 7
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;                    // slot 8
    char ChunkType[5];                              // +0x04 four-character code + terminator
    CChunkedFileSuperChunk* Parent;                 // +0x0c
};

struct SCountBlock {
    long m_references;                              // +0
    void (__fastcall* m_pfnDestroy)(void*);         // +4
    void* m_pObject;                                // +8
};

struct CChunkPointer {                              // CCountedPointer<CChunkedFileChunk>
    CChunkedFileChunk* Object;                      // +0
    SCountBlock* Count;                             // +4
};

struct CChunkArray {                                // retail CArray<CCountedPointer<CChunkedFileChunk>>: STLport vector, 12 bytes
    CChunkPointer* m_pFirst;
    CChunkPointer* m_pLast;
    CChunkPointer* m_pEnd;
    CChunkPointer* begin() const { return m_pFirst; }
    CChunkPointer* end() const { return m_pLast; }
};

struct CChunkedFileSuperChunk : CChunkedFileChunk {
    CChunkArray SubChunks;                          // +0x10
    CChunkArray RegisteredSubChunkTypes;            // +0x1c
    unsigned long CurrentChunkIndex;                // +0x28
    virtual void WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const;
};

void CChunkedFileSuperChunk::WriteToFile(CDataOutputStream& stream, CChunkedFileSaver& saver) const
{
    for (CChunkPointer* it = SubChunks.begin(); it != SubChunks.end(); ++it) {
        stream.Write(it->Object->ChunkType, 4);
        stream.WriteLong(0);
        long start = stream.GetPosition();
        it->Object->WriteToFile(stream, saver);
        long finish = stream.GetPosition();
        stream.SetPosition(start - 4);
        stream.WriteLong(finish - start);
        stream.SetPosition(finish);
    }
}