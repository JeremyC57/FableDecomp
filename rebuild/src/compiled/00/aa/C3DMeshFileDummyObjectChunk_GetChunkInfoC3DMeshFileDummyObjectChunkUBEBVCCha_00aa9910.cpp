// C3DMeshFileDummyObjectChunk::GetChunkInfo() const -- append six-decimal Direction XYZ to the inherited helper-point description
// Retail @ 0x00AA9910, 263 bytes. The later donor header puts chunk-derived fields +8; retail uses a 0x2c-byte C3DMeshFileChunk base, Position +0x2c, SubMeshIndex +0x38, BoneIndex +0x3c, Name +0x40 and Direction +0x44. FormatDouble is 0x009A08B0; inherited helper-point GetChunkInfo is 0x00AA96B0.
#if !defined(_MSC_VER)
#define __fastcall
#endif
struct CCharString {
    void* m_pStringData;
    CCharString();
    CCharString(const char* text, long length); // 0x0099EBF0
    ~CCharString(); // 0x0099EAE0
    static CCharString __fastcall FormatDouble(long digits, double value); // 0x009A08B0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right); // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right); // 0x0099F600
// Local virtual shell matches shared CChunkedFileChunk storage while declaring its virtual slots;
// donor 16-byte superchunk vectors conflict with the 12-byte retail vectors used here.
struct CDataInputStream; struct CDataOutputStream; struct CChunkedFileLoader; struct CChunkedFileSaver;
struct CChunkedFileChunk {
    virtual void v00(); virtual void v01(); virtual CCharString GetChunkInfo() const;
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;
    virtual CChunkedFileChunk* CreateNewInstance() const;
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk*);
    char ChunkType[5]; unsigned char _pad[3]; void* Parent;
};
struct CChunkedFileSuperChunk : CChunkedFileChunk {
    struct CArrayPair { void* begin; void* end; void* capacity; };
    CArrayPair SubChunks; CArrayPair RegisteredSubChunkTypes; unsigned long CurrentChunkIndex;
};
struct C3DMeshFileChunk : CChunkedFileSuperChunk {};
struct C3DVector { float X, Y, Z; };
struct CMatrix3x4 { float row[3][4]; };
struct C3DMeshFileHelperPointChunk : C3DMeshFileChunk {
    C3DVector Position; long SubMeshIndex; long BoneIndex; CCharString Name;
    CCharString GetChunkInfo() const; // 0x00AA96B0
};
struct C3DMeshFileDummyObjectChunk : C3DMeshFileHelperPointChunk {
    C3DVector Direction; CMatrix3x4 Transform; bool UseLocalOrigin;
    CCharString GetChunkInfo() const;
};
CCharString C3DMeshFileDummyObjectChunk::GetChunkInfo() const
{
    return C3DMeshFileHelperPointChunk::GetChunkInfo() + ", Direction: (" +
        CCharString::FormatDouble(6, Direction.X) + ", " +
        CCharString::FormatDouble(6, Direction.Y) + ", " +
        CCharString::FormatDouble(6, Direction.Z);
}