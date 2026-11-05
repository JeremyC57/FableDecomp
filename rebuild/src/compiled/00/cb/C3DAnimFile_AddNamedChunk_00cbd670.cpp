// C3DAnimFile::AddNamedChunk(const CCharString&, const CCountedPointer<C3DAnimFileRootChunk>&) -- append a {name, chunk} record and mark the file changed
// Leaf: AddNamedChunk
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pBlock->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pItem->DeleteFunc are the header CCPPointerInfo's (same offsets).
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
#include "engine/CCountedPointer.h"


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