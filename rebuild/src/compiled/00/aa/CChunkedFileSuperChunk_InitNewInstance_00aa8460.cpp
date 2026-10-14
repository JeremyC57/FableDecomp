// CChunkedFileSuperChunk::InitNewInstance(CChunkedFileChunk*) const -- copy this super chunk's registered sub-chunk types into the new instance (reached through its GetAsSuperChunk) and hand the instance back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA8460, length 34 bytes.
// Callees: CChunkedFileChunk vtable slot 6 GetAsSuperChunk() (+0x18, the non-const overload @
// 0x00AA76B0 / 0x00AA7630), CArray<CCountedPointer<CChunkedFileChunk>>::operator= @ 0x00AA9330
// (auto-named "Vector_Assign_CountedPointerEntity_v2"; `ret 4`, the source by reference).
// Mangled name from the manifest: ?InitNewInstance@CChunkedFileSuperChunk@@MBEPAVCChunkedFileChunk@@PAV2@@Z
// (protected virtual, const). Retail layout = engine/CChunkedFileSuperChunk.h minus 4 per CArray
// (12-byte arrays): SubChunks +0x10, RegisteredSubChunkTypes +0x1c -- hence the `_Retail` model.
// The destination array address is formed from the virtual call's result (`add eax,1Ch`), the
// source from `this` (`add edi,1Ch`) only after the call, so the super-chunk pointer is a named
// local (VC evaluates the assignment's right operand first otherwise), and the destination array
// is addressed into its own named pointer (`add eax,1Ch ; mov ecx,eax` rather than a `lea` into
// ecx); the argument is returned unchanged.
struct CChunkedFileSuperChunk_Retail;

struct CChunkedFileChunk_Retail {
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual CChunkedFileSuperChunk_Retail* GetAsSuperChunk();   // slot 6 (+0x18)
    virtual void v07();
    char ChunkType[5];                                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk_Retail* Parent;                  // +0x0c
};

struct CCountedChunk { void* m_pObject; void* m_pCount; };

struct CRetailChunkArray {                                  // retail CArray<CCountedPointer<CChunkedFileChunk>>: {first, last, end}
    CCountedChunk* first; CCountedChunk* last; CCountedChunk* end_of_storage;
    CRetailChunkArray& operator=(const CRetailChunkArray& other);   // 0x00AA9330
};

struct CChunkedFileSuperChunk_Retail : CChunkedFileChunk_Retail {
    CRetailChunkArray SubChunks;                            // +0x10
    CRetailChunkArray RegisteredSubChunkTypes;              // +0x1c
    unsigned long     CurrentChunkIndex;                    // +0x28
    CChunkedFileChunk_Retail* InitNewInstance(CChunkedFileChunk_Retail* instance) const;
};

CChunkedFileChunk_Retail* CChunkedFileSuperChunk_Retail::InitNewInstance(CChunkedFileChunk_Retail* instance) const
{
    CChunkedFileSuperChunk_Retail* super = instance->GetAsSuperChunk();
    CRetailChunkArray* types = &super->RegisteredSubChunkTypes;
    *types = RegisteredSubChunkTypes;
    return instance;
}