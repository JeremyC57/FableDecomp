// C3DAnimFile::AddStringKeyedChunk(const CCharString&, const SNamedChunk&) -- append a {key string, name, chunk} record and mark the file changed
// Leaf: AddStringKeyedChunk
// Plan phase 5 (2026-11-05), CCountedPointer wave: the local CCountedPointer / count-block stand-ins replaced by the shared engine/CCountedPointer.h (Ego_r.pdb: CCountedPointer<T> Data +0x0 / Info +0x4; CCPPointerInfo RefCount +0x0 / DeleteFunc +0x4 / Data +0x8). Also: the local CCountedPointer members m_pObject->Data, m_pBlock->Info are the header CCountedPointer's (same offsets); the local SCountBlock (= CCPPointerInfo) members m_references->RefCount, m_pItem->DeleteFunc are the header CCPPointerInfo's (same offsets).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A73840, 151 bytes. Callees: CCharString copy constructor
// 0x0099EC30, the record vector's insert-at-end 0x00A73530 (16-byte records: CCharString key +0, SNamedChunk {Name +4, Chunk +8}; `this` = the vector at +0),
// operator delete 0x00BFE9BC, CCharString destructor 0x0099EAE0.
// The catalog name "C3DAnimFile::C3DAnimFile" is propagated: no constructor (ends by setting the byte at
// +0xD). Twin of 0x00A737B0 with a CCharString key: the 16-byte pair temporary copies the key string,
// then the {CCharString, CCountedPointer} value through its implicit copy, is pushed by address and
// destroyed (inline counted release, value string, key string).
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

struct SKeyedChunk {
    CCharString Key;                                       // +0x00
    SNamedChunk Value;                               // +0x04
    SKeyedChunk(const CCharString& key, const SNamedChunk& value) : Key(key), Value(value) {}
};

struct CKeyedChunkVector {
    SKeyedChunk* m_pFirst;                           // +0x00
    SKeyedChunk* m_pLast;                            // +0x04
    SKeyedChunk* m_pEnd;                             // +0x08
    void push_back_00a73530(const SKeyedChunk& record);       // 0x00A73530
};

class C3DAnimFile { public:
    CKeyedChunkVector m_Chunks;                      // +0x00
    unsigned char m_flag0C;                          // +0x0C
    bool m_Changed;                                  // +0x0D
    void AddStringKeyedChunk(const CCharString& key, const SNamedChunk& value);
};

void C3DAnimFile::AddStringKeyedChunk(const CCharString& key, const SNamedChunk& value)
{
    m_Chunks.push_back_00a73530(SKeyedChunk(key, value));
    m_Changed = true;
}