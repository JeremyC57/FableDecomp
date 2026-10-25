// C3DAnimFileXAllocationSizeChunk::CreateNewInstance() const -- a new "XALO" C3DAnimFileXAllocationSizeChunk passed through InitNewInstance (the chunk prototype's factory)
// Leaf: ?CreateNewInstance@C3DAnimFileXAllocationSizeChunk@@UBEPAVCChunkedFileChunk@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4530, 62 bytes (+ int3 padding), thiscall.
// Identity: C3DAnimFileXAllocationSizeChunk::CreateNewInstance() const -- proven; evidence: it is slot 9 of vtable 0x0129E144 (RTTI .?AVC3DAnimFileXAllocationSizeChunk@@ :
//   CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass) and builds an object with that same vtable; FableWin.pdb name set holds
//   ?CreateNewInstance@C3DAnimFileXAllocationSizeChunk@@UBEPAVCChunkedFileChunk@@XZ. Family G048 (8 chunk factories; each row's own catalog label agrees).
// Callees: operator new 0x00BFEA1A (0x14 bytes), CChunkedFileChunk::CChunkedFileChunk(const char*) 0x00AA7590 (FableWin.pdb protected
//   ??0CChunkedFileChunk@@IAE@PBD@Z), virtual InitNewInstance(CChunkedFileChunk*) const (slot 10,
//   +0x28; FableWin.pdb ?InitNewInstance@CChunkedFileChunk@@MBEPAV1@PAV1@@Z).
// Shape: the derived constructor is inline (base call with the four-character type literal 0x0129D500, then the vtable store);
//   the null result of new still goes through InitNewInstance; no EH frame (built without /GX).
class CChunkedFileSuperChunk;
class CChunkedFileChunk {
public:
    virtual ~CChunkedFileChunk();                            // slot 0
    virtual void v01(); virtual void v02();                  // GetChunkDescription, GetChunkInfo
    virtual int IsSuperChunk() const;                        // slot 3
    virtual int IsUnknownType() const;                       // slot 4
    virtual void v05(); virtual void v06();                  // GetAsSuperChunk (both constnesses)
    virtual void v07(); virtual void v08();                  // ReadFromFile, WriteToFile
    virtual CChunkedFileChunk* CreateNewInstance() const = 0;   // slot 9
protected:
    CChunkedFileChunk(const char* chunkType);                // 0x00AA7590
    virtual CChunkedFileChunk* InitNewInstance(CChunkedFileChunk* chunk) const;   // slot 10 (+0x28)
    char ChunkType[5];                                       // +0x04
    CChunkedFileSuperChunk* Parent;                          // +0x0c
};
class C3DVector { public: float X, Y, Z; };
class C3DAnimFileXAllocationSizeChunk : public CChunkedFileChunk {
public:
    C3DAnimFileXAllocationSizeChunk() : CChunkedFileChunk("XALO") { }
    virtual CChunkedFileChunk* CreateNewInstance() const;
    unsigned long AllocSize;                                 // +0x10
};

CChunkedFileChunk* C3DAnimFileXAllocationSizeChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileXAllocationSizeChunk);
}