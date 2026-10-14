// C3DMeshFilePartitionChunkFactory::CreateNewInstance() -- registers `new C3DMeshFilePartitionChunk` (inline ctor: CChunkedFileChunk("MPRT") + null counted pointer, 0x18 bytes) through factory slot 10
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AC29C0, length 76 bytes.
// Callees: 0x00BFEA1A operator new, 0x00AA7590 CChunkedFileChunk ctor, vslot +0x28
//
#include "engine/C3DMeshFilePartitionChunk.h"   // PDB layout agrees with retail here (counted pointer at +0x10)
struct CChunkedFileChunk {
    virtual void v0();
    CChunkedFileChunk(const char* chunkType);           // 0x00AA7590
    unsigned char _base_body[0x0c];                     // ChunkType[5] + pad + Parent  (sizeof == 0x10)
};
struct C3DMeshPartitionElement;
struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T* m_pData; CCPPointerInfo* m_pInfo;
    CCountedPointer() : m_pData(0), m_pInfo(0) {}
    void Reset(T* p);                                   // 0x00AC38A0
};
struct C3DMeshFilePartitionChunkRetail : CChunkedFileChunk {
    virtual void v1(); virtual void v2(); virtual void v3(); virtual void v4(); virtual void v5(); virtual void v6(); virtual void v7();
    CCountedPointer<C3DMeshPartitionElement> PartitionTree;   // +0x10 / +0x14
    C3DMeshFilePartitionChunkRetail() : CChunkedFileChunk("MPRT") {}
};
FABLE_STATIC_ASSERT(sizeof(C3DMeshFilePartitionChunkRetail) == sizeof(C3DMeshFilePartitionChunk));
struct CChunkedFileChunkFactory {
    virtual void f0(); virtual void f1(); virtual void f2(); virtual void f3(); virtual void f4();
    virtual void f5(); virtual void f6(); virtual void f7(); virtual void f8(); virtual void f9();
    virtual CChunkedFileChunk* RegisterNewChunk(CChunkedFileChunk* chunk);   // slot 10 (+0x28)
};

struct C3DMeshFilePartitionChunkFactory : CChunkedFileChunkFactory {
    CChunkedFileChunk* CreateNewInstance();
};
CChunkedFileChunk* C3DMeshFilePartitionChunkFactory::CreateNewInstance()
{
    return RegisterNewChunk(new C3DMeshFilePartitionChunkRetail);
}
