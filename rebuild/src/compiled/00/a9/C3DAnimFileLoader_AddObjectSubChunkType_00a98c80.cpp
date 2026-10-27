// C3DAnimFileLoader::AddObjectSubChunkType(const CChunkedFileChunk& prototype) -- register a new instance of the prototype under the ObjectChunkType's super chunk and return it
// Leaf: ?AddObjectSubChunkType@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00a98c80, 157 bytes, thiscall, hidden return pointer, `ret 8`.
// Identity (proven): it registers under this+0x18, which is Ego_r.pdb C3DAnimFileLoader::ObjectChunkType (the landed constructor 0x00A999B0's layout); bound ?AddObjectSubChunkType@C3DAnimFileLoader@@QAE?AV?$CCountedPointer@VCChunkedFileChunk@@@@ABVCChunkedFileChunk@@@Z; called by the landed C3DAnimFileLoader::C3DAnimFileLoader at 0x00A99B56 / 0x00A99BF3 (sequence, bone-mask prototypes); also C3DAnimFileXLoader::C3DAnimFileXLoader 0x00AA4752.
// Callees: the prototype's CreateNewInstance() const (CChunkedFileChunk vtable slot 9), operator new 0x00BFEA1A (the 12-byte
//   count block, deleter Destroy_00a99ff0 0x00A99FF0), the parent's GetAsSuperChunk() (slot 6, 0x00AA76B0 in the super-chunk
//   vtables), CChunkedFileSuperChunk::RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>&) 0x00AA87E0, operator
//   delete 0x00BFE9BC (the local's release).
// Shape (family L112): the counted pointer is a frame local, copied into the return slot (count bump) and released (count
//   re-read after the decrement); RegisterSubChunkType's argument is pushed before the virtual call that yields its `this`.
//   Classes as in the landed C3DAnimFileLoader::C3DAnimFileLoader 0x00A999B0 (only what this body touches).
void __fastcall Destroy_00a99ff0(void* object);   // 0x00A99FF0: the deleter stored in the count block

struct SCountBlock {
    long m_references;                      // +0
    void (__fastcall* m_pfnDestroy)(void*); // +4
    void* m_pObject;                        // +8
    SCountBlock(void* object) : m_references(1), m_pfnDestroy(Destroy_00a99ff0), m_pObject(object) { }
};
template <class T>
class CCountedPointer { public:
    T* m_pObject;                           // +0
    SCountBlock* m_pCount;                  // +4
    CCountedPointer(T* object) : m_pObject(object)
    {
        if (object)
            m_pCount = new SCountBlock(m_pObject);
        else
            m_pCount = 0;
    }
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pCount(other.m_pCount)
    {
        if (m_pCount)
            ++m_pCount->m_references;
    }
    void Release()
    {
        if (m_pCount) {
            --m_pCount->m_references;
            if (m_pCount->m_references == 0) {
                m_pCount->m_pfnDestroy(m_pCount->m_pObject);
                delete m_pCount;            // operator delete @ 0x00BFE9BC
            }
        }
    }
    ~CCountedPointer()
    {
        Release();
        m_pObject = 0;
        m_pCount = 0;
    }
};

class CChunkedFileSuperChunk;
class CChunkedFileChunk { public:                  // 0x10
    virtual ~CChunkedFileChunk();
    virtual void Slot1();
    virtual void Slot2();
    virtual void Slot3();
    virtual void Slot4();
    virtual void Slot5();
    virtual CChunkedFileSuperChunk* GetAsSuperChunk();                    // slot 6
    virtual void Slot7();
    virtual void Slot8();
    virtual CChunkedFileChunk* CreateNewInstance() const;                 // slot 9
    char ChunkType[5];                      // +0x04
    unsigned char _pad_0x09[3];
    CChunkedFileSuperChunk* Parent;         // +0x0c
};
class CChunkedFileSuperChunk : public CChunkedFileChunk { public:
    void RegisterSubChunkType(const CCountedPointer<CChunkedFileChunk>& chunkType);   // 0x00AA87E0
};
class C3DAnimFileRootChunk : public CChunkedFileSuperChunk { };

class CChunkedFileLoader { public:                 // 0x10
    virtual ~CChunkedFileLoader();
    long ReadUnknownChunkTypes;             // +0x04
    char SupportedFileType[5];              // +0x08
};

struct C3DAnimFileLoader : CChunkedFileLoader {
    CCountedPointer<C3DAnimFileRootChunk> RootChunkType;        // +0x10
    CCountedPointer<CChunkedFileChunk> ObjectChunkType;          // +0x18
    CCountedPointer<CChunkedFileChunk> SequenceChunkType;        // +0x20
    CCountedPointer<CChunkedFileChunk> BoneMaskChunkType;        // +0x28
    CCountedPointer<CChunkedFileChunk> HelpersChunkType;         // +0x30
    CCountedPointer<CChunkedFileChunk> MovementVectorChunkType;  // +0x38
    CCountedPointer<CChunkedFileChunk> MovementTrackChunkType;   // +0x40
    CCountedPointer<CChunkedFileChunk> TimingEventChunkType;     // +0x48
    CCountedPointer<CChunkedFileChunk> AddObjectSubChunkType(const CChunkedFileChunk& prototype);
};

CCountedPointer<CChunkedFileChunk> C3DAnimFileLoader::AddObjectSubChunkType(const CChunkedFileChunk& prototype)
{
    CCountedPointer<CChunkedFileChunk> chunkType(prototype.CreateNewInstance());
    ObjectChunkType.m_pObject->GetAsSuperChunk()->RegisterSubChunkType(chunkType);
    return chunkType;
}