// C3DMeshFileHelperPointChunk::GetChunkInfo() const -- "Name: <n>, Position: (x, y, z), Sub-mesh: <s>, Bone: <b>"
// Leaf: ?GetChunkInfo@C3DMeshFileHelperPointChunk
// Retail @ 0x00AA96B0, 425 bytes (ret 4: hidden CCharString result).
// Callees: CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   operator+(const CCharString&, const CCharString&) 0x0099F570, operator+(const CCharString&, const char*) 0x0099F600,
//   GFIntToCharString(long) 0x0099F830 (__fastcall, hidden result in ecx), CCharString::FormatDouble(long, double) 0x009A08B0.
// Shape: one right-to-left evaluated concatenation chain. VC7.1 builds every leaf operand first (Bone, Sub-mesh,
// Z, Y, X, then the explicit "Name: " string), then folds the eleven operator+ calls left to right; the last
// result is built straight into the caller's hidden result slot and the 16 temporaries die in reverse order.
// Layout: retail C3DMeshFileChunk base is 0x2c bytes (the Ego_r.pdb header puts it at 0x34, every member 8 higher);
// Position +0x2c, SubMeshIndex +0x38, BoneIndex +0x3c, Name +0x40 -- the same retail model as the landed
// C3DMeshFileHelperPointChunk ctor (0x00AA9B20) and C3DMeshFileDummyObjectChunk::GetChunkInfo (0x00AA9910).
#if !defined(_MSC_VER)
#define __fastcall
#endif
struct CCharString {
    void* m_pStringData;
    CCharString();
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
    static CCharString __fastcall FormatDouble(long digits, double value); // 0x009A08B0
};
CCharString __fastcall operator+(const CCharString& left, const CCharString& right); // 0x0099F570
CCharString __fastcall operator+(const CCharString& left, const char* right);        // 0x0099F600
CCharString __fastcall GFIntToCharString(long value);                                 // 0x0099F830

struct CDataInputStream; struct CDataOutputStream; struct CChunkedFileLoader; struct CChunkedFileSaver;
struct CChunkedFileChunk {
    virtual void v00(); virtual void v01(); virtual CCharString GetChunkInfo() const;
    virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06();
    virtual void ReadFromFile(CDataInputStream&, unsigned long, CChunkedFileLoader&);
    virtual void WriteToFile(CDataOutputStream&, CChunkedFileSaver&) const;
    char ChunkType[5]; unsigned char _pad[3]; void* Parent;
};
struct CChunkedFileSuperChunk : CChunkedFileChunk {
    struct CArrayPair { void* begin; void* end; void* capacity; };
    CArrayPair SubChunks; CArrayPair RegisteredSubChunkTypes; unsigned long CurrentChunkIndex;
};
struct C3DMeshFileChunk : CChunkedFileSuperChunk {};
struct C3DVector { float X, Y, Z; };
struct C3DMeshFileHelperPointChunk : C3DMeshFileChunk {
    C3DVector Position;      // +0x2c
    long SubMeshIndex;       // +0x38
    long BoneIndex;          // +0x3c
    CCharString Name;        // +0x40
    CCharString GetChunkInfo() const;
};

CCharString C3DMeshFileHelperPointChunk::GetChunkInfo() const
{
    return CCharString("Name: ") + Name + ", Position: (" +
        CCharString::FormatDouble(6, Position.X) + ", " +
        CCharString::FormatDouble(6, Position.Y) + ", " +
        CCharString::FormatDouble(6, Position.Z) + "), Sub-mesh: " +
        GFIntToCharString(SubMeshIndex) + ", Bone: " +
        GFIntToCharString(BoneIndex);
}