// C3DAnimFile::AddNamedChunk(const CCharString&, const CCountedPointer<C3DAnimFileRootChunk>&) -- append a {name, chunk} record and mark the file changed
// Leaf: AddNamedChunk
// Retail: Fable.exe (TLC, VC7.1) @ 0x00CBD670, 144 bytes. Callees: CCharString copy constructor
// 0x0099EC30, the record vector's push_back 0x00CBD310 (12-byte records, `this` = the vector at +0),
// operator delete 0x00BFE9BC, CCharString destructor 0x0099EAE0.
// The catalog name "C3DAnimFile::C3DAnimFile" is propagated: the body is no constructor (no `mov eax,this`,
// it ends by setting the byte at +0xD). Shape: a 12-byte record temporary {CCharString, CCountedPointer}
// built with the out-of-line string copy and the inline counted-pointer copy (`inc [block]`), pushed
// by address, then destroyed: the counted-pointer destructor re-reads the block after the decrement
// (`--count; if (count == 0)`), calls the block's destroy pointer-to-member on the item (+4 / +8),
// deletes the block and clears both words; the string destructor runs last.
void __cdecl operator delete(void* block);          // 0x00BFE9BC

class CCharString { public:
    void* m_pData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

struct CCountedItem { };
struct SCountBlock {
    long m_references;                               // +0x00
    void (CCountedItem::*m_pfnDestroy)();            // +0x04 (called with the item as `this`)
    CCountedItem* m_pItem;                           // +0x08
};

template <class T>
class CCountedPointer { public:
    T* m_pObject;                                    // +0x00
    SCountBlock* m_pBlock;                           // +0x04
    CCountedPointer(const CCountedPointer& other) : m_pObject(other.m_pObject), m_pBlock(other.m_pBlock)
    {
        if (m_pBlock) ++m_pBlock->m_references;
    }
    ~CCountedPointer()
    {
        if (m_pBlock) {
            --m_pBlock->m_references;
            if (m_pBlock->m_references == 0) {
                (m_pBlock->m_pItem->*m_pBlock->m_pfnDestroy)();
                operator delete(m_pBlock);
            }
        }
        m_pObject = 0;
        m_pBlock = 0;
    }
};

class C3DAnimFileRootChunk;

struct SNamedChunk {
    CCharString Name;                                // +0x00
    CCountedPointer<C3DAnimFileRootChunk> Chunk;     // +0x04
    SNamedChunk(const CCharString& name, const CCountedPointer<C3DAnimFileRootChunk>& chunk) : Name(name), Chunk(chunk) {}
};

struct CNamedChunkVector {
    SNamedChunk* m_pFirst;                           // +0x00
    SNamedChunk* m_pLast;                            // +0x04
    SNamedChunk* m_pEnd;                             // +0x08
    void push_back(const SNamedChunk& record);       // 0x00CBD310
};

class C3DAnimFile { public:
    CNamedChunkVector m_Chunks;                      // +0x00
    unsigned char m_flag0C;                          // +0x0C
    bool m_Changed;                                  // +0x0D
    void AddNamedChunk(const CCharString& name, const CCountedPointer<C3DAnimFileRootChunk>& chunk);
};

void C3DAnimFile::AddNamedChunk(const CCharString& name, const CCountedPointer<C3DAnimFileRootChunk>& chunk)
{
    m_Chunks.push_back(SNamedChunk(name, chunk));
    m_Changed = true;
}