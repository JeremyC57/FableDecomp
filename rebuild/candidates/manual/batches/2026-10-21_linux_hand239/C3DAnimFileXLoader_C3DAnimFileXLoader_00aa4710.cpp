// C3DAnimFileXLoader::C3DAnimFileXLoader() -- build the anim-file loader and register the XSEQ / XALO extension chunk types
// Leaf: ??0C3DAnimFileXLoader
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AA4710, length 365 bytes.
// Identity: stores vtable 0x0129E1E4 after the C3DAnimFileLoader base ctor; the prototype chunks store the
//          C3DAnimFileXSequenceChunk (0x0129E194, "XSEQ" @ 0x0129D4F8) and C3DAnimFileXAllocationSizeChunk
//          (0x0129E144, "XALO" @ 0x0129D500) vtables (rtti-vtables.tsv).
// Callees: C3DAnimFileLoader::C3DAnimFileLoader @ 0x00A999B0, CChunkedFileChunk::CChunkedFileChunk(const char*) @ 0x00AA7590,
//          ~CChunkedFileChunk @ 0x00AA75C0, the three sub-chunk-type registrars @ 0x00A98C80 (into ObjectChunkType +0x18),
//          0x00A98DC0 (MovementVectorChunkType +0x38), 0x00A98D20 (HelpersChunkType +0x30) -- each clones the prototype
//          (slot 9 CreateNewInstance), adds it to the parent chunk type and returns the CCountedPointer by value
//          (manifest "CGuiVarTransferStruct::Add" is propagated); operator delete @ 0x00BFE9BC, free @ 0x00BFEA14.
// Shape: each prototype lives in its own block (one shared frame slot); the returned CCountedPointer is a named
//          block local declared after it, so it is released first (count block: references, __fastcall deleter,
//          object) and its dtor's two clearing stores survive (an unnamed temporary loses them as dead stores);
//          then the prototype's implicit dtor frees the XSEQ byte array and calls the base dtor.
// Layout: PDB C3DAnimFileLoader (struct_layouts_egor.tsv): CChunkedFileLoader base 0x10, RootChunkType +0x10,
//          ObjectChunkType +0x18 .. TimingEventChunkType +0x48, ChunkTypes +0x50. No engine header exists for the loaders.
extern "C" void __cdecl free(void* p);                 // 0x00BFEA14

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4: per-type deleter, object in ecx
    void* m_pObject;                        // +8
};
template <class T>
struct CCountedPointer {
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    ~CCountedPointer()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
        m_pObject = 0;
        m_pCount = 0;
    }
};

template <class T>
struct CArray {                             // retail CArray is 12 bytes (the PDB's is 16)
    T* m_pBegin;
    T* m_pEnd;
    T* m_pEndOfStorage;
    CArray() : m_pBegin(0), m_pEnd(0), m_pEndOfStorage(0) {}
    ~CArray() { if (m_pBegin) free(m_pBegin); }
};

struct CChunkedFileSuperChunk;
struct CChunkedFileChunk {                  // PDB sizeof 0x10
    char ChunkType[5];                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk* Parent;         // +0x0c
    CChunkedFileChunk(const char* type);    // 0x00AA7590
    virtual ~CChunkedFileChunk();           // 0x00AA75C0
};
struct C3DAnimFileXSequenceChunk : CChunkedFileChunk {          // 0x1c
    CArray<unsigned char> CompressedData;   // +0x10
    C3DAnimFileXSequenceChunk() : CChunkedFileChunk("XSEQ") {}
};
struct C3DAnimFileXAllocationSizeChunk : CChunkedFileChunk {    // 0x14
    unsigned long AllocSize;                // +0x10
    C3DAnimFileXAllocationSizeChunk() : CChunkedFileChunk("XALO") {}
};

struct C3DAnimFileLoader {                  // 0x60
    C3DAnimFileLoader();                    // 0x00A999B0
    virtual ~C3DAnimFileLoader();
    unsigned char _chunkedFileLoader[0x0c]; // +0x04 CChunkedFileLoader body
    CCountedPointer<CChunkedFileChunk> RootChunkType;            // +0x10
    CCountedPointer<CChunkedFileChunk> ObjectChunkType;          // +0x18
    CCountedPointer<CChunkedFileChunk> SequenceChunkType;        // +0x20
    CCountedPointer<CChunkedFileChunk> BoneMaskChunkType;        // +0x28
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;         // +0x30
    CCountedPointer<CChunkedFileChunk> MovementVectorChunkType;  // +0x38
    CCountedPointer<CChunkedFileChunk> MovementTrackChunkType;   // +0x40
    CCountedPointer<CChunkedFileChunk> TimingEventChunkType;     // +0x48
    CArray<CChunkedFileChunk*>         ChunkTypes;               // +0x50
    CCountedPointer<CChunkedFileChunk> AddObjectSubChunkType(const CChunkedFileChunk& prototype);          // 0x00A98C80
    CCountedPointer<CChunkedFileChunk> AddMovementVectorSubChunkType(const CChunkedFileChunk& prototype);  // 0x00A98DC0
    CCountedPointer<CChunkedFileChunk> AddHelpersSubChunkType(const CChunkedFileChunk& prototype);         // 0x00A98D20
};

struct C3DAnimFileXLoader : C3DAnimFileLoader {
    C3DAnimFileXLoader();
    virtual ~C3DAnimFileXLoader();
};

C3DAnimFileXLoader::C3DAnimFileXLoader()
{
    {
        C3DAnimFileXSequenceChunk prototype;
        CCountedPointer<CChunkedFileChunk> registered = AddObjectSubChunkType(prototype);
    }
    {
        C3DAnimFileXSequenceChunk prototype;
        CCountedPointer<CChunkedFileChunk> registered = AddMovementVectorSubChunkType(prototype);
    }
    {
        C3DAnimFileXAllocationSizeChunk prototype;
        CCountedPointer<CChunkedFileChunk> registered = AddHelpersSubChunkType(prototype);
    }
}
