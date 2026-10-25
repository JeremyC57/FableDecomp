// C3DAnimFileRootChunk::CreateNewInstance() const -- a new "ANRT" C3DAnimFileRootChunk passed through InitNewInstance (the chunk prototype's factory)
// Leaf: ?CreateNewInstance@C3DAnimFileRootChunk@@UBEPAVCChunkedFileChunk@@XZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A98AB0, 62 bytes (+ int3 padding), thiscall.
// Identity: C3DAnimFileRootChunk::CreateNewInstance() const -- proven; evidence: it is slot 9 of vtable 0x0129DFF0 (RTTI .?AVC3DAnimFileRootChunk@@ :
//   CChunkedFileSuperChunk : CChunkedFileChunk : CBaseClassNonCopyable : CBaseClass) and builds an object with that same vtable; FableWin.pdb name set holds
//   ?CreateNewInstance@C3DAnimFileRootChunk@@UBEPAVCChunkedFileChunk@@XZ. Family G048 (8 chunk factories; each row's own catalog label agrees).
// Callees: operator new 0x00BFEA1A (0x34 bytes), CChunkedFileSuperChunk::CChunkedFileSuperChunk(const char*) 0x00AA8360 (FableWin.pdb protected
//   ??0CChunkedFileSuperChunk@@IAE@PBD@Z; the catalog's "CShopItemDef::CShopItemDef" label is wrong), virtual InitNewInstance(CChunkedFileChunk*) const (slot 10,
//   +0x28; FableWin.pdb ?InitNewInstance@CChunkedFileChunk@@MBEPAV1@PAV1@@Z).
// Shape: the derived constructor is inline (base call with the four-character type literal 0x0129DFBC, then the vtable store);
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
class CChunkedFileSuperChunk : public CChunkedFileChunk {
protected:
    CChunkedFileSuperChunk(const char* chunkType);           // 0x00AA8360
    struct SArray { void* start; void* finish; void* endOfStorage; };   // CArray<CCountedPointer<CChunkedFileChunk> >
    SArray SubChunks;                                        // +0x10
    SArray RegisteredSubChunkTypes;                          // +0x1c
    unsigned long CurrentChunkIndex;                         // +0x28
};
class C3DVector { public: float X, Y, Z; };
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk {
public:
    C3DAnimFileRootChunk() : CChunkedFileSuperChunk("ANRT") { }
    virtual CChunkedFileChunk* CreateNewInstance() const;
    bool LoopFlag;                                           // +0x2c
    float Duration;                                          // +0x30
};

CChunkedFileChunk* C3DAnimFileRootChunk::CreateNewInstance() const
{
    return InitNewInstance(new C3DAnimFileRootChunk);
}